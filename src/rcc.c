#include "rcc.h"

#include "bit.h"
#include "power.h"
#include "flash.h"

#define RCC_BASE (0x40023800)

/* clock control register */
enum CR_BIT_FIELD {
	// main PLL clock ready flag
	BF_PLLRDY = 25,
	// main PLL enable
	BF_PLLON = 24,
	// clock security system enable
	BF_CSSON = 19,
	// HSE clock ready flag
	BF_HSERDY = 17,
	// HSE clock enable
	BF_HSEON = 16,
	// HSI clock enable
	BF_HSION = 0
};

/* PLL configuration register */
enum PLLCFGR_BIT_FIELD {
	// main PPL (PLL) division factor for USB OTG FS, and SDIO clocks.
	BF_PLLQ = 24,
#define PLLQ_BITS (4)
	// main PLL (PLL) and audio PLL (PLLI2S) entry clock source
	BF_PLLSRC = 22,
#define PLLSRC_BITS (1)
	// main PLL (PLL) division factor for main system clock
	BF_PLLP = 16,
#define PLLP_BITS (2)
	// main PLL (PLL) multiplication factor for VCO
	BF_PLLN = 6,
#define PLLN_BITS (9)
	// division factor for the main PLL (PLL) input clock
	BF_PLLM = 0,
#define PLLM_BITS (6)
};

/* clock configuration register */
enum CFGR_BIT_FIELD {
	// APB2 clock prescaler
	BF_PPRE2 = 13,
#define PPRE2_BITS (3)
	// APB1 clock prescaler
	BF_PPRE1 = 10,
#define PPRE1_BITS (3)
	// AHB clock prescaler
	BF_HPRE = 4,
#define HPRE_BITS (4)
	// system clock switch status
	BF_SWS = 2,
#define SWS_BITS (2)
	// system clock switch
	BF_SW = 0,
#define SW_BITS (2)
};

enum PLL_SRC {
	PLL_SRC_HSI = 0b0,
	PLL_SRC_HSE = 0b1,
};

enum SYSCLK_SRC {
	SYSCLK_SRC_HSI = 0b00,
	SYSCLK_SRC_HSE = 0b01,
	SYSCLK_SRC_PLL = 0b10,
};

enum PLLP_VAL {
	PLLP_VAL_2 = 0b00,
	PLLP_VAL_4 = 0b01,
	PLLP_VAL_6 = 0b10,
	PLLP_VAL_8 = 0b11,
};

enum HPRE_DIVISOR {
	HPRE_1 = 0b0000,
	HPRE_2 = 0b1000,
	HPRE_4 = 0b1001,
	HPRE_8 = 0b1010,
	HPRE_16 = 0b1011,
	HPRE_64 = 0b1100,
	HPRE_128 = 0b1101,
	HPRE_256 = 0b1110,
	HPRE_512 = 0b1111,
};

enum PPRE_DIVISOR {
	PPRE_1 = 0b000,
	PPRE_2 = 0b100,
	PPRE_4 = 0b101,
	PPRE_8 = 0b110,
	PPRE_16 = 0b111,
};

#define CLKEN_BITS (1)
// AHB1 peripheral clock enable register
enum AHB1ENR_BIT_FIELD {
	// IO port A clock enable
	BF_GPIOAEN = 0,
	// IO port C clock enable
	BF_GPIOCEN = 2,
};

// APB1 peripheral clock enable register
enum APB1ENR_BIT_FIELD {
	// power interface clock enable
	BF_PWREN = 28,
	// window watchdog clock enable
	BF_WWDGEN = 11,
};

// APB2 peripheral clock enable register
enum APB2ENR_BIT_FIELD {
	// system configuration controller clock enable
	BF_SYSCFGEN = 14,
	// timer11 clock enable
	BF_TIM11EN = 18,
	// USART1 clock enable
	BF_USART1EN = 4,
};

static RCCType *rRCC = (RCCType *)RCC_BASE;

int EnableHighSpeedExternalClock(void)
{
	uint32_t status = 0;

	// set HSEON in CR register
	SetBits(&rRCC->cr, BF_HSEON, 1, BIT_ENABLE);
	status = BIT_DISABLE;
	do { // check HSERDY in CR register
		GetBits(&rRCC->cr, BF_HSERDY, 1, &status);
	} while (status != BIT_ENABLE);

	/* PLLCLK = ((HSE / M) * N) / P = ((25 MHz / 25) * 192) / 2 = 96 MHz */
	/* PLL48CLK = ((HSE / M) * N) / P = ((25 MHz / 25) * 192) / 4 = 48 MHz */
	// set PLLM in PLLCFGR register to 25
	SetBits(&rRCC->pllcfgr, BF_PLLM, PLLM_BITS, 25);
	// set PLLN in PLLCFGR register to 192
	SetBits(&rRCC->pllcfgr, BF_PLLN, PLLN_BITS, 192);
	// set PLLP in PLLCFGR register to 2
	SetBits(&rRCC->pllcfgr, BF_PLLP, PLLP_BITS, PLLP_VAL_2);
	// set PLLQ in PLLCFGR register to 4
	SetBits(&rRCC->pllcfgr, BF_PLLQ, PLLQ_BITS, 4);
	// set PLLSRC in PLLCFGR to select HSE clock as PLL clock source
	SetBits(&rRCC->pllcfgr, BF_PLLSRC, PLLSRC_BITS, PLL_SRC_HSE);

	// set HPRE in CFGR to get HCLK from SYSCLK
	SetBits(&rRCC->cfgr, BF_HPRE, HPRE_BITS, HPRE_1);
	// set PPRE1 in CFGR to get APB1CLK from HCLK
	SetBits(&rRCC->cfgr, BF_PPRE1, PPRE1_BITS, PPRE_2);
	// set PPRE2 in CFGR to get APB2CLK from HCLK
	SetBits(&rRCC->cfgr, BF_PPRE2, PPRE2_BITS, PPRE_1);

	// set PLLON in CR register
	SetBits(&rRCC->cr, BF_PLLON, 1, BIT_ENABLE);
	status = BIT_DISABLE;
	do {
		// check PLLRDY in CR register
		GetBits(&rRCC->cr, BF_PLLRDY, 1, &status);
	} while (status != BIT_ENABLE);

	// set SW in CFGR to select PLLCLK as SYSCLK
	uint32_t sys_clk_src = SYSCLK_SRC_PLL;
	SetBits(&rRCC->cfgr, BF_SW, SW_BITS, sys_clk_src);
	status = -1;
	do {
		// check SWS in CFGR
		GetBits(&rRCC->cfgr, BF_SWS, SWS_BITS, &status);
	} while (status != sys_clk_src);

	// enable CSSON in CR register
	SetBits(&rRCC->cr, BF_CSSON, 1, BIT_ENABLE);
	// reset HSION in CR to turn off HSI
	SetBits(&rRCC->cr, BF_HSION, 1, BIT_DISABLE);

	return 0;
}

int EnableSysConfClock(void)
{
	SetBits(&(rRCC->apb2enr), BF_SYSCFGEN, CLKEN_BITS, BIT_ENABLE);
	return 0;
}

int EnablePowerClock(void)
{
	SetBits(&rRCC->apb1enr, BF_PWREN, 1, BIT_ENABLE);
	return 0;
}

int EnableWWDGClock(void)
{
	SetBits(&rRCC->apb1enr, BF_WWDGEN, 1, BIT_ENABLE);
	return 0;
}

int EnableGPIOClock(void)
{
	SetBits(&(rRCC->ahb1enr), BF_GPIOAEN, CLKEN_BITS, BIT_ENABLE);
	SetBits(&(rRCC->ahb1enr), BF_GPIOCEN, CLKEN_BITS, BIT_ENABLE);
	return 0;
}

int EnableTimer11Clock(void)
{
	SetBits(&(rRCC->apb2enr), BF_TIM11EN, CLKEN_BITS, BIT_ENABLE);
	return 0;
}

int SetUart0ClockStatus(bool bOnOff)
{
	SetBits(&(rRCC->apb2enr), BF_USART1EN, CLKEN_BITS, bOnOff);
	return 0;
}
