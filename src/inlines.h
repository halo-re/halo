static inline double atan2(double y, double x)
{
  double r = 0;
  asm volatile ("fpatan" : "=t"(r) : "u"(y), "0"(x) : "st(1)");
  return r;
}

static inline void *qmemcpy(void *s1, const void *s2, size_t n)
{
  char *dest = (char *)s1;
  const char *src = (const char *)s2;
  while (n--) {
    *dest++ = *src++;
  }
  return s1;
}
