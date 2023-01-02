#include "../syscall.h"

void HariMain(void) {
	char *buf;
	int win, x, y, r, g, b;
	init_malloc();
	buf = malloc(144 * 164);
	win = create_window(buf, 144, 164, -1, "color");
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
