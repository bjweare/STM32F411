#include "wwdg.h"

#include "bit.h"

#define WWDG_BASE (0x40002C00)

#define WDGA_START_BIT (7)

WWDGType *rWWDG = (WWDGType *)WWDG_BASE;

int DisableWWDG(void)
{
	SetBits(&rWWDG->cr, WDGA_START_BIT, 1, BIT_DISABLE);
	return 0;
}
