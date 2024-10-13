/*
 * Joystick.cpp
 *
 *  Created on: Apr 20, 2024
 *      Author: jacob
 */

/* SlidePot.cpp
 * Students put your names here
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "../inc/ADC.h"
#include "../inc/Clock.h"
#include "Joystick.h"
#define ADCVREF_VDDA 0x000
#define ADCVREF_INT  0x200

#define PA17INDEX 38
Joystick::Joystick() {


    // Create an array of pointers

x = 0;
y = 0; //raw y value
flag = 0;// 0 if data is not valid, 1 if valid
angle = 0;
distance = 0;// distance in 0.001cm
// distance = (slope*data+offset)/4096
slope = 0;// calibration coeffients
offset = 0;


}

// Assumes 40 MHz bus
void Joystick::ADC_InitDual(uint32_t channel1, uint32_t channel2,
uint32_t reference) {
 // Reset ADC and VREF
 // RSTCLR
 //   bits 31-24 unlock key 0xB1
 //   bit 1 is Clear reset sticky bit
 //   bit 0 is reset ADC

ADC0->ULLMEM.GPRCM.RSTCTL = (uint32_t) 0xB1000003;
if (reference == ADCVREF_INT) {
VREF->GPRCM.RSTCTL = (uint32_t) 0xB1000003;
}
	// Enable power ADC and VREF
	// PWREN
	//   bits 31-24 unlock key 0x26
	//   bit 0 is Enable Power
ADC0->ULLMEM.GPRCM.PWREN = (uint32_t) 0x26000001;
if (reference == ADCVREF_INT) {
VREF->GPRCM.PWREN = (uint32_t) 0x26000001;
}
Clock_Delay(24); // time for ADC and VREF to power up
ADC0->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // ULPCLK
  // bits 31-24 key=0xA9
  // bit 5 CCONSTOP= 0 not continuous clock in stop mode
  // bit 4 CCORUN= 0 not continuous clock in run mode
  // bit 1-0 0=ULPCLK,1=SYSOSC,2=HFCLK
ADC0->ULLMEM.CLKFREQ = 7; // 40 to 48 MHz
ADC0->ULLMEM.CTL0 = 0x03010000;
 // bits 26-24 = 011 divide by 8
 // bit 16 PWRDN=1 for manual, =0 power down on completion, if no pending trigger
 // bit 0 ENC=0 disable (1 to 0 will end conversion)
ADC0->ULLMEM.CTL1 = 0x00010000;
 // bits 30-28 =0  no shift
 // bits 26-24 =0  no averaging
 // bit 20 SAMPMODE=0 high phase
 // bits 17-16 CONSEQ=01 ADC at start to end
 // bit 8 SC=0 for stop, =1 to software start
 // bit 0 TRIGSRC=0 software trigger
ADC0->ULLMEM.CTL2 = 0x02010000;
  // bits 28-24 ENDADD=2 (which  MEMCTL to end)
  // bits 20-16 STARTADD=1 (which  MEMCTL to start)
  // bits 15-11 SAMPCNT (for DMA)
  // bit 10 FIFOEN=0 disable FIFO
  // bit 8  DMAEN=0 disable DMA
  // bits 2-1 RES=0 for 12 bit (=1 for 10bit,=2for 8-bit)
  // bit 0 DF=0 unsigned formant (1 for signed, left aligned)
ADC0->ULLMEM.MEMCTL[1] = reference + channel1;
ADC0->ULLMEM.MEMCTL[2] = reference + channel2;
  // bit 28 WINCOMP=0 disable window comparator
  // bit 24 TRIG trigger policy, =0 for auto next, =1 for next requires trigger
  // bit 20 BCSEN=0 disable burn out current
  // bit 16 = AVGEN =0 for no averaging
  // bit 12 = STIME=0 for SCOMP0
  // bits 9-8 VRSEL = 10 for internal VREF,(00 for VDDA)
  // bits 4-0 channel = 0 to 7 available
ADC0->ULLMEM.SCOMP0 = 0; // 8 sample clocks
//  adc12->ULLMEM.GEN_EVENT.ICLR |= 0x0100; // clear flag MEMCTL[1] ??
ADC0->ULLMEM.GEN_EVENT.IMASK = 0; // no interrupt
if (reference == ADCVREF_INT) {
VREF->CLKSEL = 0x00000008; // bus clock
VREF->CLKDIV = 0; // divide by 1
VREF->CTL0 = 0x0001;
// bit 8 SHMODE = off
// bit 7 BUFCONFIG=0 for 2.4 (=1 for 1.4)
// bit 0 is enable
VREF->CTL2 = 0;
// bits 31-16 HCYCLE=0
// bits 15-0 SHCYCLE=0
while ((VREF->CTL1 & 0x01) == 0) {
}; // wait for VREF to be ready
}
}

// sample 12-bit ADC
void Joystick::ADC_InDual() {
ADC0->ULLMEM.CTL0 |= 0x00000001; // enable conversions
ADC0->ULLMEM.CTL1 |= 0x00000100; // start ADC
uint32_t volatile delay = ADC0->ULLMEM.STATUS; // time to let ADC start
while ((ADC0->ULLMEM.STATUS & 0x01) == 0x01) {
}; // wait for completion
x = ADC0->ULLMEM.MEMRES[1];
y = ADC0->ULLMEM.MEMRES[2];
}

uint32_t Joystick::getX() {
return x;
}
uint32_t Joystick::getY() {
return y;
}
// x = 2018 on center, y= 1982 on center



//create angle vector number. This means it returns a number between 0 and 16
uint32_t Joystick::createAngleNum() {
ADC_InDual();

uint32_t numX = (x) / 460;
uint32_t numY = (y) / 460;

uint32_t val = lookUp[numY][numX];
return val;

}

//}

/*
 uint32_t SlidePot::In(void){
 // write code to sample ADC1 channel 5, PB18 once
 // return digital result (0 to 4095)
 ADC1->ULLMEM.CTL0 |= 0x00000001;             // 1) enable conversions
 ADC1->ULLMEM.CTL1 |= 0x00000100;             // 2) start ADC
 uint32_t volatile delay=ADC1->ULLMEM.STATUS; // 3) time to let ADC start
 while((ADC1->ULLMEM.STATUS&0x01)==0x01){}    // 4) wait for completion
 return ADC1->ULLMEM.MEMRES[0];
 }


 // constructor, invoked on creation of class
 // m and b are linear calibration coefficents
 SlidePot::SlidePot(uint32_t m, uint32_t b){
 // write this, runs on creation
 slope = m;
 offset = b;
 }

 void SlidePot::Save(uint32_t n){
 // write this
 data = n;
 flag = 1;
 }
 uint32_t SlidePot::Convert(uint32_t n){
 // write this
 // convert ADC raw sample n to fixed-point distance, 0.001cm
 return ((slope*n)>>12)+offset; // replace this with solution
 }
 // do not use this function
 // it is added just to show you how SLOW floating point in on a Cortex M0+
 float SlidePot::FloatConvert(uint32_t input){
 return 0.00048828125*input -0.0001812345;
 }

 void SlidePot::Sync(void){
 // write this

 while(!flag){}
 flag = 0;

 // wait for semaphore, then clear semaphore


 }


 // distance = (slope*data+offset)/4096
 uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
 // write this;
 return (slope*data+offset)/4096;
 }
 */

