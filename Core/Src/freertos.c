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
  .stack_size = 1024 * 4,
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

	printf("voltage:%f\r\n", voltage);

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

  /* Create the thread(s) */
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

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
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
  printf("StartLEDTask\r\n");
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
    // 使用 UART6_SendData 发送电量百分比数据
    UART6_SendData("BA", battery_data);
	printf("battery: %d%%\r\n", battery);
    osDelay(5000);
  }
  /* USER CODE END StartLedTask */
}

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorTask */
void StartMotorTask(void *argument)
{
  /* USER CODE BEGIN StartMotorTask */
  can_bsp_init();
  //app_motor_task_init();
	//motorStartQueue = osMessageQueueNew(10, sizeof(uint32_t), NULL);
	//motorStopQueue  = osMessageQueueNew(10, sizeof(uint32_t), NULL);
	//motorZeroQueue  = osMessageQueueNew(10, sizeof(uint32_t), NULL);
  //motorModelQueue = osMessageQueueNew(10, sizeof(uint32_t), NULL);

  //motor_control_init();
  /* Infinite loop */
  for(;;)
  {
	//motor_control_task();
//		printf("G:%.3f,%.3f,%.3f;A:%.3f,%.3f,%.3f;T:%.2f\r\n",
//				g_system_state.rs485_sensor_data_1.imu_data_1.gyro.x, g_system_state.rs485_sensor_data_1.imu_data_1.gyro.y, g_system_state.rs485_sensor_data_1.imu_data_1.gyro.z,
//				g_system_state.rs485_sensor_data_1.imu_data_1.acc.x, g_system_state.rs485_sensor_data_1.imu_data_1.acc.y, g_system_state.rs485_sensor_data_1.imu_data_1.acc.z,
//				g_system_state.rs485_sensor_data_1.encoder_data.angle);

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
  printf("StartIMUTask\r\n");

  float gyro[3], accel[3], temp;

  while(BMI088_init())
  {
	  printf("BMI088_init\r\n");
	  osDelay(100);
  }
  g_device_status.bmi088_pcb_devicestatus = true;

  /* Infinite loop */
  for(;;)
  {

	BMI088_read(gyro, accel, &temp);

	g_system_state.bmi088_imu_data.acc.x = accel[0];
	g_system_state.bmi088_imu_data.acc.x = accel[1];
	g_system_state.bmi088_imu_data.acc.x = accel[2];

	g_system_state.bmi088_imu_data.gyro.x = gyro[0];
	g_system_state.bmi088_imu_data.gyro.x = gyro[1];
	g_system_state.bmi088_imu_data.gyro.x = gyro[2];

	g_system_state.bmi088_imu_data.dataReady = true;

    // 格式化数据到缓冲区
    //uint8_t tx_buffer[64]; // 用于存储发送数据的缓冲区
    //int len;
    // 格式: "G:x,y,z;A:x,y,z;T:t\r\n"
//	printf("G:%.3f,%.3f,%.3f;A:%.3f,%.3f,%.3f;T:%.2f\r\n",
//				   gyro[0], gyro[1], gyro[2],
//				   accel[0], accel[1], accel[2],
//				   temp);
    // 通过串口发送数据
    // HAL_UART_Transmit(&huart1, tx_buffer, len, 0xFFFF);

    osDelay(10);
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
        printf("RS485 sensors initialization failed\r\n");
    } else {
        printf("RS485 sensors initialized successfully\r\n");
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
            printf("RS485 sensors re-initialized ing\r\n");

            // 检查是否所有设备都已初始化
            if (rs485_sensor_1.initialized && rs485_sensor_2.initialized) {
                printf("RS485 sensors re-initialized successfully\r\n");

                // 启动100Hz数据采集定时器
                osTimerStart(rs485DataTimerId, 10); // 10ms = 100Hz

            	//RS485_AUTO_SendDataStart(&rs485_sensor_1);
            	//RS485_AUTO_SendDataStart(&rs485_sensor_2);
            }
        }

       //RS485_DataAutoProcess();

		printf("G1:%.3f,%.3f,%.3f;A1:%.3f,%.3f,%.3f;T1:%.2f \t",
				g_system_state.rs485_sensor_data_1.imu_data_1.gyro.x, g_system_state.rs485_sensor_data_1.imu_data_1.gyro.y, g_system_state.rs485_sensor_data_1.imu_data_1.gyro.z,
				g_system_state.rs485_sensor_data_1.imu_data_1.acc.x, g_system_state.rs485_sensor_data_1.imu_data_1.acc.y, g_system_state.rs485_sensor_data_1.imu_data_1.acc.z,
				g_system_state.rs485_sensor_data_1.encoder_data.angle);

		printf("G2:%.3f,%.3f,%.3f;A2:%.3f,%.3f,%.3f;T2:%.2f\r\n",
				g_system_state.rs485_sensor_data_2.imu_data_1.gyro.x, g_system_state.rs485_sensor_data_2.imu_data_1.gyro.y, g_system_state.rs485_sensor_data_2.imu_data_1.gyro.z,
				g_system_state.rs485_sensor_data_2.imu_data_1.acc.x, g_system_state.rs485_sensor_data_2.imu_data_1.acc.y, g_system_state.rs485_sensor_data_2.imu_data_1.acc.z,
				g_system_state.rs485_sensor_data_2.encoder_data.angle);


        // 延时1秒
        osDelay(50);
    }
  /* USER CODE END StartS485Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

