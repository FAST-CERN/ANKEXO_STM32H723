#ifndef _IMU_MAHONY_H_
#define _IMU_MAHONY_H_

// ================= includes ================
#include "math.h"
#include "stdint.h"
#include "stdio.h"

#include "arm_math.h"	// DSP

// =============== definitions ===============
#define AHRS_SAMPLE_FREQ_DEFAULT 50	// default sample frequency in Hz
#define AHRS_KP_DEFAULT 12.0f		// default proportional gain
#define AHRS_KI_DEFAULT 0.5f		// default integral gain


typedef struct {
	float rpy[3];			// roll, pitch, yaw in rad
	float quaternion[4]; 	// quaternion values (w, x, y, z)
}orient;


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint8_t is_init;		// is initialized

	// params
	uint16_t sample_freq;	// sample frequency in Hz
	float inv_sampfreq;		// inverse of sample frequency
	float k_p;				// 2x portion gain
	float k_i;				// 2x integration gain
	float tau;				// time constant for low-pass filter

	float mag_norm_t;	// magnetometer normalization threshold

	// output
	orient ori;		// orientation
	double integral[3];		// integral error
}AHRS_Param;


// =============== functions ===============
void ahrs_init(float accel[3], AHRS_Param *param);
void ahrs_init_mag(float accel[3], float mag[3], AHRS_Param *param);

void ahrs_update(float gyro[3], float accel[3], AHRS_Param *param);
void ahrs_update_mag(float gyro[3], float accel[3], float mag[3], AHRS_Param *param);
void ahrs_update_mag_adapt(float gyro[3], float accel[3], float mag[3], AHRS_Param *param);

void ahrs_param_deinit(AHRS_Param *param);
#ifdef __cplusplus
}
#endif

void quat_2_rpy(float quat[4],float rpy[3]);

float inv_sqrt_f32(float x);
float inv_sqrt_f(float x);
double inv_sqrt_d(double x);


#endif // _IMU_DEV_H_
