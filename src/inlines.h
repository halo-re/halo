#define qmemcpy memcpy

#ifdef MSVC
  #ifdef MSVC_INTRINSIC_DEFS
    #define INCLUDE_PRAGMA_FUNCTIONS
    #define INCLUDE_DEFS
  #else
    #define INCLUDE_PRAGMA_INTRINSICS
    #define INCLUDE_DECLS
  #endif
  #define INLINE
  #define atan2 atan2_
#else
  #define INLINE static inline
  #define INCLUDE_DEFS
#endif


#ifdef INCLUDE_DECLS
double atan2(double y, double x);
void *memcpy(void *s1, const void *s2, size_t n);
size_t strlen(const char *s);
#undef INCLUDE_DECLS
#endif


#ifdef INCLUDE_PRAGMA_FUNCTIONS
// #pragma function(atan2)
#pragma function(memcpy)
#pragma function(strlen)
#undef INCLUDE_PRAGMA_FUNCTIONS
#endif


#ifdef INCLUDE_PRAGMA_INTRINSICS
// #pragma intrinsic(atan2)
#pragma intrinsic(memcpy)
#pragma intrinsic(strlen)
#undef INCLUDE_PRAGMA_INTRINSICS
#endif


#ifdef INCLUDE_DEFS

INLINE double atan2(double y, double x)
{
  double r = 0;
#ifdef MSVC
  __asm {
    fld y
    fld x
    fpatan
    fstp r
  }
#else
  asm volatile ("fpatan" : "=t"(r) : "u"(y), "0"(x) : "st(1)");
#endif
  return r;
}

INLINE void *memcpy(void *s1, const void *s2, size_t n)
{
  char *dest = (char *)s1;
  const char *src = (const char *)s2;
  while (n--) {
    *dest++ = *src++;
  }
  return s1;
}

INLINE size_t strlen(const char *s)
{
  size_t c = 0;
  while (*s++) {
    c++;
  }
  return c;
}

#undef INCLUDE_DEFS
#endif // INCLUDE_DEFS

#ifdef INLINE
#undef INLINE
#endif
