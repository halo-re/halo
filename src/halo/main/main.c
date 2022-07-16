#ifdef DECOMP_CUSTOM
static void print_startup_banner(void)
{
    error(2, "DECOMP BUILD %s (%s)", build_rev, build_date);
    error(2, "--------------------------------------------------------------");
}
#endif

// FIXME: Add debug macros

void main_loop(void)
{
    bool v0; // cc
    char v1; // bl
    bool v2; // bl
    bool v3; // al
    float a2; // [esp+4h] [ebp-14h]
    float a2a; // [esp+4h] [ebp-14h]
    float a2b; // [esp+4h] [ebp-14h]
    float a2_4; // [esp+8h] [ebp-10h]
    float a2_4a; // [esp+8h] [ebp-10h]
    char v9[4]; // [esp+10h] [ebp-8h] BYREF
    int v10; // [esp+14h] [ebp-4h]

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
        if (game_in_editor()) {
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
        } else {
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
            if (main_won_map_private_pending)
                main_won_map_private();
            if (byte_46DA3C) {
                if (!(unsigned __int8)game_time_end() &&
                    !cinematic_in_progress()) {
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
            if (main_change_map_name_pending)
                main_change_map_name();
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
            if (main_menu_load_pending)
                main_menu_load();
            if (main_load_last_solo_map_pending)
                main_load_last_solo_map();
            if (xbox_demos_launch_pending) {
                xbox_demos_launch_pending = 0;
                xbox_demos_launch();
            }
            if (main_skip_private_pending)
                main_skip_private();
            if (byte_46DA50) {
                if (cache_files_precache_in_progress() &&
                    (unsigned __int16)cache_files_precache_map_status(
                        (float *)v9) == 1)
                    cache_files_precache_map_end();
                if (!cache_files_precache_in_progress()) {
                    cache_files_precache_map_begin(&byte_46DC55, 0);
                    byte_46DA50 = 0;
                }
            }
        }
        profile_frame_start();
        input_frame_begin();
        input_update();
        input_abstraction_update();
        shell_idle();
        event_manager_update();
        telnet_console_process();
        if (!shell_application_is_paused())
            break;
    LABEL_100:
        input_frame_end();
        profile_frame_end();
        main_frame_rate_debug();
        if (byte_46DA47) {
            byte_46DA47 = 0;
            dword_46D9E0 = system_milliseconds();
            qword_46D9E8 = qword_325678;
            byte_46DA46 = 1;
        }
    }
    v1 = 1;
    if (word_46DA0C == 1) {
        if (!network_game_client_start_frame()) {
            display_error_when_main_menu_loaded(6);
            error(2, "the game host went down");
            network_game_abort();
        }
        goto LABEL_63;
    }
    if (word_46DA0C == 2) {
        if (!network_game_client_start_frame() ||
            !network_game_server_start_frame()) {
            display_error_when_main_menu_loaded(1);
            error(2, "the game host went down");
            network_game_abort();
        }
    LABEL_63:
        main_update_time();
        process_ui_widgets();
        bink_playback_update();
        if (!game_in_editor() &&
                (input_key_is_down(0x55u) || input_key_is_down(0)) ||
            editor_should_exit()) {
            if (dword_46DA10) {
                bitmap_delete(dword_46DA10);
                dword_46DA10 = 0;
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
                v10 = (unsigned __int8)byte_46DA46;
                a2 = (double)(unsigned __int8)byte_46DA46 * flt_46DA08;
                player_control_update(a2);
                if (word_46DA0C > 0 && word_46DA0C <= 2 &&
                    !network_game_client_end_frame()) {
                    display_error_when_main_menu_loaded(1);
                    network_game_abort();
                }
                v10 = (unsigned __int8)byte_46DA46;
                a2a = (double)(unsigned __int8)byte_46DA46 * flt_46DA08;
                game_time_update(a2a);
                v2 = byte_46DA42 ||
                     byte_46DA46 && ((unsigned __int8)game_time_end() ||
                                     game_time_get_elapsed() > 0 ||
                                     game_time_get_speed() < 1.0);
                v3 = !game_engine_running() || game_time_get() >= 3;
                v1 = v3 && v2;
                collision_log_continue_period(1);
                v10 = (unsigned __int8)byte_46DA46;
                a2b = (double)(unsigned __int8)byte_46DA46 * flt_46DA08;
                director_update(a2b);
                v10 = (unsigned __int8)byte_46DA46;
                a2_4 = (double)(unsigned __int8)byte_46DA46 * flt_46DA08;
                observer_update(a2_4);
                collision_log_end_period();
                v10 = (unsigned __int8)byte_46DA46;
                a2_4a = (double)(unsigned __int8)byte_46DA46 * flt_46DA08;
                game_engine_update_non_deterministic(a2_4a);
            }
            if (byte_46DA28)
                main_save_map_private();
            if (!v1 || debug_no_drawing)
                goto LABEL_97;
            profile_render_start();
            main_game_render(flt_46DA08);
        } else {
            profile_render_start();
            main_pregame_render();
        }
        profile_render_end();
    LABEL_97:
        main_rasterizer_throttle();
        if (v1 && !debug_no_drawing)
            main_present_frame();
        goto LABEL_100;
    }
    if (word_46DA0C != 3)
        goto LABEL_63;
    error(2, "end of saved film");
    if (word_46DA0C == 1) {
        dispose_global_network_game_server();
    } else if (word_46DA0C == 2) {
        dispose_global_network_game_server();
        dispose_global_network_game_client();
    }
    game_dispose_from_old_map();
    game_dispose();
    debug_keys_dispose();
    console_dispose();
}

void main_setup_connection(void)
{
    game_options_t game_options; // [esp+0h] [ebp-10Ch] BYREF

    if (byte_46DA45) {
        main_menu_load_pending = 0;
        word_46DA0C = 3;
        error(2, "error opening saved film");
        main_menu_load_pending = 1;
        main_menu_load();
    } else if (main_menu_load_pending) {
        main_menu_load();
    } else {
        word_46DA0C = 0;
        game_options_new(&game_options);
        csstrncpy(game_options.map_name, map_name,
                  sizeof(game_options.map_name) - 1);
        game_options.map_name[sizeof(game_options.map_name) - 1] = 0;
        game_options.difficulty = global_difficulty_level;
        game_precache_new_map(game_options.map_name, 1);
        game_dispose_from_old_map();
        main_new_map(&game_options);
    }
}
