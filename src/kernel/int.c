#include "boot.h"

void init_pic(void) {
    asm_io_out8(PIC0_IMR, 0xff);
    asm_io_out8(PIC1_IMR, 0xff);

    asm_io_out8(PIC0_ICW1, 0x11);
    asm_io_out8(PIC0_ICW2, 0x20);
    asm_io_out8(PIC0_ICW3, 1 << 2);
    asm_io_out8(PIC0_ICW4, 0x01);

    asm_io_out8(PIC1_ICW1, 0x11);
    asm_io_out8(PIC1_ICW2, 0x28);
    asm_io_out8(PIC1_ICW3, 2);
    asm_io_out8(PIC1_ICW4, 0x01);

    asm_io_out8(PIC0_IMR, 0xfb);
    asm_io_out8(PIC1_IMR, 0xff);

    return;
}

void inthandler27(int *esp) {
    asm_io_out8(PIC0_OCW2, 0x67);
    return;
}
