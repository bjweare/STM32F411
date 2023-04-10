#include "gpio.h"

#include "bit.h"

#define GPIOA_BASE (0x40020000)
#define GPIOC_BASE (0x40020800)

GPIOType *rGPIOA = (GPIOType *)GPIOA_BASE;
GPIOType *rGPIOC = (GPIOType *)GPIOC_BASE;

bool led_on = false;
bool key_pressed = false;
uint8_t led_pin = GPIO_PIN_13;
uint8_t key_pin = GPIO_PIN_0;

int InitGPIO(uint8_t led_pin, bool led_on, uint8_t key_pin)
{
	// set mode to output
	SetBits(&(rGPIOC->mode), GPIO_MODE_BITS * led_pin, GPIO_MODE_BITS,
					GPIO_MODE_OUTPUT);
	// set output type to open-drain
	SetBits(&(rGPIOC->otype), GPIO_OTYPE_BITS * led_pin, GPIO_OTYPE_BITS,
					GPIO_OTYPE_OD);
	// set output speed to medium speed
	SetBits(&(rGPIOC->ospeed), GPIO_OSPEED_BITS * led_pin, GPIO_OSPEED_BITS,
					GPIO_OSPEED_MS);
	// set the GPIO PC13 to led_on
	SetLEDStatus(led_pin, led_on);

	// set mode to input
	SetBits(&(rGPIOA->mode), GPIO_MODE_BITS * key_pin, GPIO_MODE_BITS,
					GPIO_MODE_INPUT);
	// set port to pull-up
	SetBits(&(rGPIOA->pupd), GPIO_PUPD_BITS * key_pin, GPIO_PUPD_BITS,
					GPIO_PUPD_PU);

	return 0;
}

int SetLEDStatus(uint8_t led_pin, bool led_on)
{
	if (true == led_on) {
		// reset GPIOC_13
		SetBits(&(rGPIOC->bs), GPIO_BITSET_BITS * led_pin + 16, GPIO_BITSET_BITS,
						1);
	} else {
		// set the GPIO PC13 to led_on
		SetBits(&(rGPIOC->bs), GPIO_BITSET_BITS * led_pin, GPIO_BITSET_BITS, 1);
	}
	return 0;
}
