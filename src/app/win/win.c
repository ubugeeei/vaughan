#include "../syscall.h"

void HariMain(void) {
	int win;
	char *buf = alloca(150 * 50);
	win = open(buf, 150, 50, -1, "hello");
	for (;;) {
		if (get_key(1) == 0x0a) { break; }
	}
	api_end();
}
