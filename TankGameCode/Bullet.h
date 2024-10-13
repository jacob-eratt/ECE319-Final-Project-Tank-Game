/*
 * Bullet.h
 *
 *  Created on: Apr 21, 2024
 *      Author: jacob
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "../inc/ST7735.h"
#include <ti/devices/msp/msp.h>
#include <stdint.h>
#include "../images/images.h"
#include "wall.h"

#define fixedPoint 6
#define NUM_BOUNCE 3
class Bullet {

private:

	const int velocity[32] = { 64, 0, 59, -24, 45, -45, 24, -59, 0, -64,
			-24, -59, -45, -45, -59, -24, -64, 0, -59, 24, -45, 45, -24, 59, 0,
			64, 24, 59, 45, 45, 59, 24
	};


	//returns angle based on reflection. This means if angle was 0 and was reflected across the x-axis it should be 8. Hence index 0 corresponds to 8 in reflectionXAxis
	const uint8_t reflectXAxis[16] = {8,15,14,13,12,11,10,9,0,7,6,5,4,3,2,1};
	const uint8_t reflectYAxis[16] = {8,7,6,5,12,3,2,1,0,15,14,13,4,11,10,9};
	//will be filled in with fixed Point table

	const int angleToOffset[32] = { 18, -6, 18, -11, 18, -18, 10, -18, 6, -18,
			2, -18, -5, -18, -5, -10, -5, -6, -5, -2, -5, 5, 2, 5, 6, 5, 11, 5,
			18, 5, 18, -2 };

	uint32_t angle;
	uint32_t xPos;
	uint32_t yPos;
	uint8_t numBounce;
	bool inUse;

	uint32_t convert(uint32_t a);
	void reflect(bool reflectX);
	bool validCreate(uint32_t x, uint32_t y);




public:
	Bullet() {
		angle = -1;
		xPos = 0;
		yPos = 0;
		inUse = false;
		numBounce = 1;
	}

	bool createBullet(uint32_t _xPos, uint32_t _yPos, uint32_t _vIndex);

	void destroyBullet();
	//getters
	uint32_t getXPos() const {
		return xPos;
	}
	uint32_t getYPos() const {
		return yPos;
	}
	bool getInUse() const {
		return inUse;
	}

	// Setters

	void setXPos(uint32_t x) {
		xPos = x;
	}
	void setYPos(uint32_t y) {
		yPos = y;
	}
	void setInUse(bool i) {
		inUse = i;
	}

	void updatePos();
	void displayBullet();
	void makeBulletLine();
	int wallCollision(const vector<wall>& walls);

	int tankCollision(uint32_t tankX, uint32_t tankY);

};

#endif /* BULLET_H_ */
