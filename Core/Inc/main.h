/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ACC_CS_Pin GPIO_PIN_0
#define ACC_CS_GPIO_Port GPIOC
#define GYRO_CS_Pin GPIO_PIN_3
#define GYRO_CS_GPIO_Port GPIOC
#define ACC_INT_Pin GPIO_PIN_10
#define ACC_INT_GPIO_Port GPIOE
#define ACC_INT_EXTI_IRQn EXTI15_10_IRQn
#define GYRO_INT_Pin GPIO_PIN_12
#define GYRO_INT_GPIO_Port GPIOE
#define GYRO_INT_EXTI_IRQn EXTI15_10_IRQn
#define KEY_Pin GPIO_PIN_12
#define KEY_GPIO_Port GPIOB
#define BUZZ_Pin GPIO_PIN_15
#define BUZZ_GPIO_Port GPIOB
#define KEY4_Pin GPIO_PIN_10
#define KEY4_GPIO_Port GPIOD
#define KEY3_Pin GPIO_PIN_11
#define KEY3_GPIO_Port GPIOD
#define KEY2_Pin GPIO_PIN_12
#define KEY2_GPIO_Port GPIOD
#define KEY1_Pin GPIO_PIN_13
#define KEY1_GPIO_Port GPIOD
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOD
#define CAN1_EN_Pin GPIO_PIN_2
#define CAN1_EN_GPIO_Port GPIOD
#define CAN2_EN_Pin GPIO_PIN_3
#define CAN2_EN_GPIO_Port GPIOD
#define OET3V3_EN_Pin GPIO_PIN_7
#define OET3V3_EN_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
#define MOTOR1_EN_ON()    HAL_GPIO_WritePin(CAN1_EN_GPIO_Port,CAN1_EN_Pin, GPIO_PIN_SET)
#define MOTOR1_EN_OFF()   HAL_GPIO_WritePin(CAN1_EN_GPIO_Port,CAN1_EN_Pin, GPIO_PIN_RESET)

#define MOTOR2_EN_ON()    HAL_GPIO_WritePin(CAN2_EN_GPIO_Port,CAN2_EN_Pin, GPIO_PIN_SET)
#define MOTOR2_EN_OFF()   HAL_GPIO_WritePin(CAN2_EN_GPIO_Port,CAN2_EN_Pin, GPIO_PIN_RESET)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
