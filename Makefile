DEBUG = y

INPUT_DIR = halo-patched
INPUT_XBE = $(INPUT_DIR)/cachebeta.xbe

OUTPUT_DIR = $(INPUT_DIR)
OUTPUT_XBE = $(OUTPUT_DIR)/default.xbe
OUTPUT_ISO = halo-patched.iso

REIMPLEMENTATION_EXE = main.exe

LD = lld
LDFLAGS = \
	-flavor link \
	-subsystem:windows \
	-fixed:no \
	-base:0x10000 \
	-entry:_start

CC = clang
CFLAGS = \
	-Wall \
	-Werror \
	-target i386-pc-win32 \
	-march=pentium3 \
	-nostdlib \
	-ffreestanding \
	-fno-builtin \
	-fno-exceptions \
	-I"$(CURDIR)/src" \
	-include src/common.h

ifeq ($(DEBUG),y)
	CFLAGS += -Og -gdwarf-4
	LDFLAGS += -debug
else
	CFLAGS += -O2
endif

SRCS = \
	src/entrypoint.c \
	src/thunks.c \
	src/build_info.c \
	src/typechecks.c \
	$(shell find src/halo -name '*.c')

OBJS = $(SRCS:.c=.obj)
DEPFILES = $(addsuffix .d, $(SRCS))
IMPORTLIBS = src/xboxkrnl.exe.lib src/halo.xbe.lib
GENERATED_FILES = \
	main.exe main.pdb main.lib $(OBJS) $(DEPFILES) $(IMPORTLIBS) \
	src/typechecks.c src/build_info.c src/decl.h src/halo.xbe.def src/thunks.c

$(OUTPUT_ISO): $(OUTPUT_XBE)
	extract-xiso -c $(shell dirname $(OUTPUT_XBE))

$(OUTPUT_XBE): $(INPUT_XBE) $(REIMPLEMENTATION_EXE)
	./tools/patch.py $(INPUT_XBE) $(REIMPLEMENTATION_EXE) $(OUTPUT_XBE)

$(REIMPLEMENTATION_EXE): $(OBJS) $(IMPORTLIBS)
	$(LD) $(LDFLAGS) -out:'$@' $^

src/halo.xbe.lib: src/halo.xbe.def

%.lib: %.def
	lld-link /def:$< /out:$@ /machine:x86

%.obj: %.c
	$(CC) $(CFLAGS) -MD -MP -MT '$@' -MF '$(patsubst %.obj,%.c.d,$@)' -c -o '$@' '$<'

.PHONY: src/build_info.c
src/build_info.c:
	echo "#define BUILD_REV \"$(shell git rev-parse HEAD 2>/dev/null | tr -d '\n' | cut -c-7)\"" > $@
	echo "#define BUILD_DATE \"$(shell date)\"" >> $@
	echo "const char *build_rev = BUILD_REV;" >> $@
	echo "const char *build_date = BUILD_DATE;" >> $@
	echo "const char *build_ui_widget_text = \"HDP \" BUILD_REV;" >> $@

src/typechecks.c: src/types.h
	python tools/gen-struct-checks.py $< > $@

$(OBJS): src/decl.h

src/decl.h src/halo.xbe.def src/thunks.c: kb.json
	python tools/knowledge.py \
		--gen-header src/decl.h \
		--gen-def src/halo.xbe.def \
		--gen-thunks src/thunks.c

.PHONY: clean
clean:
	rm -f $(GENERATED_FILES)

.PHONY: .FORCE

ifneq ("$(wildcard src/private/Makefile.inc)","")
	-include src/private/Makefile.inc
endif

-include $(DEPFILES)
