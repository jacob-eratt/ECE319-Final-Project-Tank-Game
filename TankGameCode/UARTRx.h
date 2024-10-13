#ifndef UARTRX_H_
#define UARTRX_H_
/**
 * initialize UART2 for 2000 bps baud rate.<br>
 * PA22 UART2 Rx from other microcontroller PA8 Tx<br>
 * no transmit, interrupt on receive timeout
 * @param none
 * @return none
 * @brief  Initialize UART1
*/
void UART2_Init(void);

bool UART2_getShoot();

bool UART2_getUp();

bool UART2_getLeft();

bool UART2_getDown();

bool UART2_getRight();

uint8_t UART2_getAngle();

#endif /* UARTRX_H_ */
