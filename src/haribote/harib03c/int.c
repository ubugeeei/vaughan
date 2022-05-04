#include "bootpack.h"

void init_pic(void)
{
	io_out8(PIC0_IMR, 0xff); // no recieve interrupt
	io_out8(PIC1_IMR, 0xff); //no recieve interruptい

	io_out8(PIC0_ICW1, 0x11);
	io_out8(PIC0_ICW2, 0x20);

	io_out8(PIC0_ICW3, 0x01 << 0x02);
	io_out8(PIC0_ICW4, 0x01);

	io_out8(PIC1_ICW1, 0x11);
	io_out8(PIC1_ICW2, 0x28);

	io_out8(PIC1_ICW3, 0x02);
	io_out8(PIC1_ICW4, 0x01);

	io_out8(PIC0_IMR, 0xfb);
	io_out8(PIC1_IMR, 0xff);

	return;
}
