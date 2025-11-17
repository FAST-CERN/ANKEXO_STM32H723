/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include <imu_task.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor_task.h"
#include "WS2812B.h"
#include "BMI088driver.h"
#include "stdio.h"
#include "string.h"
#include "S485_master.h"
#include "global_param.h"
#include "adc.h"
#include "dm_motor_ctrl.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Definitions for LCDTask */
osThreadId_t LCDTaskHandle;
const osThreadAttr_t LCDTask_attributes = {
  .name = "LCDTask",
  .stack_size = 768 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal7,
};

/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 768 * 4,
  .priority = (osPriority_t) osPriorityNormal3,
};

/* Definitions for IMUTask */
osThreadId_t IMUTaskHandle;
const osThreadAttr_t IMUTask_attributes = {
  .name = "IMUTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityNormal7,
};

/* Definitions for S485Task */
osThreadId_t S485TaskHandle;
const osThreadAttr_t S485Task_attributes = {
  .name = "S485Task",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};






/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint32_t adc_buffer = 0;
uint8_t ADC_battery(uint32_t adc) {

	HAL_ADC_Start_DMA(&hadc1, &adc_buffer, 1);

	const double min_voltage = 2.02f;
    // 1. 将 ADC 值转换为电压（假设 ADC 满量程对应 3.3V）
	double voltage = (double)adc_buffer * 3.3f / 65535.0f / min_voltage * 22.2 - 0.35;

//	printf("voltage:%f\r\n", voltage);

    // 2. 定义电池电压有效范围

    // 3. 计算百分比（限制在 0~100%）
    double percentage = (voltage-20) / (25.2-20) * 100.0f;
    if (percentage < 0.0f) percentage = 0.0f;
    if (percentage > 100.0f) percentage = 100.0f;

    // 4. 转换为整数百分比
    return percentage;
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartLedTask(void *argument);
void StartMotorTask(void *argument);
void StartIMUTask(void *argument);
void StartS485Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */
	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* USER CODE BEGIN RTOS_THREADS */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

	/* creation of LCDTask */
 	LCDTaskHandle = osThreadNew(StartLedTask, NULL, &LCDTask_attributes);

	/* creation of MotorTask */
	MotorTaskHandle = osThreadNew(StartMotorTask, NULL, &MotorTask_attributes);

	/* creation of IMUTask */
	IMUTaskHandle = osThreadNew(StartIMUTask, NULL, &IMUTask_attributes);

	/* creation of S485Task */
	S485TaskHandle = osThreadNew(StartS485Task, NULL, &S485Task_attributes);
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  printf("[INFO] DefaultTask Started.\r\n");

  // print system info
  printf("[INFO] System Clock: %ld Hz\r\n", HAL_RCC_GetSysClockFreq());
  printf("[INFO] System Tick: %d Hz\r\n", 1000);

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartLedTask */
/**
* @brief Function implementing the LCDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedTask */
void StartLedTask(void *argument)
{
  /* USER CODE BEGIN StartLedTask */
  printf("[INFO] LedTask Started.\r\n");
//  Set_LED(0,255,255,255);
//  Set_Brightness(1);
//  WS2812_Send();
  Color_TypeDef temp;

  temp.R = 0x00;
  temp.G = 0xff;
  temp.B = 0x00;
  WS2812B_FillColor(0,1,&temp);
  WS2812B_RefreshPixel();
  osDelay(1000);

  /* Infinite loop */
  for(;;)
  {
	temp.R = (temp.R + 0x0f);
	temp.G = (0x00 + 0x00);
	temp.B = (0x00 + 0x00);
	WS2812B_FillColor(0,1,&temp);
	WS2812B_RefreshPixel();

	uint8_t battery = ADC_battery(adc_buffer);
    char battery_data[16];
    // 格式化为 BA100 格式 (BA + 电量百分比)
    snprintf(battery_data, sizeof(battery_data), "%d", battery);
    // 使用 UART6_SendData 发送电量百分比数据(TO NRF)
    UART6_SendData("BA", battery_data);
//	printf("[INFO] Battery Percentage: %d%%\r\n", battery);	// print battery percentage
    osDelay(5000); 	// 延时5秒
  }
  /* USER CODE END StartLedTask */
}

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* 定时器句柄，用于100Hz的数据采集 & 控制 */
osTimerId_t dm_motor_TimerId;
/* USER CODE END Header_StartMotorTask */
void StartMotorTask(void *argument)
{
	printf("[INFO] MotorTask started\r\n");
	/* USER CODE BEGIN StartMotorTask */
	can_bsp_init();

	// motor init
	dm_motor_init();

	dm_motor_disable(&hfdcan1,&motor[Motor1]);
	osDelay(100);
	dm_motor_enable(&hfdcan1,&motor[Motor1]);
	osDelay(100);
	save_pos_zero(&hfdcan1,1,MIT_MODE);

//	dm_motor_disable(&hfdcan1,&motor[Motor2]);
//	osDelay(100);
//	dm_motor_enable(&hfdcan1,&motor[Motor2]);
//	osDelay(100);

	// 创建定时器，用于100Hz的控制信号输出
	osTimerAttr_t timer_attr = {
		.name = "dm_motor_Timer"
	};
	dm_motor_TimerId = osTimerNew(dm_motor_timer_cbk, osTimerPeriodic, NULL, &timer_attr);
	osTimerStart(dm_motor_TimerId, 10);	// start 100hz soft timer

	/* Infinite loop */
	for(;;)
	{

		osDelay(20);
	}
	/* USER CODE END StartMotorTask */
}


/* USER CODE BEGIN Header_StartIMUTask */
/**
* @brief Function implementing the IMUTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIMUTask */
void StartIMUTask(void *argument)
{
	/* USER CODE BEGIN StartIMUTask */
	printf("[INFO] IMUTask started\r\n");
	imu_task_init();	// IMU 任务初始化

	/* Infinite loop */
	for(;;)
	{
		imu_task_loop();	// IMU 数据处理循环
		osDelay(1); 		// 延时1ms/tick
	}
  /* USER CODE END StartIMUTask */
}

/* USER CODE BEGIN Header_StartS485Task */
/**
* @brief Function implementing the S485Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartS485Task */
void StartS485Task(void *argument)
{
	/* USER CODE BEGIN StartS485Task */
	osDelay(5000);
    if (!RS485_InitializeSensors()) {
        printf("[INFO] RS485 sensors initialization failed\r\n");
    } else {
        printf("[INFO] RS485 sensors initialized successfully\r\n");
        osTimerStart(rs485DataTimerId, 10);	// start 100Hz data collection timer
    }

    // 任务主循环
    for (;;) {
    	 //printf("RS485 For cir\r\n");
        // 监控设备状态
        if (!rs485_sensor_1.initialized || !rs485_sensor_2.initialized) {
            // 如果设备未初始化，尝试重新初始化
            RS485_QueryStatus(&rs485_sensor_1);
            osDelay(100);
            RS485_QueryStatus(&rs485_sensor_2);
            printf("[INFO] RS485 sensors initial ongoing.\r\n");

            // 检查是否所有设备都已初始化
            if (rs485_sensor_1.initialized && rs485_sensor_2.initialized) {
                printf("[INFO] RS485 sensors initial successfully.\r\n");

                // 启动100Hz数据采集定时器
                osTimerStart(rs485DataTimerId, 10); // 10ms = 100Hz

            	//RS485_AUTO_SendDataStart(&rs485_sensor_1);
            	//RS485_AUTO_SendDataStart(&rs485_sensor_2);
            }
        }

        if( g_system_state.rs485_sensor_data_2.imu_data_1.dataReady &&
			g_system_state.rs485_sensor_data_2.imu_data_2.dataReady) {
//			printf("RS485 Sensor 2 Data Ready\r\n");
			g_system_state.rs485_sensor_data_2.imu_data_1.dataReady = false;
			g_system_state.rs485_sensor_data_2.imu_data_2.dataReady = false;

			// RS485 Sensor 2 imu-1
			float inc_angle;
			float q[4] = {
				g_system_state.rs485_sensor_data_2.imu_data_1.quat.w,
				g_system_state.rs485_sensor_data_2.imu_data_1.quat.x,
				g_system_state.rs485_sensor_data_2.imu_data_1.quat.y,
				g_system_state.rs485_sensor_data_2.imu_data_1.quat.z
			};
			inc_angle = quat_to_incangle(q,0);
			g_system_state.rs485_sensor_data_2.imu_data_1.inc_angle = inc_angle;

			// RS485 Sensor 2 imu-2
			q[0] = g_system_state.rs485_sensor_data_2.imu_data_2.quat.w;
			q[1] = g_system_state.rs485_sensor_data_2.imu_data_2.quat.x;
			q[2] = g_system_state.rs485_sensor_data_2.imu_data_2.quat.y;
			q[3] = g_system_state.rs485_sensor_data_2.imu_data_2.quat.z;
			inc_angle = quat_to_incangle(q,0);
			g_system_state.rs485_sensor_data_2.imu_data_2.inc_angle = inc_angle;

//			printf("[DATA] SENSOR 2 IMU 1[inc]:%.3f\r\n",
//					g_system_state.rs485_sensor_data_2.imu_data_1.inc_angle);
//			printf("[DATA] SENSOR 2 IMU 2[inc]:%.3f\r\n",
//					g_system_state.rs485_sensor_data_2.imu_data_2.inc_angle);

//			printf("[DATA] imu-1[quat]:%.3f,%.3f,%.3f,%.3f\r\n",
//					g_system_state.rs485_sensor_data_2.imu_data_1.quat.w,
//					g_system_state.rs485_sensor_data_2.imu_data_1.quat.x,
//					g_system_state.rs485_sensor_data_2.imu_data_1.quat.y,
//					g_system_state.rs485_sensor_data_2.imu_data_1.quat.z);

			// process data here
			// ...
		}


        osDelay(1);
    }
    /* USER CODE END StartS485Task */
}
