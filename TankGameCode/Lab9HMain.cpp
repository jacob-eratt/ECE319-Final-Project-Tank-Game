// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 1/1/2024

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/SlidePot.h"
#include "SmallFont.h"

#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "LED.h"
#include "Joystick.h"
#include "Tank.h"
#include "UARTRx.h"

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz

bool tank2Up;
bool tank2Down;
bool tank2Left;
bool tank2Right;
bool tank2Shoot;
int angle;
Tank player1(1, 30, 50);
Tank player2(0, 100, 140);

void PLL_Init(void) { // set phase lock loop (PLL)
// Clock_Init40MHz(); // run this line for 40MHz
	Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M = 1;
uint32_t Random32(void) {
	M = 1664525 * M + 1013904223;
	return M;
}
uint32_t Random(uint32_t n) {
	return (Random32() >> 16) % n;
}

SlidePot Sensor(1500, 0); // copy calibration from Lab 7

// games  engine runs at 30Hz

uint8_t TExaS_LaunchPadLogicPB27PB26(void) {
	return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));
}

typedef enum {
	English, Spanish, Portuguese, French
} Language_t;
Language_t myLanguage = English;
typedef enum {
	HELLO, GOODBYE, LANGUAGE
} phrase_t;
const char Hello_English[] = "Hello";
const char Hello_Spanish[] = "\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] = "All\x83";
const char Goodbye_English[] = "Goodbye";
const char Goodbye_Spanish[] = "Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[] = "English";
const char Language_Spanish[] = "Espa\xA4ol";
const char Language_Portuguese[] = "Portugu\x88s";
const char Language_French[] = "Fran\x87" "ais";
const char *Phrases[3][4] = { { Hello_English, Hello_Spanish, Hello_Portuguese,
		Hello_French }, { Goodbye_English, Goodbye_Spanish, Goodbye_Portuguese,
		Goodbye_French }, { Language_English, Language_Spanish,
		Language_Portuguese, Language_French } };
// use main1 to observe special characters'

//testing LEDS and switches (main1)
int main1(void) {
	LaunchPad_Init();
	LED_Init();
	Switch_Init();
	while (1) {
		int x = Switch_In();
		int y = UP;
		if (Switch_In() & 1 << 17) {
			LED_On(GreenLED);
		}
		x = Switch_In();
		y = DOWN;
		if (Switch_In() & DOWN) {
			LED_On(RedLED);
		}
		x = Switch_In();
		y = LEFT;
		if (Switch_In() & LEFT) {
			LED_On(YellowLED);
		}
		x = Switch_In();
		y = RIGHT;
		if (Switch_In() & RIGHT) {
			LED_On(RedLED);
		}
	}
}

#define ADCVREF_VDDA 0x000
//testing joystick raw data (main2)
int main2() {
	LaunchPad_Init();
	Clock_Init80MHz(0);
	ST7735_InitPrintf();

	Joystick turret;
	turret.ADC_InitDual(3, 2, ADCVREF_VDDA);
	while (1) {
		turret.ADC_InDual();
		ST7735_SetCursor(0, 0);
		printf("%d, %d", turret.getX(), turret.getY());

	}
	return 1;
}

int main3() { //main3
	LaunchPad_Init();
	Clock_Init80MHz(0);
	ST7735_InitPrintf();
	Switch_Init();
	__disable_irq();
	//note: if you colors are weird, see different options for
	//ST7735_InitR(INITR_REDTAB);// inside ST7735_InitPrintf()
	ST7735_FillScreen(ST7735_BLACK);
	int x = 40;
	int y = 50;
	while (1) {
		ST7735_DrawBitmap(x, y, RedTankUp, 17, 17);
		if (Switch_In() & RIGHT) {
			//ST7735_DrawBitmap(x,50, blackHorizontal, 21,14);
			if (x < 99) {
				x++;
			}

		}
		if (Switch_In() & LEFT) {
			//ST7735_DrawBitmap(x,50, blackHorizontal, 21,14);
			if (x > 0) {
				x--;
			}
		}
		if (Switch_In() & UP) {
			//ST7735_DrawBitmap(x,50, blackHorizontal, 21,14);
			if (y > 0) {
				y--;
			}
		}
		if (Switch_In() & DOWN) {
			//ST7735_DrawBitmap(x,50, blackHorizontal, 21,14);
			if (y < 140) {
				y++;
			}
		}
		Clock_Delay1ms(70);
	}
}

//testing Rotation function
int main4() { //main4
	LaunchPad_Init();
	Clock_Init80MHz(0);
	ST7735_InitPrintf();
	Switch_Init();
	__disable_irq();
	//note: if you colors are weird, see different options for
	//ST7735_InitR(INITR_REDTAB);// inside ST7735_InitPrintf()
	ST7735_FillScreen(ST7735_BLACK);
	Tank player1(1, 30, 50);
	while (1) {
		player1.updateTank();

		//Clock_Delay1ms(70);

	}

}

//testing bullets appears in the same direction the tank is facing
int main5() {
	LaunchPad_Init();
	Clock_Init80MHz(0);
	ST7735_InitPrintf();
	Switch_Init();
	ST7735_FillScreen(ST7735_BLACK);
	Tank player1(1, 30, 50);
	while (1) {
		player1.updateTank();

		player1.isShoot();

		Clock_Delay1ms(70);
	}

}

//testing shooting functions
int main6() { //main6
	LaunchPad_Init();
	Clock_Init80MHz(0);
	ST7735_InitPrintf();
	Switch_Init();
	ST7735_FillScreen(ST7735_BLACK);
	Tank player1(1, 30, 50);
	while (1) {
		player1.updateTank();
		Clock_Delay1ms(70);
		//player1.updateBullets(walls);
	}
}

int main7() {
	LaunchPad_Init();
	Clock_Init80MHz(0);
	ST7735_InitPrintf();
	Switch_Init();
	ST7735_FillScreen(ST7735_WHITE);
	Tank player1(1, 30, 50);
	wall wall1(40, 80, 70, 70);
	walls.push_back(wall1);
	ST7735_DrawBitmap(40, 80, HorizontalWall, 30, 10);
	while (1) {
		player1.updateTank();
		Clock_Delay1ms(70);
		//player1.updateBullets(walls);
	}
}

#define gun (1<<1)
#define upButton (1<<2)
#define lftButton 1<<3
#define dwnButton 1<<4
#define rhtButton 1<<5

void TIMG12_IRQHandler(void) {
	uint32_t pos, msg;
	if ((TIMG12->CPU_INT.IIDX) == 1) { // this will acknowledge
		GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
		GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
		uint8_t frame;

		player2.setShoot(UART2_getShoot());
		player2.setUp(UART2_getUp());
		player2.setLeft(UART2_getLeft());
		player2.setDown(UART2_getDown());
		player2.setRight(UART2_getRight());
		player2.setCheckAngle(UART2_getAngle());

		uint32_t x = Switch_In();
		player1.setShoot(Switch_In() & 1 << 17);
		player1.setUp(Switch_In() & UP);
		player1.setDown(Switch_In() & DOWN);
		player1.setLeft(Switch_In() & LEFT);
		player1.setRight(Switch_In() & RIGHT);
		player1.setAngle();

		GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)

	}
}


int main() {
	__disable_irq();
	LaunchPad_Init();
	Clock_Init80MHz(0);
	ST7735_InitPrintf();
	ST7735_InvertDisplay(1);
	ST7735_FillScreen(ST7735_BLACK);
	UART2_Init();
	Switch_Init();
	Sound_Init();
	TimerG12_IntArm(2666666, 2);
	uint32_t wasHitP1;
	uint32_t wasHitP2 = 0;
	ST7735_SetCursor(0, 0);
	ST7735_OutString("For English press UP.");

	ST7735_OutString("Para espa\xa4ol,\n presione el bot\xa2n\nDOWN.");

	bool isEnglish = false;

	while (!(Switch_In() & UP) && !(Switch_In()&DOWN) ) {

	}
	if(Switch_In() & UP){
		isEnglish = true;
	}


	while (1) {
		__disable_irq();
		ST7735_FillScreen(ST7735_BLACK);
		player1.setXPos(60);
		player1.setYPos(20);
		player2.setXPos(60);
		player2.setYPos(150);
		wall wall2(45,120, 75,110 );
		wall wall1(45, 40, 75,30 );
		walls.push_back(wall1);
		walls.push_back(wall2);

		ST7735_DrawBitmap(45, 40, HorizontalWall, 30, 10);
		ST7735_DrawBitmap(45, 120, HorizontalWall, 30, 10);

		if (player1.getLives() <= 0 || player2.getLives() <= 0) {
			break;
		}

		__enable_irq();
		while (1) {
			player1.updateTank();
			player2.updateTank();
			Clock_Delay1ms(35);
			wasHitP1 = player1.updateBullets(walls, player2.getBulletArr());
			wasHitP2 = player2.updateBullets(walls, player1.getBulletArr());

			if (wasHitP1 || wasHitP2) {
				Sound_Explosion();
				if (wasHitP1) {
					player1.setLives(player1.getLives() - wasHitP1);
					player1.destroyTank();
					Clock_Delay1ms(1000);
				} else {
					player2.setLives(player2.getLives() - wasHitP2);
					player2.destroyTank();
					Clock_Delay1ms(1000);
				}
				player1.destroyBullets();
				player2.destroyBullets();
				Clock_Delay1ms(1000);
				break;
			}

		}
	}

	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(0, 0);
	if(isEnglish){
	printf("player1 score: %d\n", player1.getLives());
	printf("player2 score: %d", player2.getLives());
	}
	else{
		ST7735_OutString("Jugador 1:\npuntuaci\xa2n: ");
		printf("%d\n", player1.getLives());
		ST7735_OutString("Jugador 2:\npuntuaci\xa2n: ");
		printf("%d", player2.getLives());
	}
	return 0;
}

