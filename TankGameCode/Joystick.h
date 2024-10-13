/*
 * Joystick.h
 *
 *  Created on: Apr 20, 2024
 *      Author: jacob
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>


class Joystick{
  private:
  uint32_t x; // raw ADC value
  uint32_t y; //raw y value
  int32_t flag;      // 0 if data is not valid, 1 if valid
  uint32_t angle;
  uint32_t distance; // distance in 0.001cm
// distance = (slope*data+offset)/4096
  uint32_t slope;    // calibration coeffients
  uint32_t offset;

  const uint8_t lookUp[9][9] ={
  {	14, 14, 13, 13, 12, 11, 11, 10, 10},
  {	14, 14, 14, 13, 12, 11, 10, 10, 10},
  {	15, 15, 14, 13, 12, 11, 10, 10, 9},
  {	15, 15, 15, 14, 12, 10, 9, 9, 9},
  {	0, 0, 0, 0, 127, 8, 8, 8, 8},
  {	1, 1, 1, 2, 4, 6, 7, 7, 7},
  {	1, 2, 2, 3, 4, 5, 6, 7, 7},
  {	2, 2, 3, 3, 4, 5, 6, 6, 6},
  {	2, 2, 3, 3, 4, 5, 5, 6, 6}
  };
public:

Joystick();
void ADC_InitDual(uint32_t channel1,uint32_t channel2, uint32_t reference);
void ADC_InDual();
uint32_t getX();
uint32_t getY();
uint32_t createAngleNum();

  /*
  SlidePot(uint32_t m, uint32_t b); // initialize slide pot
  void Init(void);                  // initialize ADC1
  uint32_t In(void);                // return last ADC sample value (0 to 4095)
  void Save(uint32_t n);            // save ADC, set semaphore
  void Sync(void);                  // wait for semaphore
  uint32_t Convert(uint32_t n);     // convert ADC raw sample to fixed-point distance, 0.001cm
  float FloatConvert(uint32_t n);   // do not use this function
  uint32_t Distance(void);          // return last distance value (0 to 2000), 0.001cm
  */
};


#endif /* JOYSTICK_H_ */
