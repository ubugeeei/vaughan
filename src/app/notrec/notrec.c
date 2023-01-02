#include "../syscall.h"

void HariMain(void) {
	int win;
	char buf[150 * 70];
	win = create_window(buf, 150, 70, 255, "notrec");
	box_fill_window(win, 0, 50, 34, 69, 255);
	box_fill_window(win, 115, 50, 149, 69, 255);
	box_fill_window(win, 50, 30, 99, 49, 255);
	for (;;) {
		if (get_key(1) == 0x0a) { break; } // Enter to break
	}
	api_end();
}
