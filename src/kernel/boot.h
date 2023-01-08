#include "../include/stdio.h"
#include "../include/string.h"
#include "rust.h"

/*
 *
 * asm head
 *
 */
struct BootInfo {
    char cyls;
    char leds;
    char vmode;
    char reserve;
    short scrnx, scrny;
    char *vram;
};
#define ADR_BOOT_INFO 0x00000ff0
#define ADR_DISK_IMG 0x00100000

/*
 *
 * asm func
 *
 */
void asm_io_hlt(void);
void asm_io_cli(void);
void asm_io_sti(void);
void asm_io_stihlt(void);
int asm_io_in8(int port);
void asm_io_out8(int port, int data);
int asm_io_load_eflags(void);
void asm_io_store_eflags(int eflags);
void asm_load_gdtr(int limit, int addr);
void asm_load_idtr(int limit, int addr);
int asm_load_cr0(void);
void asm_store_cr0(int cr0);
void asm_load_tr(int tr);
void asm_inthandler0c(void);
void asm_inthandler0d(void);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
unsigned int asm_test_memory_sub(unsigned int start, unsigned int end);
void asm_far_jmp(int eip, int cs);
void asm_far_call(int eip, int cs);
void asm_os_api(void);
void asm_start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
void asm_end_app(void);

/*
 *
 * queue
 *
 */
struct Queue {
    int *buf;
    int p, q, size, free, flags;
    struct Task *task;
};
void queue_init(struct Queue *queue, int size, int *buf, struct Task *task);
int queue_put(struct Queue *queue, int data);
int queue_get(struct Queue *queue);
int queue_status(struct Queue *queue);

/*
 *
 * graphic.c
 *
 */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
// clang-format off
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
// clang-format on
void init_screen8(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
// clang-format off
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);
// clang-format on
#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

/*
 *
 * dsctbl
 *
 */
struct SegmentDescriptor {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};
void init_gdt_idt(void);
// clang-format off
void set_segment_descriptor(struct SegmentDescriptor *sd, unsigned int limit, int base, int ar);
void set_gate_descriptor(struct GATE_DESCRIPTOR *gd, int offset, int selector,int ar);
// clang-format on
#define ADR_IDT 0x0026f800
#define LIMIT_IDT 0x000007ff
#define ADR_GDT 0x00270000
#define LIMIT_GDT 0x0000ffff
#define ADR_BOTPAK 0x00280000
#define LIMIT_BOTPAK 0x0007ffff
#define AR_DATA32_RW 0x4092
#define AR_CODE32_ER 0x409a
#define AR_LDT 0x0082
#define AR_TaskStatusSegment 0x0089
#define AR_INTGATE32 0x008e

/*
 *
 * int
 *
 */
void init_pic(void);
void inthandler27(int *esp);
#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_IMR 0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_IMR 0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1

/*
 *
 * keyboard
 *
 */
void inthandler21(int *esp);
void wait_KBC_sendready(void);
void init_keyboard(struct Queue *queue, int data0);
#define PORT_KEYDAT 0x0060
#define PORT_KEYCMD 0x0064

/*
 *
 * mouse
 *
 */
struct MouseDec {
    unsigned char buf[3], phase;
    int x, y, btn;
};
void inthandler2c(int *esp);
void enable_mouse(struct Queue *queue, int data0, struct MouseDec *mdec);
int mouse_decode(struct MouseDec *mdec, unsigned char dat);

/*
 *
 * memory
 *
 */
#define MEMMAN_FREES 4090
#define MEMMAN_ADDR 0x003c0000
struct FREE_MEMORY_INFO {
    unsigned int addr, size;
};
struct MemoryManagement {
    int frees, maxfrees, lostsize, losts;
    struct FREE_MEMORY_INFO free[MEMMAN_FREES];
};
// clang-format off
unsigned int test_memory(unsigned int start, unsigned int end);
void memman_init(struct MemoryManagement *man);
unsigned int memman_total(struct MemoryManagement *man);
unsigned int memman_alloc(struct MemoryManagement *man, unsigned int size);
int memman_free(struct MemoryManagement *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(struct MemoryManagement *man, unsigned int size);
int memman_free_4k(struct MemoryManagement *man, unsigned int addr, unsigned int size);
// clang-format on

/*
 *
 * timer
 *
 */
#define MAX_TIMER 500
struct Timer {
    struct Timer *next;
    unsigned int timeout;
    char flags, flags2;
    struct Queue *queue;
    int data;
};
struct TimerCtl {
    unsigned int count, next, using;
    struct Timer *t0;
    struct Timer timers0[MAX_TIMER];
};
extern struct TimerCtl timer_ctl;
void init_pit(void);
struct Timer *timer_alloc(void);
void timer_free(struct Timer *timer);
void timer_init(struct Timer *timer, struct Queue *queue, int data);
void timer_settime(struct Timer *timer, unsigned int timeout);
int timer_cancel(struct Timer *timer);
void timer_cancel_all(struct Queue *queue);
void inthandler20(int *esp);

/*
 *
 * mtask
 *
 */
#define MAX_TASKS 1000
#define TASK_GDT0 3
#define MAX_TASKS_LV 100
#define MAX_TASK_LEVELS 10
struct TaskStatusSegment {
    int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    int es, cs, ss, ds, fs, gs;
    int ldtr, iomap;
};
struct Task {
    int sel, flags;
    int level, priority;
    struct Queue queue;
    struct TaskStatusSegment tss;
    struct SegmentDescriptor ldt[2];
    struct Console *cons;
    int ds_base, cons_stack;
    struct FileHandle *fhandle;
    int *fat;
    char *cmdline;
    char lang_mode, lang_byte1;
};
struct TaskLevel {
    int running;
    int now;
    struct Task *tasks[MAX_TASKS_LV];
};
struct TaskCtl {
    int now_lv;
    char lv_change;
    struct TaskLevel level[MAX_TASK_LEVELS];
    struct Task tasks0[MAX_TASKS];
};
extern struct TaskCtl *task_ctl;
extern struct Timer *task_timer;
struct Task *task_now(void);
struct Task *task_init(struct MemoryManagement *memory_management);
struct Task *task_alloc(void);
void task_run(struct Task *task, int level, int priority);
void task_switch(void);
void task_sleep(struct Task *task);

/*
 *
 * sheet
 *
 */
#define MAX_SHEETS 256
struct Sheet {
    unsigned char *buf;
    int bxsize, bysize, vx0, vy0, col_inv, height, flags;
    struct SheetCtl *ctl;
    struct Task *task;
};
struct SheetCtl {
    unsigned char *vram, *map;
    int xsize, ysize, top;
    struct Sheet *sheets[MAX_SHEETS];
    struct Sheet sheets0[MAX_SHEETS];
};
// clang-format off
struct SheetCtl *shtctl_init(struct MemoryManagement *memory_management, unsigned char *vram, int xsize, int ysize);
struct Sheet *sheet_alloc(struct SheetCtl *ctl);
void sheet_setbuf(struct Sheet *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
// clang-format on
void sheet_updown(struct Sheet *sht, int height);
void sheet_refresh(struct Sheet *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct Sheet *sht, int vx0, int vy0);
void sheet_free(struct Sheet *sht);

/*
 *
 * window
 *
 */
// clang-format off
void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void putfonts8_asc_sht(struct Sheet *sht, int x, int y, int c, int b, char *s, int l);
// clang-format on
void make_textbox8(struct Sheet *sht, int x0, int y0, int sx, int sy, int c);
void make_wtitle8(unsigned char *buf, int xsize, char *title, char act);
void change_wtitle8(struct Sheet *sht, char act);

/*
 *
 * console
 *
 */
struct Console {
    struct Sheet *sht;
    int cur_x, cur_y, cur_c;
    struct Timer *timer;
};
struct FileHandle {
    char *buf;
    int size;
    int pos;
};
void console_task(struct Sheet *sheet, unsigned int memtotal);
void cons_putchar(struct Console *cons, int chr, char move);
void cons_putstr0(struct Console *cons, char *s);
void cons_putstr1(struct Console *cons, char *s, int l);
void cons_newline(struct Console *cons);
// clang-format off
void cons_run_cmd(char *cmdline, struct Console *cons, int *fat, unsigned int memtotal);
// clang-format on
void cmd_free(struct Console *cons, unsigned int memtotal);
void cmd_clear(struct Console *cons);
void cmd_ls(struct Console *cons);
void cmd_cat(struct Console *cons, int *fat, char *cmdline);
void cmd_exit(struct Console *cons, int *fat);
void cmd_start(struct Console *cons, char *cmdline, int memtotal);
void cmd_ncst(struct Console *cons, char *cmdline, int memtotal);
void cmd_lang(struct Console *cons, char *cmdline);
int cmd_app(struct Console *cons, int *fat, char *cmdline);
// clang-format off
int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
// clang-format on
int *inthandler0c(int *esp);
int *inthandler0d(int *esp);
// clang-format off
void hrb_draw_line_window(struct Sheet *sht, int x0, int y0, int x1, int y1, int col);
// clang-format on

/*
 *
 * file
 *
 */
struct FileInfo {
    unsigned char name[8], ext[3], type;
    char reserve[10];
    unsigned short time, date, cluster_num;
    unsigned int size;
};
void file_read_fat(int *fat, unsigned char *img);
void file_load_file(int cluster_num, int size, char *buf, int *fat, char *img);
struct FileInfo *file_search(char *name, struct FileInfo *finfo, int max);
char *file_load_file2(int cluster_num, int *psize, int *fat);

/*
 *
 * tek
 *
 */
int tek_getsize(unsigned char *p);
int tek_decomp(unsigned char *p, char *q, int size);

/*
 *
 * boot
 *
 */
#define KEYCMD_LED 0xed
struct Task *open_console_task(struct Sheet *sht, unsigned int memtotal);
struct Sheet *open_console(struct SheetCtl *shtctl, unsigned int memtotal);
void close_console(struct Sheet *sht);
void close_console_task(struct Task *task);
void key_window_off(struct Sheet *key_win);
void key_window_on(struct Sheet *key_win);
// keytable
static char KEY_TABLE[0x80] = {
    0,   0,   '1', '2', '3', '4',  '5', '6', '7', '8', '9',  '0', '-', '=', 0,
    0,   'Q', 'W', 'E', 'R', 'T',  'Y', 'U', 'I', 'O', 'P',  '[', ']', 0,   0,
    'A', 'S', 'D', 'F', 'G', 'H',  'J', 'K', 'L', ':', '\'', 0,   0,   0,   'Z',
    'X', 'C', 'V', 'B', 'N', 'M',  ',', '.', '/', 0,   '*',  0,   ' ', 0,   0,
    0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,    '7', '8', '9', '-',
    '4', '5', '6', '+', '1', '2',  '3', '0', '.', 0,   0,    0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,    0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0x5c, 0,   0,   0,   0,
    0,   0,   0,   0,   0,   0x5c, 0,   0};
static char SHIFTED_KEY_TABLE[0x80] = {
    0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '|',
    0,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,   0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', 0,   0,   '}', 'Z',
    'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-',
    '4', '5', '6', '+', '1', '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   '_', 0,   0,   0,   0,
    0,   0,   0,   0,   0,   '|', 0,   0};