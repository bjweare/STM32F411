#ifndef __DEV_EBOX_WWDG_H__
#define __DEV_EBOX_WWDG_H__

#include <stdint.h>

typedef struct WWDGType {
	/* offset: 0x00 */
	// control register
	uint32_t cr;
	// configuration register
	uint32_t cfr;
	// status register
	uint32_t sr;
} WWDGType;

int DisableWWDG(void);

#endif
