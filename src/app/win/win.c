#include "../../include/stdio.h"

void HariMain(void) {
	int win;
	char *buf = alloca(150 * 50);
	win = create_window(buf, 150, 50, -1, "hello");
	for (;;) {
		if (getkey(1) == 0x0a) { break; }
	}
	exit();
}
