#ifdef MSVC
#ifdef strlen
#undef strlen
#endif

#define MSVC_INTRINSIC_DEFS
#include "inlines.h"
#endif
