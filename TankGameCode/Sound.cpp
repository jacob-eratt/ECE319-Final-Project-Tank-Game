// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "DAC5.h"
#include "../inc/Timer.h"
#include "../inc/LaunchPad.h"	//***** Only for 5 Bit DAC Init

static const uint8_t *soundPtr;
static uint32_t soundLen;
static uint32_t count;

void SysTick_IntArm(uint32_t period, uint32_t priority) {

}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void) {
	DAC5_Init();
	SysTick->CTRL = 0;				// disable during init
	SysTick->LOAD = 7272 - 1;		// 11kHz ISR Frequency
	SCB->SHP[1] = (SCB->SHP[1] & (0xC0000000) | (1 << 30)); //Priority of 1
	SysTick->VAL = 0;				// Clear count to reload
	SysTick->CTRL = 0x05;			// CLK_SRC to 1, INTEN to 0, ENABLE to 1
}
extern "C" void SysTick_Handler(void);
void SysTick_Handler(void) { // called at 11 kHz
	// output one value to DAC if a sound is active;
	if (count > soundLen) {
		SysTick->CTRL = 0x05;
	}
	DAC5_Out(soundPtr[count++] >> 3);
}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t len) {
// write this
	SysTick->CTRL = 0x05;
	soundPtr = pt;
	soundLen = len;
	count = 0;
	SysTick->CTRL = 0x07;
}
void Sound_Shoot(void) {
	Sound_Start(shoot, 4080);
}
void Sound_Killed(void) {
	Sound_Start(invaderkilled, 3377);
}
void Sound_Explosion(void) {
	Sound_Start(explosion, 2000);
}

//void Sound_Fastinvader1(void) {
//	Sound_Start(fastinvader1, 982);
//}
//void Sound_Fastinvader2(void) {
//	Sound_Start(fastinvader2, 1042);
//}
//void Sound_Fastinvader3(void) {
//	Sound_Start(fastinvader3, 1054);
//}
//void Sound_Fastinvader4(void) {
//	Sound_Start(fastinvader4, 1098);
//}
//void Sound_Highpitch(void) {
//	Sound_Start(highpitch, 1802);
//}
