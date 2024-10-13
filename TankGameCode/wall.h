/*
 * wall.h
 *
 *  Created on: Apr 22, 2024
 *      Author: jacob
 */

#ifndef WALL_H_
#define WALL_H_

#include <vector>
using namespace std;
class wall{

public:
	uint32_t xBR;
	uint32_t yBR;
	uint32_t xTL;
	uint32_t yTL;

	wall(uint32_t _xBR, uint32_t _yBR, uint32_t _xTL, uint32_t _yTL){
		xBR = _xBR;
		yBR = _yBR;
		xTL = _xTL;
		yTL = _yTL;
	}

};

extern vector<wall> walls;

#endif /* WALL_H_ */
