#include "../../include/stdio.h"

void HariMain(void) {
    int i, timer;
    timer = alloc_timer();
    init_timer(timer, 128);
    for (i = 20000000; i >= 20000; i -= i / 100) {
        // 20kHz~20Hz: Human can hear
        // decrement i 1%
        beep(i);
        set_timeout(timer, 1);  // 0.01s
        if (getkey(1) != 128) {
            break;
        }
    }
    beep(0);
    exit();
}
