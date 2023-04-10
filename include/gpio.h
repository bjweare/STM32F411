#ifndef __DEV_EBOX_GPIO_H__
#define __DEV_EBOX_GPIO_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum GPIO_PIN {
	GPIO_PIN_0 = 0,
	GPIO_PIN_1 = 1,
	GPIO_PIN_2 = 2,
	GPIO_PIN_3 = 3,
	GPIO_PIN_4 = 4,
	GPIO_PIN_5 = 5,
	GPIO_PIN_6 = 6,
	GPIO_PIN_7 = 7,
	GPIO_PIN_8 = 8,
	GPIO_PIN_9 = 9,
	GPIO_PIN_10 = 10,
	GPIO_PIN_11 = 11,
	GPIO_PIN_12 = 12,
	GPIO_PIN_13 = 13,
	GPIO_PIN_14 = 14,
	GPIO_PIN_15 = 15
} GPIO_PIN;

#define GPIO_MODE_BITS (2)
#define GPIO_OTYPE_BITS (1)
#define GPIO_OSPEED_BITS (2)
#define GPIO_PUPD_BITS (2)
#define GPIO_BITSET_BITS (1)
#define GPIO_AF_BITS (4)
typedef enum GPIO_MODE {
	GPIO_MODE_INPUT = 0b00,
	GPIO_MODE_OUTPUT = 0b01,
	GPIO_MODE_AF = 0b10,
	GPIO_MODE_ANALOG = 0b11
} GPIO_MODE;

typedef enum GPIO_OTYPE { // Output Type
	GPIO_OTYPE_PP = 0, // Push-Pull
	GPIO_OTYPE_OD = 1 // Open-Drain
} GPIO_OTYPE;

typedef enum GPIO_OSPEED { // Output Speed
	GPIO_OSPEED_LS = 0b00, // Low Speed
	GPIO_OSPEED_MS = 0b01, // Medium Speed
	GPIO_OSPEED_FS = 0b10, // Fast Speed
	GPIO_OSPEED_HS = 0b11 // High Speed
} GPIO_OSPEED;

typedef enum GPIO_PUPD {
	GPIO_PUPD_NONE = 0b00, // No pull-up, pull-down
	GPIO_PUPD_PU = 0b01, // Pull-Up
	GPIO_PUPD_PD = 0b10, // Pull-down
	GPIO_PUPD_RESERVED = 0b11 // No pull-up, pull-down
} GPIO_PUPD;

typedef enum GPIO_STATUS {
	GPIO_STATUS_LOW = 0, // Low Level
	GPIO_STATUS_HIGH = 1 // High Level
} GPIO_STATUS;

typedef enum GPIO_AF {
	GPIO_AF_SYSTEM = 0b0000,
	GPIO_AF_TIM_1_2 = 0b0001,
	GPIO_AF_TIM_3_5 = 0b0010,
	GPIO_AF_TIM_9_11 = 0b0011,
	GPIO_AF_I2C_1_3 = 0b0100,
	GPIO_AF_SPI_1_4 = 0b0101,
	GPIO_AF_SPI_3_5 = 0b0110,
	GPIO_AF_USART_1_2 = 0b0111,
	GPIO_AF_USART_6 = 0b1000,
	GPIO_AF_I2C_2_3 = 0b1001,
	GPIO_AF_OTG_FS = 0b1010,
	GPIO_AF_11 = 0b1011,
	GPIO_AF_SDIO = 0b1100,
	GPIO_AF_13 = 0b1101,
	GPIO_AF_14 = 0b1110,
	GPIO_AF_EVENTOUT = 0b1111,
} GPIO_ALTERNATE_FUNC;

typedef struct GPIOType {
	// 0x00~0x0c - four configuration registers
	// 0x00 - GPIO port mode register
	uint32_t mode;
	// 0x04 - GPIO port output type register
	uint32_t otype;
	// 0x08 - GPIO port output speed register
	uint32_t ospeed;
	// 0x0c - GPIO port pull-up/pull-down register
	uint32_t pupd;
	// 0x10~0x14 - two data registers
	// 0x10 - GPIO port input data register
	uint32_t id;
	// 0x14 - GPIO port output data register
	uint32_t od;
	// 0x18 - a bit set/reset register
	// 0x18 - GPIO port bit set/reset register
	uint32_t bs;
	// 0x1c - a locking register
	// 0x1c - GPIO port configuration lock register
	uint32_t lck;
	// 0x20~0x24 - two alternate function selection registers
	// 0x20 - GPIO alternate function low register
	uint32_t afrl;
	// 0x24 - GPIO alternate function high register
	uint32_t afrh;
} GPIOType;

extern GPIOType *rGPIOA;
extern GPIOType *rGPIOC;
extern bool led_on;
extern bool key_pressed;
extern uint8_t led_pin;
extern uint8_t key_pin;

int InitGPIO(uint8_t led_pin, bool led_on, uint8_t key_pin);
int SetLEDStatus(uint8_t led_pin, bool led_on);

#endif
