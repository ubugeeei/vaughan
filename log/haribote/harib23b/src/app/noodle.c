int open(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void putstr_window(int win, int x, int y, int col, int len, char *str);
void box_fill_window(int win, int x0, int y0, int x1, int y1, int col);
void init_malloc(void);
char *malloc(int size);
int get_key(int mode);
int alloc_timer(void);
void init_timer(int timer, int data);
void set_timeout(int timer, int time);
void api_end(void);

void sprintf(char *str, char *fmt, ...);

void HariMain(void) {
    char *buf, s[12];
    int win, timer, sec = 0, min = 0, hou = 0;
    init_malloc();
    buf = malloc(150 * 50);
    win = open(buf, 150, 50, -1, "noodle");
    timer = alloc_timer();
    init_timer(timer, 128);
    for (;;) {
        sprintf(s, "%d:%d:%d", hou, min, sec);
        box_fill_window(win, 28, 27, 115, 41, 7);
        putstr_window(win, 28, 27, 0, 11, s);
        set_timeout(timer, 100);
        if (get_key(1) != 128) {
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
    api_end();
}
