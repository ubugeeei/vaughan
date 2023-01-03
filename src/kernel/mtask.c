#include "boot.h"

struct TaskCtl *task_ctl;
struct Timer *task_timer;

struct Task *task_now(void) {
    struct TaskLevel *tl = &task_ctl->level[task_ctl->now_lv];
    return tl->tasks[tl->now];
}

void task_add(struct Task *task) {
    struct TaskLevel *tl = &task_ctl->level[task->level];
    tl->tasks[tl->running] = task;
    tl->running++;
    task->flags = 2;
    return;
}

void task_remove(struct Task *task) {
    int i;
    struct TaskLevel *tl = &task_ctl->level[task->level];

    for (i = 0; i < tl->running; i++) {
        if (tl->tasks[i] == task) {
            break;
        }
    }

    tl->running--;
    if (i < tl->now) {
        tl->now--;
    }
    if (tl->now >= tl->running) {
        tl->now = 0;
    }
    task->flags = 1;

    for (; i < tl->running; i++) {
        tl->tasks[i] = tl->tasks[i + 1];
    }

    return;
}

void task_switchsub(void) {
    int i;

    for (i = 0; i < MAX_TASK_LEVELS; i++) {
        if (task_ctl->level[i].running > 0) {
            break;
        }
    }
    task_ctl->now_lv = i;
    task_ctl->lv_change = 0;
    return;
}

void task_idle(void) {
    for (;;) {
        asm_io_hlt();
    }
}

struct Task *task_init(struct MemoryManagement *memory_management) {
    int i;
    struct Task *task, *idle;
    struct SegmentDescriptor *gdt = (struct SegmentDescriptor *)ADR_GDT;

    task_ctl = (struct TaskCtl *)memory_management_alloc_4k(memory_management, sizeof(struct TaskCtl));
    for (i = 0; i < MAX_TASKS; i++) {
        task_ctl->tasks0[i].flags = 0;
        task_ctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
        task_ctl->tasks0[i].tss.ldtr = (TASK_GDT0 + MAX_TASKS + i) * 8;
        // clang-format off
        set_segment_descriptor(gdt + TASK_GDT0 + i, 103, (int)&task_ctl->tasks0[i].tss, AR_TaskStatusSegment);
        set_segment_descriptor(gdt + TASK_GDT0 + MAX_TASKS + i, 15, (int) task_ctl->tasks0[i].ldt, AR_LDT);
        // clang-format on
    }
    for (i = 0; i < MAX_TASK_LEVELS; i++) {
        task_ctl->level[i].running = 0;
        task_ctl->level[i].now = 0;
    }

    task = task_alloc();
    task->flags = 2;
    task->priority = 2;
    task->level = 0;
    task_add(task);
    task_switchsub();
    asm_load_tr(task->sel);
    task_timer = timer_alloc();
    timer_settime(task_timer, task->priority);

    idle = task_alloc();
    idle->tss.esp = memory_management_alloc_4k(memory_management, 64 * 1024) + 64 * 1024;
    idle->tss.eip = (int)&task_idle;
    idle->tss.es = 1 * 8;
    idle->tss.cs = 2 * 8;
    idle->tss.ss = 1 * 8;
    idle->tss.ds = 1 * 8;
    idle->tss.fs = 1 * 8;
    idle->tss.gs = 1 * 8;
    task_run(idle, MAX_TASK_LEVELS - 1, 1);

    return task;
}

struct Task *task_alloc(void) {
    int i;
    struct Task *task;
    for (i = 0; i < MAX_TASKS; i++) {
        if (task_ctl->tasks0[i].flags == 0) {
            task = &task_ctl->tasks0[i];
            task->flags = 1;
            task->tss.eflags = 0x00000202;
            task->tss.eax = 0;
            task->tss.ecx = 0;
            task->tss.edx = 0;
            task->tss.ebx = 0;
            task->tss.ebp = 0;
            task->tss.esi = 0;
            task->tss.edi = 0;
            task->tss.es = 0;
            task->tss.ds = 0;
            task->tss.fs = 0;
            task->tss.gs = 0;
            task->tss.iomap = 0x40000000;
            task->tss.ss0 = 0;
            return task;
        }
    }
    return 0;
}

void task_run(struct Task *task, int level, int priority) {
    if (level < 0) {
        level = task->level;
    }
    if (priority > 0) {
        task->priority = priority;
    }

    if (task->flags == 2 && task->level != level) {
        task_remove(task);
    }
    if (task->flags != 2) {
        task->level = level;
        task_add(task);
    }

    task_ctl->lv_change = 1;
    return;
}

void task_sleep(struct Task *task) {
    struct Task *now_task;
    if (task->flags == 2) {
        now_task = task_now();
        task_remove(task);
        if (task == now_task) {
            task_switchsub();
            now_task = task_now();
            asm_far_jmp(0, now_task->sel);
        }
    }
    return;
}

void task_switch(void) {
    struct TaskLevel *tl = &task_ctl->level[task_ctl->now_lv];
    struct Task *new_task, *now_task = tl->tasks[tl->now];
    tl->now++;
    if (tl->now == tl->running) {
        tl->now = 0;
    }
    if (task_ctl->lv_change != 0) {
        task_switchsub();
        tl = &task_ctl->level[task_ctl->now_lv];
    }
    new_task = tl->tasks[tl->now];
    timer_settime(task_timer, new_task->priority);
    if (new_task != now_task) {
        asm_far_jmp(0, new_task->sel);
    }
    return;
}
