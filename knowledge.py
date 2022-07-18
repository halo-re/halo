import json
from collections import defaultdict
from typing import Sequence, Union, Optional
import logging
import os.path

try:
	import clang.cindex as clang
except ImportError:
	clang = None

log = logging.getLogger(__name__)
root_dir = os.path.dirname(os.path.abspath(__file__))


def parse_string(s: str):
	s = '#include "src/common.h"\n' + s
	index = clang.Index.create()
	tu = index.parse('tmp.h', unsaved_files=[('tmp.h', s)], options=0)
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

	@classmethod
	def deserialize(cls, o):
		return Function(**o)

	def serialize(self):
		return {'decl': self.decl, 'addr': hex(self.addr)}


class Data(Symbol):
	def __repr__(self):
		return f'<Data "{self.decl}">'


class Function(Symbol):
	def __repr__(self):
		return f'<Function "{self.decl}">'


class KnowledgeBase:

	kb_path: str = os.path.join(root_dir, 'kb.json')

	def __init__(self):
		self.symbols = []
		self.object_to_symbols = defaultdict(list)
		self.symbol_to_object = {}
		self.object_to_source = {}
		self.name_to_addr = {}
		self.expected_md5 = 'c7869590a1c64ad034e49a5ee0c02465'
		self.addr_to_symbols = {}

	def add_symbols(self, symbols: Sequence[Symbol]):
		self.symbols.extend(symbols)

	def build_header(self):
		log.info('Generating header...')

		with open('src/decl_generated.h', 'w') as f:
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
						f.write(f'HFUNC {s.decl}\n')
				f.write('\n')

			f.write('\n'
					'#undef HFUNC\n'
					'#undef HDATA\n'
					'\n'
					'//\n'
					'// AUTOMATICALLY GENERATED. DO NOT EDIT.\n'
					'//\n')

	def build_def(self):
		log.info('Generating XBE export .def file...')

		with open('src/halo.xbe.def', 'w') as f:
			f.write('LIBRARY halo.xbe\n'
					'EXPORTS\n')
			for s in sorted(self.symbols, key=lambda s: (isinstance(s, Function), s.name)):
				f.write(f'\t{s.cursor.mangled_name}{" DATA" if isinstance(s, Data) else ""}\n')

	def serialize(self):
		log.info('Saving knowledge base to %s...', self.kb_path)

		with open(self.kb_path, 'w') as f:
			out = {'objects': []}

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

		if os.path.exists('misc/load_truth.py'):
			from misc.load_truth import load_truth
			load_truth(kb)

		kb.addr_to_symbol = {s.addr:s for s in kb.symbols}

		num_symbols_with_truth = len([s for s in kb.symbols if kb.symbol_to_object[s]])
		num_symbols_without_truth = len(kb.symbols) - num_symbols_with_truth

		log.info('%d symbols were identified with objects, %d were not', num_symbols_with_truth, num_symbols_without_truth)
		num_objs_with_source = len([s for o, s in kb.object_to_source.items() if o in kb.object_to_symbols])
		log.info('%d of %d object files have known source mapping', num_objs_with_source, len(kb.object_to_symbols))

		return kb


def main():
	logging.basicConfig(level=logging.INFO)
	kb = KnowledgeBase.deserialize()
	kb.serialize()


if __name__ == '__main__':
	main()
