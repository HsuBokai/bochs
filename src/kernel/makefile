TOOLCHAIN=/usr/bin/
BUILD_DIR=/tmp
ENTRY_POINT=0xc0010000

CC=$(TOOLCHAIN)gcc
CFLAGS= -m32 -Wall -c -W -I./include -I./user/uapi -fno-builtin -Wstrict-prototypes -Wmissing-prototypes -Werror -Wno-unused-parameter -Wno-unused-function

AS=$(TOOLCHAIN)nasm
ASFLAGS= -f elf

LD=$(TOOLCHAIN)ld
LDFLAGS= -m elf_i386 -Ttext $(ENTRY_POINT) -e main

OBJS=$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/print.o \
	$(BUILD_DIR)/interrupt.o \
	$(BUILD_DIR)/intr.o \
	$(BUILD_DIR)/mem.o \
	$(BUILD_DIR)/bitmap.o \
	$(BUILD_DIR)/mm.o \
	$(BUILD_DIR)/thread.o \
	$(BUILD_DIR)/list.o \
	$(BUILD_DIR)/schedule.o \
	$(BUILD_DIR)/switch.o \
	$(BUILD_DIR)/timer.o \
	$(BUILD_DIR)/semaphore.o \
	$(BUILD_DIR)/console.o \
	$(BUILD_DIR)/keyboard.o \
	$(BUILD_DIR)/vaddr.o \
	$(BUILD_DIR)/tss.o \
	$(BUILD_DIR)/process.o \
	$(BUILD_DIR)/thread_pid.o \
	$(BUILD_DIR)/syscall_init.o \
	$(BUILD_DIR)/debug.o
HEADER_FILES= $(wildcard ./include/*.h)

.PHONY: clean all

$(BUILD_DIR)/%.o: %.c ${HEADER_FILES}
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ $(BUILD_DIR)/syscall.o

all: $(BUILD_DIR)/kernel.bin

clean:
	rm -f  $(BUILD_DIR)/*.o $(BUILD_DIR)/kernel.bin
