#!/bin/bash
set -x

rm hd60M.img
./bin/bximage -hd -mode="flat" -size=60 -q hd60M.img

nasm -I ./src/include/ -o /tmp/mbr.bin ./src/mbr.S
dd if=/tmp/mbr.bin of=hd60M.img  bs=512 conv=notrunc

nasm -I ./src/include/ -o /tmp/loader.bin ./src/loader.S
dd if=/tmp/loader.bin of=hd60M.img  bs=512 seek=2 conv=notrunc

gcc -c -o /tmp/main.o ./src/kernel/main.c
ld /tmp/main.o -Ttext 0xc0001500 -e main -o /tmp/kernel.bin

xxd -u -a -g 1 -s 0 -l 0x40 /tmp/kernel.bin
xxd -u -a -g 1 -s 0x40 -l 0x38 /tmp/kernel.bin

kernel_size=`ls -l /tmp/kernel.bin | cut -d ' ' -f5`
if [ $kernel_size -gt 51200 ]; then
	echo "kernel image too large!!!"
	exit 1
fi

dd if=/tmp/kernel.bin of=hd60M.img bs=512 seek=9 conv=notrunc

./bin/bochs -f bochsrc.disk
