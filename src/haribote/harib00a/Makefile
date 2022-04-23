MAKE = make -r
DEL = rm -f

ipl.bin: ipl.nas Makefile
	nasm ipl.nas -o ipl.bin -l ipl.lst

haribote.img : ipl.bin Makefile
	mformat -f 1440 -C -B ipl.bin -i haribote.img ::

run:
	$(MAKE) haribote.img
	qemu-system-x86_64 -fda haribote.img

clean:
	-$(DEL) ipl.bin
	-$(DEL) ipl.lst
	-$(DEL) haribote.img