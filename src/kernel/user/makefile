TOOLCHAIN=/usr/bin/
BUILD_DIR=/tmp

CC=$(TOOLCHAIN)gcc
CFLAGS= -m32 -Wall -c -W -I./uapi -fno-builtin -Wstrict-prototypes -Wmissing-prototypes -Werror -Wno-unused-parameter -Wno-unused-function

OBJS=$(BUILD_DIR)/syscall.o
HEADER_FILES= $(wildcard ./uapi/*.h)

.PHONY: clean all

$(BUILD_DIR)/%.o: %.c ${HEADER_FILES}
	$(CC) $(CFLAGS) $< -o $@

all: $(OBJS)

clean:
	rm -f  $(OBJS)
