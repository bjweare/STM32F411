#ifndef __DEV_EBOX_EXTI_H__
#define __DEV_EBOX_EXTI_H__

#define INT_MASK_BITS (1)
#define EVENT_MASK_BITS (1)
#define TRIGGER_BITS (1)
#define PENDING_BITS (1)
typedef struct ExtIntType {
	// interrupt mask register
	uint32_t imr;
	// event mask register
	uint32_t emr;
	// rising trigger selection register
	uint32_t rtsr;
	// falling trigger selection register
	uint32_t ftsr;
	// software interrupt event register
	uint32_t swier;
	// pending register
	uint32_t pr;
} ExtIntType;

#define rExtInt (*((ExtIntType *)0x40013C00))

typedef void ISRType(void);
#define EXTI0_VECTOR *((ISRType **)0x00000058)

#endif
