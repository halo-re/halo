void input_flush(void)
{
  csmemset(byte_46BA4C, 0, 0xA0u);
  csmemset(byte_46BB38, 0, 0x68u);
  csmemset(byte_46BBA0, 0, 0x68u);
  word_46BC08 = 0;
  word_46BC0A = 0;
  csmemset(dword_46BC0C, 0, 0x100u);
}
