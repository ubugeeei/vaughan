LIBPATH = ../lib/

OBJS_BOOTPACK = boot.o naskfunc.o hankaku.o graphic.o dsctbl.o int.o queue.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o $(LIBPATH)libstdio.a $(LIBPATH)libstring.a

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib

default :
	$(MAKE) ipl20.bin
	$(MAKE) haribote.sys

ipl20.bin : ipl20.nasm
	nasm $< -o $@ -l ipl20.lst

asmhead.bin : asmhead.nasm
	nasm $< -o $@ -l asmhead.lst

hankaku_txt_compiler : hankaku_txt_compiler.c
	gcc $< -o $@

hankaku.c : hankaku.txt hankaku_txt_compiler
	./hankaku_txt_compiler

# https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
boot.hrb : $(OBJS_BOOTPACK) hrb.ld
	$(CC) $(CFLAGS) $(COPTION) -T hrb.ld -Xlinker -Map=boot.map -g $(OBJS_BOOTPACK) -o $@

haribote.sys : asmhead.bin boot.hrb
	cat asmhead.bin boot.hrb > haribote.sys

# Rule

# C -> Object
%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

# NASM -> Object
# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
%.o : %.nasm
	nasm -g -f elf $*.nasm -o $*.o

# Remove
clean :
	-$(DEL) asmhead.bin
	-$(DEL) hankaku_txt_compiler
	-$(DEL) hankaku.c
	-$(DEL) *.lst
	-$(DEL) *.o
	-$(DEL) *.sys
	-$(DEL) *.hrb
	-$(DEL) *.map
	
src_only :
	$(MAKE) clean
	-$(DEL) ipl20.bin
	-$(DEL) haribote.sys
