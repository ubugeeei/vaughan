OUT_DIR = ../../target

OBJS_BOOTPACK = \
	$(OUT_DIR)/kernel/obj/boot.o \
	$(OUT_DIR)/kernel/obj/naskfunc.o \
	$(OUT_DIR)/kernel/obj/hankaku.o \
	$(OUT_DIR)/kernel/obj/graphic.o \
	$(OUT_DIR)/kernel/obj/dsctbl.o \
	$(OUT_DIR)/kernel/obj/int.o \
	$(OUT_DIR)/kernel/obj/queue.o \
	$(OUT_DIR)/kernel/obj/keyboard.o \
	$(OUT_DIR)/kernel/obj/mouse.o \
	$(OUT_DIR)/kernel/obj/memory.o \
	$(OUT_DIR)/kernel/obj/sheet.o \
	$(OUT_DIR)/kernel/obj/timer.o \
	$(OUT_DIR)/kernel/obj/mtask.o \
	$(OUT_DIR)/kernel/obj/window.o \
	$(OUT_DIR)/kernel/obj/console.o \
	$(OUT_DIR)/kernel/obj/file.o \
	$(OUT_DIR)/kernel/obj/tek.o \
	$(OUT_DIR)/lib/libstdio.a \
	$(OUT_DIR)/lib/libstring.a \
	$(OUT_DIR)/lib/libstdlib.a \
	$(OUT_DIR)/kernel/rust/rustlib.a # FIXME: いい感じにリンクできない

PREMAKE = \
	mkdir -p $(OUT_DIR)/kernel/obj &&\
	mkdir -p $(OUT_DIR)/kernel/bin &&\
	mkdir -p $(OUT_DIR)/lib


MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
# FIXME: いい感じにリンクできない
# CFLAGS = -m32 -fno-builtin -L$(OUT_DIR)/kernel/rust/rustlib.a
CFLAGS = -m32 -fno-builtin
# COPTION = -march=i486 -nostdlib -L $(OUT_DIR)/kernel/rust/rustlib.a
COPTION = -march=i486 -nostdlib

default :
	$(PREMAKE)
	$(MAKE) -C rust
	$(MAKE) $(OUT_DIR)/kernel/ipl10.bin
	$(MAKE) $(OUT_DIR)/kernel/vaughan.sys

$(OUT_DIR)/kernel/ipl10.bin : ipl10.nasm
	nasm $< -o $@

$(OUT_DIR)/kernel/bin/asmhead.bin : asmhead.nasm
	nasm $< -o $@

$(OUT_DIR)/kernel/bin/hankaku_txt_compiler : hankaku_txt_compiler.c
	gcc $< -o $@

hankaku.c : ../../static/hankaku.txt $(OUT_DIR)/kernel/bin/hankaku_txt_compiler
	$(OUT_DIR)/kernel/bin/hankaku_txt_compiler

# https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
$(OUT_DIR)/kernel/bin/boot.hrb : $(OBJS_BOOTPACK) hrb.ld
	$(CC) $(CFLAGS) $(COPTION) -T hrb.ld -Xlinker -Map=$(OUT_DIR)/kernel/boot.map -g $(OBJS_BOOTPACK) -o $@

$(OUT_DIR)/kernel/vaughan.sys : $(OUT_DIR)/kernel/bin/asmhead.bin $(OUT_DIR)/kernel/bin/boot.hrb
	cat $(OUT_DIR)/kernel/bin/asmhead.bin $(OUT_DIR)/kernel/bin/boot.hrb > $(OUT_DIR)/kernel/vaughan.sys

# Rule

# C -> Object
$(OUT_DIR)/kernel/obj/%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $(OUT_DIR)/kernel/obj/$*.o

# NASM -> Object
# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
$(OUT_DIR)/kernel/obj/%.o : %.nasm
	nasm -g -f elf $*.nasm -o $(OUT_DIR)/kernel/obj/$*.o

# Remove
clean :
	-$(DEL) $(OUT_DIR)/kernel/bin/asmhead.bin
	-$(DEL) $(OUT_DIR)/kernel/bin/hankaku_txt_compiler
	-$(DEL) hankaku.c
	-$(DEL) $(OUT_DIR)/kernel/obj/*.o
	-$(DEL) $(OUT_DIR)/kernel/bin/*.sys
	-$(DEL) $(OUT_DIR)/kernel/bin/*.hrb
	-$(DEL) $(OUT_DIR)/kernel/*.map
	
src_only :
	$(MAKE) clean
	-$(DEL) $(OUT_DIR)/kernel/ipl10.bin
	-$(DEL) $(OUT_DIR)/kernel/vaughan.sys
