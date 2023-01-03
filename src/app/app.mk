OUT_DIR = ../../../target
LIB_PATH = $(OUT_DIR)/lib/
API_LIB_PATH   = $(OUT_DIR)/kernel/system_call/
KERNEL_PATH = $(OUT_DIR)/kernel/

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib
COSLD = -T hrb.ld
CAPPLD = -T ../app.ld
CC_WITH_OPTION = i386-elf-gcc -m32 -march=i486 -nostdlib

ifndef STACK
	STACK = 0x500
endif

default :
	$(MAKE) $(APP).hrb

$(APP).hrb : $(APP).o $(LIB_PATH)libstdio.a $(LIB_PATH)libstring.a $(LIB_PATH)libstdlib.a $(API_LIB_PATH)libapi.a ../app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -Wl,'--defsym=__stack=$(STACK)' -g -o $@ $< $(LIB_PATH)libstdio.a $(LIB_PATH)libstring.a $(LIB_PATH)libstdlib.a $(API_LIB_PATH)libapi.a

# Image
$(OUT_DIR)/haribote.img : $(KERNEL_PATH)ipl10.bin $(KERNEL_PATH)haribote.sys $(APP).hrb
	mformat -f 1440 -C -B $(KERNEL_PATH)ipl10.bin -i $(OUT_DIR)/haribote.img ::
	mcopy -i $(OUT_DIR)/haribote.img $(KERNEL_PATH)haribote.sys $(APP).hrb ../nihongo/nihongo.fnt ::

# Rule
%.o : %.c
	$(CC) $(CFLAGS) -g -c $*.c -o $*.o

# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
%.o : %.nasm
	nasm -g -f elf $*.nasm -o $*.o

# Commands
run :
	$(MAKE) $(OUT_DIR)/haribote.img
	qemu-system-i386 -drive file=$(OUT_DIR)/haribote.img,format=raw,if=floppy -boot a

full :
	$(MAKE) -C $(API_LIB_PATH)
	$(MAKE) $(APP).hrb

run_full :
	$(MAKE) -C $(API_LIB_PATH)
	$(MAKE) -C ../haribote
	$(MAKE) run

clean :
	-$(DEL) *.o
	-$(DEL) *.lst
	-$(DEL) *.map
	-$(DEL) $(OUT_DIR)/haribote.img

src_only :
	$(MAKE) clean
	-$(DEL) $(APP).hrb
