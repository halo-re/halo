void game_time_initialize(void)
{
    game_time_globals = game_state_malloc("game time globals", NULL, sizeof(game_time_globals_t));
    csmemset(game_time_globals, 0, sizeof(game_time_globals_t));
}

void game_time_start()
{
    assert_halt(game_time_globals && game_time_globals->initialized);
    assert_halt(!game_time_globals->active);
    assert_halt(game_time_globals);
    game_time_globals->speed = 1.0;
    game_time_globals->unk_28 = 0;
    game_time_globals->active = 1;
    byte_457069 = 1;
    byte_457068 = 0;
    switch (game_connection()) {
    case 0:
    case 2:
        update_server_start();
        break;
    case 1:
    case 3:
        update_client_start();
        break;
    default:
        return;
    }
}
