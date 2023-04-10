#ifndef __DEV_EBOX_POWER_H__
#define __DEV_EBOX_POWER_H__

#include <stdint.h>

typedef struct PowerType {
	/* offset: 0x00 */
	// control register
	uint32_t cr;
	// control/status register
	uint32_t csr;
} PowerType;

typedef enum VOS_VAL {
	VOS_SCALE_RESERVED = 0b00, // Scale 3 mode selected
	VOS_SCALE_3_64MHz = 0b01,
	VOS_SCALE_2_84MHz = 0b10,
	VOS_SCALE_1_100MHz = 0b11,
} VOS_VAL;

int SetVOS(VOS_VAL scale);

#endif
