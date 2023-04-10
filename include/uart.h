#ifndef __DEV_EBOX_UART_H__
#define __DEV_EBOX_UART_H__

#include <stdio.h>
#include <stdint.h>

typedef struct UartType {
	/* offset: 0x00 */
	// status register
	uint32_t sr;
	// data register
	uint32_t dr;
	// baud rate register
	uint32_t brr;
	// control register 1
	uint32_t cr1;
	/* offset: 0x10 */
	// control register 2
	uint32_t cr2;
	// control register 3
	uint32_t cr3;
	// guard time and prescaler register
	uint32_t gtpr;
} UartType;

int InitUart(uint32_t baudrate);
int UartTransmitOneByte(uint8_t data);
int UartTransmitMultipleBytes(uint8_t *data, size_t size);

#endif
