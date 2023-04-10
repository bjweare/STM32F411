#include "nvic.h"

#include "bit.h"

#define EXTI0_START_BIT (6)
#define TIM1_TRG_COM_TIM11_START_BIT (26)
#define INTERRUPT_EN_BITS (1)

static NVICType *rNVIC = (NVICType *)NVIC_BASE;

int EnableEXTI0Interrupt(void)
{
	SetBits(&rNVIC->iser0, EXTI0_START_BIT, INTERRUPT_EN_BITS, BIT_ENABLE);
	return 0;
}

int EnableTimer11Interrupt(void)
{
	SetBits(&rNVIC->iser0, TIM1_TRG_COM_TIM11_START_BIT, INTERRUPT_EN_BITS,
					BIT_ENABLE);
	return 0;
}
