#!/usr/bin/env python3
import itertools
import struct
import subprocess
import os
import logging
import hashlib

import pefile
from xbe import Xbe, XbeSection, XbeSectionHeader, XbeKernelImage

from misc import color
from knowledge import KnowledgeBase


log = logging.getLogger(__name__)


def round_up(value, round_to):
    value += (round_to - 1)
    value &= ~(round_to - 1)
    return value


def ensure_unique_section_name(xbe, name):
    # FIXME: Section names don't actually need to be unique, but pyxbe uses a dict to track them, so we simply tack
    #        on a number to make it unique.
    base_name = name
    attempt = 1
    while name in xbe.sections:
        name = f'{base_name}_{attempt}'
        attempt += 1
    return name


# FIXME: Move to pyxbe
def write_to_vaddr(xbe: Xbe, vaddr: int, data: bytes):
    found, section = False, None
    for section in xbe.sections.values():
        min_addr = section.header.virtual_addr
        max_addr = min_addr + section.header.virtual_size
        if min_addr <= vaddr < max_addr:
            found = True
            break
    assert found, "Could not find section for patch offset"

    raw_offset = vaddr - section.header.virtual_addr
    new_data = bytearray(section.data)
    new_data[raw_offset:raw_offset + len(data)] = data
    section.data = bytes(new_data)


def main():
    output_dir = 'halo-patched'
    input_xbe_path = os.path.join(output_dir, 'cachebeta.xbe')
    output_xbe_path = os.path.join(output_dir, 'default.xbe')
    output_iso_path = f'{output_dir}.iso'

    if not os.path.isdir(output_dir):
        log.error('Output directory %s does not exist. Prepare with disc content.', output_dir)
        exit(1)

    if not os.path.isfile(input_xbe_path):
        log.error('Could not find input XBE %s', input_xbe_path)
        exit(1)
    log.info('Original XBE: %s', input_xbe_path)

    kb = KnowledgeBase.deserialize()


    # Load xbe
    log.info('Verifying original XBE MD5')
    with open(input_xbe_path, 'rb') as f:
        md5 = hashlib.md5(f.read()).hexdigest()
        if md5 == kb.expected_md5:
            log.info('Ok')
        else:
            log.error('Incorrect MD5 hash of XBE')
            exit(1)

    log.info('Loading original XBE')
    xbe = Xbe.from_file(input_xbe_path)

    # Determine new base address for EXE
    base_addr = round_up(max(s.header.virtual_addr + s.header.virtual_size for s in xbe.sections.values()), 0x1000)

    # Rebuild re-implementation from source
    log.info('Generating definitions...')
    kb.build_header() # FIXME: Only build if different
    kb.build_def()

    log.info('Compiling source...')
    subprocess.check_call(['make', '-C', 'src', '-j8'])
    input_exe_path = 'src/main.exe'
    if not os.path.exists(input_exe_path):
        log.error('Failed to generate patch executable')
        exit(1)

    # Patch EXE into XBE
    log.info('Loading EXE %s', input_exe_path)
    pe = pefile.PE(input_exe_path)
    log.info('EXE image base %x', pe.OPTIONAL_HEADER.ImageBase)

    pe_original_base = pe.OPTIONAL_HEADER.ImageBase
    if pe_original_base != base_addr:
        log.info('Rebasing EXE from %x to %x', pe_original_base, base_addr)
        if not pe.has_relocs():
            log.error('EXE was not built with relocation info')
            exit(1)
        pe.relocate_image(base_addr)

    exe_to_xbe_section_map = {}

    # Add EXE header, up to first section start, as a section
    first_section = sorted(pe.sections, key=lambda s: s.VirtualAddress)[0]
    pe_header_data = pe.trim()
    if len(pe_header_data) > first_section.VirtualAddress:
        pe_header_data = pe_header_data[:first_section.VirtualAddress]

    hdr = XbeSectionHeader()
    hdr.flags = hdr.FLAG_PRELOAD
    hdr.virtual_addr = base_addr
    hdr.virtual_size = round_up(len(pe_header_data), 0x1000)
    hdr.raw_addr = 0
    hdr.raw_size = len(pe_header_data)

    vstart, size = pe_original_base, len(pe_header_data)
    name = ensure_unique_section_name(xbe, 'hdr.patch')
    log.info('Patching EXE header from [%x:%x) into XBE as section "%s" [%x:%x) length %x' % (
        vstart, vstart + size, name, hdr.virtual_addr, hdr.virtual_addr + hdr.virtual_size, size))
    xbe.sections[name] = XbeSection(name, hdr, pe_header_data)

    for section in pe.sections:
        name = section.Name.rstrip(b'\x00').decode('ascii')
        if name.startswith('/') or name in ['.reloc']:
            log.info('Skipping section "%s"' % name)
            continue
        new_name = ensure_unique_section_name(xbe, name + '.patch')

        hdr = XbeSectionHeader()
        hdr.flags = hdr.FLAG_PRELOAD | hdr.FLAG_WRITABLE  # FIXME: Writable for import resolve
        flag_text = 'r'
        if section.__dict__.get('IMAGE_SCN_MEM_WRITE', False):
            hdr.flags |= hdr.FLAG_WRITABLE
            flag_text += 'w'
        if section.__dict__.get("IMAGE_SCN_MEM_EXECUTE", False):
            hdr.flags |= hdr.FLAG_EXECUTABLE
            flag_text += 'x'
        hdr.virtual_addr = section.VirtualAddress + base_addr
        hdr.virtual_size = round_up(section.Misc_VirtualSize, 0x1000)
        hdr.raw_addr = 0
        hdr.raw_size = section.SizeOfRawData

        vstart = section.VirtualAddress + pe_original_base
        log.info('Patching EXE section into XBE:\n'
                 '    "%s" [%x:%x) length %x, raw length %x, flags %s (EXE)\n'
                 '  ->"%s" [%x:%x) length %x (XBE)' % (
                     name, vstart, vstart + section.Misc_VirtualSize, section.Misc_VirtualSize, section.SizeOfRawData,
                     flag_text,
                     new_name, hdr.virtual_addr, hdr.virtual_addr + hdr.virtual_size, hdr.virtual_size))
        data = section.get_data()[:]
        xbe.sections[new_name] = XbeSection(new_name, hdr, data)
        exe_to_xbe_section_map[name] = hdr.virtual_addr

    # Verify sections don't overlap
    for a, b in itertools.combinations(xbe.sections.values(), 2):
        a_start = a.header.virtual_addr
        a_end = a_start + a.header.virtual_size
        b_start = b.header.virtual_addr
        b_end = b_start + b.header.virtual_size
        if not (b_start >= a_end or a_start >= b_end):
            log.error('Section "%s" and "%s" overlap!', a.name, b.name)
            exit(1)

    # Process XBE
    xboxkrnl_import_table_base_addr = 0
    kernel_export_name_to_ordinal = {n: o for o, n in XbeKernelImage.exports.items()}
    existing_kernel_import_ordinals = {kernel_export_name_to_ordinal[n] for n in xbe.kern_imports}

    for de in pe.DIRECTORY_ENTRY_IMPORT:
        image_name = de.dll.decode('ascii')
        if image_name not in ('xboxkrnl.exe', 'halo.xbe'):
            log.error('Unexpected import from image "%s"', image_name)
            exit(1)
        log.info('Processing imports from image "%s"', image_name)

        # Print out xboxkrnl.exe imports (patched at runtime)
        if image_name == 'xboxkrnl.exe':
            for i in de.imports:
                flags = ' ** New **' if i.ordinal not in existing_kernel_import_ordinals else ''
                log.info('EXE imports ordinal %d (%s) at %x%s', i.ordinal, XbeKernelImage.exports[i.ordinal], i.address, flags)
            xboxkrnl_import_table_base_addr = min(i.address for i in de.imports)

        # Patch XBE imports now
        elif image_name == 'halo.xbe':
            for i in de.imports:
                name = i.name.decode('ascii')
                if name not in kb.name_to_addr:
                    log.error('Where is "%s" in the original XBE?', name)
                    exit(1)
                addr_of_original_function_in_xbe = kb.name_to_addr[name]
                log.info('Patching EXE import of XBE symbol "%s" at %x with %x', name, i.address, addr_of_original_function_in_xbe)
                write_to_vaddr(xbe, i.address, struct.pack('<I', addr_of_original_function_in_xbe))

    # Gather EXE exports
    log.debug('All EXE exports:')
    export_name_to_addr = {}
    for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        name = exp.name.decode('ascii')
        addr = base_addr + exp.address
        log.debug('- %s @ %x', name, addr)
        export_name_to_addr[name] = addr

    # Patch special exports
    special_exports = {
        'exe_base': base_addr,
        'exe_import_table': xboxkrnl_import_table_base_addr,
        'original_xbe_entry': xbe.entry_addr
    }
    for n, addr in special_exports.items():
        if n in export_name_to_addr:
            log.info('Patching EXE exported data "%s" at %x with %x', n, export_name_to_addr[n], addr)
            write_to_vaddr(xbe, export_name_to_addr[n], struct.pack('<I', addr))

    # Override the XBE entry point with our new handler
    entry_addr = pe.OPTIONAL_HEADER.AddressOfEntryPoint + base_addr
    log.info('Patching XBE with EXE entry point of %x', entry_addr)
    xbe.header.entry_addr = entry_addr ^ (Xbe.ENTRY_DEBUG if xbe.is_debug else Xbe.ENTRY_RETAIL)
    special_exports['_start'] = entry_addr

    # Hook all functions in the XBE that have been re-implemented
    patch_functions = [n for n in export_name_to_addr if n not in special_exports]
    for n in patch_functions:
        if n not in kb.name_to_addr:
            log.error('Where is "%s" in the original XBE?', n)
            exit(1)
        addr_of_original_in_xbe = kb.name_to_addr[n]
        addr_of_reimplementation = export_name_to_addr[n]
        log.info('Patching "%s" at %x in XBE with redirect to re-implementation at %x', n, addr_of_original_in_xbe, addr_of_reimplementation)
        patch_bytes = b'\x68' + struct.pack('<I', addr_of_reimplementation) + b'\xc3'  # push addr, ret
        write_to_vaddr(xbe, addr_of_original_in_xbe, patch_bytes)

    log.info('Generating patched XBE (%s)...', output_xbe_path)
    with open(output_xbe_path, 'wb') as f:
        f.write(xbe.pack())

    # Generate new ISO image
    log.info('Generating new ISO (%s)...', output_iso_path)
    if os.path.exists(output_iso_path):
        os.unlink(output_iso_path)
    subprocess.check_call(['extract-xiso', '-c', output_dir])

    # Generate GDB init script
    log.info('Generating GDB init script')
    with open('.gdbinit', 'w') as f:
        f.write('set arch i386\n')
        f.write('add-symbol-file src/main.exe ' + hex(exe_to_xbe_section_map['.text']) + ' ' +
                ' '.join(f'-s {n} {addr:#x}' for n, addr in exe_to_xbe_section_map.items() if
                         n != '.text' and not n.startswith('/')) + '\n')
        f.write('layout src\n')
        f.write('target remote 127.0.0.1:1234\n')
        f.write('set disassembly-flavor intel\n')
        # FIXME: Add other symbols

    log.info('Build complete')
    log.info('- Patched XBE: %s', output_xbe_path)
    log.info('- Patched ISO: %s', output_iso_path)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO, handlers=[color.ColorLogHandler()])
    main()
