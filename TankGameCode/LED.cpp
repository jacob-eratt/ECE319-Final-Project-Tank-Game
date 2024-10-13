/*
 * LED.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "LED.h"
// LaunchPad.h defines all the indices into the PINCM table
#define PB16INDEX 32
#define PB17INDEX 42
#define RedLED 1<<16
#define YellowLED 1<<17
#define GreenLED 1<<19
//Does not work as of now 4/18
// initialize your LEDs
void LED_Init(void){
    // write this
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00000081; // GPIO output //RED
    IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00000081; // GPIO output //Yellow
    IOMUX->SECCFG.PINCM[PB19INDEX] = 0x00000081; // GPIO output // Green
    GPIOB->DOE31_0 = 1<<16|1<<17|1<<19;
}
// data specifies which LED to turn on

void LED_On(uint32_t data){
    // write this
    GPIOB->DOUTSET31_0 = data;
    // use DOUTSET31_0 register so it does not interfere with other GPIO

  
}

// data specifies which LED to turn off
void LED_Off(uint32_t data){
    // write this
    // use DOUTCLR31_0 register so it does not interfere with other GPIO
    GPIOB->DOUTCLR31_0 = data;
 
}

// data specifies which LED to toggle
void LED_Toggle(uint32_t data){
    // write this
    // use DOUTTGL31_0 register so it does not interfere with other GPIO
    GPIOB->DOUTTGL31_0 = data;
 
}
