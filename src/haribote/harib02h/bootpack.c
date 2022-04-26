// assembly function
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
extern void sprintf(char *str, char *fmt, ...);

// prototypes
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
								 int pysize, int px0, int py0, char *buf, int bxsize);

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15
// custom
#define COL8_BLACK2 16
#define COL8_BLACK3 17

struct BOOTINFO
{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *)0x0ff0;
	extern char hankaku[4096];
	char s[40], mcursor[256];
	int mx, my;

	init_palette();
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "Hello, Ubugeeei!");
	putfonts8_asc(binfo->vram, binfo->scrnx, 8, 32, COL8_FFFFFF, "This is my first OS implementation.");

	putfonts8_asc(binfo->vram, binfo->scrnx, 80, 96, COL8_BLACK2, "Effective Font Test.");
	putfonts8_asc(binfo->vram, binfo->scrnx, 79, 95, COL8_BLACK3, "Effective Font Test.");
	putfonts8_asc(binfo->vram, binfo->scrnx, 78, 94, COL8_FFFFFF, "Effective Font Test.");

	sprintf(s, "scrnx = %d, scrny = %d", binfo->scrnx, binfo->scrny);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);

	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 32);
	init_mouse_cursor8(mcursor, COL8_BLACK2);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

	for (;;)
	{
		io_hlt();
	}
}

void init_palette(void)
{
	static unsigned char table_rgb[18 * 3] = {
			0x00, 0x00, 0x00, //  0:black
			0xff, 0x00, 0x00, //  1:red
			0x00, 0xff, 0x00, //  2:green
			0xff, 0xff, 0x00, //  3:yellow
			0x00, 0x00, 0xff, //  4:blue
			0xff, 0x00, 0xff, //  5:purple
			0x00, 0xff, 0xff, //  6:water blue
			0xff, 0xff, 0xff, //  7:white
			0xc6, 0xc6, 0xc6, //  8:gray
			0x84, 0x00, 0x00, //  9:dark red
			0x00, 0x84, 0x00, // 10:dark green
			0x84, 0x84, 0x00, // 11:dark yellow
			0x00, 0x00, 0x84, // 12:dark blue
			0x84, 0x00, 0x84, // 13:dark purple
			0x00, 0x84, 0x84, // 14:dark water blue
			0x84, 0x84, 0x48, // 15:dark gray
			// custom color
			0x26, 0x29, 0x2D, // 16: black 2
			0x1F, 0x20, 0x23 // 16: black 3
	};
	set_palette(0, 17, table_rgb);
	return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();
	io_cli();
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++)
	{
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++)
	{
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

void init_screen(char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_BLACK2, 0, 0, x - 1, y - 29);
	boxfill8(vram, x, COL8_000000, 0, y - 28, x - 1, y - 28);
	boxfill8(vram, x, COL8_BLACK3, 0, y - 27, x - 1, y - 27);
	boxfill8(vram, x, COL8_BLACK3, 0, y - 26, x - 1, y - 1);

	boxfill8(vram, x, COL8_BLACK2, 3, y - 24, 59, y - 24);
	boxfill8(vram, x, COL8_BLACK2, 2, y - 24, 2, y - 4);
	boxfill8(vram, x, COL8_BLACK2, 3, y - 4, 59, y - 4);
	boxfill8(vram, x, COL8_BLACK2, 59, y - 23, 59, y - 5);
	boxfill8(vram, x, COL8_BLACK2, 2, y - 3, 59, y - 3);
	boxfill8(vram, x, COL8_BLACK2, 60, y - 24, 60, y - 3);

	boxfill8(vram, x, COL8_BLACK2, x - 47, y - 24, x - 4, y - 24);
	boxfill8(vram, x, COL8_BLACK2, x - 47, y - 23, x - 47, y - 4);
	boxfill8(vram, x, COL8_BLACK2, x - 47, y - 3, x - 4, y - 3);
	boxfill8(vram, x, COL8_BLACK2, x - 3, y - 24, x - 3, y - 3);
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++)
	{
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0)
		{
			p[0] = c;
		}
		if ((d & 0x40) != 0)
		{
			p[1] = c;
		}
		if ((d & 0x20) != 0)
		{
			p[2] = c;
		}
		if ((d & 0x10) != 0)
		{
			p[3] = c;
		}
		if ((d & 0x08) != 0)
		{
			p[4] = c;
		}
		if ((d & 0x04) != 0)
		{
			p[5] = c;
		}
		if ((d & 0x02) != 0)
		{
			p[6] = c;
		}
		if ((d & 0x01) != 0)
		{
			p[7] = c;
		}
	}
	return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	for (; *s != 0x00; s++)
	{
		putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
	}
	return;
}

void init_mouse_cursor8(char *mouse, char bc)
{
	static char cursor[16][16] = {
			"**************..",
			"*OOOOOOOOOOO*...",
			"*OOOOOOOOOO*....",
			"*OOOOOOOOO*.....",
			"*OOOOOOOO*......",
			"*OOOOOOO*.......",
			"*OOOOOOO*.......",
			"*OOOOOOOO*......",
			"*OOOO**OOO*.....",
			"*OOO*..*OOO*....",
			"*OO*....*OOO*...",
			"*O*......*OOO*..",
			"**........*OOO*.",
			"*..........*OOO*",
			"............*OO*",
			".............***"};
	int x, y;

	for (y = 0; y < 16; y++)
	{
		for (x = 0; x < 16; x++)
		{
			if (cursor[y][x] == '*')
			{
				mouse[y * 16 + x] = COL8_BLACK2;
			}
			if (cursor[y][x] == 'O')
			{
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.')
			{
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize,
								 int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++)
	{
		for (x = 0; x < pxsize; x++)
		{
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}
