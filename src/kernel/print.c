#include "print.h"
#include "mem.h"

#define SCREEN_LENGTH 80
#define SCREEN_HEIGHT 25

uint32_t video_ptr = 0;
int16_t video_char = 0x0700;

static void check_roll_screen(void)
{
	if (video_ptr < SCREEN_LENGTH * SCREEN_HEIGHT)
		return;

	mem_cpy((uint32_t*)0xc00b8000, (uint32_t*)0xc00b80a0, SCREEN_LENGTH * SCREEN_HEIGHT * 2 / 4);
	video_ptr = SCREEN_LENGTH * (SCREEN_HEIGHT-1);
}

static void print_char(const int8_t c)
{
	check_roll_screen();

	video_char = (int16_t)(0x0700 | c);
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

static void print_cursor(void)
{
	check_roll_screen();

	asm volatile(" \
		pushl %eax;			\
		movl video_ptr, %eax;		\
		shll $1, %eax;			\
		movw $36827, %gs:(%eax);	\
		popl %eax;			\
	");
}

void put_str(const int8_t* str)
{

	if (0 == str)
		return;

	for(; '\0' != *str; str++) {
		if ('\n' == *str) {
			print_char(' ');
			video_ptr -= (video_ptr % SCREEN_LENGTH);
			video_ptr += SCREEN_LENGTH;
		} else {
			print_char(*str);
		}
	}
	print_cursor();
}

void put_int(const uint8_t num)
{
	int8_t num_h = (num >> 4) & 0x0f;
	int8_t num_l = num & 0x0f;
	print_char('0');
	print_char('x');
	print_char((num_h >= 10) ? 'a' + (num_h - 10) : '0' + num_h);
	print_char((num_l >= 10) ? 'a' + (num_l - 10) : '0' + num_l);
	print_cursor();
}
