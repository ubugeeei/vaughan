helloos.bin: helloos.nas Makefile
	nasm helloos.nas -o helloos.bin -l helloos.lst

helloos.img: helloos.bin Makefile
	mformat -f 1440 -C -B helloos.bin -i helloos.img ::

img:
	make -r helloos.img

asm:
	make -r helloos.bin

run:
	make img
	qemu-system-x86_64 -fda helloos.img