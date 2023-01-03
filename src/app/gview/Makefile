APP = gview
STACK = 0x445C00
# 4480k(4480000) = 0x445C00

OUT_DIR = ../../../target/
LIBPATH = ../../lib/
APILIBPATH   = $(OUT_DIR)/kernel/system_call/
KERNEL_PATH = $(OUT_DIR)/kernel/

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib
COSLD = -T hrb.ld
CAPPLD = -T ../app.ld
CC_WITH_OPTION = i386-elf-gcc -m32 -march=i486 -nostdlib

default :
	mkdir -p $(OUT_DIR)/app/$(APP)/obj
	$(MAKE) $(OUT_DIR)/app/$(APP)/$(APP).hrb

$(OUT_DIR)/app/$(APP)/$(APP).hrb : $(OUT_DIR)/app/$(APP)/obj/$(APP).o $(OUT_DIR)/app/$(APP)/obj/bmp.o $(OUT_DIR)/app/$(APP)/obj/jpeg.o $(APILIBPATH)libapi.a ../app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -Wl,'--defsym=__stack=$(STACK)' -g -o $@ $< $(OUT_DIR)/app/$(APP)/obj/bmp.o $(OUT_DIR)/app/$(APP)/obj/jpeg.o $(APILIBPATH)libapi.a

# Rule
$(OUT_DIR)/app/$(APP)/obj/%.o : %.c
	$(CC) $(CFLAGS) -g -c $*.c -o $(OUT_DIR)/app/$(APP)/obj/$*.o

# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
$(OUT_DIR)/app/$(APP)/obj/%.o : %.nasm
	nasm -g -f elf $*.nasm -o $(OUT_DIR)/app/$(APP)/obj/$*.o

# Commands

src_only :
	-$(DEL) $(OUT_DIR)/app/$(APP)
