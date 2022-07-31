void ui_widget_game_data_function_invoke(
  void *widget, unsigned __int16 game_data_input_reference_function)
{
  assert_halt(widget);

  if (game_data_input_reference_function > 40u) {
    error(2, "invalid game_data_input_reference_function");
  } else {
    ui_widget_game_data_function_table[game_data_input_reference_function](
      widget);
  }
}

void ui_widget_game_data_build_version(int widget)
{
  wchar_t *v1, *v2; // eax

  if (!ui_widget_game_data_build_version_wide_str[0]) {
    ascii_to_wide(
#if DECOMP_CUSTOM
      build_ui_widget_text,
#else
      "01.10.12.2276",
#endif
      ui_widget_game_data_build_version_wide_str,
      sizeof(ui_widget_game_data_build_version_wide_str));
  }

  if (!*(uint32_t *)(widget + 60)) {
    v1 =
      ui_widget_realloc(0, sizeof(ui_widget_game_data_build_version_wide_str),
                        __FILE__, __LINE__);
    *(uint32_t *)(widget + 60) = (uint32_t)v1;
    if (v1) {
      csmemset(v1, 0, sizeof(ui_widget_game_data_build_version_wide_str));
    }
  }
  v2 = *(wchar_t **)(widget + 60);
  if (v2) {
    ustrncpy(v2, ui_widget_game_data_build_version_wide_str, 0x3Fu);
    *(wchar_t *)(*(uint32_t *)(widget + 60) + 126) = 0;
  }
}
