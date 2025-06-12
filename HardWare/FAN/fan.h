/*
 * fan.h
 *
 *  Created on: Jun 11, 2025
 *      Author: user
 */

#ifndef FAN_FAN_H_
#define FAN_FAN_H_

#include "main.h"

extern TIM_HandleTypeDef htim15;

void Start_FAN();
void Stop_FAN();

#endif /* FAN_FAN_H_ */
