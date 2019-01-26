#!/bin/bash
set -x

rm hd60M.img /tmp/*.bin /tmp/*.o
./bin/bximage -hd -mode="flat" -size=60 -q hd60M.img

nasm -I ./src/include/ -o /tmp/mbr.bin ./src/mbr.S
dd if=/tmp/mbr.bin of=hd60M.img  bs=512 conv=notrunc
[ $? -ne 0 ] && echo "nasm mbr.S fail!" && exit 1

nasm -I ./src/include/ -o /tmp/loader.bin ./src/loader.S
dd if=/tmp/loader.bin of=hd60M.img  bs=512 seek=2 conv=notrunc
[ $? -ne 0 ] && echo "nasm loader.S fail!" && exit 1

gcc -m32 -c -o /tmp/interrupt.o ./src/kernel/interrupt.c -I ./src/kernel/include
gcc -m32 -c -o /tmp/print.o ./src/kernel/print.c -I ./src/kernel/include
gcc -m32 -c -o /tmp/main.o ./src/kernel/main.c -I ./src/kernel/include
ld -m elf_i386 /tmp/main.o /tmp/print.o /tmp/interrupt.o -Ttext 0xc0010000 -e main -o /tmp/kernel.bin

#elf_header_size=`readelf -e /tmp/kernel.bin | grep "Size of this header" | cut -d ":" -f2 | cut -d "(" -f1`
#xxd -u -a -g 1 -s 0 -l $elf_header_size /tmp/kernel.bin
#program_header_start=`readelf -e /tmp/kernel.bin | grep "Start of program header" | cut -d ":" -f2 | cut -d "(" -f1`
#program_header_size=`readelf -e /tmp/kernel.bin | grep "Size of program header" | cut -d ":" -f2 | cut -d "(" -f1`
#xxd -u -a -g 1 -s $program_header_start -l $program_header_size /tmp/kernel.bin

kernel_size=`ls -l /tmp/kernel.bin | cut -d ' ' -f5`
if [ $kernel_size -gt 51200 ]; then
	echo "kernel image too large!!!"
	exit 1
fi

readelf -e /tmp/kernel.bin

objdump -t /tmp/kernel.bin

dd if=/tmp/kernel.bin of=hd60M.img bs=512 seek=9 conv=notrunc
[ $? -ne 0 ] && echo "compile kernel.bin fail!" && exit 1

./bin/bochs -f bochsrc.disk

# ---------- for bochs debug ----------
# pb 0x900 -> set breakpoint in physical address
# xp 0x1500 -> print memory content by physical address
# info gdt -> show GDT
# info tab -> show page mapping
# print-stack
# reg -> show general registers
# sreg -> show segment registers
