#ifndef __DEV_EBOX_TIMER_H__
#define __DEV_EBOX_TIMER_H__

#include <stdint.h>

typedef struct Tim1xType {
	/* offset: 0x00 */
	// control register 1
	uint32_t cr1;
	uint32_t reserved1;
	// slave mode control register
	uint32_t smcr; // reserved
	// interrupt enable register
	uint32_t dier;
	/* offset: 0x10 */
	// status register
	uint32_t sr;
	// event generation register
	uint32_t egr;
	// capture/compare mode register 1
	uint32_t ccmr1;
	uint32_t reserved2;
	/* offset: 0x20 */
	// capture/compare enable register
	uint32_t ccer;
	// counter
	uint32_t cnt;
	// prescaler
	uint32_t psc;
	// auto-reload register
	uint32_t arr;
	/* offset: 0x30 */
	uint32_t reserved4;
	// capture/compare register 1
	uint32_t ccr1;
	uint32_t reserved5[6];
	/* offset: 0x50 */
	// option register
	/* clang-format off */
	uint32_t or;
	/* clang-format on */
} Tim1xType;

#undef TIM10_BASE
#undef TIM11_BASE
#define TIM10_BASE (0x40014400)
#define TIM11_BASE (0x40014800)

int initTimer11(uint32_t periodInMs);
void handleTimerUpdate(void);

#endif
