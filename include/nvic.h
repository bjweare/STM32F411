#ifndef __DEV_EBOX_NVIC_H__
#define __DEV_EBOX_NVIC_H__

#include <stdint.h>

typedef struct NVICType {
	/* offset: 0x00 */
	// interrupt set-enable register 0
	uint32_t iser0;
	// interrupt set-enable register 1
	uint32_t iser1;
	// interrupt set-enable register 2
	uint32_t iser2;
	// interrupt set-enable register 3
	uint32_t iser3;

	/* offset: 0x10 */
	// interrupt set-enable register 4
	uint32_t iser4;
	// interrupt set-enable register 5
	uint32_t iser5;
	// interrupt set-enable register 6
	uint32_t iser6;
	// interrupt set-enable register 7
	uint32_t iser7;
	// TODO(bjweare): register below starts from offset 0x80
} NVICType;

int EnableEXTI0Interrupt(void);
int EnableTimer11Interrupt(void);

#endif
