data_t *game_state_data_new(char *name, __int16 maximum_count, __int16 size)
{
  data_t *data; // esi
  int allocation_size; // [esp-Ch] [ebp-18h]

  allocation_size = data_allocation_size(maximum_count, size);
  data = (data_t *)game_state_malloc(name, "data array", allocation_size);
  data_initialize(data, name, maximum_count, size);
  return data;
}
