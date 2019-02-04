#ifndef __BITMAP_H
#define __BITMAP_H
#include "stdint.h"

#define ROUND_UP(a,b) ((a)/(b) + ((a)%(b))?1:0)

typedef struct bitmap {
	uint32_t bits_len;
	uint8_t *start_ptr;
} bitmap_t;

void memset(uint8_t *ptr, const uint8_t value, const uint32_t bytes_len);

void bitmap_init(bitmap_t *self, const uint32_t bytes_len, uint8_t *start_ptr);
void bitmap_set(bitmap_t *self, uint32_t bit_idx, uint8_t value);
uint8_t bitmap_get(bitmap_t *self, uint32_t bit_idx);
int bitmap_scan(bitmap_t *self, uint32_t count, uint32_t *bit_offset);

#endif /* __BITMAP_H */
