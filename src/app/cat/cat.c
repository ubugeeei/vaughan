#include "../../include/stdio.h"

void HariMain(void) {
	int fh;
	char c, cmdline_buf[30], *p;

	cmdline(cmdline_buf, 30);
	for (p = cmdline_buf; *p > ' '; p++) { } // Skip until space
	for (; *p == ' '; p++) { } // Skip space
	fh = fopen(p);
	if (fh != 0) {
		for (;;) {
			if (fread(&c, 1, fh) == 0) { break; }
			putchar(c);
		}
	} else {
		puts("File not found.\n");
	}
	exit();
}
