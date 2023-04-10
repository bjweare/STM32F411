#include "rcc.h"

#include "bit.h"
#include "power.h"
#include "flash.h"

#define RCC_BASE (0x40023800)

// AHB1 peripheral clock enable register
#define GPIOAEN_START_BIT (0)
#define GPIOCEN_START_BIT (2)

// APB1 peripheral clock enable register
#define PWREN_START_BIT (28)
#define WWDGEN_START_BIT (11)

// APB2 peripheral clock enable register
#define SYSCFG_START_BIT (14)
#define TIMER11_START_BIT (18)

/* clock control register */
// HSI clock enable
#define HSION_START_BIT (0)
// HSE clock enable
#define HSEON_START_BIT (16)
// HSE clock ready flag
#define HSERDY_START_BIT (17)
// clock security system enable
#define CSSON_START_BIT (19)
// main PLL enable
#define PLLON_START_BIT (24)
// main PLL clock ready flag
#define PLLRDY_START_BIT (25)

/* PLL configuration register */
#define PLLM_START_BIT (0)
#define PLLM_BITS (6)
#define PLLN_START_BIT (6)
#define PLLN_BITS (9)
#define PLLP_START_BIT (16)
#define PLLP_BITS (2)
#define PLLSRC_START_BIT (22)
#define PLLSRC_BITS (1)
#define PLLQ_START_BIT (24)
#define PLLQ_BITS (4)

/* clock configuration register */
// APB2 clock prescaler
#define PPRE2_START_BIT (13)
#define PPRE2_BITS (3)
// APB1 clock prescaler
#define PPRE1_START_BIT (10)
#define PPRE1_BITS (3)
// AHB clock prescaler
#define HPRE_START_BIT (4)
#define HPRE_BITS (4)
// system clock switch
#define SW_START_BIT (0)
#define SW_BITS (2)
// system clock switch status
#define SWS_START_BIT (2)
#define SWS_BITS (2)

#define CLKEN_BITS (1)

typedef enum PLL_SRC {
	PLL_SRC_HSI = 0,
	PLL_SRC_HSE = 1,
} PLL_SRC;

typedef enum SYSCLK_SRC {
	SYSCLK_SRC_HSI = 0b00,
	SYSCLK_SRC_HSE = 0b01,
	SYSCLK_SRC_PLL = 0b10,
} SYSCLK_SRC;

typedef enum PLLP_VAL {
	PLLP_VAL_2 = 0b00,
	PLLP_VAL_4 = 0b01,
	PLLP_VAL_6 = 0b10,
	PLLP_VAL_8 = 0b11,
} PLLP_VAL;

typedef enum HPRE_DIVISOR {
	HPRE_DIVISOR_1 = 0b0000,
	HPRE_DIVISOR_2 = 0b1000,
	HPRE_DIVISOR_4 = 0b1001,
	HPRE_DIVISOR_8 = 0b1010,
	HPRE_DIVISOR_16 = 0b1011,
	HPRE_DIVISOR_64 = 0b1100,
	HPRE_DIVISOR_128 = 0b1101,
	HPRE_DIVISOR_256 = 0b1110,
	HPRE_DIVISOR_512 = 0b1111,
} HPRE_DIVISOR;

typedef enum PPRE_DIVISOR {
	PPRE_DIVISOR_1 = 0b000,
	PPRE_DIVISOR_2 = 0b100,
	PPRE_DIVISOR_4 = 0b101,
	PPRE_DIVISOR_8 = 0b110,
	PPRE_DIVISOR_16 = 0b111,
} PPRE_DIVISOR;

static RCCType *rRCC = (RCCType *)RCC_BASE;

int EnableHighSpeedExternalClock(void)
{
	uint32_t status = 0;

	// set HSEON in CR register
	SetBits(&rRCC->cr, HSEON_START_BIT, 1, BIT_ENABLE);
	status = BIT_DISABLE;
	do {
		// check HSERDY in CR register
		GetBits(&rRCC->cr, HSERDY_START_BIT, 1, &status);
	} while (status != BIT_ENABLE);

	/* PLLCLK = ((HSE / M) * N) / P = ((25 MHz / 25) * 192) / 2 = 96 MHz */
	/* PLL48CLK = ((HSE / M) * N) / P = ((25 MHz / 25) * 192) / 4 = 48 MHz */
	// set PLLM in PLLCFGR register to 25
	SetBits(&rRCC->pllcfgr, PLLM_START_BIT, PLLM_BITS, 25);
	// set PLLN in PLLCFGR register to 192
	SetBits(&rRCC->pllcfgr, PLLN_START_BIT, PLLN_BITS, 192);
	// set PLLP in PLLCFGR register to 2
	SetBits(&rRCC->pllcfgr, PLLP_START_BIT, PLLP_BITS, PLLP_VAL_2);
	// set PLLQ in PLLCFGR register to 4
	SetBits(&rRCC->pllcfgr, PLLQ_START_BIT, PLLQ_BITS, 4);
	// set PLLSRC in PLLCFGR to select HSE clock as PLL clock source
	SetBits(&rRCC->pllcfgr, PLLSRC_START_BIT, PLLSRC_BITS, PLL_SRC_HSE);

	// set HPRE in CFGR to get HCLK from SYSCLK
	SetBits(&rRCC->cfgr, HPRE_START_BIT, HPRE_BITS, HPRE_DIVISOR_1);
	// set PPRE1 in CFGR to get APB1CLK from HCLK
	SetBits(&rRCC->cfgr, PPRE1_START_BIT, PPRE1_BITS, PPRE_DIVISOR_2);
	// set PPRE2 in CFGR to get APB2CLK from HCLK
	SetBits(&rRCC->cfgr, PPRE2_START_BIT, PPRE2_BITS, PPRE_DIVISOR_1);

	// set PLLON in CR register
	SetBits(&rRCC->cr, PLLON_START_BIT, 1, BIT_ENABLE);
	status = BIT_DISABLE;
	do {
		// check PLLRDY in CR register
		GetBits(&rRCC->cr, PLLRDY_START_BIT, 1, &status);
	} while (status != BIT_ENABLE);

	// set SW in CFGR to select PLLCLK as SYSCLK
	uint32_t sys_clk_src = SYSCLK_SRC_PLL;
	SetBits(&rRCC->cfgr, SW_START_BIT, SW_BITS, sys_clk_src);
	status = -1;
	do {
		// check SWS in CFGR
		GetBits(&rRCC->cfgr, SWS_START_BIT, SWS_BITS, &status);
	} while (status != sys_clk_src);

	// enable CSSON in CR register
	SetBits(&rRCC->cr, CSSON_START_BIT, 1, BIT_ENABLE);
	// reset HSION in CR to turn off HSI
	SetBits(&rRCC->cr, HSION_START_BIT, 1, BIT_DISABLE);

	return 0;
}

int EnableSysConfClock(void)
{
	SetBits(&(rRCC->apb2enr), SYSCFG_START_BIT, CLKEN_BITS, BIT_ENABLE);
	return 0;
}

int EnablePowerClock(void)
{
	SetBits(&rRCC->apb1enr, PWREN_START_BIT, 1, BIT_ENABLE);
	return 0;
}

int EnableWWDGClock(void)
{
	SetBits(&rRCC->apb1enr, WWDGEN_START_BIT, 1, BIT_ENABLE);
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
