#include "main.h"
#include "imu.h"
#include <stdio.h>
#include "cmsis_os.h"

#include "global_param.h"
//#include <cstring>

#include "FreeRTOS.h"



void IMU_INIT(void)
{
	BMI088_init();
	bmi088_accel_init();
	bmi088_gyro_init();
    printf("Starting IMU initialization...\r\n");


    osDelay(100); // �ȶ� IMU ��ʼ��


    int i = 100;
    // �ȶ�IMU����
    while(i--) {

        osDelay(4); // �ȶ� IMU ���ݶ�ȡ
    }

    // �ȴ������豸��ʼ�����
    while (!all_devices_initialized()) {
        osDelay(100);
    }

    printf("All devices initialized, entering IMU main loop.\r\n");
}




void IMU_LOOP(void)
{
    //printf("IMU_LOOP\r\n");
}


