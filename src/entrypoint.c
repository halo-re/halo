int _fltused = 1;

#define EXPORT __declspec(dllexport)

// 'Special' exports that will be patched during build process
EXPORT void *exe_base;
EXPORT uint32_t *exe_import_table;
EXPORT void *original_xbe_entry;

static const uint32_t kernel_image_base = 0x80010000;
static void init_kernel_imports(void);

__attribute__((no_stack_protector)) void _start(void)
{
  init_kernel_imports();
  ((void (*)())original_xbe_entry)();
}

static void init_kernel_imports(void)
{
  if (!exe_import_table) {
    return;
  }

  // FIXME: Be smarter about resolving imports
  // int import_table_offset = 0x2e8; // release
  int import_table_offset = 0x328; // debug
  uint32_t *kernel_export_table =
    (uint32_t *)(kernel_image_base + import_table_offset);
  for (int i = 0;; i++) {
    uint32_t ordinal = exe_import_table[i] & 0xffff;
    if (ordinal == 0) {
      break;
    }
    exe_import_table[i] = kernel_image_base + kernel_export_table[ordinal - 1];
  }
}
