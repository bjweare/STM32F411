#ifndef __DEV_EBOX_SYS_CFG__
#define __DEV_EBOX_SYS_CFG__

#include <stdint.h>

typedef struct SysCfgType {
	// bits[1:0] memory mapping selection
	// 0b00 - main flash memory mapped at 0x00000000
	// 0b01 - system flash memory mapped at 0x00000000
	// 0b11 - embeded sram mapped at 0x00000000
	uint32_t memrmp;
	// peripheral mode configuration
	uint32_t pmc;
	// external interrupt configuration register 1
	uint32_t exticr1;
	// external interrupt configuration register 2
	uint32_t exticr2;
	// external interrupt configuration register 3
	uint32_t exticr3;
	// external interrupt configuration register 4
	uint32_t exticr4;
	// compensation cell control
	uint32_t cmpcr;
} SysCfgType;

#define EXTI_SRC_BITS (4)
typedef enum EXTI_SRC {
	EXTI_SRC_PA = 0b0000,
	EXTI_SRC_PB = 0b0001,
	EXTI_SRC_PC = 0b0010,
	EXTI_SRC_PD = 0b0011,
	EXTI_SRC_PE = 0b0100,
	EXTI_SRC_PH = 0b0111,
} EXTI_SRC;

extern SysCfgType *rSysCfg;

#endif
