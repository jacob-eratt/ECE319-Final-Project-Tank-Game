/*
 * Switch.h
 *
 *  Created on: Nov 5, 2023
 *      Author: jonat
 */

#ifndef SWITCH_H_
#define SWITCH_H_

// initialize your switches
#define UP 1<<26
#define LEFT 1<<27
#define RIGHT 1<<31
#define DOWN 1<<28

void Switch_Init(void);

// return current state of switches
uint32_t Switch_In(void);


#endif /* SWITCH_H_ */
