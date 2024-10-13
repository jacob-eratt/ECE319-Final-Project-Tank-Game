#include <ti/devices/msp/msp.h>
#include "UARTTx.h"
#include "../inc/Clock.h"
#define PA8INDEX 18

// power Domain PD0
// for 80MHz bus clock, UART clock is ULPCLK 40MHz
// initialize UART1 for 9600 baud rate
// blind, no synchronization on transmit
void UART1_Init(void) {
	UART1->GPRCM.RSTCTL = 0xB1000003;
	UART1->GPRCM.PWREN = 0x26000001;
	Clock_Delay(24); // UART1 Power up
	IOMUX->SECCFG.PINCM[PA8INDEX] = 0x00000082;
//	IOMUX->SECCFG.PINCM[PA11INDEX]  = 0x00040082;
	UART1->CLKSEL = 0x08; // bus clock
	UART1->CLKDIV = 0x00; // no divide
	UART1->CTL0 &= ~0x01; // disable UART1
	UART1->CTL0 = 0x00020018;
	// assumes an 80 MHz bus clock
	UART1->IBRD = 21; //   divider = 21+45/64 = 21.703125
	UART1->FBRD = 45; // baud =2,500,000/21.703125 = 115,191
	UART1->LCRH = 0x00000030; // 8 bits, 1 stop bit, no parity
	UART1->CTL0 |= 0x01; // enable UART1
}

//------------UART1_OutChar------------
// Output 8-bit to serial port
// blind synchronization
// 10 bit frame, 115200 baud
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none


/**
 * Output a frame containing Button data
 */
void UART1_OutButtons(uint8_t data) {
	UART1->TXDATA = data | 0x01;
}

/**
 * Output a frame containing Joystick data
 */
void UART1_OutJoystick(uint8_t data) {
	UART1->TXDATA = data & ~0x01;
}

/**
 * Output a frame containing LED data
 */
//void UART1_OutLED(uint8_t data) {
//	UART1->TXDATA = data & ~0x80;
//}
