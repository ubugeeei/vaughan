void io_hlt(void);
void write_mem8(int addr, int data);

void HariMain(void)
{
	int i;

	for (i = 0xa0000; i <= 0xaffff; i++) { // メモリアドレス 0xa0000から0xaffffまで15で埋める
		write_mem8(i, i & 0x0f);
	}

	fin:
		io_hlt();
		goto fin;
}