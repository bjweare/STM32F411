#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bit.h"
#include "rcc.h"
#include "exti.h"
#include "sys_cfg.h"
#include "timer.h"
#include "nvic.h"
#include "wwdg.h"
#include "power.h"
#include "flash.h"
#include "gpio.h"
#include "uart.h"

int main(void)
{
	// see 3.4.1 in RM0383 for more details about relation between CPU clock and
	// flash read time.
	EnablePowerClock();
	SetVOS(VOS_SCALE_1_100MHz);
	SetFlashLatency(FLASH_LATENCY_3);
	// config PLL to archive 96 MHz PLL clock output based on 25 MHz HSE clock
	EnableHighSpeedExternalClock();
	EnableSysConfClock();
	EnableGPIOClock();
	EnableTimer11Clock();

	InitUart(115200);

	InitGPIO(led_pin, led_on, key_pin);
	// TODO(bjweare): set interrupt priority

	initKeyInterrupt();

	// TODO(bjweare): set interrupt priority
	initTimer11(1000);

	// enable NVIC interrupt
	EnableEXTI0Interrupt();
	EnableTimer11Interrupt();

	while (true) {
		if (!key_pressed) {
			continue;
		}

		UartTransmitOneByte('A');
		UartTransmitOneByte('\n');
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
