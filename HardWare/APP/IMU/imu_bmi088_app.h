#ifndef IMU_H
#define IMU_H


#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============= MACROS ==================//

// ============= ENUMS ==================//
//typedef enum {
//	IMU_6DOF_RAW=0,
//	IMU_6DOF_CAL,
//	IMU_6DOF_DMP,
//	IMU_9DOF_RAW,
//	IMU_9DOF_CAL,
//	IMU_9DOF_DMP
//} icm_20948_mode_t;


void imu_bmi_init(void);
void imu_bmi_time(void);
void imu_bmi_loop(void);

//void callback_IMUTimer(void *argument);
//osStatus_t start_IMUTimer(void);

//ICM_20948_Status_e ICM_20948_i2c_write(uint8_t regaddr, uint8_t *pdata, uint32_t len, void *user);
//ICM_20948_Status_e ICM_20948_i2c_read(uint8_t regaddr, uint8_t *pdata, uint32_t len, void *user);
//
//bool ICM_20948_init_6dof(void);
//bool ICM_20948_init_9dof_dmp(void);
//
//void ICM_20948_read_dmp(void);

#ifdef __cplusplus
}
#endif


#endif /* IMU_H */
