void game_time_initialize(void)
{
    game_time_globals = game_state_malloc("game time globals", 0, sizeof(game_time_globals_t));
    csmemset(game_time_globals, 0, sizeof(game_time_globals_t));
}
