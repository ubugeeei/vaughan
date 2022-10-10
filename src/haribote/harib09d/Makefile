OBJS_BOOTPACK = $(OBJ_PATH)/bootpack.o $(OBJ_PATH)/naskfunc.o $(OBJ_PATH)/hankaku.o $(OBJ_PATH)/graphic.o $(OBJ_PATH)/dsctbl.o $(OBJ_PATH)/mysprintf.o $(OBJ_PATH)/int.o $(OBJ_PATH)/fifo.o $(OBJ_PATH)/keyboard.o $(OBJ_PATH)/mouse.o $(OBJ_PATH)/memory.o $(OBJ_PATH)/sheet.o $(OBJ_PATH)/timer.o

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin

TARGET_PATH = ./target
OBJ_PATH = ./target/obj
BIN_PATH = ./target/bin

default : 
	$(MAKE) img

target/bin/hankaku_txt_compiler : src/hankaku_txt_compiler.c
	gcc $< -o $@

src/hankaku.c : static/hankaku.txt $(BIN_PATH)/hankaku_txt_compiler
	./$(BIN_PATH)/hankaku_txt_compiler

target/bin/ipl10.bin : src/asm/ipl10.nas
	nasm $< -o $@ -l $(TARGET_PATH)/ipl10.lst

target/bin/asmhead.bin : src/asm/asmhead.nas
	nasm $< -o $@ -l $(TARGET_PATH)/asmhead.lst

$(OBJ_PATH)/naskfunc.o : src/asm/naskfunc.nas 
	nasm -g -f elf $< -o $@ -l $(TARGET_PATH)/naskfunc.lst

target/bootpack.hrb : $(OBJS_BOOTPACK) hrb.ld
	$(CC) $(CFLAGS) -march=i486 -nostdlib -T hrb.ld -g $(OBJS_BOOTPACK) -o $@

target/haribote.sys : $(TARGET_PATH)/bin/asmhead.bin $(TARGET_PATH)/bootpack.hrb Makefile
	cat $(TARGET_PATH)/bin/asmhead.bin $(TARGET_PATH)/bootpack.hrb > $(TARGET_PATH)/haribote.sys

target/haribote.img : $(TARGET_PATH)/bin/ipl10.bin $(TARGET_PATH)/haribote.sys Makefile
	mformat -f 1440 -C -B $(TARGET_PATH)/bin/ipl10.bin -i $@ ::
	mcopy -i $@ $(TARGET_PATH)/haribote.sys ::


$(OBJ_PATH)/%.o : src/%.c
	$(CC) $(CFLAGS) -c src/$*.c -o $(OBJ_PATH)/$*.o


img :
	$(MAKE) $(TARGET_PATH)/haribote.img

run :
	$(MAKE) img
	qemu-system-i386 -drive file=$(TARGET_PATH)/haribote.img,format=raw,if=floppy -boot a

src_only :
	-$(DEL) $(TARGET_PATH)/*
