APPS = \
	app/a/a.hrb \
	app/hello3/hello3.hrb \
	app/hello4/hello4.hrb \
	app/hello5/hello5.hrb \
	app/win/win.hrb \
	app/win2/win2.hrb \
	app/win3/win3.hrb \
	app/star1/star1.hrb \
	app/stars/stars.hrb \
	app/star2/star2.hrb \
	app/lines/lines.hrb \
	app/walk/walk.hrb \
	app/noodle/noodle.hrb \
	app/beepdown/beepdown.hrb \
	app/color/color.hrb \
	app/color2/color2.hrb \
	app/sosu/sosu.hrb \
	app/sosu2/sosu2.hrb \
	app/sosu3/sosu3.hrb \
	app/cat/cat.hrb \
	app/iroha/iroha.hrb \
	app/chklang/chklang.hrb \
	app/notrec/notrec.hrb \
	app/bball/bball.hrb \
	app/invador/invador.hrb \
	app/calc/calc.hrb \
	app/tview/tview.hrb \
	app/mmlplay/mmlplay.hrb \
	app/gview/gview.hrb

MAKE = make -r
DEL = rm -f

default : 
	$(MAKE) haribote.img

# mtool: https://www.gnu.org/software/mtools/manual/html_node/mformat.html
# -f: size, -C: hidden_sectors, -B: boot_sector, -i: 
haribote.img : app/haribote/ipl10.bin app/haribote/haribote.sys $(APPS)
	mformat -f 1440 -C -B app/haribote/ipl10.bin -i haribote.img ::
	mcopy -i haribote.img app/haribote/haribote.sys app/haribote/ipl10.nasm $(APPS) mmldata/kirakira.mml mmldata/fujisan.mml mmldata/daigo.mml mmldata/daiku.mml pictdata/fujisan.jpg pictdata/night.bmp font/jp.fnt ::

# Commands
run :
	$(MAKE) haribote.img
	qemu-system-i386 -soundhw pcspk -drive file=haribote.img,format=raw,if=floppy -boot a

# install :

full :
	$(MAKE) -C lib
	$(MAKE) -C app/haribote
	$(MAKE) -C app/apilib
	$(MAKE) -C app/a
	$(MAKE) -C app/hello3
	$(MAKE) -C app/hello4
	$(MAKE) -C app/hello5
	$(MAKE) -C app/win
	$(MAKE) -C app/win2
	$(MAKE) -C app/win3
	$(MAKE) -C app/star1
	$(MAKE) -C app/stars
	$(MAKE) -C app/star2
	$(MAKE) -C app/lines
	$(MAKE) -C app/walk
	$(MAKE) -C app/noodle
	$(MAKE) -C app/beepdown
	$(MAKE) -C app/color
	$(MAKE) -C app/color2
	$(MAKE) -C app/sosu
	$(MAKE) -C app/sosu2
	$(MAKE) -C app/sosu3
	$(MAKE) -C app/cat
	$(MAKE) -C app/iroha
	$(MAKE) -C app/chklang
	$(MAKE) -C app/notrec
	$(MAKE) -C app/bball
	$(MAKE) -C app/invador
	$(MAKE) -C app/calc
	$(MAKE) -C app/tview
	$(MAKE) -C app/mmlplay
	$(MAKE) -C app/gview
	$(MAKE) haribote.img

run_full :
	$(MAKE) full
	qemu-system-i386 -drive file=haribote.img,format=raw,if=floppy -boot a

# install_full :

run_os :
	$(MAKE) -C lib
	$(MAKE) -C app/haribote
	$(MAKE) run

clean :

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img

clean_full :
	$(MAKE) -C lib clean
	$(MAKE) -C app/haribote clean
	$(MAKE) -C app/apilib clean
	$(MAKE) -C app/a clean
	$(MAKE) -C app/hello3 clean
	$(MAKE) -C app/hello4 clean
	$(MAKE) -C app/hello5 clean
	$(MAKE) -C app/win clean
	$(MAKE) -C app/win2 clean
	$(MAKE) -C app/win3 clean
	$(MAKE) -C app/star1 clean
	$(MAKE) -C app/stars clean
	$(MAKE) -C app/star2 clean
	$(MAKE) -C app/lines clean
	$(MAKE) -C app/walk	 clean
	$(MAKE) -C app/noodle clean
	$(MAKE) -C app/beepdown clean
	$(MAKE) -C app/color clean
	$(MAKE) -C app/color2 clean
	$(MAKE) -C app/sosu clean
	$(MAKE) -C app/sosu2 clean
	$(MAKE) -C app/sosu3 clean
	$(MAKE) -C app/cat clean
	$(MAKE) -C app/iroha clean
	$(MAKE) -C app/chklang clean
	$(MAKE) -C app/notrec clean
	$(MAKE) -C app/bball clean
	$(MAKE) -C app/invador clean
	$(MAKE) -C app/calc clean
	$(MAKE) -C app/tview clean
	$(MAKE) -C app/mmlplay clean
	$(MAKE) -C app/gview clean

src_only_full :
	$(MAKE) -C lib src_only
	$(MAKE) -C app/haribote src_only
	$(MAKE) -C app/apilib src_only
	$(MAKE) -C app/a src_only
	$(MAKE) -C app/hello3 src_only
	$(MAKE) -C app/hello4 src_only
	$(MAKE) -C app/hello5 src_only
	$(MAKE) -C app/win src_only
	$(MAKE) -C app/win2 src_only
	$(MAKE) -C app/win3 src_only
	$(MAKE) -C app/star1 src_only
	$(MAKE) -C app/stars src_only
	$(MAKE) -C app/star2 src_only
	$(MAKE) -C app/lines src_only
	$(MAKE) -C app/walk	 src_only
	$(MAKE) -C app/noodle src_only
	$(MAKE) -C app/beepdown src_only
	$(MAKE) -C app/color src_only
	$(MAKE) -C app/color2 src_only
	$(MAKE) -C app/sosu src_only
	$(MAKE) -C app/sosu2 src_only
	$(MAKE) -C app/sosu3 src_only
	$(MAKE) -C app/cat src_only
	$(MAKE) -C app/iroha src_only
	$(MAKE) -C app/chklang src_only
	$(MAKE) -C app/notrec src_only
	$(MAKE) -C app/bball src_only
	$(MAKE) -C app/invador src_only
	$(MAKE) -C app/calc src_only
	$(MAKE) -C app/tview src_only
	$(MAKE) -C app/mmlplay src_only
	$(MAKE) -C app/gview src_only
	-$(DEL) haribote.img

refresh :
	$(MAKE) full
	$(MAKE) clean_full
	-$(DEL) haribote.img

NAME = new_app
new_app:
	mkdir $(NAME)
	touch $(NAME)/$(NAME).c
	echo "APP = $(NAME)\n\ninclude ../app_make.txt" > $(NAME)/Makefile
