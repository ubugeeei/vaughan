#include "../syscall.h"

void HariMain(void) {
    char c, cmdline_buf[120];

    cmdline(cmdline_buf, 120);

    // skip "echo "
    for (int i = 5;; i++) {
        c = cmdline_buf[i];
        if (c == 0) {
            break;
        } else {
            putchar(cmdline_buf[i]);
        }
    }
    api_end();
}
