#ifndef __IMU_TASK_H__
#define __IMU_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


void app_imu_task_init();

void app_imu_task_loop();

void start_imu_monitoring();

void stop_imu_monitoring() ;





#ifdef __cplusplus
}
#endif

#endif

