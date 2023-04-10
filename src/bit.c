#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "bit.h"

static uint32_t bits_mask[33] = {
	/* clang-format off */
	0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
	0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
	0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
	0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
	0xffffffff
	/* clang-format on */
};

int SetBits(uint32_t *p, uint8_t start_bit, uint8_t bits, uint32_t data)
{
	if (p == NULL || start_bit > 31 || bits > 32) {
		return -1;
	}
	*p |= (data & bits_mask[bits]) << start_bit;
	return 0;
}

int GetBits(uint32_t *p, uint8_t start_bit, uint8_t bits, uint32_t *data)
{
	if (p == NULL || data == NULL || start_bit > 31 || bits > 32) {
		return -1;
	}
	*data = (*p >> start_bit) & bits_mask[bits];
	return 0;
}
