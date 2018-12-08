#!/bin/bash
set -x

./bin/bximage -hd -mode="flat" -size=60 -q hd60M.img
nasm -o /tmp/mbr.bin ./src/mbr.S
dd if=/tmp/mbr.bin of=hd60M.img  bs=512 count=1 conv=notrunc
./bin/bochs -f bochsrc.disk
