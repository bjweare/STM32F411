#ifndef __DEV_EBOX_BIT_H__
#define __DEV_EBOX_BIT_H__

#include <stdint.h>

typedef enum BIT_STATUS {
	BIT_DISABLE = 0,
	BIT_ENABLE = 1,
} BIT_STATUS;

int SetBits(uint32_t *p, uint8_t start_bit, uint8_t bits, uint32_t data);
int GetBits(uint32_t *p, uint8_t start_bit, uint8_t bits, uint32_t *data);

#endif
