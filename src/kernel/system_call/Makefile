OUT_DIR = ../../../target
OBJS_API = \
	$(OUT_DIR)/kernel/system_call/obj/api001.o \
	$(OUT_DIR)/kernel/system_call/obj/api002.o \
	$(OUT_DIR)/kernel/system_call/obj/api003.o \
	$(OUT_DIR)/kernel/system_call/obj/api004.o \
	$(OUT_DIR)/kernel/system_call/obj/api005.o \
	$(OUT_DIR)/kernel/system_call/obj/api006.o \
	$(OUT_DIR)/kernel/system_call/obj/api007.o \
	$(OUT_DIR)/kernel/system_call/obj/api008.o \
	$(OUT_DIR)/kernel/system_call/obj/api009.o \
	$(OUT_DIR)/kernel/system_call/obj/api010.o \
	$(OUT_DIR)/kernel/system_call/obj/api011.o \
	$(OUT_DIR)/kernel/system_call/obj/api012.o \
	$(OUT_DIR)/kernel/system_call/obj/api013.o \
	$(OUT_DIR)/kernel/system_call/obj/api014.o \
	$(OUT_DIR)/kernel/system_call/obj/api015.o \
	$(OUT_DIR)/kernel/system_call/obj/api016.o \
	$(OUT_DIR)/kernel/system_call/obj/api017.o \
	$(OUT_DIR)/kernel/system_call/obj/api018.o \
	$(OUT_DIR)/kernel/system_call/obj/api019.o \
	$(OUT_DIR)/kernel/system_call/obj/api020.o \
	$(OUT_DIR)/kernel/system_call/obj/api021.o \
	$(OUT_DIR)/kernel/system_call/obj/api022.o \
	$(OUT_DIR)/kernel/system_call/obj/api023.o \
	$(OUT_DIR)/kernel/system_call/obj/api024.o \
	$(OUT_DIR)/kernel/system_call/obj/api025.o \
	$(OUT_DIR)/kernel/system_call/obj/api026.o \
	$(OUT_DIR)/kernel/system_call/obj/api027.o \
	$(OUT_DIR)/kernel/system_call/obj/alloca.o
MAKE = make -r
DEL = rm -f

default :
	mkdir -p $(OUT_DIR)/kernel/system_call/obj
	$(MAKE) $(OUT_DIR)/kernel/system_call/libapi.a

# Library
# r: replace files existing inside the archive
# c: create a archive if not already existent
# s: create an object-file index into the archive
$(OUT_DIR)/kernel/system_call/libapi.a : $(OBJS_API)
	i386-elf-ar rcs $@ $(OUT_DIR)/kernel/system_call/$^

# NASM -> Object
# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
$(OUT_DIR)/kernel/system_call/obj/%.o : %.nasm
	nasm -g -f elf $*.nasm -o $(OUT_DIR)/kernel/system_call/obj/$*.o

# Remove files
clean : 
	-$(DEL) $(OUT_DIR)/kernel/system_call/obj/*.o

src_only :
	$(MAKE) clean
	-$(DEL) $(OUT_DIR)/kernel/system_call/libapi.a
