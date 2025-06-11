/*
 * BUZZ.h
 *
 *  Created on: Jun 7, 2025
 *      Author: user
 */

#ifndef BUZZ_BUZZ_H_
#define BUZZ_BUZZ_H_

#include "main.h"
#include "time.h"

extern TIM_HandleTypeDef htim12;

void Set_BUZZ_PWM_Frequency(uint16_t freq_hz);
void Set_BUZZ_PWM_Duty(uint16_t duty_percent);
void Start_BUZZ();
void Stop_BUZZ();




#endif /* BUZZ_BUZZ_H_ */
