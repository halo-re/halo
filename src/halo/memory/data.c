int data_allocation_size(__int16 count, __int16 size)
{
  return size * count + sizeof(data_t);
}

void data_iterator_new(data_iter_t *iter, data_t *data)
{
  data_verify(data);
  assert_halt(data->valid);
  iter->data = data;
  iter->cookie = (unsigned int)data ^ 0x69746572;
  iter->unk_4 = 0;
  iter->unk_8 = -1;
}
