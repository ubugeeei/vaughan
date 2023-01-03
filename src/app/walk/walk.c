#include "../syscall.h"

void HariMain(void) {
    char *buf;
    int win, i, x, y;
    init_malloc();
    buf = malloc(160 * 100);
    win = create_window(buf, 160, 100, -1, "walk");
    box_fill_window(win, 4, 24, 155, 95, 0);
    x = 76;
    y = 56;
    putstr_window(win, x, y, 3, 1, "*");
    for (;;) {
        i = getkey(1);
        putstr_window(win, x, y, 0, 1, "*");
        if (i == '4' && x > 4) {
            x -= 8;
        }
        if (i == '6' && x < 148) {
            x += 8;
        }
        if (i == '8' && y > 24) {
            y -= 8;
        }
        if (i == '2' && y < 80) {
            y += 8;
        }
        if (i == 0x0a) {
            break;
        }
        putstr_window(win, x, y, 3, 1, "*");
    }
    close_window(win);
    exit();
}
