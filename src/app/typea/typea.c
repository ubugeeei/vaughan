#include "../syscall.h"

void HariMain(void) {
	int fh;
	char c;
	fh = fopen("a.hrb");
	if (fh != 0) {
		for (;;) {
			if (fread(&c, 1, fh) == 0) {
				break;
			}
			putchar(c);
		}
	}
	api_end();
}
