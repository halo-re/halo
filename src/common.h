//
// This header is included by default in all source files.
//
#ifndef COMMON_H
#define COMMON_H

#ifdef MSVC
#pragma runtime_checks("scu", off)
#endif

#define DECOMP_CUSTOM 1 // Logic that is added to aid decompilation, etc
#define DEBUG_BUILD 1 // Logic that appears only in debug builds

extern const char *build_rev;
extern const char *build_date;
extern const char *build_ui_widget_text;

#include "types.h"
#include "inlines.h"
#include "decl.h"

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

#define CLAMP(x, low, high) \
  ((x) < (low) ? (low) : ((x) > (high) ? (high) : (x)))

#define MAXIMUM_GAMEPADS 4
#define MAXIMUM_NUMBER_OF_LOCAL_PLAYERS 4
#define MAXIMUM_STRING_SIZE 0x2000

static const int _scenario_type_main_menu = 2;

#ifdef DEBUG_BUILD
#define strlen csstrlen
#endif

#endif
