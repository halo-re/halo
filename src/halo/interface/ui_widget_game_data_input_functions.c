void __cdecl ui_widget_game_data_build_version(int a2)
{
  void *v1; // eax
  int v2; // eax

  if ( !word_46CE68 ) {
#if DECOMP_CUSTOM
    ascii_to_wide(build_ui_widget_text, (int)&word_46CE68, 0x80u);
#else
    ascii_to_wide("01.10.12.2276", (int)&word_46CE68, 0x80u);
#endif
  }

  if ( !*(uint32_t *)(a2 + 60) )
  {
    v1 = (void *)ui_widget_realloc(
                   0,
                   0x80u,
                   (int)__FILE__,
                   606);
    *(uint32_t *)(a2 + 60) = (uint32_t)v1;
    if ( v1 )
      csmemset(v1, 0, 0x80u);
  }
  v2 = *(uint32_t *)(a2 + 60);
  if ( v2 )
  {
    ustrncpy(v2, (int)&word_46CE68, 0x3Fu);
    *(uint16_t *)(*(uint32_t *)(a2 + 60) + 126) = 0;
  }
}
