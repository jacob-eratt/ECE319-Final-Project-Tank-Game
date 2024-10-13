/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table

void Switch_Init(void){
    IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00040081; // GPIO input//UP
    IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081; // GPIO input //Left
    IOMUX->SECCFG.PINCM[PA28INDEX] = 0x00040081; // GPIO input //PA28 /Down
    IOMUX->SECCFG.PINCM[PA31INDEX] = 0x00040081; // GPIO input //PA31 //Right
    IOMUX->SECCFG.PINCM[PA17INDEX] = 0x00050081; //joystick switch PA17
}
// return current state of switches
uint32_t Switch_In(void){
    int x = GPIOA->DIN31_0;
    return x;

  //replace this your code
}
