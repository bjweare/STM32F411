#include "flash.h"

#include "bit.h"

#define FLASH_BASE (0x40023C00)

#define LATENCY_START_BIT (0)
#define LATENCY_BITS (4)

FlashType *rFlash = (FlashType *)FLASH_BASE;

int SetFlashLatency(FlashLatency latency)
{
	// set LATENCY in ACR to change the wait states
	SetBits(&rFlash->acr, LATENCY_START_BIT, LATENCY_BITS, latency);
	return 0;
}
