/*
 * Bullet.cpp
 *
 *  Created on: Apr 21, 2024
 *      Author: jacob
 */

#include "Bullet.h"
using namespace std;

void Bullet::destroyBullet() {
	ST7735_DrawBitmap(convert(xPos), convert(yPos), clearBullet, 4, 4);
	inUse = false;
}

void Bullet::updatePos() {
	uint32_t changex = velocity[2 * angle];
	uint32_t cahngeY = velocity[2 * angle + 1];
	xPos += velocity[2 * angle];
	yPos += velocity[2 * angle + 1];

}

bool Bullet::validCreate(uint32_t x, uint32_t y) {
	if (x < 1 || x > 124 || y < 1 || y > 156) {
		return false;
	}

	for (int i = 0; i < walls.size(); i++) {
		if (x >= walls[i].xBR - 4 && x <= walls[i].xTL+2 && y >= walls[i].yTL-2
				&& y <= walls[i].yBR + 4) {
			return false;
		}
	}
	return true;
}

bool Bullet::createBullet(uint32_t _xPos, uint32_t _yPos, uint32_t _vIndex) {

	uint32_t index = angleToOffset[2 * _vIndex];
	xPos = (_xPos + angleToOffset[2 * _vIndex]);


	index = angleToOffset[2 * _vIndex + 1];
	yPos = (_yPos + angleToOffset[2 * _vIndex + 1]);

	if (!validCreate(xPos,yPos)) {
		inUse = false;
		return false;
	}
	xPos = xPos << fixedPoint;
	inUse = true;
	yPos = yPos << fixedPoint;
	angle = _vIndex;
	numBounce = NUM_BOUNCE;
	return true;
}

void Bullet::displayBullet() {
	uint32_t x = xPos;
	x += (1 << (fixedPoint - 1));
	x = x >> fixedPoint;

	uint32_t y = yPos;
	y += (1 << (fixedPoint - 1));
	y = y >> fixedPoint;

	ST7735_DrawBitmap(x, y, bullet, 4, 4);

}

uint32_t Bullet::convert(uint32_t a) {
	a += (1 << (fixedPoint - 1));
	a = a >> fixedPoint;
	return a;

}

void Bullet::makeBulletLine() {

	uint32_t x = convert(xPos);
	uint32_t y = convert(yPos);
	ST7735_DrawBitmap(x, y, clearBullet, 4, 4);

	updatePos();
	x = convert(xPos);
	y = convert(yPos);

	ST7735_DrawBitmap(x, y, bullet, 4, 4);

}

void Bullet::reflect(bool reflectX) {
	if (reflectX) {
		angle = reflectXAxis[angle];
	} else {
		angle = reflectYAxis[angle];
	}
}

/*
 *  3210
 *  		1
 *  	________
 *    2| 		|0
 * 		--------
 * 			3
 */
int Bullet::wallCollision(const vector<wall> &walls) {
	static bool PreviousCollision = false;
	uint8_t returnVal = 0;
	uint8_t bitWall = 0x0F;
	uint32_t x = convert(xPos);
	uint32_t y = convert(yPos);

	if (x < 1 || x > 124) {
		reflect(false);
		numBounce--;
		returnVal = 1;
	}

	if (y < 1 || y > 156) {
		reflect(true);
		numBounce--;
		returnVal = 1;
	}
	if (numBounce <= 0) {
		destroyBullet();
	}

	for (int i = 0; i < walls.size(); i++) {
		if (x >= walls[i].xBR - 4 && x <= walls[i].xTL && y >= walls[i].yTL
				&& y <= walls[i].yBR + 4 && !PreviousCollision) {
			PreviousCollision = true;
			if (x < walls[i].xTL || velocity[2 * angle] > 0) {
				bitWall &= ~0x1; //not going to hit wall 0
			}
			if (x > walls[i].xBR - 4 || velocity[2 * angle] < 0) { //x is moving left on the screen
				bitWall &= ~(1 << 2); //not going to hit wall 2
			}
			if (y > walls[i].yTL || velocity[2 * angle + 1] < 0) { //y is moving up on the screen
				bitWall &= ~(1 << 1); //not going to hit wall 1
			}
			if (y < walls[i].yBR + 4 || velocity[2 * angle + 1] > 0) { //y is moving down on the screen
				bitWall &= ~(1 << 3); //not going to hit
			}

			if (bitWall & (1 << 3 | 1 << 1)) {
				//passing in true means you need to flip the Y velocity vectors (i.e) reflect over the X axis
				bool reflectOverX = true;
				reflect(reflectOverX);
			} else {
				reflect(false);
			}

			numBounce--;
			if (numBounce <= 0) {
				destroyBullet();
			}
			returnVal = 1;
		}

	}

	PreviousCollision = false;
	return returnVal;

}

int Bullet::tankCollision(uint32_t tankX, uint32_t tankY) {
	uint32_t x = convert(xPos);
	uint32_t y = convert(yPos);

	if (x < tankX + 17 && x > tankX && y < tankY && y > tankY - 17) {
		destroyBullet();
		return 1;
	}

	return 0;

}
