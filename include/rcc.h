#ifndef __DEV_EBBOX_RCC_H__
#define __DEV_EBBOX_RCC_H__

#include <stdint.h>

// RCC - Reset and Clock Control

typedef struct RCCType {
	/* offset: 0x00 */
	// clock control register
	uint32_t cr;
	// PLL configuration register
	uint32_t pllcfgr;
	// clock configuration register
	uint32_t cfgr;
	// clock interrupt register
	uint32_t cir;
	/* offset: 0x10 */
	// AHB1 peripheral reset register
	uint32_t ahb1rstr;
	// AHB2 peripheral reset register
	uint32_t ahb2rstr;
	uint32_t reserved1;
	uint32_t reserved2;
	/* offset: 0x20 */
	// APB1 peripheral reset register
	uint32_t apb1rstr;
	// APB2 peripheral reset register
	uint32_t apb2rstr;
	uint32_t reserved3;
	uint32_t reserved4;
	/* offset: 0x30 */
	// AHB1 peripheral clock enable register
	uint32_t ahb1enr;
	// AHB2 peripheral clock enable register
	uint32_t ahb2enr;
	uint32_t reserved5;
	uint32_t reserved6;
	/* offset: 0x40 */
	// APB1 peripheral clock enable register
	uint32_t apb1enr;
	// APB2 peripheral clock enable register
	uint32_t apb2enr;
} RCCType;

int EnableSysConfClock(void);
int EnableGPIOClock(void);
int EnableTimer11Clock(void);

#endif
