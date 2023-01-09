MAKE = make -r
OUT_DIR = ../../../target

default :
	mkdir -p $(OUT_DIR)/kernel/rust
	$(MAKE) $(OUT_DIR)/kernel/rust/rustlib.a

target/dos/release/librust.rlib:
	cargo build --release --target=dos.json

$(OUT_DIR)/kernel/rust/rustlib.a: target/dos/release/librust.rlib
	i386-elf-ar rcs $@ target/dos/release/librust.rlib