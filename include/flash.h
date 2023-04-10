#ifndef __DEV_EBOX_FLASH_H__
#define __DEV_EBOX_FLASH_H__
#include <stdint.h>

typedef struct FlashType {
	/* offset: 0x00 */
	// access control register
	uint32_t acr;
	// key register
	uint32_t keyr;
	// option key register
	uint32_t optkeyr;
	// status register
	uint32_t sr;
	/* offset: 0x10 */
	// control register
	uint32_t cr;
	// option control register
	uint32_t optcr;
} FlashType;

typedef enum FlashLatency {
	// see Table 5 in RM0383 for more details
	FLASH_LATENCY_0,
	FLASH_LATENCY_1,
	FLASH_LATENCY_2,
	FLASH_LATENCY_3,
	FLASH_LATENCY_4,
	FLASH_LATENCY_5,
	FLASH_LATENCY_6,
	FLASH_LATENCY_7,
	FLASH_LATENCY_8,
	FLASH_LATENCY_9,
	FLASH_LATENCY_10,
	FLASH_LATENCY_11,
	FLASH_LATENCY_12,
	FLASH_LATENCY_13,
	FLASH_LATENCY_14,
	FLASH_LATENCY_15,
} FlashLatency;

int SetFlashLatency(FlashLatency latency);

#endif
