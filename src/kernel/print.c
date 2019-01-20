#include "stdint.h"

#define VIDEO_DESC_BASE_HIGH_H	0x00
#define VIDEO_DESC_BASE_HIGH_L	0x0b
#define VIDEO_DESC_BASE_LOW	0x8000
#define VIDEO_DESC_BASE		((VIDEO_DESC_BASE_HIGH_H << 24) | (VIDEO_DESC_BASE_HIGH_L << 16) | VIDEO_DESC_BASE_LOW)

void put_str(uint8_t* str)
{
	static uint16_t* video_ptr = (uint16_t*)VIDEO_DESC_BASE;

	if (0 == str)
		return;

	for(str; '\0' != *str; str++) {
		if ('\n' == *str) {
			video_ptr -= (video_ptr - (uint16_t*)VIDEO_DESC_BASE) % 80;
			video_ptr += 80;
		} else {
			(*video_ptr) = (uint16_t)(0x0700 | *str);
			video_ptr++;
		}
	}
}

