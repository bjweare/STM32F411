#include "uart.h"

#include "bit.h"
#include "gpio.h"
#include "rcc.h"

/*
 * CTS - Clear To Send
 * RTS - Request To Send
 * TXE - transmit data register empty
 * RXNE - read data register not empty
 */

#define USART0_BASE (0x40011000)

/* start bit of each bit-field in status register */
enum SR_BIT_FIELD {
	// CTS flag
	BF_CTS = 9,
	// LIN break detection flag
	BF_LBD = 8,
	// transmit data register empty
	BF_TXE = 7,
	// transmission complete
	BF_TC = 6,
	// read data register not empty
	BF_RXNE = 5,
	// IDLE line detected
	BF_IDLE = 4,
	// overrun error
	BF_ORE = 3,
	// noise detected flag
	BF_NF = 2,
	// framing error
	BF_FE = 1,
	// parity error
	BF_PE = 0,
};

/* start bit of each bit-field in data register */
enum DR_BIT_FIELD {
	// data value
	BF_DR = 0,
#define DR_BITS (9)
};

/* start bit of each bit-field in baud rate register */
enum BRR_BIT_FIELD {
	// mantissa of USARTDIV
	BF_DIV_Mantissa = 4,
#define DIV_Mantissa_BITS (12)
	// fraction of USARTDIV, when BF_OVER8=1, the DIV_Fraction3 bit is not
	// considered and must be kept cleared
	BF_DIV_Fraction = 0,
#define DIV_Fraction_BITS (4)
};

/* start bit of each bit-field in control register 1 */
enum CR1_BIT_FIELD {
	// oversampling mode
	BF_OVER8 = 15,
	// USART enable
	BF_UE = 13,
	// word length
	BF_M = 12,
	// wakeup method
	BF_WAKE = 11,
	// parity control enable
	BF_PCE = 10,
	// parity selection
	BF_PS = 9,
	// Parity Error interrupt enable
	BF_PEIE = 8,
	// TranSmit data register Empty interrupt enable
	BF_TXEIE = 7,
	// Transmission Complete interrupt enable
	BF_TCIE = 6,
	// RXNE interrupt enable
	BF_RXNEIE = 5,
	// IDLE interrupt enable
	BF_IDELIE = 4,
	// transmitter enable
	BF_TE = 3,
	// receiver enable
	BF_RE = 2,
	// receiver wakeup
	BF_RWU = 1,
	// send break
	BF_SBK = 0,
};

enum OVER8_MODE {
	OVERSAMPLING_MODE_8 = 0b01,
	OVERSAMPLING_MODE_16 = 0b00,
};

enum M_VAL {
	// 1 start bit, 8 data bits
	WORD_LENGTH_8BITS = 0b0,
	// 1 start bit, 9 data bits
	WORD_LENGTH_9BITS = 0b1,
};

/* start bit of each bit-field in control register 2 */
enum CR2_BIT_FIELD {
	// LIN mode enable
	BF_LINEN = 14,
	// stop bits
	BF_STOP = 12,
#define STOP_BITS (2)
	// clock enable
	BF_CLKEN = 11,
	// clock polarity
	BF_CPOL = 10,
	// clock phase
	BF_CPHA = 9,
	// last bit clock pulse, used only in synchronous mode
	BF_LBCL = 8,
	// LIN break detection interrupt enable
	BF_LBDIE = 6,
	// LIN break detection length
	BF_LBDL = 5,
	// address of the USART node
	BF_ADD = 0,
#define ADD_BITS (4)
};

enum STOP_VAL {
	// 0.5 stop bit
	STOP_BITS_0_5 = 0b01,
	// 1 stop bit
	STOP_BITS_1 = 0b00,
	// 2 stop bits
	STOP_BITS_2 = 0b10,
};

/* start bit of each bit-field in control register 3 */
enum CR3_BIT_FIELD {
	// one sample bit method enable
	BF_ONEBIT = 11,
	// CTS interrupt enable
	BF_CTSIE = 10,
	// CTS enable
	BF_CTSE = 9,
	// RTS enable
	BF_RTSE = 8,
	// DMA enable transmitter
	BF_DMAT = 7,
	// DMA enable receiver
	BF_DMAR = 6,
	// smartcard mode enable
	BF_SCEN = 5,
	// smartcard NACK enable
	BF_NACK = 4,
	// single-wire half-duplex selection
	BF_HDSEL = 3,
	// IrDA low-power
	BF_IRLP = 2,
	// IrDA mode enable
	BF_IREN = 1,
	// error interrupt enable
	BF_EIE = 0,
};

enum ONEBIT_VAL {
	// three sample bit method
	THREE_SAMPLE_BIT_METHOD = 0b0,
	// one sample bit method
	ONE_SAMPLE_BIT_METHOD = 0b1,
};

/* start bit of each bit-field in control register 3 */
enum GTPR_BIT_FIELD {
	// guard time value, used only in smartcard mode
	BF_GT = 8,
#define GT_BITS (8)
	// prescaler value, used only in smartcard mode and IrDA mode
	BF_PSC = 0,
#define PSC_BITS (8)
};

UartType *rUart0 = (UartType *)USART0_BASE;

int InitUart(uint32_t baudrate)
{
	// configure alternate function of PA9/PA10 to UART1 TXD/RXD
	// set mode to AF
	uint8_t gpio_pin = GPIO_PIN_9;
	SetBits(&rGPIOA->otype, GPIO_OTYPE_BITS * gpio_pin, GPIO_OTYPE_BITS,
					GPIO_OTYPE_PP);
	SetBits(&rGPIOA->mode, GPIO_MODE_BITS * gpio_pin, GPIO_MODE_BITS,
					GPIO_MODE_AF);
	SetBits(&rGPIOA->afrh, GPIO_AF_BITS * (gpio_pin - 8), GPIO_AF_BITS,
					GPIO_AF_USART_1_2);
	gpio_pin = GPIO_PIN_10;
	SetBits(&rGPIOA->otype, GPIO_OTYPE_BITS * gpio_pin, GPIO_OTYPE_BITS,
					GPIO_OTYPE_PP);
	SetBits(&rGPIOA->mode, GPIO_MODE_BITS * gpio_pin, GPIO_MODE_BITS,
					GPIO_MODE_AF);
	SetBits(&rGPIOA->afrh, GPIO_AF_BITS * (gpio_pin - 8), GPIO_AF_BITS,
					GPIO_AF_USART_1_2);

	// enable USART0 clock
	SetUart0ClockStatus(BIT_ENABLE);
	// reset PCE in CR1 to disable parity control
	SetBits(&rUart0->cr1, BF_PCE, 1, BIT_DISABLE);
	// reset OVER8 in CR1 to enable oversampling 16
	bool over8 = BIT_DISABLE;
	SetBits(&rUart0->cr1, BF_OVER8, 1, over8);
	// reset ONEBIT in CR3 select three sample bit method
	SetBits(&rUart0->cr3, BF_ONEBIT, 1, THREE_SAMPLE_BIT_METHOD);
	// reset CTSE in CR3 to disable CTS
	SetBits(&rUart0->cr3, BF_CTSE, 1, BIT_DISABLE);
	// reset RTSE in CR3 to disable RTS
	SetBits(&rUart0->cr3, BF_RTSE, 1, BIT_DISABLE);

	/* start - description below takes from RM0383 19.3.2 */
	// enable the USART by writting the UE bit in CR1 register to 1.
	SetBits(&rUart0->cr1, BF_UE, 1, BIT_ENABLE);

	// program the M bit in CR1 to define the word length.
	SetBits(&rUart0->cr1, BF_M, 1, WORD_LENGTH_8BITS);

	// program the number of stop bits by setting the STOP bits in CR2.
	SetBits(&rUart0->cr2, BF_STOP, STOP_BITS, STOP_BITS_1);

	// select DMA enable (DMAT) in CR3 if multi buffer communication is to take
	// place. configure the DMA register as explained in multibuffer
	// communication.
	// TODO(bjweare): add DMA transmitter&receiver support

	// select the desired baud rate using BRR register
	uint8_t factor = (over8 == BIT_DISABLE) ? 16 : 8;
	uint16_t mantissa = (APB2CLK_MHz * 1000 * 1000) / baudrate / factor;
	uint16_t fraction = ((APB2CLK_MHz * 1000 * 1000) / baudrate) % factor;
	SetBits(&rUart0->brr, BF_DIV_Mantissa, DIV_Mantissa_BITS, mantissa);
	SetBits(&rUart0->brr, BF_DIV_Fraction, DIV_Fraction_BITS, fraction);

	/* end */

	return 0;
}

// TODO(bjweare): support interrupt transmitter and receiver
int UartTransmitOneByte(uint8_t data)
{
	// clear TC in SR by writting 0
	SetBits(&rUart0->sr, BF_TC, 1, BIT_DISABLE);

	/* start - description below takes from RM0383 19.3.2 */
	// set the TE bit in CR1 to send an idle frame as first transmission.
	SetBits(&rUart0->cr1, BF_TE, 1, BIT_ENABLE);

	// write the data to send in the DR register (this clears the TXE bit).
	// repeat this for each data to be transmitted in case of single buffer.
	SetBits(&rUart0->dr, BF_DR, DR_BITS, data);

	// after writting the last data into the DR register, wait until TC=1. this
	// indicates that the transmission of the last frame is complete. this is
	// required for instance when the USART is disabled or enters the HALT mode
	// to avoid corrupting the last transmission.
	uint32_t status = 0;
	do {
		GetBits(&rUart0->sr, BF_TC, 1, &status);
	} while (status != BIT_ENABLE);

	/* end */

	return 0;
}

// TODO(bjweare): support interrupt transmitter and receiver
int UartTransmitMultipleBytes(uint8_t *data, size_t size)
{
	// clear TC in SR by writting 0
	SetBits(&rUart0->sr, BF_TC, 1, BIT_DISABLE);

	/* start - description below takes from RM0383 19.3.2 */
	// set the TE bit in CR1 to send an idle frame as first transmission.
	SetBits(&rUart0->cr1, BF_TE, 1, BIT_ENABLE);

	uint32_t status = 0;
	// write the data to send in the DR register (this clears the TXE bit).
	// repeat this for each data to be transmitted in case of single buffer.
	for (int i = 0; i < size; i++) {
		SetBits(&rUart0->dr, BF_DR, DR_BITS, data[i]);

		// after writting the last data into the DR register, wait until TC=1. this
		// indicates that the transmission of the last frame is complete. this is
		// required for instance when the USART is disabled or enters the HALT mode
		// to avoid corrupting the last transmission.
		do {
			GetBits(&rUart0->sr, BF_TC, 1, &status);
		} while (status != BIT_ENABLE);
	}

	/* end */

	return 0;
}
