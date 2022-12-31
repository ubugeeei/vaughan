#include "boot.h"

void console_task(struct SHEET *sheet, unsigned int memtotal) {
    struct TASK *task = task_now();
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    int i, *fat = (int *)memman_alloc_4k(memman, 4 * 2880);
    struct CONSOLE cons;
    char cmdline[30];
    cons.sht = sheet;
    cons.cur_x = 8;
    cons.cur_y = 28;
    cons.cur_c = -1;
    task->cons = &cons;

    if (sheet != 0) {
        cons.timer = timer_alloc();
        timer_init(cons.timer, &task->queue, 1);
        timer_settime(cons.timer, 50);
    }
    file_read_fat(fat, (unsigned char *)(ADR_DISK_IMG + 0x000200));

    cons_putchar(&cons, '>', 1);

    for (;;) {
        io_cli();
        if (queue32_status(&task->queue) == 0) {
            task_sleep(task);
            io_sti();
        } else {
            i = queue32_get(&task->queue);
            io_sti();
            if (i <= 1) {
                if (i != 0) {
                    timer_init(cons.timer, &task->queue, 0);
                    if (cons.cur_c >= 0) {
                        cons.cur_c = COL8_FFFFFF;
                    }
                } else {
                    timer_init(cons.timer, &task->queue, 1);
                    if (cons.cur_c >= 0) {
                        cons.cur_c = COL8_000000;
                    }
                }
                timer_settime(cons.timer, 50);
            }
            if (i == 2) {
                cons.cur_c = COL8_FFFFFF;
            }
            if (i == 3) {
                // clang-format off
                boxfill8(sheet->buf, sheet->bxsize, COL8_000000, cons.cur_x, cons.cur_y, cons.cur_x + 7, cons.cur_y + 15);
                // clang-format on
                cons.cur_c = -1;
            }
            if (i == 4) {
                cmd_exit(&cons, fat);
            }
            if (256 <= i && i <= 511) {
                if (i == 8 + 256) {
                    if (cons.cur_x > 16) {
                        cons_putchar(&cons, ' ', 0);
                        cons.cur_x -= 8;
                    }
                } else if (i == 10 + 256) {
                    cons_putchar(&cons, ' ', 0);
                    cmdline[cons.cur_x / 8 - 2] = 0;
                    cons_newline(&cons);
                    cons_run_cmd(cmdline, &cons, fat, memtotal);
                    if (sheet == 0) {
                        cmd_exit(&cons, fat);
                    }
                    cons_putchar(&cons, '>', 1);
                } else {
                    if (cons.cur_x < 240) {
                        cmdline[cons.cur_x / 8 - 2] = i - 256;
                        cons_putchar(&cons, i - 256, 1);
                    }
                }
            }

            if (sheet != 0) {
                if (cons.cur_c >= 0) {
                    // clang-format off
                    boxfill8(sheet->buf, sheet->bxsize, cons.cur_c, cons.cur_x, cons.cur_y, cons.cur_x + 7, cons.cur_y + 15);
                    // clang-format on
                }
                // clang-format off
                sheet_refresh(sheet, cons.cur_x, cons.cur_y, cons.cur_x + 8, cons.cur_y + 16);
                // clang-format on
            }
        }
    }
}

void cons_putchar(struct CONSOLE *cons, int chr, char move) {
    char s[2];
    s[0] = chr;
    s[1] = 0;
    if (s[0] == 0x09) {
        for (;;) {
            if (cons->sht != 0) {
                // clang-format off
				putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, COL8_FFFFFF, COL8_000000, " ", 1);
                // clang-format on
            }
            cons->cur_x += 8;
            if (cons->cur_x == 8 + 240) {
                cons_newline(cons);
            }
            if (((cons->cur_x - 8) & 0x1f) == 0) {
                break;
            }
        }
    } else if (s[0] == 0x0a) {
        cons_newline(cons);
    } else if (s[0] == 0x0d) {
    } else {
        if (cons->sht != 0) {
            // clang-format off
			putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, COL8_FFFFFF, COL8_000000, s, 1);
            // clang-format on
        }
        if (move != 0) {
            cons->cur_x += 8;
            if (cons->cur_x == 8 + 240) {
                cons_newline(cons);
            }
        }
    }
    return;
}

void cons_putstr0(struct CONSOLE *cons, char *s) {
    for (; *s != 0; s++) {
        cons_putchar(cons, *s, 1);
    }
    return;
}

void cons_putstr1(struct CONSOLE *cons, char *s, int l) {
    int i;
    for (i = 0; i < l; i++) {
        cons_putchar(cons, s[i], 1);
    }
    return;
}

void cons_newline(struct CONSOLE *cons) {
    int x, y;
    struct SHEET *sheet = cons->sht;
    if (cons->cur_y < 28 + 112) {
        cons->cur_y += 16;
    } else {
        if (sheet != 0) {
            for (y = 28; y < 28 + 112; y++) {
                for (x = 8; x < 8 + 240; x++) {
                    sheet->buf[x + y * sheet->bxsize] =
                        sheet->buf[x + (y + 16) * sheet->bxsize];
                }
            }
            for (y = 28 + 112; y < 28 + 128; y++) {
                for (x = 8; x < 8 + 240; x++) {
                    sheet->buf[x + y * sheet->bxsize] = COL8_000000;
                }
            }
            sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 128);
        }
    }
    cons->cur_x = 8;
    return;
}

// clang-format off
void cons_run_cmd(char *cmdline, struct CONSOLE *cons, int *fat, unsigned int memtotal) {
    // clang-format on
    if (strcmp(cmdline, "free") == 0) {
        cmd_free(cons, memtotal);
    } else if (strcmp(cmdline, "clear") == 0) {
        cmd_clear(cons);
    } else if (strcmp(cmdline, "ls") == 0) {
        cmd_ls(cons);
    } else if (strncmp(cmdline, "cat ", 4) == 0) {
        cmd_cat(cons, fat, cmdline);
    } else if (strcmp(cmdline, "exit") == 0) {
        cmd_exit(cons, fat);
    } else if (strncmp(cmdline, "start ", 6) == 0) {
        cmd_start(cons, cmdline, memtotal);
    } else if (strncmp(cmdline, "ncst ", 5) == 0) {
        cmd_ncst(cons, cmdline, memtotal);
    } else if (cmdline[0] != 0) {
        if (cmd_app(cons, fat, cmdline) == 0) {
            cons_putstr0(cons, "Bad command.\n\n");
        }
    }
    return;
}

void cmd_free(struct CONSOLE *cons, unsigned int memtotal) {
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    char s[60];
    // clang-format off
    sprintf(s, "total   %dMB\nfree %dKB\n\n", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    // clang-format on
    cons_putstr0(cons, s);
    return;
}

void cmd_clear(struct CONSOLE *cons) {
    int x, y;
    struct SHEET *sheet = cons->sht;
    for (y = 28; y < 28 + 128; y++) {
        for (x = 8; x < 8 + 240; x++) {
            sheet->buf[x + y * sheet->bxsize] = COL8_000000;
        }
    }
    sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 128);
    cons->cur_y = 28;
    return;
}

void cmd_ls(struct CONSOLE *cons) {
    struct FILEINFO *finfo = (struct FILEINFO *)(ADR_DISK_IMG + 0x002600);
    int i, j;
    char s[30];
    for (i = 0; i < 224; i++) {
        if (finfo[i].name[0] == 0x00) {
            break;
        }
        if (finfo[i].name[0] != 0xe5) {
            if ((finfo[i].type & 0x18) == 0) {
                sprintf(s, "filename.ext   %d\n", finfo[i].size);
                for (j = 0; j < 8; j++) {
                    s[j] = finfo[i].name[j];
                }
                s[9] = finfo[i].ext[0];
                s[10] = finfo[i].ext[1];
                s[11] = finfo[i].ext[2];
                cons_putstr0(cons, s);
            }
        }
    }
    cons_newline(cons);
    return;
}

void cmd_cat(struct CONSOLE *cons, int *fat, char *cmdline) {
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct FILEINFO *finfo = file_search(
        cmdline + 4, (struct FILEINFO *)(ADR_DISK_IMG + 0x002600), 224);
    char *p;
    int i;

    if (finfo != 0) {
        p = (char *)memman_alloc_4k(memman, finfo->size);
        // clang-format off
        file_load_file(finfo->cluster_num, finfo->size, p, fat, (char *)(ADR_DISK_IMG + 0x003e00));
        // clang-format on
        cons_putstr1(cons, p, finfo->size);
        memman_free_4k(memman, (int)p, finfo->size);
    } else {
        cons_putstr0(cons, "File not found.\n");
    }
    cons_newline(cons);
    return;
}

void cmd_exit(struct CONSOLE *cons, int *fat) {
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct TASK *task = task_now();
    struct SHTCTL *shtctl = (struct SHTCTL *)*((int *)0x0fe4);
    struct Queue32 *queue = (struct Queue32 *)*((int *)0x0fec);
    timer_cancel(cons->timer);
    memman_free_4k(memman, (int)fat, 4 * 2880);
    io_cli();
    if (cons->sht != 0) {
        queue32_put(queue, cons->sht - shtctl->sheets0 + 768);  // 768~1023
    } else {
        queue32_put(queue, task - taskctl->tasks0 + 1024);  // 1024~2023
    }
    io_sti();
    for (;;) {
        task_sleep(task);
    }
}

void cmd_start(struct CONSOLE *cons, char *cmdline, int memtotal) {
    struct SHTCTL *shtctl = (struct SHTCTL *)*((int *)0x0fe4);
    struct SHEET *sht = open_console(shtctl, memtotal);
    struct Queue32 *queue = &sht->task->queue;
    int i;
    sheet_slide(sht, 32, 4);
    sheet_updown(sht, shtctl->top);
    for (i = 6; cmdline[i] != 0; i++) {
        queue32_put(queue, cmdline[i] + 256);
    }
    queue32_put(queue, 10 + 256);
    cons_newline(cons);
    return;
}

void cmd_ncst(struct CONSOLE *cons, char *cmdline, int memtotal) {
    struct TASK *task = open_cons_task(0, memtotal);
    struct Queue32 *queue = &task->queue;
    int i;
    for (i = 5; cmdline[i] != 0; i++) {
        queue32_put(queue, cmdline[i] + 256);
    }
    queue32_put(queue, 10 + 256);
    cons_newline(cons);
    return;
}

int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline) {
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct FILEINFO *finfo;
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
    char name[18], *p, *q;
    struct TASK *task = task_now();
    int i, segment_size, data_size, esp, data_hrb;
    struct SHTCTL *shtctl;
    struct SHEET *sht;

    for (i = 0; i < 13; i++) {
        if (cmdline[i] <= ' ') {
            break;
        }
        name[i] = cmdline[i];
    }
    name[i] = 0;

    finfo =
        file_search(name, (struct FILEINFO *)(ADR_DISK_IMG + 0x002600), 224);

    if (finfo == 0 && name[i - 1] != '.') {
        name[i] = '.';
        name[i + 1] = 'H';
        name[i + 2] = 'R';
        name[i + 3] = 'B';
        name[i + 4] = 0;
        // clang-format off
        finfo = file_search(name, (struct FILEINFO *)(ADR_DISK_IMG + 0x002600), 224);
        // clang-format on
    }

    if (finfo != 0) {
        p = (char *)memman_alloc_4k(memman, finfo->size);
        // clang-format off
        file_load_file(finfo->cluster_num, finfo->size, p, fat, (char *)(ADR_DISK_IMG + 0x003e00));
        // clang-format on
        if (finfo->size >= 36 && strncmp(p + 4, "Hari", 4) == 0 && *p == 0x00) {
            segment_size = *((int *)(p + 0x0000));
            esp = *((int *)(p + 0x000c));
            data_size = *((int *)(p + 0x0010));
            data_hrb = *((int *)(p + 0x0014));
            q = (char *)memman_alloc_4k(memman, segment_size);
            task->ds_base = (int)q;
            // clang-format off
            set_segmdesc(gdt + task->sel / 8 + 1000, finfo->size - 1, (int)p, AR_CODE32_ER + 0x60);
            set_segmdesc(gdt + task->sel / 8 + 2000, segment_size - 1, (int)q, AR_DATA32_RW + 0x60);
            // clang-format on
            for (i = 0; i < data_size; i++) {
                q[esp + i] = p[data_hrb + i];
            }
            // clang-format off
            start_app(0x1b, task->sel + 1000 * 8, esp, task->sel + 2000 * 8, &(task->tss.esp0));
            // clang-format on
            shtctl = (struct SHTCTL *)*((int *)0x0fe4);
            for (i = 0; i < MAX_SHEETS; i++) {
                sht = &(shtctl->sheets0[i]);
                if ((sht->flags & 0x11) == 0x11 && sht->task == task) {
                    sheet_free(sht);
                }
            }
            timer_cancel_all(&task->queue);
            memman_free_4k(memman, (int)q, segment_size);
        } else {
            cons_putstr0(cons, ".hrb file format error.\n");
        }
        memman_free_4k(memman, (int)p, finfo->size);
        cons_newline(cons);
        return 1;
    }

    return 0;
}

// clang-format off
int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax) {
    // clang-format on
    struct TASK *task = task_now();
    int ds_base = task->ds_base;
    struct CONSOLE *cons = task->cons;
    struct SHTCTL *shtctl = (struct SHTCTL *)*((int *)0x0fe4);
    struct SHEET *sht;
    // clang-format off
    int *reg = &eax + 1;  // Next address after edx
            // reg[0] : EDI,   reg[1] : ESI,   reg[2] : EBP,   reg[3] : ESP
            // reg[4] : EBX,   reg[5] : EDX,   reg[6] : ECX,   reg[7] : EAX
    // clang-format on
    int i;

    if (edx == 1) {
        cons_putchar(cons, eax & 0xff, 1);
    } else if (edx == 2) {
        cons_putstr0(cons, (char *)ebx + ds_base);
    } else if (edx == 3) {
        cons_putstr1(cons, (char *)ebx + ds_base, ecx);
    } else if (edx == 4) {
        return &(task->tss.esp0);
    } else if (edx == 5) {
        sht = sheet_alloc(shtctl);
        sht->task = task;
        sht->flags |= 0x10;
        sheet_setbuf(sht, (char *)ebx + ds_base, esi, edi, eax);
        make_window8((char *)ebx + ds_base, esi, edi, (char *)ecx + ds_base, 0);
        // clang-format off
        sheet_slide(sht, ((shtctl->xsize - esi) / 2) & 0xfffffffc, (shtctl->ysize - edi) / 2);
        // clang-format on
        sheet_updown(sht, shtctl->top);
        reg[7] = (int)sht;
    } else if (edx == 6) {
        sht = (struct SHEET *)(ebx & 0xfffffffe);
        // clang-format off
        putfonts8_asc(sht->buf, sht->bxsize, esi, edi, eax, (char *)ebp + ds_base);
        // clang-format on
        if ((ebx & 1) == 0) {
            sheet_refresh(sht, esi, edi, esi + ecx * 8, edi + 16);
        }
    } else if (edx == 7) {
        sht = (struct SHEET *)(ebx & 0xfffffffe);
        boxfill8(sht->buf, sht->bxsize, ebp, eax, ecx, esi, edi);
        if ((ebx & 1) == 0) {
            sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
        }
    } else if (edx == 8) {
        memman_init((struct MEMMAN *)(ebx + ds_base));
        ecx &= 0xfffffff0;
        memman_free((struct MEMMAN *)(ebx + ds_base), eax, ecx);
    } else if (edx == 9) {
        ecx = (ecx + 0x0f) & 0xfffffff0;
        memman_free((struct MEMMAN *)(ebx + ds_base), eax, ecx);
        reg[7] = memman_alloc((struct MEMMAN *)(ebx + ds_base), ecx);
    } else if (edx == 10) {
        ecx = (ecx + 0x0f) & 0xfffffff0;
        memman_free((struct MEMMAN *)(ebx + ds_base), eax, ecx);
    } else if (edx == 11) {
        sht = (struct SHEET *)(ebx & 0xfffffffe);
        sht->buf[sht->bxsize * edi + esi] = eax;
        if ((ebx & 1) == 0) {
            sheet_refresh(sht, esi, edi, esi + 1, edi + 1);
        }
    } else if (edx == 12) {
        sht = (struct SHEET *)ebx;
        sheet_refresh(sht, eax, ecx, esi, edi);
    } else if (edx == 13) {
        sht = (struct SHEET *)(ebx & 0xfffffffe);
        hrb_draw_line_window(sht, eax, ecx, esi, edi, ebp);
        if ((ebx & 1) == 0) {
            sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
        }
    } else if (edx == 14) {
        sheet_free((struct SHEET *)ebx);
    } else if (edx == 15) {
        for (;;) {
            io_cli();
            if (queue32_status(&task->queue) == 0) {
                if (eax != 0) {
                    task_sleep(task);
                } else {
                    io_sti();
                    reg[7] = -1;
                    return 0;
                }
            }
            i = queue32_get(&task->queue);
            io_sti();
            if (i <= 1) {
                timer_init(cons->timer, &task->queue, 1);
                timer_settime(cons->timer, 50);
            }
            if (i == 2) {
                cons->cur_c = COL8_FFFFFF;
            }
            if (i == 3) {
                cons->cur_c = -1;
            }
            if (256 <= i && i <= 511) {
                reg[7] = i - 256;
                return 0;
            }
        }
    } else if (edx == 16) {
        reg[7] = (int)timer_alloc();
        ((struct TIMER *)reg[7])->flags2 = 1;
    } else if (edx == 17) {
        timer_init((struct TIMER *)ebx, &task->queue, eax + 256);
    } else if (edx == 18) {
        timer_settime((struct TIMER *)ebx, eax);
    } else if (edx == 19) {
        timer_free((struct TIMER *)ebx);
    } else if (edx == 20) {
        if (eax == 0) {
            i = io_in8(0x61);
            io_out8(0x61, i & 0x0d);
        } else {
            i = 1193180000 / eax;
            io_out8(0x43, 0xb6);
            io_out8(0x42, i & 0xff);
            io_out8(0x42, i >> 8);
            i = io_in8(0x61);
            io_out8(0x61, (i | 0x03) & 0x0f);
        }
    }
    return 0;
}

int *inthandler0c(int *esp) {
    struct TASK *task = task_now();
    struct CONSOLE *cons = task->cons;
    char s[30];
    cons_putstr0(cons, "\nINT 0C :\n Stack Exception.\n");
    sprintf(s, "EIP = %X\n", esp[11]);
    cons_putstr0(cons, s);
    return &(task->tss.esp0);
}

int *inthandler0d(int *esp) {
    struct TASK *task = task_now();
    struct CONSOLE *cons = task->cons;
    char s[30];
    cons_putstr0(cons, "\nINT 0D :\n General Protected Exception.\n");
    sprintf(s, "EIP = %X\n", esp[11]);
    cons_putstr0(cons, s);
    return &(task->tss.esp0);  // Exception halt
}

// clang-format off
void hrb_draw_line_window(struct SHEET *sht, int x0, int y0, int x1, int y1, int col) {
    // clang-format on
    int i, x, y, len, dx, dy;

    dx = x1 - x0;
    dy = y1 - y0;
    x = x0 << 10;
    y = y0 << 10;
    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }
    if (dx >= dy) {
        len = dx + 1;

        if (x0 > x1) {
            dx = -1024;
        } else {
            dx = 1024;
        }

        if (y0 <= y1) {
            dy = ((y1 - y0 + 1) << 10) / len;
        } else {
            dy = ((y1 - y0 - 1) << 10) / len;
        }
    } else {
        len = dy + 1;

        if (y0 > y1) {
            dy = -1024;
        } else {
            dy = 1024;
        }

        if (x0 <= x1) {
            dx = ((x1 - x0 + 1) << 10) / len;
        } else {
            dx = ((x1 - x0 - 1) << 10) / len;
        }
    }

    for (i = 0; i < len; i++) {
        sht->buf[(y >> 10) * sht->bxsize + (x >> 10)] = col;
        x += dx;
        y += dy;
    }

    return;
}
