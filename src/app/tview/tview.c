#include "../../include/stdio.h"

int strtol(char *s, char **endp, int base);  // stdlib.h

char *skip_space(char *p);
void text_view(int win, int w, int h, int xskip, char *p, int tab, int lang);
// clang-format off
char *line_view(int win, int w, int y, int xskip, unsigned char *p, int tab, int lang);
// clang-format on
int put_tab(int x, int w, int xskip, char *s, int tab);

void HariMain(void) {
    char win_buf[1024 * 757], txt_buf[240 * 1024];
    int w = 30, h = 10, t = 4, spd_x = 1, spd_y = 1;
    int win, i, j, lang = get_lang_mode(), xskip = 0;
    char s[30], *p, *q = 0, *r = 0;

    // Parse command line
    cmdline(s, 30);
    for (p = s; *p > ' '; p++) {
    }  // Skip until space
    for (; *p != 0;) {
        p = skip_space(p);
        if (*p == '-') {
            if (p[1] == 'w') {
                w = strtol(p + 2, &p, 0);
                if (w < 20) {
                    w = 20;
                }
                if (w > 126) {
                    w = 126;
                }
            } else if (p[1] == 'h') {
                h = strtol(p + 2, &p, 0);
                if (h < 1) {
                    h = 1;
                }
                if (h > 45) {
                    h = 45;
                }
            } else if (p[1] == 't') {
                t = strtol(p + 2, &p, 0);
                if (t < 1) {
                    t = 4;
                }
            } else {
            err:
                puts(" >tview file [-w30 -h10 -t4]\n");
                exit();
            }
        } else {  // Found file
            if (q != 0) {
                goto err;
            }
            q = p;
            for (; *p > ' '; p++) {
            }  // Skip until space
            r = p;
        }
    }
    if (q == 0) {
        goto err;
    }

    // Prepare window
    win = create_window(win_buf, w * 8 + 16, h * 16 + 37, -1, "tview");
    box_fill_window(win, 6, 27, w * 8 + 9, h * 16 + 30, 7);

    // Read file
    *r = 0;
    i = fopen(q);
    if (i == 0) {
        puts("file create_window error.\n");
        exit();
    }
    j = fsize(i, 0);
    if (j >= 240 * 1024 - 1) {
        j = 240 * 1024 - 2;
    }
    txt_buf[0] = 0x0a;  // New line for sentinel
    fread(txt_buf + 1, j, i);
    fclose(i);
    txt_buf[j + 1] = 0;
    q = txt_buf + 1;
    for (p = txt_buf + 1; *p != 0; p++) {  // Remove 0x0d
        if (*p != 0x0d) {
            *q = *p;
            q++;
        }
    }
    *q = 0;

    // Main
    p = txt_buf + 1;
    for (;;) {
        text_view(win, w, h, xskip, p, t, lang);
        i = getkey(1);
        if (i == 'Q' || i == 'q') {
            exit();
        }
        if ('A' <= i && i <= 'F') {
            spd_x = 1 << (i - 'A'); /* 1, 2, 4, 8, 16, 32 */
        }
        if ('a' <= i && i <= 'f') {
            spd_y = 1 << (i - 'a'); /* 1, 2, 4, 8, 16, 32 */
        }
        if (i == '<' && t > 1) {
            t /= 2;
        }
        if (i == '>' && t < 256) {
            t *= 2;
        }
        if (i == '4') {
            for (;;) {
                xskip -= spd_x;
                if (xskip < 0) {
                    xskip = 0;
                }
                // If not '4' is pressed
                if (getkey(0) != '4') {
                    break;
                }
            }
        }
        if (i == '6') {
            for (;;) {
                xskip += spd_x;
                if (getkey(0) != '6') {
                    break;
                }
            }
        }
        if (i == '8') {
            for (;;) {
                for (j = 0; j < spd_y; j++) {
                    if (p == txt_buf + 1) {
                        break;
                    }
                    for (p--; p[-1] != 0x0a; p--) {
                    }  // Go back until 0x0a
                }
                if (getkey(0) != '8') {
                    break;
                }
            }
        }
        if (i == '2') {
            for (;;) {
                for (j = 0; j < spd_y; j++) {
                    for (q = p; *q != 0 && *q != 0x0a; q++) {
                    }
                    if (*q == 0) {
                        break;
                    }
                    p = q + 1;
                }
                if (getkey(0) != '2') {
                    break;
                }
            }
        }
    }
}

char *skip_space(char *p) {
    for (; *p == ' '; p++) {
    }  // Skip until space
    return p;
}

void text_view(int win, int w, int h, int xskip, char *p, int tab, int lang) {
    int i;
    box_fill_window(win + 1, 8, 29, w * 8 + 7, h * 16 + 28, 7);
    for (i = 0; i < h; i++) {
        p = line_view(win, w, i * 16 + 29, xskip, p, tab, lang);
    }
    refresh_window(win, 8, 29, w * 8 + 8, h * 16 + 29);
    return;
}

// clang-format off
char *line_view(int win, int w, int y, int xskip, unsigned char *p, int tab, int lang) {
    // clang-format on
    int x = -xskip;
    char s[130];
    for (;;) {
        if (*p == 0) {
            break;
        }
        if (*p == 0x0a) {
            p++;
            break;
        }
        if (lang == 0) { /* ASCII */
            if (*p == 0x09) {
                x = put_tab(x, w, xskip, s, tab);
            } else {
                if (0 <= x && x < w) {
                    s[x] = *p;
                }
                x++;
            }
            p++;
        }
        if (lang == 1) { /* SJIS */
            if (*p == 0x09) {
                x = put_tab(x, w, xskip, s, tab);
                p++;
                // clang-format off
            } else if ((0x81 <= *p && *p <= 0x9f) || (0xe0 <= *p && *p <= 0xfc)) {
                // clang-format on
                // Zenkaku
                if (x == -1) {
                    s[0] = ' ';
                }
                if (0 <= x && x < w - 1) {
                    s[x] = *p;
                    s[x + 1] = p[1];
                }
                if (x == w - 1) {
                    s[x] = ' ';
                }
                x += 2;
                p += 2;
            } else {
                if (0 <= x && x < w) {
                    s[x] = *p;
                }
                x++;
                p++;
            }
        }
        if (lang == 2) { /* EUC */
            if (*p == 0x09) {
                x = put_tab(x, w, xskip, s, tab);
                p++;
            } else if (0xa1 <= *p && *p <= 0xfe) {
                // Zenkaku
                if (x == -1) {
                    s[0] = ' ';
                }
                if (0 <= x && x < w - 1) {
                    s[x] = *p;
                    s[x + 1] = p[1];
                }
                if (x == w - 1) {
                    s[x] = ' ';
                }
                x += 2;
                p += 2;
            } else {
                if (0 <= x && x < w) {
                    s[x] = *p;
                }
                x++;
                p++;
            }
        }
    }
    if (x > w) {
        x = w;
    }
    if (x > 0) {
        s[x] = 0;
        putstr_window(win + 1, 8, y, 0, x, s);
    }
    return p;
}

int put_tab(int x, int w, int xskip, char *s, int tab) {
    for (;;) {
        if (0 <= x && x < w) {
            s[x] = ' ';
        }
        x++;
        if ((x + xskip) % tab == 0) {
            break;
        }
    }
    return x;
}
