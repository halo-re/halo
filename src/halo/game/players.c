void players_initialize(void)
{
  player_data = game_state_data_new("players", 16, sizeof(player_data_t));
  team_data = game_state_data_new("teams", 16, sizeof(team_data_t));
  players_globals = (players_globals_t *)game_state_malloc(
    "players globals", NULL, sizeof(players_globals_t));
  csmemset(&players_globals->local_players[0], -1, sizeof(datum_index_t) * MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
  players_globals->unk_0 = -1;
  players_globals->local_player_count = 0;
  player_control_globals = (player_control_globals_t *)game_state_malloc(
    "player control globals", 0, sizeof(player_control_globals_t));
}

void players_initialize_for_new_map(void)
{
  player_control_dispose();
  csmemset(players_globals, 0,  sizeof(players_globals_t));
  csmemset(&players_globals->local_players[0], -1, sizeof(datum_index_t) * MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
  csmemset(&players_globals->local_dead_players[0], -1, sizeof(datum_index_t) * MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
  players_globals->unk_0 = -1;
  players_globals->input_disabled = false;
  players_globals->double_speed_ticks_remaining = 0;
  players_globals->are_all_dead = false;
  players_globals->unk_tag_index = -1;
  players_globals->respawn_failure = 0;
  data_make_valid(player_data);
  data_make_valid(team_data);
  csmemset(&unk_46B668, -1, 16 * MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
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

void player_delete(datum_index_t player)
{
  datum_delete(player_data, player);
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
    player_index = players_globals->local_players[local_player_index].handle;

  return player_index;
}

void local_player_set_player_index(int16_t local_player_index, datum_index_t index)
{
  assert_halt(local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

  if (players_globals->local_players[local_player_index].handle != -1) {
    player_data_t* player = datum_get(player_data, players_globals->local_players[local_player_index]);
    player->local_player_index = -1;
  }

  players_globals->local_players[local_player_index] = index;
  if (index.handle != -1)
  {
    player_data_t* player = datum_get(player_data, players_globals->local_players[local_player_index]);
    player->local_player_index = local_player_index;
  }
}

int16_t local_player_count(void)
{
  return players_globals->local_player_count;
}

datum_index_t local_player_get_next(datum_index_t local_player)
{
  datum_index_t next_player = INVALID_DATUM_HANDLE;
  datum_index_t *player = &players_globals->local_players[0];

  for (int i=0; i<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; i++) {
    if (player->handle != -1 && i > local_player.index && (i < next_player.index || next_player.index == -1)) {
        next_player.handle = i;
    }
    player++;
  }

  return next_player;
}

// TODO: player_index_from_unit_index
// TODO: player_died

bool players_are_all_dead(void)
{
  return players_globals->are_all_dead;
}

// TODO: players_set_local_player_unit

void* players_get_combined_pvs_local(void)
{
  return players_globals->combined_pvs_local;
}

void* players_get_combined_pvs(void)
{
  return players_globals->combined_pvs;
}

void player_input_enable(bool enabled)
{
  players_globals->input_disabled = enabled == 0;
}

bool player_input_enabled(void)
{
  return players_globals->input_disabled == 0;
}

// TODO: any_player_is_in_the_air

bool any_player_is_dead(void)
{
  player_data_t* player;
  data_iter_t iter;

  data_iterator_new(&iter, player_data);
  player = data_iterator_next(&iter);

  if (!player)
    return false;

  while (player->slave_unit_index.handle != -1)
  {
    player = data_iterator_next(&iter);
    if ( !player )
      return false;
  }

  return true;
}

// TODO: is_player_in_trigger
// TODO: player_pseudo_kill
// TODO: player_control_fix_for_loaded_game_state
// TODO: player_get_starting_location_count
// TODO: player_get_starting_location
// TODO: placement_data_set_change_color
// TODO: create_weapon
// TODO: players_compute_combined_pvs

int players_compute_local_player_count()
{
  int count = 0;

  for (int i=0; i<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; i++) {
    if (players_globals->local_players[i].handle != -1)
      count++;
  }

  return count;
}

void player_examine_nearby_unit()
{

}

// TODO: unit_should_autopick_weapon
// TODO: player_reset_action_result (eax = player datum index)
// TODO: player_handle_weapon_swap
// TODO: player_over_shield_screen_effect
// TODO: player_active_camo_screen_effect
// TODO: player_health_pack_screen_effect
// TODO: player_powerup_on
// TODO: player_powerup_additional
// TODO: player_powerup_off
// TODO: players_handle_deleted_object
// TODO: direction3d random_direction3d(int seed);
// TODO: valid_real_vector2d
// TODO: player_new
// TODO: player_add_equipment
// TODO: player_aiming_vector_from_facing
// TODO: player_teleport_internal
// TODO: player_teleport
// TODO: find_best_starting_location_index
// TODO: player_spawn
// TODO: player_set_action_result
// TODO: player_handle_action
// TODO: player_handle_powerup
// TODO: player_handle_powerup_minor
// TODO: player_update_powerups
// TODO: players_debug_render
// TODO: debug_player_teleport
// TODO: players_respawn_coop
// TODO: player_teleport_on_bsp_switch
// TODO: players_reconnect_to_structure_bsp
// TODO: player_examine_nearby_vehicle
// TODO: player_examine_nearby_device
// TODO: player_handle_powerup_equipment
// TODO: players_update_before_game
// TODO: player_examine_nearby_item
// TODO: player_examine_nearby_objects
// TODO: players_update_after_game
