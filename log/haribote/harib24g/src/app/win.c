#include "apilib.h"

char buf[150 * 50];

void HariMain(void) {
    int win;
    win = open(buf, 150, 50, -1, "hello");
    api_end();
}
