/*
 * BUZZ.c
 *
 *  Created on: Jun 7, 2025
 *      Author: user
 */
#include "BUZZ.h"


// 1KHz == 1000
void Set_BUZZ_PWM_Frequency(uint16_t freq_hz)
{
	__HAL_TIM_DISABLE(&htim12);
	 uint32_t timer_clk = HAL_RCC_GetPCLK1Freq() * 2;
	uint32_t arr = (timer_clk / freq_hz) - 1;

	__HAL_TIM_SET_AUTORELOAD(&htim12, arr); // 修改ARR值
//	__HAL_TIM_SET_PRESCALER(&htim12, psc);
	__HAL_TIM_ENABLE(&htim12);
}

// 0~100%
void Set_BUZZ_PWM_Duty(uint16_t duty_percent)
{
	__HAL_TIM_DISABLE(&htim12);
	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim12);
	uint32_t ccr = (duty_percent / 100.0) * (arr + 1);

	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, ccr);

	__HAL_TIM_ENABLE(&htim12);
}


void Start_BUZZ()
{
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);
}

void Stop_BUZZ()
{
	HAL_TIM_PWM_Stop(&htim12,TIM_CHANNEL_2);
}



