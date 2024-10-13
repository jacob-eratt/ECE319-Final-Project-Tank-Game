/*
 * Tank.h
 *
 *  Created on: Apr 20, 2024
 *      Author: jacob
 */

#ifndef TANK_H_
#define TANK_H_
#include "Joystick.h"
#include <stdint.h>
#include "Switch.h"
#include "LED.h"
#include "../inc/ST7735.h"

#include "../images/images.h"
#include "Bullet.h"
#include "wall.h"
#define NUM_BULLETS 16

class Tank{
private:
	uint32_t tankNum;
	uint32_t xPos;
	uint32_t yPos;
	uint32_t lives;
	uint32_t angle;
	Joystick joystick;
	uint8_t numBullets;
	Bullet bullets[16];
	bool shoot;
	bool up;
	bool down;
	bool left;
	bool right;
	bool wasPressed;
	int checkAngle;


	//used to determin rotation (arrays contain each image a tank can be in
	const unsigned short* bitMapP1[16] = {BlueTank0, BlueTank1, BlueTank2, BlueTank3, BlueTank4, BlueTank5, BlueTank6,
			BlueTank7, BlueTank8, BlueTank9, BlueTank10, BlueTank11, BlueTank12, BlueTank13, BlueTank14, BlueTank15
	};
	const unsigned short* bitMapP2[16] = {RedTank0, RedTank1, RedTank2, RedTank3, RedTank4, RedTank5, RedTank6,
			RedTank7, RedTank8, RedTank9, RedTank10, RedTank11, RedTank12, RedTank13, RedTank14, RedTank15
	};
	int angleToOffset[32] = {18,6,  18,3, 18,-3,  11,-3,  7,-3,  3,-3, -3,-3,
			-3,3,  -3,6,   -3,11, -3,18, 3,18, 7,18,  11,18, 18,18,
			18,11};

public:
	Tank(uint32_t tankNum, uint32_t _xPos, uint32_t _yPos);
	//getters
	uint32_t getAngle();
	uint32_t getLives();
	uint32_t getXPos();
	uint32_t getYPos();
	Bullet* getBulletArr(){return bullets;}
    bool getShoot() const { return shoot; }
    bool getUp() const { return up; }
    bool getDown() const { return down; }
    bool getLeft() const { return left; }
    bool getRight() const { return right; }
    uint32_t getCheckAngle(){return checkAngle;}

	//setters
    void setShoot(bool value) { shoot = value; }
    void setUp(bool value) { up = value; }
    void setDown(bool value) { down = value; }
    void setLeft(bool value) { left = value; }
    void setRight(bool value) { right = value; }
    void setCheckAngle(uint32_t val){checkAngle = val;}
	void setYPos(uint32_t y);
	void setXPos(uint32_t x);
	void setLives(uint32_t l);
	void setAngle();

	//updates position of tank
	void updatePos();

	//updates direction tank is facing based on joystick
	void updateRotation();

	//updates the bullets the tank has shot
	uint32_t updateBullets(const vector<wall>& walls,Bullet *bulletsP2);

	//creates bullet
	void isShoot();

	///updates position rotation,bullets, and creates new bullets
	void updateTank();


	bool wallCollision(const vector<wall>& walls);

	void destroyBullets();

	int destroyTank();



};




#endif /* TANK_H_ */
