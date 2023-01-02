#include "../syscall.h"

struct DLL_STR_PIC_ENV {	// 64KB
	int work[64 * 1024 / 4];
};

struct RGB {
	unsigned char b, g, r, t;
};

/* bmp.nasm */
int info_BMP(struct DLL_STR_PIC_ENV *env, int *info, int size, char *fp);
int decode0_BMP(struct DLL_STR_PIC_ENV *env, int size, char *fp, int b_type, char *buf, int skip);

/* jpeg.c */
int info_JPEG(struct DLL_STR_PIC_ENV *env, int *info, int size, char *fp);
int decode0_JPEG(struct DLL_STR_PIC_ENV *env, int size, char *fp, int b_type, char *buf, int skip);

unsigned char rgb2pal(int r, int g, int b, int x, int y);
void error(char *s);

void HariMain(void) {
	struct DLL_STR_PIC_ENV env;
	char filebuf[512 * 1024], win_buf[1040 * 805];
	char s[32], *p;
	int win, i, j, file_size, xsize, info[8];
	struct RGB pic_buf[1024 * 768], *q;

	// Parse command line
	cmdline(s, 30);
	for (p = s; *p > ' '; p++) { } // Skip until space
	for (; *p == ' '; p++) { } // Skip space

	// Read file
	i = fopen(p); if (i == 0) { error("file not found.\n"); }
	file_size = fsize(i, 0);
	if (file_size > 512 * 1024) {
		error("file too large.\n");
	}
	fread(filebuf, file_size, i);
	fclose(i);

	// Check file format
	if (info_BMP(&env, info, file_size, filebuf) == 0) {
		// BMP
		if (info_JPEG(&env, info, file_size, filebuf) == 0) {
			// JPEG
			puts("file type unknown.\n");
			api_end();
		}
	}

	if (info[2] > 1024 || info[3] > 768) {
		error("picture too large.\n");
	}

	// Prepare window
	xsize = info[2] + 16;
	if (xsize < 136) {
		xsize = 136;
	}
	win = create_window(win_buf, xsize, info[3] + 37, -1, "gview");

	// Convert to image data
	if (info[0] == 1) {
		i = decode0_BMP (&env, file_size, filebuf, 4, (char *) pic_buf, 0);
	} else {
		i = decode0_JPEG(&env, file_size, filebuf, 4, (char *) pic_buf, 0);
	}

	if (i != 0) {
		error("decode error.\n");
	}

	// Display
	for (i = 0; i < info[3]; i++) {
		p = win_buf + (i + 29) * xsize + (xsize - info[2]) / 2;
		q = pic_buf + i * info[2];
		for (j = 0; j < info[2]; j++) {
			p[j] = rgb2pal(q[j].r, q[j].g, q[j].b, j, i);
		}
	}
	refresh_window(win, (xsize - info[2]) / 2, 29, (xsize - info[2]) / 2 + info[2], 29 + info[3]);

	// Wait for quit
	for (;;) {
		i = getkey(1);
		if (i == 'Q' || i == 'q') {
			api_end();
		}
	}
}

unsigned char rgb2pal(int r, int g, int b, int x, int y) {
	static int table[4] = { 3, 1, 0, 2 };
	int i;
	x &= 1;
	y &= 1;
	i = table[x + y * 2];
	r = (r * 21) / 256;
	g = (g * 21) / 256;
	b = (b * 21) / 256;
	r = (r + i) / 4;
	g = (g + i) / 4;
	b = (b + i) / 4;
	return 16 + r + g * 6 + b * 36;
}

void error(char *s) {
	puts(s);
	api_end();
}
