#include "key.h"

#include <stdbool.h>

#include "bit.h"
#include "gpio.h"
#include "exti.h"

void handleKeyInterrupt(void)
{
	// disable interrupt request in interrupt mask register??
	// SetBits(&rExtInt->imr, INT_MASK_BITS * 0, INT_MASK_BITS, BIT_DISABLE);
	uint32_t pending = BIT_DISABLE;
	GetBits(&rExtInt->pr, INT_MASK_BITS * 0, INT_MASK_BITS, &pending);

	// flip led status
	SetLEDStatus(led_pin, led_on);
	led_on = (led_on == true) ? false : true;

	key_pressed = true;
	// clear pending bit in the pending register
	SetBits(&rExtInt->pr, PENDING_BITS * 0, PENDING_BITS, BIT_ENABLE);
}
