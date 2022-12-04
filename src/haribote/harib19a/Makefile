# command alias
MAKE = make -r
PRE_MAKE = mkdir -p target/bin && mkdir -p target/obj/lib && mkdir -p target/obj/app && mkdir -p target/app
DEL = rm -rf
CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin


# paths
TARGET_PATH = ./target
OBJ_PATH = ./target/obj
BIN_PATH = ./target/bin


# files
OBJS_boot = $(OBJ_PATH)/boot.o \
	$(OBJ_PATH)/naskfunc.o \
	$(OBJ_PATH)/graphic.o \
	$(OBJ_PATH)/dsctbl.o \
	$(OBJ_PATH)/int.o \
	$(OBJ_PATH)/queue.o \
	$(OBJ_PATH)/keyboard.o \
	$(OBJ_PATH)/mouse.o \
	$(OBJ_PATH)/memory.o \
	$(OBJ_PATH)/sheet.o \
	$(OBJ_PATH)/timer.o \
	$(OBJ_PATH)/mtask.o \
	$(OBJ_PATH)/window.o \
	$(OBJ_PATH)/console.o \
	$(OBJ_PATH)/file.o \
	$(OBJ_PATH)/lib/sprintf.o \
	$(OBJ_PATH)/lib/strcmp.o \
	$(OBJ_PATH)/lib/hankaku.o

APPS = $(TARGET_PATH)/app/hello.hrb \
		$(TARGET_PATH)/app/hello2.hrb \
		$(TARGET_PATH)/app/a.hrb \
		$(TARGET_PATH)/app/hello3.hrb \
		$(TARGET_PATH)/app/crack1.hrb \
		$(TARGET_PATH)/app/crack2.hrb \
		$(TARGET_PATH)/app/crack3.hrb \
		$(TARGET_PATH)/app/crack4.hrb \
		$(TARGET_PATH)/app/crack5.hrb \
		$(TARGET_PATH)/app/crack6.hrb \


# Commands
default : 
	$(MAKE) img

img :
	$(PRE_MAKE)
	$(MAKE) $(TARGET_PATH)/haribote.img

run :
	$(PRE_MAKE)
	$(MAKE) img
	qemu-system-i386 -drive file=$(TARGET_PATH)/haribote.img,format=raw,if=floppy -boot a -m 1G

src_only :
	-$(DEL) $(TARGET_PATH)/*



# os img
target/haribote.img : $(TARGET_PATH)/bin/ipl10.bin $(TARGET_PATH)/haribote.sys $(APPS) Makefile
	mformat -f 1440 -C -B $(TARGET_PATH)/bin/ipl10.bin -i $@ ::
	mcopy -i $(TARGET_PATH)/haribote.img $(TARGET_PATH)/haribote.sys $(APPS) static/main.c ::



# libs
target/bin/hankaku_txt_compiler : src/lib/hankaku_txt_compiler.c
	gcc $< -o $@
src/lib/hankaku.c : static/hankaku.txt $(BIN_PATH)/hankaku_txt_compiler
	./$(BIN_PATH)/hankaku_txt_compiler
$(OBJ_PATH)/lib/%.o : src/lib/%.c
	$(CC) $(CFLAGS) -c src/lib/$*.c -o $(OBJ_PATH)/lib/$*.o



# Build
target/bin/ipl10.bin : src/asm/ipl10.nas
	nasm $< -o $@ -l $(TARGET_PATH)/ipl10.lst

target/bin/asmhead.bin : src/asm/asmhead.nas
	nasm $< -o $@ -l $(TARGET_PATH)/asmhead.lst

$(OBJ_PATH)/naskfunc.o : src/asm/naskfunc.nas 
	nasm -g -f elf $< -o $@ -l $(TARGET_PATH)/naskfunc.lst

target/boot.hrb : $(OBJS_boot) hrb.ld
	$(CC) $(CFLAGS) -march=i486 -nostdlib -T hrb.ld -Xlinker -Map=$(TARGET_PATH)/boot.map -g $(OBJS_boot) -o $@

target/haribote.sys : $(TARGET_PATH)/bin/asmhead.bin $(TARGET_PATH)/boot.hrb Makefile
	cat $(TARGET_PATH)/bin/asmhead.bin $(TARGET_PATH)/boot.hrb > $(TARGET_PATH)/haribote.sys

$(OBJ_PATH)/%.o : src/%.c
	$(CC) $(CFLAGS) -c src/$*.c -o $(OBJ_PATH)/$*.o

#  Application
$(TARGET_PATH)/app/%.hrb : $(OBJ_PATH)/app/%.o $(OBJ_PATH)/app/a_nask.o src/app/app.ld
	$(CC) $(CFLAGS) -march=i486 -nostdlib -T src/app/app.ld -g $(OBJ_PATH)/app/$*.o $(OBJ_PATH)/app/a_nask.o -o $@

$(TARGET_PATH)/app/%.hrb : src/app/%.nas	
	nasm $< -o $@ -l $(TARGET_PATH)/app/$*.lst

$(OBJ_PATH)/app/%.o : src/app/%.c
	$(CC) $(CFLAGS) -c src/app/$*.c -o $(OBJ_PATH)/app/$*.o

$(OBJ_PATH)/app/%.o : src/app/%.nas
	nasm -g -f elf src/app/$*.nas -o $(OBJ_PATH)/app/$*.o -l $(TARGET_PATH)/app/$*.lst