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

void players_dispose_from_old_map(void)
{
  data_make_invalid(player_data);
  data_make_invalid(team_data);
}

void players_dispose(void)
{
  if (player_data)
    player_data = NULL;

  if (team_data)
    team_data = NULL;

  if (players_globals)
    players_globals = NULL;
}

void *machine_get_player_list(int16_t player_index)
{
  return (char *)&unk_46B668 + 16 * player_index;
}

// TODO: int __usercall machine_add_player@<eax>(unsigned __int16 a1@<ax>, int a2)

bool local_player_exists(int16_t player_index)
{
  player_data_t* player;
  data_iter_t iter;

  data_iterator_new(&iter, player_data);
  player = data_iterator_next(&iter);
  if (!player)
    return false;

  while (player->local_player_index != player_index)
  {
    player = data_iterator_next(&iter);
    if (!player)
      return false;
  }
  return true;
}

int find_unused_local_player_index()
{
  int v0; // edi
  int v1; // esi
  int v2; // esi

  v0 = -1;
  v1 = 0;
  while (!input_has_gamepad(v1) || local_player_exists(v1))
  {
    if (++v1 >= 4)
      goto LABEL_7;
  }
  v0 = v1;
  if (v1 != -1)
    return v0;
  LABEL_7:
  v2 = 0;
  while (local_player_exists(v2))
  {
    if (++v2 >= 4)
      return v0;
  }
  return v2;
}

void player_delete(int player_datum)
{
  datum_delete(player_data, player_datum);
}

int16_t players_get_respawn_failure(void)
{
  return players_globals->respawn_failure;
}

int local_player_get_player_index(int16_t local_player_index)
{
  int player_index;

  assert_halt(local_player_index>=NONE && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

  if (local_player_index == NONE)
    player_index = NONE;
  else
    player_index = players_globals->local_player_indices[local_player_index];

  return player_index;
}
