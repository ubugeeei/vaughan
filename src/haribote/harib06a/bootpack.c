#include "bootpack.h"

void HariMain(void) {
	struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];
	int mx, my, i;
	struct MOUSE_DEC mdec;

	init_gdtidt();
	init_pic();
	io_sti();
	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);

	init_keyboard();

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, BLACK2);

	sprintf(s, "Mouse:");
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	sprintf(s, "Cursor(x, y) = (%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 16, COL8_FFFFFF, s);

	sprintf(s, "Click = \"None\"");
	boxfill8(binfo->vram, binfo->scrnx, BLACK2, 16, 32, 320, 48);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 32, COL8_FFFFFF, s);

	sprintf(s, "KeyBoard:");
	boxfill8(binfo->vram, binfo->scrnx, BLACK2, 0, 64, 128, 80);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 64, COL8_FFFFFF, s);

	sprintf(s, "Code = \"%x\"", i);
	boxfill8(binfo->vram, binfo->scrnx, BLACK2, 16, 80, 144, 96);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 80, COL8_FFFFFF, s);

	enable_mouse(&mdec);
	for (;;) {
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {
			if (fifo8_status(&keyfifo) != 0) {
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "Code = \"%x\"", i);
				boxfill8(binfo->vram, binfo->scrnx, BLACK2, 16,
					 80, 144, 96);
				putfonts8_asc(binfo->vram, binfo->scrnx, 16, 80,
					      COL8_FFFFFF, s);
			} else if (fifo8_status(&mousefifo) != 0) {
				i = fifo8_get(&mousefifo);
				io_sti();
				if (mouse_decode(&mdec, i) != 0) {
					sprintf(s, "Click = \"None\"");
					if ((mdec.btn & 0x01) != 0) {
						sprintf(s, "Click = \"Left\"");
					}
					if ((mdec.btn & 0x02) != 0) {
						sprintf(s, "Click = \"Right\"");
					}
					if ((mdec.btn & 0x04) != 0) {
						sprintf(s,
							"Click = \"Center\"");
					}
					boxfill8(binfo->vram, binfo->scrnx,
						 BLACK2, 16, 32, 320, 48);
					putfonts8_asc(binfo->vram, binfo->scrnx,
						      16, 32, COL8_FFFFFF, s);

					boxfill8(binfo->vram, binfo->scrnx,
						 BLACK2, mx, my, mx + 15,
						 my + 15);
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (mx > binfo->scrnx - 16) {
						mx = binfo->scrnx - 16;
					}
					if (my > binfo->scrny - 16) {
						my = binfo->scrny - 16;
					}
					sprintf(s, "Cursor(x, y) = (%d, %d)",
						mx, my);
					boxfill8(binfo->vram, binfo->scrnx,
						 BLACK2, 16, 16, 216, 32);
					putfonts8_asc(binfo->vram, binfo->scrnx,
						      16, 16, COL8_FFFFFF, s);
					putblock8_8(binfo->vram, binfo->scrnx,
						    16, 16, mx, my, mcursor,
						    16);
				}
			}
		}
	}
}

