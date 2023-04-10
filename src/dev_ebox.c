#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bit.h"
#include "rcc.h"
#include "exti.h"
#include "gpio.h"
#include "sys_cfg.h"
#include "timer.h"
#include "nvic.h"

bool led_on = false;
static bool key_pressed = false;

static uint8_t led_pin = GPIO_PIN_13;
static uint8_t key_pin = GPIO_PIN_0;
int initGPIO(void)
{
	EnableGPIOClock();

	// set mode to output
	SetBits(&(rGPIOC.mode), GPIO_MODE_BITS * led_pin, GPIO_MODE_BITS,
					GPIO_MODE_OUTPUT);
	// set output type to open-drain
	SetBits(&(rGPIOC.otype), GPIO_OTYPE_BITS * led_pin, GPIO_OTYPE_BITS,
					GPIO_OTYPE_OD);
	// set output speed to medium speed
	SetBits(&(rGPIOC.ospeed), GPIO_OSPEED_BITS * led_pin, GPIO_OSPEED_BITS,
					GPIO_OSPEED_MS);
	// set the GPIO PC13 to led_on
	if (true == led_on) {
		// reset GPIOC_13
		SetBits(&(rGPIOC.bs), GPIO_BITSET_BITS * led_pin + 16, GPIO_BITSET_BITS, 1);
	} else {
		// set the GPIO PC13 to led_on
		SetBits(&(rGPIOC.bs), GPIO_BITSET_BITS * led_pin, GPIO_BITSET_BITS, 1);
	}

	// set mode to input
	SetBits(&(rGPIOA.mode), GPIO_MODE_BITS * key_pin, GPIO_MODE_BITS,
					GPIO_MODE_INPUT);
	// set port to pull-up
	SetBits(&(rGPIOA.pupd), GPIO_PUPD_BITS * key_pin, GPIO_PUPD_BITS,
					GPIO_PUPD_PU);

	// TODO(bjweare): set interrupt priority
	EnableEXTI0Interrupt();

	return 0;
}

int initKeyInterrupt(void)
{
	EnableSysConfClock();
#if 0 // not work, because 0x00000058 is mapped to flash, it could not be
	  // modified
	// 01. set ISR of EXTI0
	//*((uint32_t *)0x00000058) = (uint32_t)handleKeyInterrupt;
	EXTI0_VECTOR = handleKeyInterrupt;
#endif
	// 02. set EXTI0 interrupt source in SYSCFG external interrupt configuration 1
	SetBits(&rSysCfg.exticr1, EXTI_SRC_BITS * 0, EXTI_SRC_BITS, EXTI_SRC_PA);
	// 03. set edge detection in trigger register
	// enable rising edge detection
	SetBits(&rExtInt.rtsr, TRIGGER_BITS * 0, TRIGGER_BITS, BIT_ENABLE);
	// disable falling edge detection
	SetBits(&rExtInt.ftsr, TRIGGER_BITS * 0, TRIGGER_BITS, BIT_DISABLE);
	// 04. enable interrupt request in interrupt mask register
	SetBits(&rExtInt.imr, INT_MASK_BITS * 0, INT_MASK_BITS, BIT_ENABLE);
	return 0;
}

void handleKeyInterrupt(void)
{
	// disable interrupt request in interrupt mask register??
	// SetBits(&rExtInt.imr, INT_MASK_BITS * 0, INT_MASK_BITS, BIT_DISABLE);
	uint32_t pending = BIT_DISABLE;
	GetBits(&rExtInt.pr, INT_MASK_BITS * 0, INT_MASK_BITS, &pending);
	// flip led status
	if (true == led_on) {
		led_on = false;
	} else {
		led_on = true;
	}
	key_pressed = true;
	// clear pending bit in the pending register
	SetBits(&rExtInt.pr, PENDING_BITS * 0, PENDING_BITS, BIT_ENABLE);
}

int main(void)
{
	initGPIO();
	initKeyInterrupt();
	initTimer11();

	while (true) {
		if (!key_pressed) {
			continue;
		}
		if (true == led_on) {
			// reset GPIOC_13
			SetBits(&(rGPIOC.bs), GPIO_BITSET_BITS * led_pin + 16, GPIO_BITSET_BITS,
							1);
		} else {
			// set the GPIO PC13 to led_on
			SetBits(&(rGPIOC.bs), GPIO_BITSET_BITS * led_pin, GPIO_BITSET_BITS, 1);
		}
		key_pressed = false;
	}
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line
	 * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
	 * line)
	 */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
