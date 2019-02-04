#include "bitmap.h"
#include "debug.h"

void memset(uint8_t *ptr, const uint8_t value, const uint32_t bytes_len)
{
	uint32_t i = 0;
	for (; i < bytes_len; ++i) {
		ptr[i] = value;
	}
}

void bitmap_init(bitmap_t *self, const uint32_t bits_len, uint8_t *start_ptr)
{
	uint32_t bytes_len = ROUND_UP(bits_len,8);
	self->bits_len = bits_len;
	self->start_ptr = start_ptr;
	memset(start_ptr, 0x00, bytes_len);
}

void bitmap_set(bitmap_t *self, uint32_t bit_idx, uint8_t value)
{
	ASSERT(bit_idx < self->bits_len);
	if (value) {
		self->start_ptr[bit_idx / 8] |= 0x01 << (bit_idx % 8);
	} else {
		self->start_ptr[bit_idx / 8] &= ~(0x01 << (bit_idx % 8));
	}
}

uint8_t bitmap_get(bitmap_t *self, uint32_t bit_idx)
{
	ASSERT(bit_idx < self->bits_len);
	return self->start_ptr[bit_idx / 8] & 0x01 << (bit_idx % 8);
}

int bitmap_scan(bitmap_t *self, uint32_t count, uint32_t *bit_offset)
{
	int ret = -1;
	uint32_t len = 0, bit_idx = 0;

	ASSERT(0 < count);
	while(1) {
		if (bit_idx >= self->bits_len) {
			goto NOT_FOUND;
		}
		if (bitmap_get(self, bit_idx)) {
			len = 0;
			*bit_offset = bit_idx;
		} else {
			len++;
		}
		if (len >= count) {
			goto FOUND;
		}
		++bit_idx;
	}
FOUND:
	ret = 0;
NOT_FOUND:
	return ret;
}
