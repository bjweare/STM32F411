#include "rcc.h"
#include "bit.h"

#undef RCC_BASE
#define RCC_BASE (0x40023800)

#define GPIOAEN_START_BIT (0)
#define GPIOCEN_START_BIT (2)

#define SYSCFG_START_BIT (14)
#define TIMER11_START_BIT (18)

#define CLKEN_BITS (1)

static RCCType *rRCC = (RCCType *)RCC_BASE;

int EnableSysConfClock(void)
{
	SetBits(&(rRCC->apb2enr), SYSCFG_START_BIT, CLKEN_BITS, BIT_ENABLE);
	return 0;
}

int EnableGPIOClock(void)
{
	SetBits(&(rRCC->ahb1enr), GPIOAEN_START_BIT, CLKEN_BITS, BIT_ENABLE);
	SetBits(&(rRCC->ahb1enr), GPIOCEN_START_BIT, CLKEN_BITS, BIT_ENABLE);
	return 0;
}

int EnableTimer11Clock(void)
{
	SetBits(&(rRCC->apb2enr), TIMER11_START_BIT, CLKEN_BITS, BIT_ENABLE);
	return 0;
}
