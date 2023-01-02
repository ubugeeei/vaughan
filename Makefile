OUT_DIR = target

APPS = \
	src/app/a/a.hrb \
	src/app/hello3/hello3.hrb \
	src/app/hello4/hello4.hrb \
	src/app/hello5/hello5.hrb \
	src/app/win/win.hrb \
	src/app/win2/win2.hrb \
	src/app/win3/win3.hrb \
	src/app/star1/star1.hrb \
	src/app/stars/stars.hrb \
	src/app/star2/star2.hrb \
	src/app/lines/lines.hrb \
	src/app/walk/walk.hrb \
	src/app/noodle/noodle.hrb \
	src/app/beepdown/beepdown.hrb \
	src/app/color/color.hrb \
	src/app/color2/color2.hrb \
	src/app/sosu/sosu.hrb \
	src/app/sosu2/sosu2.hrb \
	src/app/sosu3/sosu3.hrb \
	src/app/cat/cat.hrb \
	src/app/iroha/iroha.hrb \
	src/app/chklang/chklang.hrb \
	src/app/notrec/notrec.hrb \
	src/app/bball/bball.hrb \
	src/app/invador/invador.hrb \
	src/app/calc/calc.hrb \
	src/app/tview/tview.hrb \
	src/app/mmlplay/mmlplay.hrb \
	src/app/gview/gview.hrb

MAKE = make -r
DEL = rm -rf
PRE_MAKE = mkdir -p $(OUT_DIR)

default : 
	$(MAKE) $(OUT_DIR)/haribote.img

# mtool: https://www.gnu.org/software/mtools/manual/html_node/mformat.html
# -f: size, -C: hidden_sectors, -B: boot_sector, -i: 
$(OUT_DIR)/haribote.img : src/kernel/ipl10.bin src/kernel/haribote.sys $(APPS)
	mformat -f 1440 -C -B src/kernel/ipl10.bin -i $(OUT_DIR)/haribote.img ::
	mcopy -i \
		$(OUT_DIR)/haribote.img \
		src/kernel/haribote.sys \
		src/kernel/ipl10.nasm \
		$(APPS) \
		src/app/mmldata/kirakira.mml \
		src/app/mmldata/fujisan.mml \
		src/app/mmldata/daigo.mml \
		src/app/mmldata/daiku.mml \
		src/app/pictdata/fujisan.jpg \
		src/app/pictdata/night.bmp \
		font/jp.fnt ::

# Commands
run :
	$(MAKE) $(OUT_DIR)/haribote.img
	qemu-system-i386 -soundhw pcspk -drive file=$(OUT_DIR)/haribote.img,format=raw,if=floppy -boot a

# install :

full :
	$(PRE_MAKE)
	$(MAKE) -C src/lib
	$(MAKE) -C src/kernel
	$(MAKE) -C src/app/apilib
	$(MAKE) -C src/app/a
	$(MAKE) -C src/app/hello3
	$(MAKE) -C src/app/hello4
	$(MAKE) -C src/app/hello5
	$(MAKE) -C src/app/win
	$(MAKE) -C src/app/win2
	$(MAKE) -C src/app/win3
	$(MAKE) -C src/app/star1
	$(MAKE) -C src/app/stars
	$(MAKE) -C src/app/star2
	$(MAKE) -C src/app/lines
	$(MAKE) -C src/app/walk
	$(MAKE) -C src/app/noodle
	$(MAKE) -C src/app/beepdown
	$(MAKE) -C src/app/color
	$(MAKE) -C src/app/color2
	$(MAKE) -C src/app/sosu
	$(MAKE) -C src/app/sosu2
	$(MAKE) -C src/app/sosu3
	$(MAKE) -C src/app/cat
	$(MAKE) -C src/app/iroha
	$(MAKE) -C src/app/chklang
	$(MAKE) -C src/app/notrec
	$(MAKE) -C src/app/bball
	$(MAKE) -C src/app/invador
	$(MAKE) -C src/app/calc
	$(MAKE) -C src/app/tview
	$(MAKE) -C src/app/mmlplay
	$(MAKE) -C src/app/gview
	$(MAKE) $(OUT_DIR)/haribote.img

run_full :
	$(MAKE) full
	qemu-system-i386 -drive file=$(OUT_DIR)/haribote.img,format=raw,if=floppy -boot a

# install_full :

run_os :
	$(MAKE) -C lib
	$(MAKE) -C src/kernel
	$(MAKE) run

clean :
	-$(DEL) target/

src_only :
	$(MAKE) clean
	-$(DEL) $(OUT_DIR)/haribote.img

clean_full :
	$(MAKE) -C src/lib clean
	$(MAKE) -C src/kernel clean
	$(MAKE) -C src/app/apilib clean
	$(MAKE) -C src/app/a clean
	$(MAKE) -C src/app/hello3 clean
	$(MAKE) -C src/app/hello4 clean
	$(MAKE) -C src/app/hello5 clean
	$(MAKE) -C src/app/win clean
	$(MAKE) -C src/app/win2 clean
	$(MAKE) -C src/app/win3 clean
	$(MAKE) -C src/app/star1 clean
	$(MAKE) -C src/app/stars clean
	$(MAKE) -C src/app/star2 clean
	$(MAKE) -C src/app/lines clean
	$(MAKE) -C src/app/walk	 clean
	$(MAKE) -C src/app/noodle clean
	$(MAKE) -C src/app/beepdown clean
	$(MAKE) -C src/app/color clean
	$(MAKE) -C src/app/color2 clean
	$(MAKE) -C src/app/sosu clean
	$(MAKE) -C src/app/sosu2 clean
	$(MAKE) -C src/app/sosu3 clean
	$(MAKE) -C src/app/cat clean
	$(MAKE) -C src/app/iroha clean
	$(MAKE) -C src/app/chklang clean
	$(MAKE) -C src/app/notrec clean
	$(MAKE) -C src/app/bball clean
	$(MAKE) -C src/app/invador clean
	$(MAKE) -C src/app/calc clean
	$(MAKE) -C src/app/tview clean
	$(MAKE) -C src/app/mmlplay clean
	$(MAKE) -C src/app/gview clean
	$(DEL) target/

src_only_full :
	$(MAKE) -C src/lib src_only
	$(MAKE) -C src/kernel src_only
	$(MAKE) -C src/app/apilib src_only
	$(MAKE) -C src/app/a src_only
	$(MAKE) -C src/app/hello3 src_only
	$(MAKE) -C src/app/hello4 src_only
	$(MAKE) -C src/app/hello5 src_only
	$(MAKE) -C src/app/win src_only
	$(MAKE) -C src/app/win2 src_only
	$(MAKE) -C src/app/win3 src_only
	$(MAKE) -C src/app/star1 src_only
	$(MAKE) -C src/app/stars src_only
	$(MAKE) -C src/app/star2 src_only
	$(MAKE) -C src/app/lines src_only
	$(MAKE) -C src/app/walk	 src_only
	$(MAKE) -C src/app/noodle src_only
	$(MAKE) -C src/app/beepdown src_only
	$(MAKE) -C src/app/color src_only
	$(MAKE) -C src/app/color2 src_only
	$(MAKE) -C src/app/sosu src_only
	$(MAKE) -C src/app/sosu2 src_only
	$(MAKE) -C src/app/sosu3 src_only
	$(MAKE) -C src/app/cat src_only
	$(MAKE) -C src/app/iroha src_only
	$(MAKE) -C src/app/chklang src_only
	$(MAKE) -C src/app/notrec src_only
	$(MAKE) -C src/app/bball src_only
	$(MAKE) -C src/app/invador src_only
	$(MAKE) -C src/app/calc src_only
	$(MAKE) -C src/app/tview src_only
	$(MAKE) -C src/app/mmlplay src_only
	$(MAKE) -C src/app/gview src_only
	-$(DEL) $(OUT_DIR)/*

refresh :
	$(MAKE) full
	$(MAKE) clean_full
	-$(DEL) $(OUT_DIR)/haribote.img

NAME = new_app
new_app:
	mkdir src/app/$(NAME)
	touch src/app/$(NAME)/$(NAME).c
	echo "APP = $(NAME)\n\ninclude ../app.mk" > src/app/$(NAME)/Makefile
