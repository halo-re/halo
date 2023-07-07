#!/usr/bin/env python3
if __name__ == "__main__":
    from check_requirements import check_requirements
    check_requirements()

import json
from collections import defaultdict
from typing import Sequence, Union, Optional
import logging
import os.path
import re
import argparse

import clang.cindex as clang


log = logging.getLogger(__name__)
root_dir = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
reg_filter_re = re.compile(r'@<(\w+)>')


def filter_reg_assignments(s: str) -> str:
	return reg_filter_re.sub('', s)

with open(os.path.join(root_dir, 'src', 'types.h')) as f:
	types_file = f.read()

def parse_string(s: str) -> str:
	s = types_file + filter_reg_assignments(s)
	index = clang.Index.create()
	tu = index.parse('tmp.h', args=['-target', 'i386-pc-win32'], unsaved_files=[('tmp.h', s)])
	return tu


class Symbol:
	def __init__(self, decl: str, addr: Optional[Union[str, int]] = None, **kwargs):
		self._parsed = None
		self.decl = decl
		if type(addr) is str:
			addr = int(addr, 16)
		self.addr = addr

	@property
	def cursor(self):
		if self._parsed is None:
			self._parsed = list(parse_string(self.decl).cursor.get_children())[-1]
		return self._parsed

	@property
	def name(self):
		if clang is None:
			return self.decl
		return self.cursor.spelling

	def serialize(self):
		return {'decl': self.decl, 'addr': hex(self.addr)}

	@property
	def requires_reg_thunk(self):
		return False

class Data(Symbol):
	def __repr__(self):
		return f'<Data "{self.decl}">'


class Function(Symbol):
	def __repr__(self):
		return f'<Function "{self.decl}">'

	@property
	def requires_reg_thunk(self):
		return reg_filter_re.search(self.decl) is not None

class KnowledgeBase:

	kb_path: str = os.path.join(root_dir, 'kb.json')

	def __init__(self):
		self.symbols = []
		self.object_to_symbols: Mapping[str, Sequence[Symbol]] = defaultdict(list)
		self.symbol_to_object: Mapping[Symbol, str] = {}
		self.object_to_source: Mapping[str, str] = {}
		self.name_to_addr: Mapping[str, int] = {}
		self.expected_md5: Optional[str] = None
		self.addr_to_symbols = {}

	def add_symbols(self, symbols: Sequence[Symbol]):
		self.symbols.extend(symbols)

	def gen_thunk(self, s: Function):
		match = reg_filter_re.search(s.decl)
		assert match is not None

		c = match.span()[0]
		text_before = s.decl[0:c]
		assert text_before.find(',') < 0, "Can't handle non-first register argument thunks yet"
		assert text_before.find('(') > 0, "Can't handle return in custom register yet"

		rtype = s.cursor.result_type.spelling
		fname = s.cursor.spelling
		args = list(s.cursor.get_arguments())
		reg = match.group(1)
		thunkname = f'thunk'
		log.info('Generating %s arg-register thunk for %s', reg, fname)

		new_func_decl = f'{rtype} (*{fname}__xbe)(/* {args[0].type.spelling} {args[0].spelling}@<{reg}>'
		if len(args) > 1:
			new_func_decl += ', */ '
			new_func_decl += ', '.join('%s %s' % (a.type.spelling, a.spelling) for a in args)
		else:
			new_func_decl += ' */ void'
		new_func_decl += ')'

		# clang apparently avoids clobbering the register we just wrote to. This should really be replaced with something
		# better though
		name = s.name
		mangled_name = s.cursor.mangled_name
		name_alternate = name + '__thunk'
		mangled_name_alternate = mangled_name.replace(name, name_alternate)
		decl = filter_reg_assignments(s.decl)[:-1]

		thunk_functions = f'''\
#ifdef MSVC
#pragma comment(linker, "/alternatename:{mangled_name}={mangled_name_alternate}")
#endif

{new_func_decl} = (void*){s.addr:#x};

{ decl.replace(name, 'THUNK('+name+')') }
{{
  asm mov { reg }, { args[0].spelling };
  { "return " if rtype != 'void' else ''}{fname}__xbe({', '.join(a.spelling for a in args[1:])});
}}
'''
		# FIXME: Generate special XBE->EXE thunker
		return thunk_functions

	def build_header(self, path: str):
		log.info('Generating header...')
		with open(path, 'w') as f:
			f.write('//\n'
					'// AUTOMATICALLY GENERATED. DO NOT EDIT.\n'
					'//\n\n'
					'#define HFUNC __declspec(dllexport)\n'
					'#define HDATA __declspec(dllimport)\n'
					'\n')

			objs_sorted = sorted(n for n in self.object_to_symbols if n is not None)
			for object_name in (objs_sorted + [None]):
				symbols = self.object_to_symbols[object_name]
				f.write(f'// obj:{object_name or "?"} src:{self.object_to_source.get(object_name, "?")}\n')
				for s in sorted(symbols, key=lambda s: (isinstance(s, Function), s.name)):
					if isinstance(s, Data):
						f.write(f'HDATA {s.decl}\n')
					elif isinstance(s, Function):
						f.write(f'HFUNC {filter_reg_assignments(s.decl)}\n')
				f.write('\n')

			f.write('\n'
					'#undef HFUNC\n'
					'#undef HDATA\n'
					'\n'
					'//\n'
					'// AUTOMATICALLY GENERATED. DO NOT EDIT.\n'
					'//\n')

	def build_thunks(self, path: str):
		log.info('Generating thunks...')
		with open(path, 'w') as f:
			f.write("""
#ifdef MSVC
#pragma code_seg(".thunks")
#define asm __asm
#define THUNK(x) x ## __thunk
#else
#define THUNK(x) __attribute__((weak, section(".thunks"))) x
#endif

""")

			objs_sorted = sorted(n for n in self.object_to_symbols if n is not None)
			for object_name in (objs_sorted + [None]):
				symbols = self.object_to_symbols[object_name]
				for s in sorted(symbols, key=lambda s: (isinstance(s, Function), s.name)):
					if isinstance(s, Function) and s.requires_reg_thunk:
							t = self.gen_thunk(s)
							f.write(t)
							# FIXME: If we have an implementation, export
							#        the thunker to the patcher

	def build_def(self, path: str):
		log.info('Generating XBE export .def file...')

		with open(path, 'w') as f:
			f.write('LIBRARY halo.xbe\n'
					'EXPORTS\n')
			for s in sorted(self.symbols, key=lambda s: (isinstance(s, Function), s.name)):
				if s.requires_reg_thunk:
					continue
				f.write('\t' + s.name)
				if isinstance(s, Data):
					f.write(' DATA\n')
				else:
					f.write('\n')

	def serialize(self):
		log.info('Saving knowledge base to %s...', self.kb_path)

		with open(self.kb_path, 'w') as f:
			out = {
				'md5': self.expected_md5,
				'objects': []
			}

			# Make sure symbols are associated with an object (or the None object)
			for s in self.symbols:
				if s not in self.symbol_to_object:
					self.object_to_symbols[None].append(s)
					self.symbol_to_object[s] = None

			objs_sorted = sorted(n for n in self.object_to_symbols if n is not None)
			for obj in (objs_sorted + [None]):
				symbols = self.object_to_symbols[obj]
				if len(symbols) == 0:
					continue
				obj_data = {'name': obj}
				if obj in self.object_to_source:
					obj_data['source'] = self.object_to_source[obj]
				data = [s.serialize() for s in sorted(symbols, key=lambda s: s.addr) if isinstance(s, Data)]
				if data:
					obj_data['data'] = data
				funcs = [s.serialize() for s in sorted(symbols, key=lambda s: s.addr) if isinstance(s, Function)]
				if funcs:
					obj_data['functions'] = funcs
				out['objects'].append(obj_data)
			json.dump(out, f, indent=2)

	@classmethod
	def deserialize(cls) -> 'KnowledgeBase':
		log.info('Loading knowledge base...')
		with open(cls.kb_path) as f:
			serialized_kb = json.load(f)

		kb = KnowledgeBase()
		kb.expected_md5 = serialized_kb['md5']
		for o in serialized_kb['objects']:
			if 'source' in o:
				kb.object_to_source[o['name']] = o['source']
			obj_syms = []
			if 'functions' in o:
				obj_syms.extend([Function(**s) for s in o['functions']])
			if 'data' in o:
				obj_syms.extend([Data(**s) for s in o['data']])
			if obj_syms:
				kb.add_symbols(obj_syms)
				for s in obj_syms:
					kb.object_to_symbols[o['name']].append(s)
					kb.symbol_to_object[s] = o['name']
					if s.addr:
						kb.name_to_addr[s.name] = s.addr

		if os.path.exists(os.path.join(root_dir, 'tools', 'load_truth.py')):
			from load_truth import load_truth
			load_truth(kb)

		kb.addr_to_symbol = {s.addr:s for s in kb.symbols}

		num_symbols_with_truth = len([s for s in kb.symbols if kb.symbol_to_object[s]])
		num_symbols_without_truth = len(kb.symbols) - num_symbols_with_truth

		log.info('%d symbols were identified with objects, %d were not', num_symbols_with_truth, num_symbols_without_truth)
		num_objs_with_source = len([s for o, s in kb.object_to_source.items() if o in kb.object_to_symbols])
		log.info('%d of %d object files have known source mapping', num_objs_with_source, len(kb.object_to_symbols))

		return kb


def main():
	ap = argparse.ArgumentParser()
	ap.add_argument('--gen-header', help='Generate import header')
	ap.add_argument('--gen-thunks', help='Generate import thunks')
	ap.add_argument('--gen-def', help='Generate import linker def file')
	ap.add_argument('--update', action='store_true', help='Re-serialize the KB')
	args = ap.parse_args()

	logging.basicConfig(level=logging.INFO)
	kb = KnowledgeBase.deserialize()

	if args.gen_header:
		kb.build_header(args.gen_header)

	if args.gen_thunks:
		kb.build_thunks(args.gen_thunks)

	if args.gen_def:
		kb.build_def(args.gen_def)

	if args.update:
		kb.serialize()


if __name__ == '__main__':
	main()
