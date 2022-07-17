int data_allocation_size(__int16 count, __int16 size)
{
  return size * count + sizeof(data_t);
}
