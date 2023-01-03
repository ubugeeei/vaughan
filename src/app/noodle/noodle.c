#include "../../include/stdio.h"

void HariMain(void) {
    char *buf, s[12];
    int win, timer, sec = 0, min = 0, hou = 0;
    init_malloc();
    buf = malloc(150 * 50);
    win = create_window(buf, 150, 50, -1, "noodle");
    timer = alloc_timer();
    init_timer(timer, 128);
    for (;;) {
        sprintf(s, "%d:%d:%d", hou, min, sec);
        box_fill_window(win, 28, 27, 115, 41, 7);
        putstr_window(win, 28, 27, 0, 11, s);
        set_timeout(timer, 100);
        if (getkey(1) != 128) {
            break;
        }
        sec++;
        if (sec == 60) {
            sec = 0;
            min++;
            if (min == 60) {
                min = 0;
                hou++;
            }
        }
    }
    exit();
}
