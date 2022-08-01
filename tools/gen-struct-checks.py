import argparse
import re

import clang.cindex as clang

def main():
	ap = argparse.ArgumentParser()
	ap.add_argument('infile')
	args = ap.parse_args()

	s = open(args.infile).read()
	index = clang.Index.create()
	tu = index.parse('tmp.h', unsaved_files=[('tmp.h', s)], options=0)
	size_matcher = re.compile(r'size\s*=\s*(\w+)')
	offset_matcher = re.compile(r'offset\s*=\s*(\w+)')

	for c in tu.cursor.get_children():
		if c.kind != clang.CursorKind.TYPEDEF_DECL:
			continue
		s = c.underlying_typedef_type
		if not s.spelling.startswith('struct'):
			continue
		if c.brief_comment:
			size = size_matcher.search(c.brief_comment)
			if size:
				print(f'cs({c.spelling}, {int(size.group(1), 0):#x});')
		for f in s.get_fields():
			if f.brief_comment:
				offset = offset_matcher.search(f.brief_comment)
				if offset:
					print(f'co({c.spelling}, {f.spelling}, {int(offset.group(1), 0):#x});')

if __name__ == '__main__':
	main()
