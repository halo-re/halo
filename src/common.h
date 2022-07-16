//
// This header is included by default in all source files.
//
#ifndef COMMON_H
#define COMMON_H

#define DECOMP_CUSTOM 1 // Logic that is added to aid decompilation, etc
#define DEBUG_BUILD 1 // Logic that appears only in debug builds

#include <stdint.h>
typedef unsigned int size_t;
typedef unsigned char bool;

extern const char *build_rev;
extern const char *build_date;
extern const char *build_ui_widget_text;

#include "decl_generated.h"

#endif
