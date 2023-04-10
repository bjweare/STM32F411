#include "exti.h"

#include "bit.h"
#include "sys_cfg.h"

#define EXTI_BASE (0x40013C00)

typedef void ISRType(void);
#define EXTI0_VECTOR *((ISRType **)0x00000058)

ExtIntType *rExtInt = (ExtIntType *)EXTI_BASE;

int initKeyInterrupt(void)
{
#if 0 // not work, 0x00000058 could not be modified,
	  // since it is mapped to flash
	// 01. set ISR of EXTI0
	//*((uint32_t *)0x00000058) = (uint32_t)handleKeyInterrupt;
	EXTI0_VECTOR = handleKeyInterrupt;
#endif
	// 02. set EXTI0 interrupt source in SYSCFG external interrupt configuration 1
	SetBits(&rSysCfg->exticr1, EXTI_SRC_BITS * 0, EXTI_SRC_BITS, EXTI_SRC_PA);
	// 03. set edge detection in trigger register
	// enable rising edge detection
	SetBits(&rExtInt->rtsr, TRIGGER_BITS * 0, TRIGGER_BITS, BIT_ENABLE);
	// disable falling edge detection
	SetBits(&rExtInt->ftsr, TRIGGER_BITS * 0, TRIGGER_BITS, BIT_DISABLE);
	// 04. enable interrupt request in interrupt mask register
	SetBits(&rExtInt->imr, INT_MASK_BITS * 0, INT_MASK_BITS, BIT_ENABLE);
	return 0;
}
