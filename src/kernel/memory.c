#include "boot.h"
#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000

unsigned int test_memory(unsigned int start, unsigned int end) {
    char flg486 = 0;
    unsigned int eflg, cr0, i;

    eflg = asm_io_load_eflags();
    eflg |= EFLAGS_AC_BIT;
    asm_io_store_eflags(eflg);
    eflg = asm_io_load_eflags();
    if ((eflg & EFLAGS_AC_BIT) != 0) {
        flg486 = 1;
    }
    eflg &= ~EFLAGS_AC_BIT;
    asm_io_store_eflags(eflg);

    if (flg486 != 0) {
        cr0 = asm_load_cr0();
        cr0 |= CR0_CACHE_DISABLE;
        asm_store_cr0(cr0);
    }

    i = asm_test_memory_sub(start, end);

    if (flg486 != 0) {
        cr0 = asm_load_cr0();
        cr0 &= ~CR0_CACHE_DISABLE;
        asm_store_cr0(cr0);
    }

    return i;
}

void memory_management_init(struct MEMORY_MANAGEMENT *man) {
    man->frees = 0;
    man->maxfrees = 0;
    man->lostsize = 0;
    man->losts = 0;
    return;
}

unsigned int memory_management_total(struct MEMORY_MANAGEMENT *man) {
    unsigned int i, t = 0;
    for (i = 0; i < man->frees; i++) {
        t += man->free[i].size;
    }
    return t;
}

unsigned int memory_management_alloc(struct MEMORY_MANAGEMENT *man, unsigned int size) {
    unsigned int i, a;
    for (i = 0; i < man->frees; i++) {
        if (man->free[i].size >= size) {
            a = man->free[i].addr;
            man->free[i].addr += size;
            man->free[i].size -= size;
            if (man->free[i].size == 0) {
                man->frees--;
                for (; i < man->frees; i++) {
                    man->free[i] = man->free[i + 1];
                }
            }
            return a;
        }
    }
    return 0;
}

int memory_management_free(struct MEMORY_MANAGEMENT *man, unsigned int addr, unsigned int size) {
    int i, j;
    for (i = 0; i < man->frees; i++) {
        if (man->free[i].addr > addr) {
            break;
        }
    }

    if (i > 0) {
        if (man->free[i - 1].addr + man->free[i - 1].size == addr) {
            man->free[i - 1].size += size;
            if (i < man->frees) {
                if (addr + size == man->free[i].addr) {
                    man->free[i - 1].size += man->free[i].size;

                    man->frees--;
                    for (; i < man->frees; i++) {
                        man->free[i] = man->free[i + 1];
                    }
                }
            }
            return 0;
        }
    }

    if (i < man->frees) {
        if (addr + size == man->free[i].addr) {
            man->free[i].addr = addr;
            man->free[i].size += size;
            return 0;
        }
    }

    if (man->frees < MEMMAN_FREES) {
        for (j = man->frees; j > i; j--) {
            man->free[j] = man->free[j - 1];
        }
        man->frees++;
        if (man->maxfrees < man->frees) {
            man->maxfrees = man->frees;
        }
        man->free[i].addr = addr;
        man->free[i].size = size;
        return 0;
    }

    man->losts++;
    man->lostsize += size;
    return -1;
}

unsigned int memory_management_alloc_4k(struct MEMORY_MANAGEMENT *man, unsigned int size) {
    unsigned int a;
    size = (size + 0xfff) & 0xfffff000;
    a = memory_management_alloc(man, size);
    return a;
}

int memory_management_free_4k(struct MEMORY_MANAGEMENT *man, unsigned int addr, unsigned int size) {
    int i;
    size = (size + 0xfff) & 0xfffff000;
    i = memory_management_free(man, addr, size);
    return i;
}
