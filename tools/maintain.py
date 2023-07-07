#!/usr/bin/env python3
if __name__ == "__main__":
    from check_requirements import check_requirements
    check_requirements()

import argparse
import logging
import os
import subprocess
from typing import Mapping, Tuple, Optional, Sequence

import clang.cindex as clang

import color
from .knowledge import KnowledgeBase


log = logging.getLogger(__name__)


class SourceManager:
    def __init__(self, kb: KnowledgeBase):
        self.kb: KnowledgeBase = kb

    @staticmethod
    def _get_tu_for_file(path: str) -> clang.TranslationUnit:
        with open(path) as f:
            s = f.read()
        index = clang.Index.create()
        tu = index.parse('tmp.h', unsaved_files=[('tmp.h', s)], options=0)
        return tu

    def _get_path_for_name(self, name: str):
        """
        Get expected path for function `name`, or None if unknown.
        """
        addr = self.kb.name_to_addr.get(name, None)
        if addr is None:
            log.error('Symbol "%s" is not in the knowledge base. Please add it!', name)
            exit(1)

        sym = self.kb.addr_to_symbol[addr]
        obj = self.kb.symbol_to_object.get(sym, None)
        if obj is None:
            return None

        path = self.kb.object_to_source.get(obj, None)
        if path is None:
            return None

        path = os.path.join('src/halo/', path)
        log.debug('Determined source path for function "%s": %s', name, path)
        return os.path.normpath(path)

    def _get_function_extents_in_file(self, path: str) -> Mapping[str, Tuple[int, int]]:
        """
        Get a mapping of function name to (start line, end line), for each known function. Line numbers begin at 1.
        """
        tu = self._get_tu_for_file(path)
        name_to_extent_map = {}

        for c in tu.cursor.get_children():
            if (c.kind != clang.CursorKind.FUNCTION_DECL or
                    c.spelling not in self.kb.name_to_addr):
                continue
            name, extents = c.spelling, (c.extent.start.line, c.extent.end.line)
            if name in name_to_extent_map:
                log.error('%s defined multiple times in %s!', name, path)
                exit(1)
            name_to_extent_map[name] = extents
            log.debug('Function "%s" located at %s lines %d-%d', name, path, extents[0], extents[1])

        # Grow extents to include anything that follows the previous function (comments, inlines, etc)
        current_line = 1
        for name, extents in name_to_extent_map.items():
            new_extents = (current_line, extents[1])
            if new_extents != extents:
                log.debug('Expanding function "%s" extents from %d-%d to %d-%d',
                          name, extents[0], extents[1], new_extents[0], new_extents[1])
            current_line = extents[1] + 1
            name_to_extent_map[name] = new_extents

        return name_to_extent_map

    @staticmethod
    def _trim_preceeding_blank_lines(lines: Sequence[str]):
        """
        Trim blank lines preceeding function text.
        """
        index = 0
        for line in lines:
            if line.strip():
                break
            index += 1
        if index:
            return lines[index:]
        return lines

    def migrate_misplaced_functions_in_file(self, path: str):
        """
        Move any misplaced functions in `path` to their expected source file.
        """
        path = os.path.normpath(path)
        log.debug('Checking for any misplaced functions in %s', path)

        name_to_extent_map = self._get_function_extents_in_file(path)
        names_sorted_by_start = list(sorted(name_to_extent_map, key=lambda n: name_to_extent_map[n][0]))

        with open(path) as f:
            lines = f.read().splitlines(keepends=False)

        num_lines_removed = 0

        for name in names_sorted_by_start:
            expected_source = self._get_path_for_name(name)
            if expected_source is None:
                continue
            if expected_source != path:
                log.info('Moving %s from %s to %s', name, path, expected_source)
                extents = name_to_extent_map[name]
                start = extents[0] - num_lines_removed - 1
                end = extents[1] - num_lines_removed
                os.makedirs(os.path.dirname(expected_source), exist_ok=True)
                with open(expected_source, 'a') as f:
                    f.write('\n'.join(lines[start:end]) + '\n')
                num_lines_removed += end - start
                lines = lines[0:start] + lines[end:]

        if num_lines_removed:
            with open(path, 'w') as f:
                f.write('\n'.join(lines))

    def sort_functions_by_address_in_file(self, path: str):
        """
        Sorts the functions in `path` by expected address.
        """
        log.debug('Checking function order in %s...', path)
        name_to_extent_map = self._get_function_extents_in_file(path)
        names_sorted_by_addr = list(sorted(name_to_extent_map, key=lambda n: self.kb.name_to_addr[n]))
        names_sorted_by_start = list(sorted(name_to_extent_map, key=lambda n: name_to_extent_map[n][0]))
        if names_sorted_by_addr == names_sorted_by_start:
            return

        log.info('Sorting functions in %s by address...', path)
        with open(path) as f:
            lines = f.read().splitlines(keepends=False)

        trailing_lines = lines[name_to_extent_map[names_sorted_by_start[-1]][1]:]

        # Merge functions together by address
        new_lines = []
        for name in names_sorted_by_addr:
            extents = name_to_extent_map[name]
            num_lines = extents[1] - extents[0] + 1
            new_start = len(new_lines) + 1
            if len(new_lines) > 0:
                new_lines.append('')
            new_lines.extend(self._trim_preceeding_blank_lines(lines[(extents[0] - 1):extents[1]]))
            new_extents = (new_start, len(new_lines))

            if new_extents[0] != extents[0]:
                log.debug('Moving function %s (%d lines) from [%d-%d] to [%d-%d]',
                         name, num_lines, extents[0], extents[1], new_extents[0], new_extents[1])
                name_to_extent_map[name] = new_extents

        new_lines.extend(trailing_lines)

        with open(path, 'w') as f:
            f.write('\n'.join(new_lines) + '\n')

    def insert_or_update_function(self, target_function_text: str, path: Optional[str] = None):
        """
        Add function defined by `target_function_text` to file at `path`. If `path` is not provided, the KB will be
        checked to attempt to automatically resolve it. If the function was already defined in `path`, replace it
        with the new implementation.
        """
        index = clang.Index.create()
        tu = index.parse('tmp.h', unsaved_files=[('tmp.h', target_function_text)], options=0)
        # We just take the last function
        c = list(tu.cursor.get_children())[-1]
        assert c.kind == clang.CursorKind.FUNCTION_DECL
        name = c.spelling
        log.info('New source provides function "%s"', name)

        # Determine path for file
        if not path:
            path = self._get_path_for_name(name)
            if path is None:
                log.error('Could not determine source file for new function "%s"! '
                          'Please provide target source file path', name)
                exit(1)

        if not os.path.exists(path):
            log.info('Creating new file %s', path)
            os.makedirs(os.path.dirname(path), exist_ok=True)
            lines = []
            name_to_extent_map = {}
        else:
            log.info('Opening existing file %s', path)
            with open(path) as f:
                lines = f.read().splitlines(keepends=False)
            name_to_extent_map = self._get_function_extents_in_file(path)

        if name in name_to_extent_map:
            # Cut existing function out and update in place
            start, end = name_to_extent_map[name]
            lines = lines[0:start - 1] + lines[end:]
            insert_at = start - 1
            log.info('Removing function "%s" in %s at line %d (%d lines)', name, path, insert_at,
                     end - start + 1)

        else:
            # Insert in address order
            insert_at = 0
            addr = self.kb.name_to_addr[name]
            for other_function_name in sorted(name_to_extent_map, key=lambda n: self.kb.name_to_addr[n]):
                extents = name_to_extent_map[other_function_name]
                other_func_addr = self.kb.name_to_addr[other_function_name]
                comes_after = other_func_addr > addr
                log.info('"%s" at [%d-%d] comes %s "%s"',
                         other_function_name, extents[0], extents[1], 'after' if comes_after else 'before',
                         name)
                if comes_after:
                    break
                insert_at = extents[1]

        # Make sure there's a blank line before function
        if insert_at > 0:
            lines.insert(insert_at, '')
            insert_at += 1

        log.info('Inserting function "%s" in %s at line %d', name, path, insert_at+1)
        lines.insert(insert_at, target_function_text.strip())

        with open(path, 'w') as f:
            if lines[-1]:
                lines.append('')
            f.write('\n'.join(lines))

    @staticmethod
    def reformat_file(path: str):
        call_args = ['clang-format', '-style=file:.clang-format', '-i', path]
        log.debug('Calling: %s', ' '.join(call_args))
        subprocess.check_call(call_args)


def main():
    logging.basicConfig(level=logging.INFO, handlers=[color.ColorLogHandler()])

    ap = argparse.ArgumentParser(description='Performs some automated maintenance on the tree: '
                                             'moving functions into place and re-formatting.')
    ap.add_argument('--update-from', default='', help='File to pull new updates from')
    ap.add_argument('source', nargs='?', default='', help='File to process, otherwise all files are processed')
    args = ap.parse_args()
    kb = KnowledgeBase.deserialize()
    sm = SourceManager(kb)

    # FIXME: We don't need to parse files so much
    def for_all_source_files(callable):
        for root, _, files in os.walk('src/halo'):
            for f in files:
                if f.endswith('.c'):
                    callable(os.path.join(root, f))

    def for_targeted_source(callable):
        if args.source:
            callable(args.source)
        else:
            for_all_source_files(callable)

    if args.update_from:
        with open(args.update_from, 'r') as f:
            function_text = f.read()
        sm.insert_or_update_function(function_text, args.source)

    log.info('Checking for misplaced functions...')
    for_targeted_source(sm.migrate_misplaced_functions_in_file)

    log.info('Checking function order...')
    for_targeted_source(sm.sort_functions_by_address_in_file)

    log.info('Re-formatting files...')
    for_targeted_source(sm.reformat_file)


if __name__ == '__main__':
    main()
