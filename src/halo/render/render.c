void render_frame_pregame(pregame_render_info_t *pregame_info, void *main_globals_movie)
{
  pregame_render_info_t *pregame_info2; // ebx
  window_parameters_t window_parameters; // [esp+Ch] [ebp-260h] BYREF
  float v4[2]; // [esp+264h] [ebp-8h] BYREF
  float v5; // [esp+274h] [ebp+8h] FORCED BYREF

  ++render;
  rasterizer_frame_begin(v4);
  rasterizer_windows_begin();
  profile_render_window_start(0);
  csmemset(&window_parameters, 0, 0x258u);
  pregame_info2 = pregame_info;
  qmemcpy(&unknown_global_camera, &pregame_info->cam0,
          sizeof(unknown_global_camera));
  render_camera_build_frustum(&unknown_global_camera, 0, global_frustum, 1);
  qmemcpy(&window_parameters.camera, &pregame_info2->cam1,
          sizeof(window_parameters.camera));
  render_camera_build_frustum(&window_parameters.camera, 0,
                              window_parameters.frustum, 1);
  window_parameters.unk_0[0] = 0;
  rasterizer_window_begin(&window_parameters);
  render_ui_widgets(0, &pregame_info2->cam1.viewport_bounds);
  bink_playback_render();
  if (game_map_loading_in_progress(&v5)) {
    progress_bar_display(v5);
  }
  rasterizer_window_end();
  profile_render_window_end();
  rasterizer_windows_end();
  rasterizer_frame_end();
}
