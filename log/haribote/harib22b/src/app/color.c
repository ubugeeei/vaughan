int open(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void init_malloc(void);
char *malloc(int size);
void refresh_window(int win, int x0, int y0, int x1, int y1);
void draw_line_window(int win, int x0, int y0, int x1, int y1, int col);
int get_key(int mode);
void api_end(void);

void HariMain(void) {
	char *buf;
	int win, x, y, r, g, b;
	init_malloc();
	buf = malloc(144 * 164);
	win = open(buf, 144, 164, -1, "color");
	for (y = 0; y < 128; y++) {
		for (x = 0; x < 128; x++) {
			r = x * 2;
			g = y * 2;
			b = 0;
			buf[(x + 8) + (y + 28) * 144] = 16 + (r / 43) + (g / 43) * 6 + (b / 43) * 36;
		}
	}
	refresh_window(win, 8, 28, 136, 156);
  get_key(1); // Wait for any keyboard input
  api_end();
}
