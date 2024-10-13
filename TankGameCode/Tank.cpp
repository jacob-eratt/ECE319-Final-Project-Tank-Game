/*
 * Tank.cpp
 *
 *  Created on: Apr 20, 2024
 *      Author: jacob
 */

#include "Tank.h"
#include "Sound.h"
#include "../inc/Clock.h"

Tank::Tank(uint32_t _tankNum, uint32_t _xPos, uint32_t _yPos) {
	xPos = _xPos;
	yPos = _yPos;
	lives = 3;
	angle = 0;
	tankNum = _tankNum;
	numBullets = 0;
	joystick.ADC_InitDual(3, 2, 0x000);

	wasPressed = false;
	bool shoot = 0;
	bool up = 0;
	bool down = 0;
	bool left = 0;
	bool right = 0;
	int checkAngle = 127;

}
// Getter for shoot
/*bool Tank::getShoot() const { return shoot; }
 bool Tank::getUp() const { return up; }
 bool Tank::getDown() const { return down; }
 bool Tank::getLeft() const { return left; }
 bool Tank::getRight() const { return right; }*/

uint32_t Tank::getAngle() {
	return angle;
}
void Tank::setAngle() {
	checkAngle = joystick.createAngleNum();
}
uint32_t Tank::getLives() {
	return lives;
}

void Tank::setLives(uint32_t l) {
	lives = l;
}
uint32_t Tank::getXPos() {
	return xPos;
}
void Tank::setXPos(uint32_t x) {
	xPos = x;
}
uint32_t Tank::getYPos() {
	return yPos;
}

void Tank::setYPos(uint32_t y) {
	yPos = y;
}

//inProgress

bool Tank::wallCollision(const vector<wall> &walls) {
	if (xPos > 111 || xPos < 0 || yPos < 17 || yPos > 160) {
		return true;
	}
	for (int i = 0; i < walls.size(); i++) {
		if (xPos >= walls[i].xBR - 17 && xPos <= walls[i].xTL + 1
				&& yPos >= walls[i].yTL - 1 && yPos <= walls[i].yBR + 17) {
			return true;
		}
	}
	return false;

}

void Tank::updatePos() {
	uint32_t prevX = xPos;
	uint32_t prevY = yPos;
	if (right) {
		xPos++;
	}
	if (left) {
		xPos--;
	}
	if (up) {
		yPos--;
	}
	if (down) {
		yPos++;
	}

	if (wallCollision(walls)) {
		xPos = prevX;
		yPos = prevY;
	}

}

//updates all the bullets the tank has shot
uint32_t Tank::updateBullets(const vector<wall> &walls, Bullet *bulletsP2) {
	for (int i = 0; i < NUM_BULLETS; i++) {
		if (bullets[i].getInUse()) {
			bullets[i].makeBulletLine();
			bullets[i].wallCollision(walls);

			if (bullets[i].tankCollision(xPos, yPos)) {
						return 1;
						break;
				}

			}
		bool use = bulletsP2[i].getInUse();
		bool col =  bulletsP2[i].tankCollision(xPos, yPos);
		if(use &col){
			return 1;
			break;
		}

	}
	return 0;
		}

//the three functions below updates if the tank has shot a bullet, if it is rotating,
//and if it has moved its positon
		void Tank::isShoot() {


			if (!shoot) {
				wasPressed = false;
			}

			if ((shoot) && !wasPressed) {
				wasPressed = true;

				for (int i = 0; i < NUM_BULLETS; i++) {
					if (!bullets[i].getInUse()) {
						if (bullets[i].createBullet(xPos, yPos, angle)) {
							bullets[i].displayBullet();
							Sound_Shoot();
							numBullets++;
						}
						break;
					}
				}
			}
		}

		void Tank::updateRotation() {
			if (checkAngle != 127) {
				angle = checkAngle;
			}
		}

//in Progress

		void Tank::updateTank() {
			//might cause problems
			isShoot();
			updatePos();
			updateRotation();
			if (tankNum == 1) {
				ST7735_DrawBitmap(xPos, yPos, bitMapP1[angle], 17, 17);
			} else {
				ST7735_DrawBitmap(xPos, yPos, bitMapP2[angle], 17, 17);
			}
		}

		int Tank::destroyTank() {

			ST7735_DrawBitmap(xPos, yPos, explosion, 17, 17);
			return 1;
		}

		void Tank::destroyBullets() {
			for (int i = 0; i < NUM_BULLETS; i++) {
				if (bullets[i].getInUse()) {
					bullets[i].destroyBullet();
				}
			}
		}
