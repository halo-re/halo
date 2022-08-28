short game_connection(void)
{
  return word_46DA0C;
}

static const short _game_connection_local = 0;

void create_local_players(void)
{
  int i;
  int j;
  int player;
  int16_t gamepad_index;
  int16_t assigned_controllers[4];
  int16_t desired_controllers[4];
  int16_t default_controllers[4];

  if (main_globals.main_menu_scenario_loaded) {
    local_player_set_player_index(0, player_new(0, -1, 0, 0));
    return;
  }

  csmemset(assigned_controllers, -1, sizeof(assigned_controllers));
  csmemset(desired_controllers, -1, sizeof(desired_controllers));
  default_controllers[0] = 0;
  default_controllers[1] = 1;
  default_controllers[2] = 2;
  default_controllers[3] = 3;

  assert_halt(game_connection() == _game_connection_local);

  for (i = 0; i < player_spawn_count; i++) {
    gamepad_index = player_ui_get_single_player_local_player_controller(i);
    desired_controllers[i] = gamepad_index;
    if (gamepad_index == -1) {
      desired_controllers[i] = default_controllers[i];
    }

    assert_halt((desired_controllers[i] >= 0) &&
                (desired_controllers[i] < MAXIMUM_GAMEPADS));

    gamepad_index = desired_controllers[i];
    if (assigned_controllers[gamepad_index] != -1) {
      for (j = 0; j < MAXIMUM_GAMEPADS; j++) {
        if (assigned_controllers[j] == -1) {
          desired_controllers[i] = j;
          assigned_controllers[(int16_t)j] = j;
          break;
        }
      }
      assert_halt(j < MAXIMUM_GAMEPADS);
    } else {
      assigned_controllers[gamepad_index] = gamepad_index;
    }
  }

  qsort(desired_controllers, 4, 2, sort_desired_local_player_controllers);

  for (i = 0; i < player_spawn_count; i++) {
    gamepad_index = desired_controllers[i];
    assert_halt((gamepad_index >= 0) && (gamepad_index < MAXIMUM_GAMEPADS));
    player = player_new(0, -1, gamepad_index, 0);
    local_player_set_player_index(gamepad_index, player);
  }
}

void main_menu_precache_resources(void)
{
  scenario_t *scenario = global_scenario_get();
  if (scenario) {
    assert_halt(scenario->type == _scenario_type_main_menu);
    predicted_resources_precache(&scenario->unk_236);
  }
}

void main_menu_load(void)
{
  if (!main_globals.main_menu_scenario_loaded) {
    main_load_ui_scenario(0);
  }
  main_screen_shell_load();
  main_menu_precache_resources();
  update_server_delete();
  update_server_new();
  update_server_start();
  game_time_dispose_from_old_map();
  game_time_initialize_for_new_map();
  game_time_start();
  hs_runtime_dispose_from_old_map();
  hs_runtime_initialize_for_new_map();
  main_menu_load_pending = false;
}

void main_pregame_render(void)
{
  vector3_t unk[3];

  collision_log_continue_period(1);
  sound_render();

  unk[2].x = 0;
  unk[2].y = 0;
  unk[2].z = 0;
  pregame_render_info.cam1.unk_0 = unk[2];

  unk[1].x = 0;
  unk[1].y = 0;
  unk[1].z = 1.0;
  pregame_render_info.cam1.unk_12 = unk[1];

  pregame_render_info.unk_0 = -1;
  pregame_render_info.unk_2 = 1;

  unk[0].x = 0;
  unk[0].y = 1.0;
  unk[0].z = 0;
  pregame_render_info.cam1.unk_24 = unk[0];

  pregame_render_info.cam1.unk_36 = 0;
  pregame_render_info.cam1.vertical_field_of_view =
    2 *
    atan2(render_camera_get_adjusted_field_of_view_tangent(1.3962634) * 0.75,
          1.0);
  compute_window_bounds(0, 1, &pregame_render_info.cam1.viewport_bounds,
                        &pregame_render_info.cam1.unk_52);
  pregame_render_info.cam1.z_near = 0.0099999998;
  pregame_render_info.cam1.z_far = 1.0;
  qmemcpy(&pregame_render_info.cam0, &pregame_render_info.cam1,
          sizeof(pregame_render_info.cam0));
  render_frame_pregame(&pregame_render_info, main_globals_movie);
  collision_log_end_period();
}

void main_present_frame(void)
{
  const char *err_msg;
  char path[512];
  file_ref_t file_ref;

  render_frame_present(0, main_globals_movie);

  if (global_screenshot_count <= 0 && main_globals_movie) {
    snprintf(path, sizeof(path), "movie\\frame%06d.tga", movie_frame_count++);
    file_reference_create_from_path(&file_ref, path, 0);
    err_msg = tiff_export(&file_ref, main_globals_movie);
    if (err_msg) {
      error(2, err_msg);
    }
  }
}

void main_setup_connection(void)
{
  game_options_t game_options;

  if (byte_46DA45) {
    main_menu_load_pending = 0;
    word_46DA0C = 3;
    error(2, "error opening saved film");
    main_menu_load_pending = 1;
  }

  if (main_menu_load_pending) {
    main_menu_load();
    return;
  }

  word_46DA0C = 0;
  game_options_new(&game_options);
  csstrncpy(game_options.map_name, map_name, sizeof(game_options.map_name) - 1);
  game_options.map_name[sizeof(game_options.map_name) - 1] = 0;
  game_options.difficulty = global_difficulty_level;
  game_precache_new_map(game_options.map_name, 1);
  game_dispose_from_old_map();
  main_new_map(&game_options);
}

void main_initialize_time(void)
{
// FIXME: d3d_find_flipcount checks handler address, so we cannot
//        provide function reference here until we re-implement it.
#define main_vertical_blank_interrupt_handler (void *)0x101CD0

  unk_time_globals.unk_0 = system_milliseconds();
  unk_time_globals.unk_8 = 0L;
  rasterizer_set_vblank_callback(main_vertical_blank_interrupt_handler);
  word_46DDDC = 0;
  csmemset(word_46DDDE, 0, 0x1Eu);
  flip_count_ptr = d3d_find_flipcount();
}

void main_game_render(double a2)
{
  bool force_single_screen;
  int player_index;
  window_t *current_window;
  void *camera;
  int num_players;
  int num_screens;
  __int16 next_player;

  lock_global_random_seed();
  collision_log_continue_period(1);
  sound_render();

  force_single_screen = game_engine_force_single_screen();
  next_player = -1;
  num_screens = CLAMP(local_player_count(), 1, 4);
  num_players = num_screens;

  if (force_single_screen || cinematic_in_progress()) {
    num_screens = 1;
    num_players = 1;
  }

  for (player_index = 0; player_index < num_players; player_index++) {
    current_window = &window[player_index];
    camera = NULL;

    compute_window_bounds(player_index, num_players, &current_window->unk_132,
                          &current_window->unk_140);

    if (!force_single_screen && player_index < num_screens) {
      if (!byte_325714 || next_player == -1) {
        if (word_46DA0C == 3) {
          next_player = 0;
        } else {
          next_player = local_player_get_next(next_player);
        }
      }
      current_window->player = next_player;
      camera = observer_get_camera(next_player);
    } else {
      current_window->player = -1;
    }

    set_window_camera_values(current_window, camera);
    current_window->unk_2 = 0;
  }

  current_window = &window[num_players];
  compute_window_bounds(0, 1, &current_window->unk_132,
                        &current_window->unk_140);
  current_window->player = -1;
  current_window->unk_2 = 1;
  set_window_camera_values(current_window, 0);

  if (global_screenshot_count <= 0) {
    render_frame(window, num_players + 1, 0, 0, main_globals_movie, a2);
  } else {
    screenshot_render(window);
  }
  collision_log_end_period();
  unlock_global_random_seed();
}

#ifdef DECOMP_CUSTOM
static void print_startup_banner(void)
{
  error(2, "DECOMP BUILD %s (%s)", build_rev, build_date);
  error(2, "--------------------------------------------------------------");
}
#endif

static __inline void abort_with_error_message(int16_t message_id)
{
  display_error_when_main_menu_loaded(message_id);
  error(2, "the game host went down");
  network_game_abort();
}

void main_loop(void)
{
  bool v0; // cc
  bool v1; // bl
  float a2; // [esp+4h] [ebp-14h]
  float a2a; // [esp+4h] [ebp-14h]
  float a2b; // [esp+4h] [ebp-14h]
  float a2_4; // [esp+8h] [ebp-10h]
  float a2_4a; // [esp+8h] [ebp-10h]
  char v9[4]; // [esp+10h] [ebp-8h] BYREF
  int x;

  if (!game_in_editor()) {
    csstrncpy(map_name, "levels\\b30\\b30", 0xFFu);
    byte_46DB54 = 0;
  }
  main_menu_load_pending = game_in_editor() == 0;
  word_46DA40 = -1;
  byte_46DA46 = 1;
  console_initialize();
  debug_keys_initialize();
  game_initialize();
  console_startup();

#ifdef DECOMP_CUSTOM
  print_startup_banner();
#endif

  main_setup_connection();
  main_initialize_time();
  while (1) {
    if (!game_in_editor()) {
      if (word_46DA40 != -1) {
        scenario_switch_structure_bsp(word_46DA40);
        word_46DA40 = -1;
        hud_load(0);
      }
      if (byte_46DA3B) {
        if (!(unsigned __int8)game_time_end()) {
          v0 = word_46DA4C++ <= 90;
          if (!v0) {
            byte_46DA3B = 0;
            word_46DA4C = 0;
            game_state_revert();
          }
        }
      }
      if (main_won_map_private_pending) {
        main_won_map_private();
      }
      if (byte_46DA3C) {
        if (!(unsigned __int8)game_time_end() && !cinematic_in_progress()) {
          v0 = word_46DA4E++ <= 90;
          if (!v0) {
            if (players_respawn_coop()) {
              byte_46DA3C = 0;
              word_46DA4E = 0;
            }
          }
        }
      }
      if (game_state_save_pending) {
        game_state_save();
        hud_autosave(0);
        game_state_save_pending = 0;
      }
      if (main_change_map_name_pending) {
        main_change_map_name();
      }
      if (game_state_revert_pending) {
        game_state_revert();
        ui_widgets_disable_pause_game(30);
        game_state_revert_pending = 0;
      }
      if (should_skip_cinematic) {
        if (cinematic_can_be_skipped()) {
          game_state_revert();
          ui_widgets_disable_pause_game(30);
          game_state_revert_pending = 0;
        }
        should_skip_cinematic = 0;
      }
      if (game_reset_pending && !(unsigned __int8)game_time_end()) {
        scenario_switch_structure_bsp(0);
        game_dispose_from_old_map();
        input_flush();
        game_initialize_for_new_map();
        create_local_players();
        game_time_start();
        game_initial_pulse();
        ui_widgets_disable_pause_game(30);
        game_reset_pending = 0;
      }
      if (game_state_save_core_pending) {
        game_state_save_core(core_name);
        game_state_save_core_pending = 0;
      }
      if (game_state_load_core_pending) {
        game_state_load_core(core_name);
        game_state_load_core_pending = 0;
      }
      if (main_menu_load_pending) {
        main_menu_load();
      }
      if (main_load_last_solo_map_pending) {
        main_load_last_solo_map();
      }
      if (xbox_demos_launch_pending) {
        xbox_demos_launch_pending = 0;
        xbox_demos_launch();
      }
      if (main_skip_private_pending) {
        main_skip_private();
      }
      if (byte_46DA50) {
        if (cache_files_precache_in_progress() &&
            (unsigned __int16)cache_files_precache_map_status((float *)v9) ==
              1) {
          cache_files_precache_map_end();
        }
        if (!cache_files_precache_in_progress()) {
          cache_files_precache_map_begin(&byte_46DC55, 0);
          byte_46DA50 = 0;
        }
      }
    } else {
      if (game_reset_pending && !(unsigned __int8)game_time_end()) {
        scenario_switch_structure_bsp(0);
        game_dispose_from_old_map();
        input_flush();
        game_initialize_for_new_map();
        create_local_players();
        game_time_start();
        game_initial_pulse();
        ui_widgets_disable_pause_game(30);
        game_reset_pending = 0;
      }
    }
    profile_frame_start();
    input_frame_begin();
    input_update();
    input_abstraction_update();
    shell_idle();
    event_manager_update();
    telnet_console_process();
    if (!shell_application_is_paused()) {
      v1 = 1;
      x = word_46DA0C;
      if (x == 1) {
        if (!network_game_client_start_frame()) {
          abort_with_error_message(6);
        }
      } else if (x == 2) {
        if (!network_game_client_start_frame()) {
          abort_with_error_message(1);
        } else if (!network_game_server_start_frame()) {
          abort_with_error_message(1);
        }
      } else if (x == 3) {
        break;
      }
      main_update_time();
      process_ui_widgets();
      bink_playback_update();
      if ((!game_in_editor() &&
           (input_key_is_down(0x55u) || input_key_is_down(0))) ||
          editor_should_exit()) {
        if (main_globals_movie) {
          bitmap_delete(main_globals_movie);
          main_globals_movie = 0;
        }
        if (!game_engine_running()) {
          word_46DA40 = -1;
          byte_46DA28 = 0;
          game_reset_pending = 1;
          byte_46DA3B = 0;
        }
      }
      if (game_in_progress()) {
        terminal_update();
        if (!console_update() || word_46DA0C) {
          debug_keys_update();
          cheats_update();
          a2 = (double)(unsigned __int8)byte_46DA46;
          a2 *= flt_46DA08;
          player_control_update(a2);
          x = word_46DA0C;
          if (x > 0 && x <= 2 && !network_game_client_end_frame()) {
            display_error_when_main_menu_loaded(1);
            network_game_abort();
          }
          a2a = (double)(unsigned __int8)byte_46DA46;
          a2a *= flt_46DA08;
          game_time_update(a2a);
          v1 = main_globals.main_menu_scenario_loaded ||
               (byte_46DA46 &&
                ((unsigned __int8)game_time_end() ||
                 game_time_get_elapsed() > 0 || game_time_get_speed() < 1.0));

          v1 &= !game_engine_running() || game_time_get() >= 3;

          collision_log_continue_period(1);
          a2b = (double)(unsigned __int8)byte_46DA46;
          a2b *= flt_46DA08;
          director_update(a2b);
          a2_4 = (double)(unsigned __int8)byte_46DA46;
          a2_4 *= flt_46DA08;
          observer_update(a2_4);
          collision_log_end_period();
          a2_4a = (double)(unsigned __int8)byte_46DA46;
          a2_4a *= flt_46DA08;
          game_engine_update_non_deterministic(a2_4a);
        }
        if (byte_46DA28) {
          main_save_map_private();
        }
        if (v1 && !debug_no_drawing) {
          profile_render_start();
          main_game_render(flt_46DA08);
          profile_render_end();
        }
      } else {
        profile_render_start();
        main_pregame_render();
        profile_render_end();
      }
      main_rasterizer_throttle();
      if (v1 && !debug_no_drawing) {
        main_present_frame();
      }
    }
    input_frame_end();
    profile_frame_end();
    main_frame_rate_debug();
    if (byte_46DA47) {
      byte_46DA47 = 0;
      unk_time_globals.unk_0 = system_milliseconds();
      unk_time_globals.unk_8 = qword_325678;
      byte_46DA46 = 1;
    }
  }
  error(2, "end of saved film");
  x = word_46DA0C;
  switch (x) {
  case 2:
    dispose_global_network_game_server();
    dispose_global_network_game_client();
    break;
  case 1:
    dispose_global_network_game_server();
    break;
  }
  game_dispose_from_old_map();
  game_dispose();
  debug_keys_dispose();
  console_dispose();
}
