/*
 * imu_task.cpp
 *
 *  Created on: Aug 5, 2025
 *      Author: fast_cern
 */
//
#include "main.h"

// IMU Application
#include <imu_bmi088_app.h>
#include <imu_icm20948_app.h>
#include <imu_task.h>

// Standard includes
#include <stdio.h>
#include "cmsis_os.h"


icm20948_app imu_main(0,I2C_1,ICM_ADDR_AD1);
osTimerId_t imu_timerID;


void imu_cbk_timer(void *argument);
osStatus_t timer_start(osTimerId_t *timerId, void (*callback_timer)(void *),uint32_t ticks);
//void timer_cbk_imu(void *argument);


// ======================================================================================
// ======================================================================================
// USER CODE BEGIN

/**
 * @brief 初始化IMU任务
 *
 * 该函数执行以下操作：
 * 1. 初始化BMI088 IMU传感器
 * 2. 初始化ICM20948 IMU传感器
 * 3. 启动定时器周期读取IMU数据
 *
 * @note 定时器周期20ms
 */
void imu_task_init(void){
	// initialize the main IMU
	// initialize the BMI088 IMU
	imu_bmi_init();

	// ========== ICM 20948 Init START =========== //
//	imu_main.init();
//	imu_main.print_acc_raw = true;

//	// calibration parameters of icm
//	imu_main_data.gyro_bias[0] = -0.002268f;
//	imu_main_data.gyro_bias[1] = 0.009274f;
//	imu_main_data.gyro_bias[2] = 0.012131f;
//
//	imu_main_data.acc_bias[0] = -0.004333f;
//	imu_main_data.acc_bias[1] = -0.015207f;
//	imu_main_data.acc_bias[2] = -0.006239f;
//
//	imu_main_data.acc_mat[0][0] = 0.998913912133527f;
//	imu_main_data.acc_mat[0][1] = -0.001175964122708f;
//	imu_main_data.acc_mat[0][2] = -0.002704207571767f;
//	imu_main_data.acc_mat[1][0] = -0.002414185899037f;
//	imu_main_data.acc_mat[1][1] = 0.999594187963936f;
//	imu_main_data.acc_mat[1][2] = -0.002283900658499f;
//	imu_main_data.acc_mat[2][0] = -0.000938779569684f;
//	imu_main_data.acc_mat[2][1] = 0.013933563274447f;
//	imu_main_data.acc_mat[2][2] = 0.988099727888229f;
//
//	imu_main_data.mag_calib_hard[0] = 2.816096f;
//	imu_main_data.mag_calib_hard[1] = -1.467801f;
//	imu_main_data.mag_calib_hard[2] = 7.796149f;
//
//	imu_main_data.mag_calib_soft[0][0] = 0.0231f;
//	imu_main_data.mag_calib_soft[0][1] = -0.00062039f;
//	imu_main_data.mag_calib_soft[0][2] = 0.00084215f;
//	imu_main_data.mag_calib_soft[1][0] = 0.0000f;
//	imu_main_data.mag_calib_soft[1][1] = 0.0223f;
//	imu_main_data.mag_calib_soft[1][2] = 0.000088439f;
//	imu_main_data.mag_calib_soft[2][0] = 0.0000f;
//	imu_main_data.mag_calib_soft[2][1] = 0.0000f;
//	imu_main_data.mag_calib_soft[2][2] = 0.0224f;
	// ========== ICM 20948 Init END =========== //


	// init timer for imu data reading
	if(timer_start(&imu_timerID,&imu_cbk_timer,IMU_TIMER_TICKS) != osOK) {
		printf("[ERROR] IMU Timer Start Failed!\r\n");
	} else {
		printf("[INFO] IMU Timer Started Successfully!\r\n");
	}
}


void imu_cbk_timer(void *argument){
	imu_bmi_time();		// BMI088 IMU数据处理定时器回调
//	imu_icm_time();
//	imu_main.timer_cbk();
}


void imu_task_loop(void){
	imu_bmi_loop();		// BMI088 IMU数据处理循环
//	imu_icm_loop();
//	imu_main.loop();		// ICM20948 IMU数据处理循环
}


void imu_task_stop(void){


}


/**
 * @brief  启动一个周期性定时器。
 *
 * @details 此函数创建一个新的周期性CMSIS-RTOS v2定时器，并立即启动它。
 *
 * @param[out] timerId         指向定时器ID变量的指针，函数会通过此指针返回新创建的定时器ID。
 * @param[in]  callback_timer  定时器到期时要执行的回调函数。
 * @param[in]  ticks           定时器的周期，以系统节拍 (tick) 为单位。
 *
 * @return 状态码，指示操作结果:
 * - osOK: 定时器成功创建并启动。
 * - osError: 定时器创建失败 (osTimerNew 返回 NULL)。
 * - 其他: osTimerStart 返回的其他错误状态码。
 */
osStatus_t timer_start(osTimerId_t *timerId, void (*callback_timer)(void *),uint32_t ticks)
{
	// create a periodic timer
	*timerId = osTimerNew(callback_timer, osTimerPeriodic, NULL, NULL);
    if (*timerId == NULL) {
        return osError;
    }

    // start timer
    osStatus_t status = osTimerStart(*timerId, ticks); 	// T = 20Ticks
    return status;
}

// 四元数转换成倾角(deg)
float quat_to_incangle(float q[4], int axis){
	// get quat elements
	float w = q[0];
	float x = q[1];
	float y = q[2];
	float z = q[3];
	float inc_ang = 0.0;

	switch(axis){
		case 0:
			inc_ang = acos(2*(x*z-w*y));
			break;
		case 1:
			inc_ang = acos(2*(y*z+w*x));
			break;
		case 2:
			inc_ang = acos(1-2*(x*x-y*y));
			break;
	}

	inc_ang *= 57.29578;		// rad to deg

	return inc_ang;
}
