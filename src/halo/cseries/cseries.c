void display_assert(const char *reason, const char *filepath, int lineno,
                    bool halt)
{
  if (halt) {
    stack_walk(0);
  }
  error(2, "EXCEPTION %s in %s,#%d: %s", halt ? "halt" : "warn", filepath,
        lineno, reason ? reason : "<no reason given>");
}

size_t __cdecl csstrlen(const char *s1)
{
  assert_halt(s1);

  size_t size = 0;

  while (s1[size]) {
        ++size;
  }

  assert_halt(size >= 0 && size < MAXIMUM_STRING_SIZE);

  return size;
}
