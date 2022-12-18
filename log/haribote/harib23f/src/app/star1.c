int open(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void box_fill_window(int win, int x0, int y0, int x1, int y1, int col);
void init_malloc(void);
char *malloc(int size);
void draw_point_window(int win, int x, int y, int col);
void api_end(void);

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
