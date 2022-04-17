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


https://user-images.githubusercontent.com/71201308/163673724-aa491991-aa75-42ce-a068-4481a4ed92c7.mov

