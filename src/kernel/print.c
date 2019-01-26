#include "stdint.h"

uint32_t video_ptr = 0;
uint16_t video_char = 0x0700;

void put_str(uint8_t* str)
{

	if (0 == str)
		return;

	for(str; '\0' != *str; str++) {
		if ('\n' == *str) {
			video_ptr -= (video_ptr % 80);
			video_ptr += 80;
		} else {
			video_char = (uint16_t)(0x0700 | *str);
			asm volatile(" \
				pusha;				\
				movl video_ptr, %eax;		\
				shll $1, %eax;			\
				movw video_char, %ebx;		\
				movw %ebx, %gs:(%eax);		\
				popa;				\
			");
			video_ptr++;
		}
	}
	asm volatile(" \
		pushl %eax;			\
		movl video_ptr, %eax;		\
		shll $1, %eax;			\
		movw $36827, %gs:(%eax);	\
		popl %eax;			\
	");
}

