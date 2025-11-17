/*
 * imu_task.h
 *
 *  Created on: Aug 5, 2025
 *      Author: fast_cern
 */

#ifndef APP_IMU_IMU_TASK_H_
#define APP_IMU_IMU_TASK_H_


#define IMU_TIMER_TICKS 10	// IMU Timer Ticks in ms
#define IMU_SAMPLE_FREQ 100	// IMU Sample Frequency in Hz
#define IMU_GYRO_CHEAT		// IMU Gyro Cheat Mode

// C wrappers for C++ functions
#ifdef __cplusplus
extern "C" {
#endif

void imu_task_init(void);
void imu_task_loop(void);
void imu_task_stop(void);

float quat_to_incangle(float q[4], int axis);
//void imu_cbk_timer(void);

#ifdef __cplusplus
}

#endif


#endif /* APP_IMU_IMU_TASK_H_ */
