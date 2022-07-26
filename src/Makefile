DEBUG = y

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
	-I"$(CURDIR)" \
	-include common.h

ifeq ($(DEBUG),y)
	CFLAGS += -Og -gdwarf-4
	LDFLAGS += -debug
else
	CFLAGS += -O2
endif

SRCS = \
	$(CURDIR)/entrypoint.c \
	$(CURDIR)/thunks.c \
	$(CURDIR)/build_info.c \
	$(CURDIR)/typechecks.c \
	$(shell find halo -name '*.c')

OBJS = $(SRCS:.c=.obj)
DEPFILES = $(addsuffix .d, $(SRCS))
IMPORTLIBS = xboxkrnl.exe.lib halo.xbe.lib
GENERATED_FILES = main.exe $(OBJS) $(DEPFILES) $(IMPORTLIBS) typechecks.c build_info.c

ifneq ("$(wildcard private/Makefile.inc)","")
	-include private/Makefile.inc
endif

main.exe: $(OBJS) $(IMPORTLIBS)
	$(LD) $(LDFLAGS) -out:'$@' $^

.PHONY: $(CURDIR)/build_info.c
$(CURDIR)/build_info.c:
	echo "#define BUILD_REV \"$(shell git rev-parse HEAD 2>/dev/null | tr -d '\n' | cut -c-7)\"" > $@
	echo "#define BUILD_DATE \"$(shell date)\"" >> $@
	echo "const char *build_rev = BUILD_REV;" >> $@
	echo "const char *build_date = BUILD_DATE;" >> $@
	echo "const char *build_ui_widget_text = \"HDP \" BUILD_REV;" >> $@

%.lib: %.def
	lld-link /def:$< /out:$@ /machine:x86

%.obj: %.c
	$(CC) $(CFLAGS) -MD -MP -MT '$@' -MF '$(patsubst %.obj,%.c.d,$@)' -c -o '$@' '$<'

$(CURDIR)/typechecks.c: $(CURDIR)/types.h
	python ../misc/gen-struct-checks.py $< > $@

.PHONY: clean
clean:
	rm -f $(GENERATED_FILES)

-include $(DEPFILES)
