#ifndef UARTTX_H_
#define UARTTX_H_
/**
 * Initialize UART1
 * PA8 is Tx
 */
void UART1_Init(void);

/**
 * Output 8-bit to serial port
 * No waiting, if transmitter busy data is lost
 * Bit #:	Lw	0	1	2	3	4	5	6	7	Hh
 * 				b0	b1	0	0	0	0	0	0
 */


/**
 * Bit #:	Lw	0	1	2	3	4	5	6	7	Hh
 * 				1	Gun	Up	Lft	Dwn	Rht	0	0
 */
void UART1_OutButtons(uint8_t data);

/**
 * Bit #:	Lw	0	1	2	3	4	5	6	7	Hh
 * 				0	b0	b1	b2	b3	b4	b5	b6
 */
void UART1_OutJoystick(uint8_t data);

//void UART1_OutLED(uint8_t data);

#endif /* UARTTX_H_ */
