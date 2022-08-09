int _fltused = 1;

#define EXPORT __declspec(dllexport)

// 'Special' exports that will be patched during build process
EXPORT void *exe_base;
EXPORT uint32_t *exe_import_table;
EXPORT void *original_xbe_entry;

static const uint32_t kernel_image_base = 0x80010000;
static void init_kernel_imports(void);

#ifndef MSVC
__attribute__((no_stack_protector))
#endif
void _start(void)
{
  init_kernel_imports();
  ((void (*)())original_xbe_entry)();
}

static void init_kernel_imports(void)
{
  if (!exe_import_table) {
    return;
  }

  uint32_t pe_offset = *(uint32_t *)(kernel_image_base + 0x3c);
  uint32_t export_dir_offset = *(uint32_t *)(kernel_image_base + pe_offset + 0x78);
  uint32_t export_table_offset = *(uint32_t *)(kernel_image_base + export_dir_offset + 0x1c);
  uint32_t *kernel_export_table = (uint32_t *)(kernel_image_base + export_table_offset);

  for (int i = 0;; i++) {
    uint32_t ordinal = exe_import_table[i] & 0xffff;
    if (ordinal == 0) {
      break;
    }
    exe_import_table[i] = kernel_image_base + kernel_export_table[ordinal - 1];
  }
}
