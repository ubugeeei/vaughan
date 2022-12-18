int open(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void init_malloc(void);
char *malloc(int size);
void refresh_window(int win, int x0, int y0, int x1, int y1);
void draw_line_window(int win, int x0, int y0, int x1, int y1, int col);
void close(int win);
int get_key(int mode);
void api_end(void);

void HariMain(void) {
    char *buf;
    int win, i;
    init_malloc();
    buf = malloc(160 * 100);
    win = open(buf, 160, 100, -1, "lines");
    for (i = 0; i < 8; i++) {
        draw_line_window(win + 1, 8, 26, 77, i * 9 + 26, i);
        draw_line_window(win + 1, 88, 26, i * 9 + 88, 89, i);
    }
    refresh_window(win, 6, 26, 154, 90);
    for (;;) {
        if (get_key(1) == 0x0a) {
            break;
        }  // 0x0a = Enter
    }
    close(win);
    api_end();
}
