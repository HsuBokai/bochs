#include "stdint.h"

#define VIDEO_DESC_BASE_HIGH_H	0x00
#define VIDEO_DESC_BASE_HIGH_L	0x0b
#define VIDEO_DESC_BASE_LOW	0x8000
#define VIDEO_DESC_BASE		(VIDEO_DESC_BASE_HIGH_H << 24) | (VIDEO_DESC_BASE_HIGH_L << 16) | (VIDEO_DESC_BASE_LOW)

void put_str(uint8_t* str)
{
	static uint16_t* ptr = VIDEO_DESC_BASE;

	if (0 == str)
		return;

	for(;'\0' != *str; ptr++, str++) {
		(*ptr) = (uint16_t)(0x0700 | *str);
	}
}

