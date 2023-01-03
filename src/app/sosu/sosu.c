#include "../../include/stdio.h"

#define MAX 1000

void HariMain(void) {
	char flag[MAX], s[8];
	int i, j;
	for (i = 0; i < MAX; i++) {
		flag[i] = 0;
	}
	for (i = 2; i < MAX; i++) {
		if (flag[i] == 0) {
			sprintf(s, "%d ", i);
			puts(s);
			for (j = i * 2; j < MAX; j += i) {
				flag[j] = 1;
			}
		}
	}
	exit();
}
