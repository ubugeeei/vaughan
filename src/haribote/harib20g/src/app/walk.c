int open(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void putstr_window(int win, int x, int y, int col, int len, char *str);
void box_fill_window(int win, int x0, int y0, int x1, int y1, int col);
void init_malloc(void);
char *malloc(int size);
void refresh_window(int win, int x0, int y0, int x1, int y1);
void draw_line_window(int win, int x0, int y0, int x1, int y1, int col);
void close(int win);
int get_key(int mode);
void api_end(void);

void HariMain(void) {
    char *buf;
    int win, i, x, y;
    init_malloc();
    buf = malloc(160 * 100);
    win = open(buf, 160, 100, -1, "walk");
    box_fill_window(win, 4, 24, 155, 95, 0);
    x = 76;
    y = 56;
    putstr_window(win, x, y, 3, 1, "*");
    for (;;) {
        i = get_key(1);
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
    close(win);
    api_end();
}
