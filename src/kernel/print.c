#include "stdint.h"

uint32_t video_ptr = 0;
uint16_t video_char = 0x0700;

static void print_char(uint8_t c)
{
	video_char = (uint16_t)(0x0700 | c);
	asm volatile(" \
		pusha;				\
		movl video_ptr, %eax;		\
		shll $1, %eax;			\
		movw video_char, %bx;		\
		movw %bx, %gs:(%eax);		\
		popa;				\
	");
	video_ptr++;
}

static void print_cursor()
{
	asm volatile(" \
		pushl %eax;			\
		movl video_ptr, %eax;		\
		shll $1, %eax;			\
		movw $36827, %gs:(%eax);	\
		popl %eax;			\
	");
}

void put_str(uint8_t* str)
{

	if (0 == str)
		return;

	for(str; '\0' != *str; str++) {
		if ('\n' == *str) {
			video_ptr -= (video_ptr % 80);
			video_ptr += 80;
		} else {
			print_char(*str);
		}
	}
	print_cursor();
}

void put_int(uint8_t num)
{
	uint8_t num_h = (num >> 4) & 0x0f;
	uint8_t num_l = num & 0x0f;
	print_char('0');
	print_char('x');
	print_char((num_h >= 10) ? (uint8_t)'a' + (num_h - 10) : (uint8_t)'0' + num_h);
	print_char((num_l >= 10) ? (uint8_t)'a' + (num_l - 10) : (uint8_t)'0' + num_l);
	print_cursor();
}
