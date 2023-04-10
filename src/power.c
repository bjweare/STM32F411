#include "power.h"

#include "bit.h"

#define POWER_BASE (0x40007000)

#define VOS_START_BIT (14)
#define VOS_BITS (2)

PowerType *rPower = (PowerType *)POWER_BASE;

int SetVOS(VOS_VAL scale)
{
	// set VOS in CR to enable higher frequency
	SetBits(&rPower->cr, VOS_START_BIT, VOS_BITS, scale);
	return 0;
}
