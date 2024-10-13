#include <ti/devices/msp/msp.h>
#include "UARTRx.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "FIFO2.h"
#define PA8INDEX  18 // UART1_TX  SPI0_CS0  UART0_RTS TIMA0_C0  TIMA1_C0N
#define PA9INDEX  19 // UART1_RX  SPI0_PICO UART0_CTS TIMA0_C1  RTC_OUT   TIMA0_C0N TIMA1_C1N CLK_OUT
#define PA22INDEX 46 // UART2_RX  TIMG8_C1  UART1_RTS TIMA0_C1  CLK_OUT   TIMA0_C0N TIMG6_C1

static uint8_t buttonFrame;
static uint8_t joystickFrame;
static uint32_t ReceiveCount = 0;

void UART2_Init(void) {
	UART2->GPRCM.RSTCTL = 0xB1000003;
	UART2->GPRCM.PWREN = 0x26000001;
	Clock_Delay(24); // UART2 Power up
	//IOMUX->SECCFG.PINCM[PA10INDEX]  = 0x00000082;
	IOMUX->SECCFG.PINCM[PA22INDEX] = 0x00040082;
	UART2->CLKSEL = 0x08; // bus clock
	UART2->CLKDIV = 0x00; // no division
	UART2->CTL0 &= ~0x09; // disable UART2
	UART2->CTL0 = 0x00020018;
	// assumes an 80 MHz bus clock
	UART2->IBRD = 21; //   divider = 21+45/64 = 21.703125
	UART2->FBRD = 45; // baud =2,500,000/21.703125 = 115,191
	UART2->LCRH = 0x00000030;
	UART2->CPU_INT.IMASK = 0x0C01;
	// bit 11 TXINT
	// bit 10 RXINT
	// bit 0  Receive timeout
	UART2->IFLS = 0x0417;
	// bits 11-8 RXTOSEL receiver timeout select 4 (0xF highest)
	// bits 6-4  RXIFLSEL 1, interrupts when 1 or more elements
	// bits 2-0  TXIFLSEL 7, FIFO >= 1 entry free
	NVIC->ICPR[0] = 1 << 14;
	NVIC->ISER[0] = 1 << 14;
	NVIC->IP[3] = (NVIC->IP[3] & (~0xFF000000)) | (0 << 22); // priority (bits 31,30)
	UART2->CTL0 |= 0x09; // enable UART2
}

extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void) {
	while ((UART2->STAT & (1 << 2)) == 0) {
		uint8_t receivedFrame = UART2->RXDATA;
		if (receivedFrame & 0x01) {			// bit 0 high, it contains button data
			buttonFrame = receivedFrame;
		} else {
			joystickFrame = receivedFrame;
		}
	}
	uint32_t status = UART2->CPU_INT.IIDX;	// Acknowledge interrupt
	++ReceiveCount;
}

bool UART2_getShoot() {
	return buttonFrame & 0x02;
}

bool UART2_getUp() {
	return buttonFrame & 0x04;
}

bool UART2_getLeft() {
	return buttonFrame & 0x08;
}

bool UART2_getDown() {
	return buttonFrame & 0x10;
}

bool UART2_getRight() {
	return buttonFrame & 0x20;
}

uint8_t UART2_getAngle() {
	return (joystickFrame >> 1);
}
