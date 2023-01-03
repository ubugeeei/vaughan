#include "boot.h"

#define PIT_CTRL 0x0043
#define PIT_CNT0 0x0040

struct TIMER_CTL timer_ctl;

#define TIMER_FLAGS_ALLOC 1
#define TIMER_FLAGS_USING 2

void init_pit(void) {
    int i;
    struct TIMER *t;
    asm_io_out8(PIT_CTRL, 0x34);
    asm_io_out8(PIT_CNT0, 0x9c);
    asm_io_out8(PIT_CNT0, 0x2e);
    timer_ctl.count = 0;
    for (i = 0; i < MAX_TIMER; i++) {
        timer_ctl.timers0[i].flags = 0;
    }
    t = timer_alloc();
    t->timeout = 0xffffffff;
    t->flags = TIMER_FLAGS_USING;
    t->flags2 = 0;
    t->next = 0;
    timer_ctl.t0 = t;
    timer_ctl.next = 0xffffffff;
    return;
}

struct TIMER *timer_alloc(void) {
    int i;
    for (i = 0; i < MAX_TIMER; i++) {
        if (timer_ctl.timers0[i].flags == 0) {
            timer_ctl.timers0[i].flags = TIMER_FLAGS_ALLOC;
            return &timer_ctl.timers0[i];
        }
    }
    return 0;
}

void timer_free(struct TIMER *timer) {
    timer->flags = 0;
    return;
}

void timer_init(struct TIMER *timer, struct QUEUE *queue, int data) {
    timer->queue = queue;
    timer->data = data;
    return;
}

void timer_settime(struct TIMER *timer, unsigned int timeout) {
    int e;
    struct TIMER *t, *s;
    timer->timeout = timeout + timer_ctl.count;
    timer->flags = TIMER_FLAGS_USING;
    e = asm_io_load_eflags();
    asm_io_cli();
    t = timer_ctl.t0;
    if (timer->timeout <= t->timeout) {
        timer_ctl.t0 = timer;
        timer->next = t;
        timer_ctl.next = timer->timeout;
        asm_io_store_eflags(e);
        return;
    }

    for (;;) {
        s = t;
        t = t->next;
        if (timer->timeout <= t->timeout) {
            s->next = timer;
            timer->next = t;
            asm_io_store_eflags(e);
            return;
        }
    }
}

void inthandler20(int *esp) {
    struct TIMER *timer;
    char ts = 0;
    asm_io_out8(PIC0_OCW2, 0x60);
    timer_ctl.count++;
    if (timer_ctl.next > timer_ctl.count) {
        return;
    }
    timer = timer_ctl.t0;
    for (;;) {
        if (timer->timeout > timer_ctl.count) {
            break;
        }
        timer->flags = TIMER_FLAGS_ALLOC;
        if (timer != task_timer) {
            queue_put(timer->queue, timer->data);
        } else {
            ts = 1;
        }
        timer = timer->next;
    }
    timer_ctl.t0 = timer;
    timer_ctl.next = timer->timeout;
    if (ts != 0) {
        task_switch();
    }
    return;
}

int timer_cancel(struct TIMER *timer) {
    int e;
    struct TIMER *t;
    e = asm_io_load_eflags();
    asm_io_cli();
    if (timer->flags == TIMER_FLAGS_USING) {
        if (timer == timer_ctl.t0) {
            t = timer->next;
            timer_ctl.t0 = t;
            timer_ctl.next = t->timeout;
        } else {
            t = timer_ctl.t0;
            for (;;) {
                if (t->next == timer) {
                    break;
                }
                t = t->next;
            }
            t->next = timer->next;
        }
        timer->flags = TIMER_FLAGS_ALLOC;
        asm_io_store_eflags(e);
        return 1;
    }
    asm_io_store_eflags(e);
    return 0;
}

void timer_cancel_all(struct QUEUE *queue) {
    int e, i;
    struct TIMER *t;
    e = asm_io_load_eflags();
    asm_io_cli();
    for (i = 0; i < MAX_TIMER; i++) {
        t = &timer_ctl.timers0[i];
        if (t->flags != 0 && t->flags2 != 0 && t->queue == queue) {
            timer_cancel(t);
            timer_free(t);
        }
    }
    asm_io_store_eflags(e);
    return;
}
