# ========= Alias =========
# paths
OUT_DIR = target
OBJ_DIR = $(OUT_DIR)/obj
BIN_DIR = $(OUT_DIR)/bin
LIB_BIN = $(BIN_DIR)/hrbapi.lib

# command alias
MAKE = make -r
PRE_MAKE = \
	mkdir -p $(BIN_DIR) &&\
	mkdir -p $(OBJ_DIR)/kernel/system_call &&\
	mkdir -p $(OBJ_DIR)/lib &&\
	mkdir -p $(OBJ_DIR)/app &&\
	mkdir -p $(OUT_DIR)/kernel &&\
	mkdir -p $(OUT_DIR)/app
DEL = rm -rf
CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
CFLAGS2 = -march=i486 -nostdlib
LINK_OPTION = -T src/app/app.ld
LINK_OPTION2 = -T src/app/app2.ld

# files
OBJS_boot = \
	$(OBJ_DIR)/kernel/boot.o \
	$(OBJ_DIR)/kernel/naskfunc.o \
	$(OBJ_DIR)/kernel/graphic.o \
	$(OBJ_DIR)/kernel/dsctbl.o \
	$(OBJ_DIR)/kernel/int.o \
	$(OBJ_DIR)/kernel/queue.o \
	$(OBJ_DIR)/kernel/keyboard.o \
	$(OBJ_DIR)/kernel/mouse.o \
	$(OBJ_DIR)/kernel/memory.o \
	$(OBJ_DIR)/kernel/sheet.o \
	$(OBJ_DIR)/kernel/timer.o \
	$(OBJ_DIR)/kernel/mtask.o \
	$(OBJ_DIR)/kernel/window.o \
	$(OBJ_DIR)/kernel/console.o \
	$(OBJ_DIR)/kernel/file.o \
	$(OBJ_DIR)/lib/sprintf.o \
	$(OBJ_DIR)/lib/strcmp.o \
	$(OBJ_DIR)/lib/hankaku.o

SYS_CALL_OBJS = \
	$(OBJ_DIR)/kernel/system_call/api001.o \
	$(OBJ_DIR)/kernel/system_call/api002.o \
	$(OBJ_DIR)/kernel/system_call/api003.o \
	$(OBJ_DIR)/kernel/system_call/api004.o \
	$(OBJ_DIR)/kernel/system_call/api005.o \
	$(OBJ_DIR)/kernel/system_call/api006.o \
	$(OBJ_DIR)/kernel/system_call/api007.o \
	$(OBJ_DIR)/kernel/system_call/api008.o \
	$(OBJ_DIR)/kernel/system_call/api009.o \
	$(OBJ_DIR)/kernel/system_call/api010.o \
	$(OBJ_DIR)/kernel/system_call/api011.o \
	$(OBJ_DIR)/kernel/system_call/api012.o \
	$(OBJ_DIR)/kernel/system_call/api013.o \
	$(OBJ_DIR)/kernel/system_call/api014.o \
	$(OBJ_DIR)/kernel/system_call/api015.o \
	$(OBJ_DIR)/kernel/system_call/api016.o \
	$(OBJ_DIR)/kernel/system_call/api017.o \
	$(OBJ_DIR)/kernel/system_call/api018.o \
	$(OBJ_DIR)/kernel/system_call/api019.o \
	$(OBJ_DIR)/kernel/system_call/api020.o

APPS = \
	$(OUT_DIR)/app/a.hrb \
	$(OUT_DIR)/app/hello.hrb \
	$(OUT_DIR)/app/hello2.hrb \
	$(OUT_DIR)/app/hello3.hrb \
	$(OUT_DIR)/app/hello4.hrb \
	$(OUT_DIR)/app/hello5.hrb \
	$(OUT_DIR)/app/walk.hrb \
	$(OUT_DIR)/app/win.hrb \
	$(OUT_DIR)/app/win2.hrb \
	$(OUT_DIR)/app/win3.hrb \
	$(OUT_DIR)/app/star1.hrb \
	$(OUT_DIR)/app/star2.hrb \
	$(OUT_DIR)/app/stars.hrb \
	$(OUT_DIR)/app/lines.hrb \
	$(OUT_DIR)/app/noodle.hrb \
	$(OUT_DIR)/app/beepdown.hrb \
	$(OUT_DIR)/app/color.hrb \
	$(OUT_DIR)/app/color2.hrb \
	$(OUT_DIR)/app/crack7.hrb



# ========= Commands =========
default : 
	$(MAKE) img

img :
	$(PRE_MAKE)
	$(MAKE) $(OUT_DIR)/haribote.img

run :
	$(PRE_MAKE)
	$(MAKE) img
	qemu-system-i386 -drive file=$(OUT_DIR)/haribote.img,format=raw,if=floppy -boot a -m 1G

src_only :
	-$(DEL) $(OUT_DIR)/*



# ========= Build =========
# OS img
target/haribote.img : $(BIN_DIR)/ipl10.bin $(OUT_DIR)/haribote.sys $(APPS) Makefile
	mformat -f 1440 -C -B $(BIN_DIR)/ipl10.bin -i $@ ::
	mcopy -i $(OUT_DIR)/haribote.img $(OUT_DIR)/haribote.sys $(APPS) static/main.c ::

# compile libs
$(BIN_DIR)/hankaku_txt_compiler : src/lib/hankaku_txt_compiler.c
	gcc $< -o $@

src/lib/hankaku.c : static/hankaku.txt $(BIN_DIR)/hankaku_txt_compiler
	./$(BIN_DIR)/hankaku_txt_compiler

$(OBJ_DIR)/lib/%.o : src/lib/%.c
	$(CC) $(CFLAGS) -c src/lib/$*.c -o $(OBJ_DIR)/lib/$*.o

# Kernel Build
$(BIN_DIR)/ipl10.bin : src/kernel/asm/ipl10.nasm
	nasm $< -o $@ -l $(OUT_DIR)/kernel/ipl10.lst

$(BIN_DIR)/asmhead.bin : src/kernel/asm/asmhead.nasm
	nasm $< -o $@ -l $(OUT_DIR)/kernel/asmhead.lst

$(OBJ_DIR)/kernel/naskfunc.o : src/kernel/asm/naskfunc.nasm 
	nasm -g -f elf $< -o $@ -l $(OUT_DIR)/kernel/naskfunc.lst

target/boot.hrb : $(OBJS_boot) hrb.ld
	$(CC) $(CFLAGS) $(CFLAGS2) -T hrb.ld -Xlinker -Map=$(OUT_DIR)/boot.map -g $(OBJS_boot) -o $@

target/haribote.sys : $(BIN_DIR)/asmhead.bin $(OUT_DIR)/boot.hrb Makefile
	cat $(BIN_DIR)/asmhead.bin $(OUT_DIR)/boot.hrb > $(OUT_DIR)/haribote.sys

$(OBJ_DIR)/kernel/%.o : src/kernel/%.c
	$(CC) $(CFLAGS) -c src/kernel/$*.c -o $(OBJ_DIR)/kernel/$*.o

$(OBJ_DIR)/kernel/system_call/%.o : src/kernel/asm/system_call/%.nasm	
	nasm -g -f elf src/kernel/asm/system_call/$*.nasm -o $(OBJ_DIR)/kernel/system_call/$*.o -l $(OUT_DIR)/kernel/$*.lst

# Libraies Build
$(LIB_BIN) : $(SYS_CALL_OBJS)
	i386-elf-ar rcs $@ $^

#  Application Build
$(OUT_DIR)/app/hello.hrb : src/app/hello.nasm
	nasm $< -o $@ -l hello.lst

$(OUT_DIR)/app/hello2.hrb : src/app/hello2.nasm
	nasm $< -o $@ -l hello2.lst

$(OUT_DIR)/app/hello5.hrb : $(OBJ_DIR)/app/hello5.o src/app/app.ld
	$(CC) $(CFLAGS) $(CFLAGS2) $(LINK_OPTION) -g $(OBJ_DIR)/app/hello5.o -o $@

$(OUT_DIR)/app/noodle.hrb : $(OBJ_DIR)/app/noodle.o $(LIB_BIN) $(OBJ_DIR)/lib/sprintf.o src/app/app2.ld
	$(CC) $(CFLAGS) $(CFLAGS2) $(LINK_OPTION2) -g $(OBJ_DIR)/app/noodle.o $(LIB_BIN) $(OBJ_DIR)/lib/sprintf.o -o $@

$(OUT_DIR)/app/win3.hrb : $(OBJ_DIR)/app/win3.o $(LIB_BIN) src/app/app.ld
	$(CC) $(CFLAGS) $(CFLAGS2) $(LINK_OPTION) -Os -fdata-sections -ffunction-sections -g $(OBJ_DIR)/app/win3.o $(LIB_BIN) -o $@ -Wl,--gc-sections
# template
$(OUT_DIR)/app/%.hrb : $(OBJ_DIR)/app/%.o $(LIB_BIN) src/app/app.ld
	$(CC) $(CFLAGS) $(CFLAGS2) $(LINK_OPTION) -g $(OBJ_DIR)/app/$*.o $(LIB_BIN) -o $@

$(OBJ_DIR)/app/%.o : src/app/%.c
	$(CC) $(CFLAGS) -c src/app/$*.c -o $(OBJ_DIR)/app/$*.o

$(OBJ_DIR)/app/%.o : src/app/%.nasm
	nasm -g -f elf src/app/$*.nasm -o $(OBJ_DIR)/app/$*.o -l $(OUT_DIR)/app/$*.lst



