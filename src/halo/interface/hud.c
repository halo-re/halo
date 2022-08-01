void hud_load(bool a1)
{
  __int16 v1;
  __int16 player;
  wchar_t *message;

  if (a1) {
    v1 = *((_WORD *)hud_globals + 492);
  } else {
    v1 = *((_WORD *)hud_globals + 493);
  }
  scripted_hud_messages_clear();
  if (v1 != -1) {
    message = hud_get_item_string(v1);
    player = local_player_get_next(-1);
    hud_print_message(player, message);
  }
}
