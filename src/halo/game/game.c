void game_initialize(void)
{
  game_globals = (game_globals_t *)game_state_malloc("game globals", 0,
                                                     sizeof(game_globals_t));
  csmemset(game_globals, 0, sizeof(game_globals_t));
  csmemset(&game_variant_global, 0, sizeof(game_variant_global));
  real_math_reset_precision();
  game_time_initialize();
  game_engine_initialize(&game_variant_global);
  game_allegiance_initialize();
  interface_initialize();
  scenario_initialize();
  director_initialize();
  observer_initialize();
  render_initialize();
  objects_initialize();
  structures_initialize();
  breakable_surfaces_initialize();
  decals_initialize();
  collision_log_initialize();
  players_initialize();
  contrails_initialize();
  particles_initialize();
  effects_initialize();
  weather_particle_systems_initialize();
  particle_systems_initialize();
  sound_classes_initialize();
  game_sound_initialize();
  rumble_initialize();
  player_effect_initialize();
  ai_initialize();
  editor_initialize();
  ui_widgets_initialize();
  hs_initialize();
  recorded_animations_initialize();
  cheats_initialize();
  transport_initialize();
  telnet_console_initialize();
  initialize_network_game_packets();
  cinematic_initialize();
  saved_game_files_initialize();
  event_manager_initialize();
  input_abstraction_initialize();
  player_ui_initialize();
  bink_playback_initialize();
  progress_bar_initialize();
}

void game_dispose(void)
{
  recorded_animations_dispose();
  cinematic_dispose();
  hs_dispose();
  cheats_dispose();
  ui_widgets_dispose();
  editor_dispose();
  ai_dispose();
  player_effect_dispose();
  rumble_dispose();
  game_sound_dispose();
  sound_classes_dispose();
  game_engine_dispose();
  particle_systems_dispose();
  weather_particle_systems_dispose();
  effects_dispose();
  particles_dispose();
  contrails_dispose();
  players_dispose();
  decals_dispose();
  breakable_surfaces_dispose();
  structures_dispose();
  render_dispose();
  objects_dispose();
  director_dispose();
  interface_dispose();
  game_allegiance_dispose();
  game_time_dispose();
  saved_game_files_dispose();
  event_manager_dispose();
  input_abstraction_dispose();
  player_ui_dispose();
  game_state_dispose();
  telnet_console_dispose();
  transport_dispose();
  bink_playback_dispose();
  progress_bar_dispose();
}

void game_dispose_from_old_map()
{
  rasterizer_dispose_from_old_map();
  game_state_dispose_from_old_map();
  cheats_dispose_from_old_map();
  recorded_animations_dispose_from_old_map();
  hs_dispose_from_old_map();
  cinematic_dispose_from_old_map();
  editor_dispose_from_old_map();
  ai_dispose_from_old_map();
  player_effect_dispose_from_old_map();
  rumble_dispose_from_old_map();
  point_physics_dispose_from_old_map();
  particle_systems_dispose_from_old_map();
  weather_particle_systems_dispose_from_old_map();
  decals_dispose_from_old_map();
  breakable_surfaces_dispose_from_old_map();
  structures_dispose_from_old_map();
  j__render_dispose_from_old_map();
  objects_dispose_from_old_map();
  director_dispose_from_old_map();
  observer_dispose_from_old_map();
  interface_dispose_from_old_map();
  players_dispose_from_old_map();
  contrails_dispose_from_old_map();
  particles_dispose_from_old_map();
  effects_dispose_from_old_map();
  game_sound_dispose_from_old_map();
  sound_classes_dispose_from_old_map();
  sound_dispose_from_old_map();
  game_allegiance_dispose_from_old_map();
  update_server_delete();
  game_engine_dispose_from_old_map();
  scenario_dispose_from_old_map();
  game_time_dispose_from_old_map();
  ui_widgets_close_all();
  ui_widgets_safe_to_load(0);
  game_globals->active = 0;
}

void game_options_new(game_options_t *options)
{
  csmemset(options, 0, sizeof(*options));
  options->unk_4 = 0;
  options->difficulty = 1;
  options->random_seed = 0xDEADBEEF;
}

void game_initial_pulse()
{
  data_iter_t iter; // [esp+0h] [ebp-10h] BYREF

  data_iterator_new(&iter, player_data);
  while (data_iterator_next(&iter))
    game_engine_player_added(iter.datum_handle);
  game_engine_game_starting();
}

static bool game_options_verify(game_options_t *options)
{
  return options->difficulty >= 0 && options->difficulty < 4;
}

bool game_load(game_options_t *options)
{
  game_globals_t *globals;
  bool loaded;

  assert_halt(!game_globals->active);
  assert_halt(!game_globals->map_loaded);
  assert_halt(game_options_verify(options));

  random_seed_debug_log(1);
  csmemcpy(&game_globals->game_options, options, sizeof(*options));

  loaded = scenario_load(options->map_name);
  globals = game_globals;

  if (loaded) {
    globals->map_loaded = true;
  }

  return globals->map_loaded;
}

void game_initialize_for_new_map(void)
{
  int random_seed;

  assert_halt(game_globals->map_loaded);
  assert_halt(!game_globals->active);

  random_seed = game_globals->game_options.random_seed;
  *get_global_random_seed_address() = random_seed;
  game_engine_dispose();
  game_engine_initialize(&game_variant_global);
  real_math_reset_precision();
  rasterizer_initialize_for_new_map();
  game_state_initialize_for_new_map();
  game_time_initialize_for_new_map();
  interface_initialize_for_new_map();
  game_allegiance_initialize_for_new_map();
  players_initialize_for_new_map();
  scenario_initialize_for_new_map();
  objects_initialize_for_new_map();
  render_initialize_for_new_map();
  structures_initialize_for_new_map();
  breakable_surfaces_initialize_for_new_map();
  decals_initialize_for_new_map();
  director_initialize_for_new_map();
  observer_initialize_for_new_map();
  contrails_initialize_for_new_map();
  particles_initialize_for_new_map();
  effects_initialize_for_new_map();
  particle_systems_initialize_for_new_map();
  sound_initialize_for_new_map();
  sound_classes_initialize_for_new_map();
  game_sound_initialize_for_new_map();
  weather_particle_systems_initialize_for_new_map();
  point_physics_initialize_for_new_map();
  game_engine_initialize_for_new_map();
  game_statistics_start();
  update_server_new();
  player_control_initialize_for_new_map();
  rumble_initialize_for_new_map();
  player_effect_initialize_for_new_map();
  ai_initialize_for_new_map();
  console_initialize_for_new_map();
  editor_initialize_for_new_map();
  cinematic_initialize_for_new_map();
  hs_initialize_for_new_map();
  recorded_animations_initialize_for_new_map();
  cheats_initialize_for_new_map();
  game_globals->active = 1;
  objects_place();
  if (!game_in_editor()) {
    ai_place();
  }
  ui_widgets_safe_to_load(1);
}
