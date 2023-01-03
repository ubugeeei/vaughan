OUT_DIR = ../../target
MAKE = make -r
DEL = rm -f


CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib
AR = i386-elf-ar

default :
	mkdir -p $(OUT_DIR)/lib/obj
	$(MAKE) $(OUT_DIR)/lib/libstdio.a
	$(MAKE) $(OUT_DIR)/lib/libstring.a
	$(MAKE) $(OUT_DIR)/lib/libstdlib.a

$(OUT_DIR)/lib/libstdio.a : $(OUT_DIR)/lib/obj/sprintf.o
	$(AR) rcs $@ $^

$(OUT_DIR)/lib/libstring.a : $(OUT_DIR)/lib/obj/strcmp.o $(OUT_DIR)/lib/obj/strncmp.o $(OUT_DIR)/lib/obj/strlen.o $(OUT_DIR)/lib/obj/memcmp.o
	$(AR) rcs $@ $^

$(OUT_DIR)/lib/libstdlib.a : $(OUT_DIR)/lib/obj/strtoul0.o $(OUT_DIR)/lib/obj/strtol.o
	$(AR) rcs $@ $^

# C -> Object
$(OUT_DIR)/lib/obj/%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $(OUT_DIR)/lib/obj/$*.o

# Remove
clean :
	-$(DEL) $(OUT_DIR)/lib/obj/*.o

src_only :
	$(MAKE) clean
	-$(DEL) $(OUT_DIR)/lib/*.a
