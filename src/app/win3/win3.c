#include "../../apilib.h"

void HariMain(void) {
    char *buf;
    int win;

    init_malloc();
    buf = malloc(150 * 50);
    win = open(buf, 150, 50, -1, "hello");
    box_fill_window(win, 8, 36, 141, 43, 6);
    putstr_window(win, 28, 28, 0, 12, "hello, world");
    api_end();
}
