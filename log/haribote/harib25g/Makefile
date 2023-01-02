APPS =\
	a/a.hrb \
	hello3/hello3.hrb \
	hello4/hello4.hrb \
	hello5/hello5.hrb \
	win/win.hrb \
	win2/win2.hrb \
	win3/win3.hrb \
	star1/star1.hrb \
	stars/stars.hrb \
	star2/star2.hrb \
	lines/lines.hrb \
	walk/walk.hrb \
	noodle/noodle.hrb \
	beepdown/beepdown.hrb \
	color/color.hrb \
	color2/color2.hrb \
	sosu/sosu.hrb \
	sosu2/sosu2.hrb \
	sosu3/sosu3.hrb \
	typea/typea.hrb \
	cat/cat.hrb \
	iroha/iroha.hrb

MAKE = make -r
DEL = rm -f

default : 
	$(MAKE) haribote.img

# mtool: https://www.gnu.org/software/mtools/manual/html_node/mformat.html
# -f: size, -C: hidden_sectors, -B: boot_sector, -i: 
haribote.img : haribote/ipl10.bin haribote/haribote.sys $(APPS)
	mformat -f 1440 -C -B haribote/ipl10.bin -i haribote.img ::
	mcopy -i haribote.img haribote/haribote.sys haribote/ipl10.nasm main.c $(APPS) font/jp.fnt ::

# Commands
run :
	$(MAKE) haribote.img
	qemu-system-i386 -drive file=haribote.img,format=raw,if=floppy -boot a

# install :

full :
	$(MAKE) -C lib
	$(MAKE) -C haribote
	$(MAKE) -C apilib
	$(MAKE) -C a
	$(MAKE) -C hello3
	$(MAKE) -C hello4
	$(MAKE) -C hello5
	$(MAKE) -C win
	$(MAKE) -C win2
	$(MAKE) -C win3
	$(MAKE) -C star1
	$(MAKE) -C stars
	$(MAKE) -C star2
	$(MAKE) -C lines
	$(MAKE) -C walk
	$(MAKE) -C noodle
	$(MAKE) -C beepdown
	$(MAKE) -C color
	$(MAKE) -C color2
	$(MAKE) -C sosu
	$(MAKE) -C sosu2
	$(MAKE) -C sosu3
	$(MAKE) -C typea
	$(MAKE) -C cat
	$(MAKE) -C iroha
	$(MAKE) haribote.img

run_full :
	$(MAKE) full
	qemu-system-i386 -drive file=haribote.img,format=raw,if=floppy -boot a

# install_full :

run_os :
	$(MAKE) -C lib
	$(MAKE) -C haribote
	$(MAKE) run

clean :

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img

clean_full :
	$(MAKE) -C lib clean
	$(MAKE) -C haribote clean
	$(MAKE) -C apilib clean
	$(MAKE) -C a clean
	$(MAKE) -C hello3 clean
	$(MAKE) -C hello4 clean
	$(MAKE) -C hello5 clean
	$(MAKE) -C win clean
	$(MAKE) -C win2 clean
	$(MAKE) -C win3 clean
	$(MAKE) -C star1 clean
	$(MAKE) -C stars clean
	$(MAKE) -C star2 clean
	$(MAKE) -C lines clean
	$(MAKE) -C walk	 clean
	$(MAKE) -C noodle clean
	$(MAKE) -C beepdown clean
	$(MAKE) -C color clean
	$(MAKE) -C color2 clean
	$(MAKE) -C sosu clean
	$(MAKE) -C sosu2 clean
	$(MAKE) -C sosu3 clean
	$(MAKE) -C typea clean
	$(MAKE) -C cat clean
	$(MAKE) -C iroha clean

src_only_full :
	$(MAKE) -C lib src_only
	$(MAKE) -C haribote src_only
	$(MAKE) -C apilib src_only
	$(MAKE) -C a src_only
	$(MAKE) -C hello3 src_only
	$(MAKE) -C hello4 src_only
	$(MAKE) -C hello5 src_only
	$(MAKE) -C win src_only
	$(MAKE) -C win2 src_only
	$(MAKE) -C win3 src_only
	$(MAKE) -C star1 src_only
	$(MAKE) -C stars src_only
	$(MAKE) -C star2 src_only
	$(MAKE) -C lines src_only
	$(MAKE) -C walk	 src_only
	$(MAKE) -C noodle src_only
	$(MAKE) -C beepdown src_only
	$(MAKE) -C color src_only
	$(MAKE) -C color2 src_only
	$(MAKE) -C sosu src_only
	$(MAKE) -C sosu2 src_only
	$(MAKE) -C sosu3 src_only
	$(MAKE) -C typea src_only
	$(MAKE) -C cat src_only
	$(MAKE) -C iroha src_only
	-$(DEL) haribote.img

refresh :
	$(MAKE) full
	$(MAKE) clean_full
	-$(DEL) haribote.img
