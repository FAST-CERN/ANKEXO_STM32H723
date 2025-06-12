/*
 * fan.c
 *
 *  Created on: Jun 11, 2025
 *      Author: user
 */
#include "FAN.h"


void Start_FAN()
{
	HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
}

void Stop_FAN()
{
	HAL_TIMEx_PWMN_Stop(&htim15,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_1);
}
