#include "../../apilib.h"
#include "../../include/stdio.h"   // sprintf
#include "../../include/string.h"  // strlen

void _putstr(int win, char *win_buf, int x, int y, int col, unsigned char *s);
void wait(int i, int timer, char *key_flag);

static unsigned char charset[16 * 8] = {

    /* invader(0) */
    0x00, 0x00, 0x00, 0x43, 0x5f, 0x5f, 0x5f, 0x7f, 0x1f, 0x1f, 0x1f, 0x1f,
    0x00, 0x20, 0x3f, 0x00,

    /* invader(1) */
    0x00, 0x0f, 0x7f, 0xff, 0xcf, 0xcf, 0xcf, 0xff, 0xff, 0xe0, 0xff, 0xff,
    0xc0, 0xc0, 0xc0, 0x00,

    /* invader(2) */
    0x00, 0xf0, 0xfe, 0xff, 0xf3, 0xf3, 0xf3, 0xff, 0xff, 0x07, 0xff, 0xff,
    0x03, 0x03, 0x03, 0x00,

    /* invader(3) */
    0x00, 0x00, 0x00, 0xc2, 0xfa, 0xfa, 0xfa, 0xfe, 0xf8, 0xf8, 0xf8, 0xf8,
    0x00, 0x04, 0xfc, 0x00,

    /* fighter(0) */
    0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x43, 0x47, 0x4f,
    0x5f, 0x7f, 0x7f, 0x00,

    /* fighter(1) */
    0x18, 0x7e, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xff, 0xff, 0xe7, 0xe7,
    0xe7, 0xe7, 0xff, 0x00,

    /* fighter(2) */
    0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc2, 0xe2, 0xf2,
    0xfa, 0xfe, 0xfe, 0x00,

    /* laser */
    0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x00};
/* invader:"abcd", fighter:"efg", laser:"h" */

void HariMain(void) {
    int win, timer, i, j, fx, laser_wait, lx = 0, ly;
    int ix, iy, move_wait0, move_wait, idir;
    int inv_line, score, high, point;
    char win_buf[336 * 261], invstr[32 * 6], s[12], key_flag[4], *p;
    static char inv_str0[32] = " abcd abcd abcd abcd abcd ";

    win = open(win_buf, 336, 261, -1, "invader");
    box_fill_window(win, 6, 27, 329, 254, 0);
    timer = alloc_timer();
    init_timer(timer, 128);

    high = 0;
    _putstr(win, win_buf, 22, 0, 7, "HIGH:00000000");

restart:
    score = 0;
    point = 1;
    _putstr(win, win_buf, 4, 0, 7, "SCORE:00000000");
    move_wait0 = 20;
    fx = 18;
    _putstr(win, win_buf, fx, 13, 6, "efg");
    wait(100, timer, key_flag);

next_group:
    wait(100, timer, key_flag);
    ix = 7;
    iy = 1;
    inv_line = 6;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 27; j++) {
            invstr[i * 32 + j] = inv_str0[j];
        }
        _putstr(win, win_buf, ix, iy + i, 2, invstr + i * 32);
    }
    key_flag[0] = 0;
    key_flag[1] = 0;
    key_flag[2] = 0;

    ly = 0;  // Hidden
    laser_wait = 0;
    move_wait = move_wait0;
    idir = +1;
    wait(100, timer, key_flag);

    for (;;) {
        if (laser_wait != 0) {
            laser_wait--;
            key_flag[2 /* space */] = 0;
        }

        wait(4, timer, key_flag);

        // Fighter
        if (key_flag[0 /* left */] != 0 && fx > 0) {
            fx--;
            _putstr(win, win_buf, fx, 13, 6, "efg ");
            key_flag[0 /* left */] = 0;
        }
        if (key_flag[1 /* right */] != 0 && fx < 37) {
            _putstr(win, win_buf, fx, 13, 6, " efg");
            fx++;
            key_flag[1 /* right */] = 0;
        }
        if (key_flag[2 /* space */] != 0 && laser_wait == 0) {
            laser_wait = 15;
            lx = fx + 1;
            ly = 13;
        }

        // Invaders' movement
        if (move_wait != 0) {
            move_wait--;
        } else {
            move_wait = move_wait0;
            if (ix + idir > 14 || ix + idir < 0) {
                if (iy + inv_line == 13) {
                    break;
                }  // GAME OVER
                idir = -idir;
                _putstr(win, win_buf, ix + 1, iy, 0,
                        "                         ");
                iy++;
            } else {
                ix += idir;
            }
            for (i = 0; i < inv_line; i++) {
                _putstr(win, win_buf, ix, iy + i, 2, invstr + i * 32);
            }
        }

        // Laser
        if (ly > 0) {
            if (ly < 13) {
                if (ix < lx && lx < ix + 25 && iy <= ly && ly < iy + inv_line) {
                    _putstr(win, win_buf, ix, ly, 2, invstr + (ly - iy) * 32);
                } else {
                    _putstr(win, win_buf, lx, ly, 0, " ");
                }
            }
            ly--;
            if (ly > 0) {
                _putstr(win, win_buf, lx, ly, 3, "h");
            } else {
                point -= 10;
                if (point <= 0) {
                    point = 1;
                }
            }
            if (ix < lx && lx < ix + 25 && iy <= ly && ly < iy + inv_line) {
                p = invstr + (ly - iy) * 32 + (lx - ix);
                if (*p != ' ') {
                    /* hit! */
                    score += point;
                    point++;
                    sprintf(s, "%d", score);
                    _putstr(win, win_buf, 10, 0, 7, s);
                    if (high < score) {
                        high = score;
                        _putstr(win, win_buf, 27, 0, 7, s);
                    }
                    for (p--; *p != ' '; p--) {
                    }
                    for (i = 1; i < 5; i++) {
                        p[i] = ' ';
                    }
                    _putstr(win, win_buf, ix, ly, 2, invstr + (ly - iy) * 32);
                    for (; inv_line > 0; inv_line--) {
                        for (p = invstr + (inv_line - 1) * 32; *p != 0; p++) {
                            if (*p != ' ') {
                                goto alive;
                            }
                        }
                    }
                    // Destroy all invaders
                    move_wait0 -= move_wait0 / 3;
                    goto next_group;
                alive:
                    ly = 0;
                }
            }
        }
    }

    // GAME OVER
    _putstr(win, win_buf, 15, 6, 1, "GAME OVER");
    wait(0, timer, key_flag);
    for (i = 1; i < 14; i++) {
        _putstr(win, win_buf, 0, i, 0,
                "                                        ");
    }
    goto restart;
}

void _putstr(int win, char *win_buf, int x, int y, int col, unsigned char *s) {
    int c, x0, i;
    char *p, *q, t[2];
    x = x * 8 + 8;
    y = y * 16 + 29;
    x0 = x;
    i = strlen(s);  // Count length of s
    box_fill_window(win + 1, x, y, x + i * 8 - 1, y + 15, 0);
    q = win_buf + y * 336;
    t[1] = 0;
    for (;;) {
        c = *s;
        if (c == 0) {
            break;
        }
        if (c != ' ') {
            if ('a' <= c && c <= 'h') {
                p = charset + 16 * (c - 'a');
                q += x;
                for (i = 0; i < 16; i++) {
                    if ((p[i] & 0x80) != 0) {
                        q[0] = col;
                    }
                    if ((p[i] & 0x40) != 0) {
                        q[1] = col;
                    }
                    if ((p[i] & 0x20) != 0) {
                        q[2] = col;
                    }
                    if ((p[i] & 0x10) != 0) {
                        q[3] = col;
                    }
                    if ((p[i] & 0x08) != 0) {
                        q[4] = col;
                    }
                    if ((p[i] & 0x04) != 0) {
                        q[5] = col;
                    }
                    if ((p[i] & 0x02) != 0) {
                        q[6] = col;
                    }
                    if ((p[i] & 0x01) != 0) {
                        q[7] = col;
                    }
                    q += 336;
                }
                q -= 336 * 16 + x;
            } else {
                t[0] = *s;
                putstr_window(win + 1, x, y, col, 1, t);
            }
        }
        s++;
        x += 8;
    }
    refresh_window(win, x0, y, x, y + 16);
    return;
}

void wait(int i, int timer, char *key_flag) {
    int j;
    if (i > 0) {
        // Wait for a moment
        set_timeout(timer, i);
        i = 128;
    } else {
        i = 0x0a; /* Enter */
    }
    for (;;) {
        j = get_key(1);
        if (i == j) {
            break;
        }
        if (j == '4') {
            key_flag[0 /* left */] = 1;
        }
        if (j == '6') {
            key_flag[1 /* right */] = 1;
        }
        if (j == ' ') {
            key_flag[2 /* space */] = 1;
        }
    }
    return;
}
