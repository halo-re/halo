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

void game_precache_new_map(char *map_name, bool a2)
{
  __int16 map_status;

  if (cache_files_precache_map_loaded(map_name))
  {
  LABEL_22:
    if (a2)
    {
      assert_halt(cache_files_precache_map_loaded(map_name));
      main_save_current_solo_map(map_name);
      main_queue_map_name(NULL);
      
      if (cache_files_precache_in_progress())
        cache_files_precache_map_end();

      if (player_spawn_count == 1)
       player_ui_remember_player1_profile(true);
    }
    return;
  }

  if (cache_files_precache_in_progress() && !cache_files_precache_is_copying_map(map_name))
  {
    if (a2)
    {
      cache_files_precache_map_end();
    }
    else
    {
      cache_files_precache_map_queue_end();
      main_queue_map_name(map_name);
    }
  }

  if (!cache_files_precache_in_progress() && !cache_files_precache_map_begin(map_name, a2))
  {
    error(2, "shouldn't be here... map '%s' doesn't exist", map_name);
    if (a2)
    {
      display_assert(
        "read the last error message for which map failed to load",
        __FILE__, __LINE__, true);
      system_exit(-1);
    }
  }

  cache_files_precache_set_priority(a2);

  if (a2)
  {
    game_globals->map_loading = true;
    game_globals->map_load_progress = 0.0f;
    assert_halt(cache_files_precache_in_progress() && cache_files_precache_is_copying_map(map_name));
    ui_widget_load_progress_widget();
    progress_bar_begin(global_scenario_index != -1);

    do
    {
      map_status = cache_files_precache_map_status(&game_globals->map_load_progress);
      main_pregame_render();
      main_rasterizer_throttle();
      main_present_frame();
    }
    while (!map_status);

    progress_bar_end();
    ui_widgets_close_all();

    if (map_status == 2)
      display_error_damaged_media();

    cache_files_precache_map_end();
    assert_halt(cache_files_precache_map_loaded(map_name));

    game_globals->map_loading = false;
    game_globals->map_load_progress = 1.0f;

    goto LABEL_22;
  }
}

bool game_map_loading_in_progress(float *progress)
{
  if (progress) {
    *progress = game_globals->map_load_progress;
  }
  return game_globals->map_loading;
}

void game_unload(void)
{
  __int16 map_status;

  if (cache_files_precache_in_progress())
  {
    game_globals->map_loading = true;
    ui_widget_load_progress_widget();

    do
    {
      map_status = cache_files_precache_map_status(&game_globals->map_load_progress);
      main_pregame_render();
      main_rasterizer_throttle();
      main_present_frame();
    }
    while (!map_status);

    ui_widgets_close_all();
    if (map_status == 2)
      display_error_damaged_media();
    cache_files_precache_map_end();
  }
  if (game_globals->map_loaded)
  {
    scenario_unload();
    random_seed_debug_log(0);
    game_globals->map_loaded = false;
  }
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

void game_frame(float elapsed)
{
  if (game_globals->players_double_speed)
    elapsed *= 0.5f;

  assert_halt(game_globals->active);

  collision_log_begin_period(1);
  particles_update(elapsed);
  contrails_update(elapsed);
  particle_systems_update(elapsed);
  widgets_update(elapsed);
  game_sound_update(elapsed);
  scenario_frame_update(elapsed);
  rasterizer_frame_update(elapsed);
  numeric_countdown_timer_update();
  collision_log_end_period();
}

// TODO: void remove_quitting_players_from_game(void);

void game_tick(void)
{
  float seconds_per_tick;

  profile_tick_start();
  collision_log_begin_period(0);
  real_math_reset_precision();
  if (profile_global_enable && byte_2EF808)
    profile_enter_private(&off_2EF800);

  assert_halt(game_globals->active);

  remove_quitting_players_from_game();
  game_allegiance_update();
  units_update();
  ai_update();
  players_update_before_game();

  seconds_per_tick = 1 / 30.0f;
  if (game_globals->players_double_speed)
    seconds_per_tick = 1 / 60.0f;

  effects_update(seconds_per_tick);
  lock_global_random_seed();
  rumble_update();
  first_person_weapons_update();
  unlock_global_random_seed();
  game_engine_update();
  editor_update();
  hs_update();
  recorded_animations_update();
  objects_update();
  players_update_after_game();
  hud_update();
  player_effect_update();
  if ( profile_global_enable && byte_2EF808 )
    profile_exit_private(&off_2EF800);
  collision_log_end_period();
  profile_tick_end();
}

void game_options_new(game_options_t *options)
{
  csmemset(options, 0, sizeof(*options));
  options->unk_4 = 0;
  options->difficulty = 1;
  options->random_seed = 0xDEADBEEF;
}

static bool game_options_verify(game_options_t *options)
{
  return options->difficulty >= 0 && options->difficulty < 4;
}

void game_initial_pulse()
{
  data_iter_t iter; // [esp+0h] [ebp-10h] BYREF

  data_iterator_new(&iter, player_data);
  while (data_iterator_next(&iter))
    game_engine_player_added(iter.datum_handle);
  game_engine_game_starting();
}

void game_set_players_are_double_speed(bool is_double_speed)
{
  game_globals->players_double_speed = is_double_speed;
}

bool game_players_are_double_speed(void)
{
  return game_globals->players_double_speed;
}

void game_difficulty_level_set(int16_t difficulty)
{
  game_globals->game_options.difficulty = difficulty;
}

int16_t game_difficulty_level_get(void)
{
  return game_globals->game_options.difficulty;
}

int game_difficulty_level_get_ignore_easy(void)
{
  int16_t difficulty = game_globals->game_options.difficulty;

  if (difficulty <= 1)
    return 1;

  return difficulty;
}

void game_set_game_variant(game_variant_t *variant)
{
  if (!variant)
    csmemset(&game_variant_global, 0, sizeof(game_variant_t));
  else
    qmemcpy(&game_variant_global, variant, sizeof(game_variant_t));
}

void game_set_game_engine_index(void)
{
  display_assert("this is broken and should get updated for the variants, ask michael",
    __FILE__, __LINE__, true);
  system_exit(-1);
}

bool game_all_quiet(void)
{
  return !dangerous_projectiles_near_player() &&
  !dangerous_items_near_player() &&
  !dangerous_effects_near_player() &&
  !any_unit_is_dangerous() &&
  !ai_enemies_can_see_player();
}

bool game_safe_to_save(void)
{
  if (ai_enemies_can_see_player()) {
    if (debug_game_save) {
      console_warning("not safe to save: ai_enemies_can_see_player");
    }
    return false;
  }

  if (dangerous_projectiles_near_player()) {
    if (debug_game_save) {
      console_warning("not safe to save: dangerous_projectiles_near_player");
    }
    return false;
  }

  if (dangerous_items_near_player()) {
    if (debug_game_save) {
      console_warning("not safe to save: dangerous_items_near_player");
    }
    return false;
  }

  if (dangerous_effects_near_player()) {
    if (debug_game_save) {
      console_warning("not safe to save: dangerous_effects_near_player");
    }
    return false;
  }

  if (any_unit_is_dangerous()) {
    if (debug_game_save) {
      console_warning("not safe to save: any_unit_is_dangerous");
    }
    return false;
  }

  if (any_player_is_in_the_air()) {
    if (debug_game_save) {
      console_warning("not safe to save: any_player_is_in_the_air");
    }
    return false;
  }

  if (any_player_is_dead()) {
    if (debug_game_save) {
      console_warning("not safe to save: any_player_is_dead");
    }
    return false;
  }

  if (vehicle_moving_near_any_player()) {
    if (debug_game_save) {
      console_warning("not safe to save: vehicle_moving_near_any_player");
    }
    return false;
  }

  return true;
}

bool game_safe_to_speak(void)
{
  return !dangerous_projectiles_near_player() && !any_player_is_dead();
}

bool game_is_cooperative(void)
{
  return player_spawn_count > 1;
}

void set_random_seed(int seed)
{
  *((int*)get_global_random_seed_address()) = seed;
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

void game_set_game_variant_from_name(const char* name)
{
  game_variant_t variant;
  game_variant_t variant_copy;

  qmemcpy(&variant_copy, game_engine_get_variant_by_name(&variant, name), sizeof(game_variant_t));
  if (!&variant_copy)
  {
    csmemset(&game_variant_global, (char)&variant_copy, sizeof(game_variant_t));
    return;
  }

  qmemcpy(&game_variant_global, (const void*)&variant_copy, sizeof(game_variant_t));
}
