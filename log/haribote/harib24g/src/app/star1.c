#include "apilib.h"

void HariMain(void) {
    char *buf;
    int win;
    init_malloc();
    buf = malloc(150 * 100);
    win = open(buf, 150, 100, -1, "star1");
    box_fill_window(win, 6, 26, 143, 93, 0);
    draw_point_window(win, 75, 59, 3);
    api_end();
}
