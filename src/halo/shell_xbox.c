int main(int argc, const char **argv, const char **envp)
{
#if DEBUG_BUILD
  update_loaded_module_section_attributes();
#endif
  rasterizer_preinitialize__fill_you_up_with_the_devils_cock();
  physical_memory_allocate();
  if (shell_initialize()) {
    main_loop();
    shell_dispose();
  }
  return 0;
}
