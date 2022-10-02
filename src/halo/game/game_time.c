bool game_time_initialized(void)
{
  return game_time_globals->initialized;
}

void game_time_initialize(void)
{
  game_time_globals =
    game_state_malloc("game time globals", NULL, sizeof(game_time_globals_t));
  csmemset(game_time_globals, 0, sizeof(game_time_globals_t));
}

void game_time_initialize_for_new_map(void)
{
  assert_halt(game_time_globals || !game_time_globals->initialized);
  csmemset(game_time_globals, 0, sizeof(game_time_globals_t));
  game_time_globals->initialized = true;
}

void game_time_dispose_from_old_map(void)
{
  if (game_time_globals)
  {
    game_time_globals->initialized = false;
    game_time_globals->active = false;
  }
}

void game_time_dispose(void)
{

}

void game_time_end(void)
{
  assert_halt(game_time_globals);
  game_time_globals->active = false;
}

int game_time_get(void)
{
  assert_halt(game_time_globals && game_time_globals->initialized);
  return game_time_globals->time;
}

__int16 game_time_get_elapsed(void)
{
  assert_halt(game_time_globals && game_time_globals->initialized);
  return game_time_globals->elapsed;
}

int local_time_get(void)
{
  assert_halt(game_time_globals && game_time_globals->initialized);
  return game_time_globals->time;
}

__int16 local_time_get_elapsed(void)
{
  assert_halt(game_time_globals && game_time_globals->initialized);
  return game_time_globals->elapsed;
}

bool game_predicting(void)
{
  assert_halt(game_time_globals && game_time_globals->initialized);
  return false;
}

bool game_in_progress(void)
{
  assert_halt(game_time_globals);
  if (game_time_globals->initialized) {
    if (game_time_globals->active) {
      return true;
    }
    if (game_time_globals->paused) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool game_time_get_paused(void)
{
  assert_halt(game_time_globals);
  return game_time_globals->paused;
}

void game_time_set_paused(bool paused)
{
  assert_halt(game_time_globals);

  if (game_time_globals->initialized)
  {
    game_time_globals->active = !paused;
  }
  game_time_globals->paused = paused;
}

float game_time_get_speed(void)
{
  assert_halt(game_time_globals);
  return game_time_globals->speed;
}

void game_time_set_speed(float speed)
{
  assert_halt(game_time_globals);
  game_time_globals->speed = speed;
}

void game_time_statistics_new(void)
{
  byte_457069 = 1;
  byte_457068 = 0;
}

// TODO: void game_time_statistics_frame(int16_t, int16_t, int16_t);

void game_time_start(void)
{
  assert_halt(game_time_globals && game_time_globals->initialized);
  assert_halt(!game_time_globals->active);
  assert_halt(game_time_globals);
  game_time_globals->speed = 1.0;
  game_time_globals->leftover_dt = 0;
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

// TODO: void game_time_update(float);