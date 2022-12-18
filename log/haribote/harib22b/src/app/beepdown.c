void api_end(void);
int get_key(int mode);
int alloc_timer(void);
void init_timer(int timer, int data);
void set_timeout(int timer, int time);
void beep(int tone);

void HariMain(void) {
    int i, timer;
    timer = alloc_timer();
    init_timer(timer, 128);
    for (i = 20000000; i >= 20000; i -= i / 100) {
        // 20kHz~20Hz: Human can hear
        // decrement i 1%
        beep(i);
        set_timeout(timer, 1);  // 0.01s
        if (get_key(1) != 128) {
            break;
        }
    }
    beep(0);
    api_end();
}
