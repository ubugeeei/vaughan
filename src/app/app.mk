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
	mkdir -p $(OUT_DIR)/app/$(APP)/obj
	$(MAKE) $(OUT_DIR)/app/$(APP)/$(APP).hrb

$(OUT_DIR)/app/$(APP)/$(APP).hrb : $(OUT_DIR)/app/$(APP)/obj/$(APP).o $(LIB_PATH)libstdio.a $(LIB_PATH)libstring.a $(LIB_PATH)libstdlib.a $(API_LIB_PATH)libapi.a ../app.ld
	mkdir -p $(OUT_DIR)/app/$(APP)/obj
	$(CC_WITH_OPTION) $(CAPPLD) -Wl,'--defsym=__stack=$(STACK)' -g -o $@ $< $(LIB_PATH)libstdio.a $(LIB_PATH)libstring.a $(LIB_PATH)libstdlib.a $(API_LIB_PATH)libapi.a

# Rule
$(OUT_DIR)/app/$(APP)/obj/%.o : %.c
	$(CC) $(CFLAGS) -g -c $*.c -o $(OUT_DIR)/app/$(APP)/obj/$*.o

# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
$(OUT_DIR)/app/$(APP)/obj/%.o : %.nasm
	nasm -g -f elf $*.nasm -o $(OUT_DIR)/app/$(APP)/obj/$*.o

src_only :
	-$(DEL) $(OUT_DIR)/app/$(APP)
