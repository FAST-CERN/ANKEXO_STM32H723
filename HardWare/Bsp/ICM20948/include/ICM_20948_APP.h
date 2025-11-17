#ifndef ICM_20948_APP_H
#define ICM_20948_APP_H

#ifdef __cplusplus
extern "C" {
#endif

// HAL
#include "main.h"

// c++ headers
#include <stdbool.h>
#include <stdio.h>
//#include <cstring>
#include <string.h>
#include "cmsis_os.h"

// ICM Drivers
#include "ICM_20948.h"
#include "ICM_20948_C.h"
#include "ICM_20948_REGISTERS.h"
#include "AK09916_REGISTERS.h"

#include "global_param.h"
#include "FreeRTOS.h"

#define IMU_COUNT 4

//typedef struct {
//    float w;
//    float x;
//    float y;
//    float z;
//} Quaternion_t;
//
//typedef struct {
//    float x;
//    float y;
//    float z;
//} Acc_t;
//
//typedef struct {
//    float x;
//    float y;
//    float z;
//} Gryo_t;
//
//typedef struct {
//    float x;
//    float y;
//    float z;
//} Meg_t;
//
//// IMU���ݽṹ�����ڴ洢������IMU����
//typedef struct {
//    Acc_t acc;
//    Gryo_t gyro;
//    Meg_t mag;
//    Quaternion_t quat;
//    bool dataReady;
//} IMU_Data_t;

// ���� IMU �� ID
typedef enum {
    IMU_LEFT_UP = 0,    // ���� IMU
    IMU_LEFT_DOWN,      // ���� IMU
    IMU_RIGHT_UP,       // ���� IMU
    IMU_RIGHT_DOWN,     // ���� IMU
} IMU_ID;



void IMU_ICM_INIT(void);

void IMU_ICM_LOOP(void);

void create_imu_process_thread(void);

#ifdef __cplusplus
}
#endif


#endif /* IMU_H */
