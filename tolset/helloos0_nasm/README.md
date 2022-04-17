install packages from brew
```sh
# emulator
$ brew install qemu

# assembler
$ brew install nasm
```

assemble to disk image
```sh
$ nasm helloos.nas
```

boot image file as a FD image on emulator
```sh
$ qemu-system-x86_64 -fda helloos
```



https://user-images.githubusercontent.com/71201308/163697130-0509679d-6db1-43ed-9ac6-715e266c5b34.mov



