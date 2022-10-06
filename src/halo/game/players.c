void players_initialize(void)
{
  player_data = game_state_data_new("players", 16, sizeof(player_data_t));
  team_data = game_state_data_new("teams", 16, sizeof(team_data_t));
  players_globals = (players_globals_t *)game_state_malloc(
    "players globals", NULL, sizeof(players_globals_t));
  csmemset(&players_globals->local_player_indices[0], -1, 0x10u);
  players_globals->unk_0 = -1;
  players_globals->local_player_count = 0;
  player_control_globals = (player_control_globals_t *)game_state_malloc(
    "player control globals", 0, sizeof(player_control_globals_t));
}

void players_initialize_for_new_map(void)
{
  player_control_dispose();
  csmemset(players_globals, 0,  sizeof(players_globals_t));
  csmemset(&players_globals->local_player_indices[0], -1, 0x10u);
  csmemset(&players_globals->local_dead_player_indices[0], -1, 0x10u);
  players_globals->unk_0 = -1;
  players_globals->input_disabled = false;
  players_globals->double_speed_ticks_remaining = 0;
  players_globals->are_all_dead = false;
  players_globals->unk_tag_index = -1;
  players_globals->respawn_failure = 0;
  data_make_valid(player_data);
  data_make_valid(team_data);
  csmemset(&unk_46B668, -1, 64);
}
