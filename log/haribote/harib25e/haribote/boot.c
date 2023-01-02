#include "boot.h"

#define KEYCMD_LED 0xed

void key_window_off(struct SHEET *key_win);
void key_window_on(struct SHEET *key_win);
struct SHEET *open_console(struct SHTCTL *shtctl, unsigned int memtotal);
void close_console(struct SHEET *sht);
void close_console_task(struct TASK *task);

void Boot(void) {
    struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
    struct SHTCTL *shtctl;
    char s[40];
    struct Queue32 queue, keycmd;
    int queue_buf[128], keycmd_buf[32], *cons_queue[2];
    int mx, my, i, new_mx = -1, new_my = 0, new_wx = 0x7fffffff, new_wy = 0;
    unsigned int memtotal;
    struct MOUSE_DEC mdec;
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    unsigned char *buf_back, buf_mouse[256], *buf_cons[2];
    struct SHEET *sht_back, *sht_mouse, *sht_cons[2];
    struct TASK *task_a, *task_cons[2], *task;
    static char keytable0[0x80] = {
        0,   0,    '1', '2', '3', '4', '5', '6', '7',  '8', '9', '0',  '-',
        '=', 0,    0,   'Q', 'W', 'E', 'R', 'T', 'Y',  'U', 'I', 'O',  'P',
        '[', ']',  0,   0,   'A', 'S', 'D', 'F', 'G',  'H', 'J', 'K',  'L',
        ':', '\'', 0,   0,   0,   'Z', 'X', 'C', 'V',  'B', 'N', 'M',  ',',
        '.', '/',  0,   '*', 0,   ' ', 0,   0,   0,    0,   0,   0,    0,
        0,   0,    0,   0,   0,   0,   '7', '8', '9',  '-', '4', '5',  '6',
        '+', '1',  '2', '3', '0', '.', 0,   0,   0,    0,   0,   0,    0,
        0,   0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,
        0,   0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0x5c, 0,
        0,   0,    0,   0,   0,   0,   0,   0,   0x5c, 0,   0};
    static char keytable1[0x80] = {
        0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',
        '+', '|', 0,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
        '{', '}', 0,   0,   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
        ':', '"', 0,   0,   '}', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<',
        '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6',
        '+', '1', '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   '_', 0,
        0,   0,   0,   0,   0,   0,   0,   0,   '|', 0,   0};
    int key_to = 0, key_shift = 0, key_leds = (binfo->leds >> 4) & 7,
        keycmd_wait = -1;
    struct CONSOLE *cons;
    int j, x, y, mmx = -1, mmy = -1, mmx2 = 0;
    struct SHEET *sht = 0, *key_win, *sht2;

    init_gdtidt();
    init_pic();
    io_sti();
    queue32_init(&queue, 128, queue_buf, 0);
    *((int *)0x0fec) = (int)&queue;
    init_pit();
    init_keyboard(&queue, 256);
    enable_mouse(&queue, 512, &mdec);
    io_out8(PIC0_IMR, 0xf8);
    io_out8(PIC1_IMR, 0xef);
    queue32_init(&keycmd, 32, keycmd_buf, 0);

    /*
     *
     * memman
     *
     */
    memtotal = memtest(0x00400000, 0xbfffffff);
    memman_init(memman);
    memman_free(memman, 0x00001000, 0x0009e000);
    memman_free(memman, 0x00400000, memtotal - 0x00400000);

    init_palette();
    shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
    task_a = task_init(memman);
    queue.task = task_a;
    task_run(task_a, 1, 2);
    *((int *)0x0fe4) = (int)shtctl;

    /*
     *
     * sheets
     *
     */

    /*
     * sht_back
     */
    sht_back = sheet_alloc(shtctl);
    buf_back =
        (unsigned char *)memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
    sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1);
    init_screen8(buf_back, binfo->scrnx, binfo->scrny);

    /*
     * sht_cons
     */
    key_win = open_console(shtctl, memtotal);

    /*
     * sht_mouse
     */
    sht_mouse = sheet_alloc(shtctl);
    sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
    init_mouse_cursor8(buf_mouse, 99);
    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;

    /*
     * sheet layouts
     */
    sheet_slide(sht_back, 0, 0);
    sheet_slide(key_win, 32, 4);
    sheet_slide(sht_mouse, mx, my);
    sheet_updown(sht_back, 0);
    sheet_updown(key_win, 1);
    sheet_updown(sht_mouse, 2);
    key_window_on(key_win);

    queue32_put(&keycmd, KEYCMD_LED);
    queue32_put(&keycmd, key_leds);

    /*
     *
     * event loop
     *
     */
    for (;;) {
        if (queue32_status(&keycmd) > 0 && keycmd_wait < 0) {
            keycmd_wait = queue32_get(&keycmd);
            wait_KBC_sendready();
            io_out8(PORT_KEYDAT, keycmd_wait);
        }

        io_cli();

        if (queue32_status(&queue) == 0) {
            if (new_mx >= 0) {
                // draw if there is no event
                io_sti();
                sheet_slide(sht_mouse, new_mx, new_my);
                new_mx = -1;
            } else if (new_wx != 0x7fffffff) {
                // draw if there is no event
                io_sti();
                sheet_slide(sht, new_wx, new_wy);
                new_wx = 0x7fffffff;
            } else {
                // sleep if there is no event
                task_sleep(task_a);
                io_sti();
            }
        } else {
            // handle event
            i = queue32_get(&queue);
            io_sti();
            if (key_win != 0 && key_win->flags == 0) {  // Window close
                if (shtctl->top == 1) {
                    key_win = 0;
                } else {
                    key_win = shtctl->sheets[shtctl->top - 1];
                    key_window_on(key_win);
                }
            }

            if (256 <= i && i <= 511) {
                /*
                 *
                 * handle keyboard event
                 *
                 */
                if (i < 0x80 + 256) {
                    if (key_shift == 0) {
                        s[0] = keytable0[i - 256];
                    } else {
                        s[0] = keytable1[i - 256];
                    }
                } else {
                    s[0] = 0;
                }

                // convert to lowercase
                if ('A' <= s[0] && s[0] <= 'Z') {
                    if (((key_leds & 4) == 0 && key_shift == 0) ||
                        ((key_leds & 4) != 0 && key_shift != 0)) {
                        s[0] += 0x20;
                    }
                }

                // normal characters
                if (s[0] != 0 && key_win != 0) {
                    queue32_put(&key_win->task->queue, s[0] + 256);
                }

                // BackSpace
                if (i == 256 + 0x0e && key_win != 0) {  // Backspace
                    queue32_put(&key_win->task->queue, 8 + 256);
                }

                // Enter
                if (i == 256 + 0x1c && key_win != 0) {  // Enter
                    queue32_put(&key_win->task->queue, 10 + 256);
                }

                // Tab
                if (i == 256 + 0x0f && key_win != 0) {  // Tab
                    key_window_off(key_win);
                    j = key_win->height - 1;
                    if (j == 0) {
                        j = shtctl->top - 1;
                    }
                    key_win = shtctl->sheets[j];
                    key_window_on(key_win);
                }

                // LShift ON
                if (i == 256 + 0x2a) {
                    key_shift |= 1;
                }
                // RShift ON
                if (i == 256 + 0x36) {
                    key_shift |= 2;
                }
                // LShift OFF
                if (i == 256 + 0xaa) {
                    key_shift &= ~1;
                }
                // RShift OFF
                if (i == 256 + 0xb6) {
                    key_shift &= ~2;
                }

                // CapsLock
                if (i == 256 + 0x3a) {
                    key_leds ^= 4;
                    queue32_put(&keycmd, KEYCMD_LED);
                    queue32_put(&keycmd, key_leds);
                }

                // NumLock
                if (i == 256 + 0x45) {
                    key_leds ^= 2;
                    queue32_put(&keycmd, KEYCMD_LED);
                    queue32_put(&keycmd, key_leds);
                }

                // ScrollLock
                if (i == 256 + 0x46) {
                    key_leds ^= 1;
                    queue32_put(&keycmd, KEYCMD_LED);
                    queue32_put(&keycmd, key_leds);
                }

                // Shift + F1
                if (i == 256 + 0x3b && key_shift != 0) {
                    task = key_win->task;
                    if (task != 0 && task->tss.ss0 != 0) {
                        cons_putstr0(task->cons, "\nBreak(key) :\n");
                        io_cli();
                        task->tss.eax = (int)&(task->tss.esp0);
                        task->tss.eip = (int)asm_end_app;
                        io_sti();
                        task_run(task, -1, 0);
                    }
                }

                // Shift + F2
                if (i == 256 + 0x3c && key_shift != 0) {
                    if (key_win != 0) {
                        // focus on console
                        key_window_off(key_win);
                    }
                    key_window_off(key_win);
                    key_win = open_console(shtctl, memtotal);
                    sheet_slide(key_win, 32, 4);
                    sheet_updown(key_win, shtctl->top);
                    key_window_on(key_win);
                }

                // F12
                if (i == 256 + 0x58 && shtctl->top > 2) {
                    sheet_updown(shtctl->sheets[1], shtctl->top - 1);
                }

                // if received data
                if (i == 256 + 0xfa) {
                    keycmd_wait = -1;
                }

                // if dont receive data
                if (i == 256 + 0xfe) {
                    wait_KBC_sendready();
                    io_out8(PORT_KEYDAT, keycmd_wait);
                }
            } else if (512 <= i && i <= 767) {
                /*
                 *
                 * handle mouse event
                 *
                 */
                if (mouse_decode(&mdec, i - 512) != 0) {
                    mx += mdec.x;
                    my += mdec.y;

                    if (mx < 0) {
                        mx = 0;
                    }
                    if (my < 0) {
                        my = 0;
                    }
                    if (mx > binfo->scrnx - 1) {
                        mx = binfo->scrnx - 1;
                    }
                    if (my > binfo->scrny - 1) {
                        my = binfo->scrny - 1;
                    }

                    new_mx = mx;
                    new_my = my;

                    if ((mdec.btn & 0x01) != 0) {
                        if (mmx < 0) {
                            for (j = shtctl->top - 1; j > 0; j--) {
                                sht = shtctl->sheets[j];
                                x = mx - sht->vx0;
                                y = my - sht->vy0;

                                // clang-format off
                                if (0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize) {
                                    if (sht->buf[y * sht->bxsize + x] != sht->col_inv) {
                                        // clang-format on
                                        if (sht != key_win) {
                                            // clang-format off
                                            key_window_off(key_win);
                                            key_win = sht;
                                            key_window_on(key_win);
                                            // clang-format on
                                        }
                                        sheet_updown(sht, shtctl->top - 1);

                                        // clang-format off
                                        if (3 <= x && x < sht->bxsize - 3 && 3 <= y && y < 21) {
                                            // clang-format on
                                            mmx = mx;
                                            mmy = my;
                                            mmx2 = sht->vx0;
                                            new_wy = sht->vy0;
                                        }

                                        // onClick [X] button
                                        // clang-format off
                                        if (sht->bxsize - 21 <= x && x < sht->bxsize - 5 && 5 <= y && y < 19) {
                                            // clang-format on
                                            if ((sht->flags & 0x10) != 0) {
                                                task = sht->task;
                                                // clang-format off
												cons_putstr0(task->cons, "\nBye :\n");
												io_cli();
												task->tss.eax = (int) &(task->tss.esp0);
												task->tss.eip = (int) asm_end_app;
                                                // clang-format on
                                                io_sti();
                                                task_run(task, -1, 0);
                                            } else {
                                                // clang-format off
                                                task = sht->task;
                                                sheet_updown(sht, -1); // hide sheet
												key_window_off(key_win);
												key_win = shtctl->sheets[shtctl->top - 1];
												key_window_on(key_win);
                                                io_cli();
                                                queue32_put(&task->queue, 4);
                                                io_sti();
                                                // clang-format on
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        } else {
                            x = mx - mmx;
                            y = my - mmy;
                            new_wx = (mmx2 + x + 2) & ~3;
                            new_wy = new_wy + y;
                            mmy = my;
                        }
                    } else {
                        mmx = -1;
                        if (new_wx != 0x7fffffff) {
                            sheet_slide(sht, new_wx, new_wy);
                            new_wx = 0x7fffffff;
                        }
                    }
                }
            } else if (768 <= i && i <= 1023) {
                close_console(shtctl->sheets0 + (i - 768));
            } else if (1024 <= i && i <= 2023) {
                close_console_task(taskctl->tasks0 + (i - 1024));
            } else if (2024 <= i && i <= 2279) {
                // Close only console
                sht2 = shtctl->sheets0 + (i - 2024);
                memman_free_4k(memman, (int)sht2->buf, 256 * 165);
                sheet_free(sht2);
            }
        }
    }
}

void key_window_off(struct SHEET *key_win) {
    change_wtitle8(key_win, 0);
    if ((key_win->flags & 0x20) != 0) {
        queue32_put(&key_win->task->queue, 3);
    }
    return;
}

void key_window_on(struct SHEET *key_win) {
    change_wtitle8(key_win, 1);
    if ((key_win->flags & 0x20) != 0) {
        queue32_put(&key_win->task->queue, 2);
    }
    return;
}

struct TASK *open_console_task(struct SHEET *sht, unsigned int memtotal) {
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct TASK *task = task_alloc();
    int *cons_queue = (int *)memman_alloc_4k(memman, 128 * 4);
    task->cons_stack = memman_alloc_4k(memman, 64 * 1024);
    task->tss.esp = task->cons_stack + 64 * 1024 - 12;
    task->tss.eip = (int)&console_task;
    task->tss.es = 1 * 8;
    task->tss.cs = 2 * 8;
    task->tss.ss = 1 * 8;
    task->tss.ds = 1 * 8;
    task->tss.fs = 1 * 8;
    task->tss.gs = 1 * 8;
    *((int *)(task->tss.esp + 4)) = (int)sht;
    *((int *)(task->tss.esp + 8)) = memtotal;
    task_run(task, 2, 2);
    queue32_init(&task->queue, 128, cons_queue, task);
    return task;
}

struct SHEET *open_console(struct SHTCTL *shtctl, unsigned int memtotal) {
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct SHEET *sht = sheet_alloc(shtctl);
    unsigned char *buf = (unsigned char *)memman_alloc_4k(memman, 256 * 165);
    struct TASK *task = task_alloc();
    int *cons_queue = (int *)memman_alloc_4k(memman, 128 * 4);
    sheet_setbuf(sht, buf, 256, 165, -1);
    make_window8(buf, 256, 165, "console", 0);
    make_textbox8(sht, 8, 28, 240, 128, COL8_000000);
    task->cons_stack = memman_alloc_4k(memman, 64 * 1024);
    task->tss.esp = task->cons_stack + 64 * 1024 - 12;
    task->tss.eip = (int)&console_task;
    task->tss.es = 1 * 8;
    task->tss.cs = 2 * 8;
    task->tss.ss = 1 * 8;
    task->tss.ds = 1 * 8;
    task->tss.fs = 1 * 8;
    task->tss.gs = 1 * 8;
    *((int *)(task->tss.esp + 4)) = (int)sht;
    *((int *)(task->tss.esp + 8)) = memtotal;
    task_run(task, 2, 2);
    sht->task = task;
    sht->flags |= 0x20;  // cursor
    queue32_init(&task->queue, 128, cons_queue, task);
    return sht;
}

void close_console_task(struct TASK *task) {
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    task_sleep(task);
    memman_free_4k(memman, task->cons_stack, 64 * 1024);
    memman_free_4k(memman, (int)task->queue.buf, 128 * 4);
    task->flags = 0;
    return;
}

void close_console(struct SHEET *sht) {
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct TASK *task = sht->task;
    memman_free_4k(memman, (int)sht->buf, 256 * 165);
    sheet_free(sht);
    close_console_task(task);
    return;
}
