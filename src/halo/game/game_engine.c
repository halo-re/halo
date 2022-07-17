void game_initial_pulse()
{
  data_iter_t iter; // [esp+0h] [ebp-10h] BYREF

  data_iterator_new(&iter, player_data);
  while ( data_iterator_next(&iter) )
    game_engine_player_added(iter.datum_handle);
  game_engine_game_starting();
}
