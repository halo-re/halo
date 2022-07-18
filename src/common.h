//
// This header is included by default in all source files.
//
#ifndef COMMON_H
#define COMMON_H

#define DECOMP_CUSTOM 1 // Logic that is added to aid decompilation, etc
#define DEBUG_BUILD 1 // Logic that appears only in debug builds

#define static_assert(cond) _Static_assert(cond, #cond)
#define __noreturn __attribute__((noreturn))

extern const char *build_rev;
extern const char *build_date;
extern const char *build_ui_widget_text;

#include "types.h"
#include "decl_generated.h"

#define assert_halt(cond)                                    \
    do {                                                     \
        if (!(cond)) {                                       \
            display_assert(#cond, __FILE__, __LINE__, true); \
            system_exit(-1);                                 \
        }                                                    \
    } while (0)

#define assert_halt_msg(cond, msg)                         \
    do {                                                   \
        if (!(cond)) {                                     \
            display_assert(msg, __FILE__, __LINE__, true); \
            system_exit(-1);                               \
        }                                                  \
    } while (0)

#endif
