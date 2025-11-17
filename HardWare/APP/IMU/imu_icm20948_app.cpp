/*
 * imu_icm20948_app.cpp
 *
 *  Created on: Aug 6, 2025
 *      Author: FAST_CERN
 */
#include "imu_icm20948_app.h"
#include "cmsis_os.h"
#include "global_param.h"

// Board includes
#include "main.h"
#include "usart.h"

// constructor
icm20948_app::icm20948_app(uint8_t id, uint8_t bus, uint8_t addr):
	lpf_gyro_x_(ICM_20948_LPF_CFREQ, ICM_20948_SAMPLE_FREQ),
	lpf_gyro_y_(ICM_20948_LPF_CFREQ, ICM_20948_SAMPLE_FREQ),
	lpf_gyro_z_(ICM_20948_LPF_CFREQ, ICM_20948_SAMPLE_FREQ),
	maf_mag_x_(ICM_20948_MAF_WINDOW_SIZE),
	maf_mag_y_(ICM_20948_MAF_WINDOW_SIZE),
	maf_mag_z_(ICM_20948_MAF_WINDOW_SIZE){
	set_i2c(id, bus, addr);
}

// destructor
icm20948_app::~icm20948_app(){
}

bool icm20948_app::init(void){
	init_imu();

	// set calibration parameters
	// todo: use auto calib or host calib
	imu_data_.gyro_bias[0] = -0.002268f;
	imu_data_.gyro_bias[1] = 0.009274f;
	imu_data_.gyro_bias[2] = 0.012131f;

	imu_data_.acc_bias[0] = -0.004333f;
	imu_data_.acc_bias[1] = -0.015207f;
	imu_data_.acc_bias[2] = -0.006239f;

	imu_data_.acc_mat[0][0] = 0.998913912133527f;
	imu_data_.acc_mat[0][1] = -0.001175964122708f;
	imu_data_.acc_mat[0][2] = -0.002704207571767f;
	imu_data_.acc_mat[1][0] = -0.002414185899037f;
	imu_data_.acc_mat[1][1] = 0.999594187963936f;
	imu_data_.acc_mat[1][2] = -0.002283900658499f;
	imu_data_.acc_mat[2][0] = -0.000938779569684f;
	imu_data_.acc_mat[2][1] = 0.013933563274447f;
	imu_data_.acc_mat[2][2] = 0.988099727888229f;

	imu_data_.mag_calib_hard[0] = 2.816096f;
	imu_data_.mag_calib_hard[1] = -1.467801f;
	imu_data_.mag_calib_hard[2] = 7.796149f;

	imu_data_.mag_calib_soft[0][0] = 0.0231f;
	imu_data_.mag_calib_soft[0][1] = -0.00062039f;
	imu_data_.mag_calib_soft[0][2] = 0.00084215f;
	imu_data_.mag_calib_soft[1][0] = 0.0000f;
	imu_data_.mag_calib_soft[1][1] = 0.0223f;
	imu_data_.mag_calib_soft[1][2] = 0.000088439f;
	imu_data_.mag_calib_soft[2][0] = 0.0000f;
	imu_data_.mag_calib_soft[2][1] = 0.0000f;
	imu_data_.mag_calib_soft[2][2] = 0.0224f;

	// Initialize filters
	lpf_gyro_x_.reset();
	lpf_gyro_y_.reset();
	lpf_gyro_z_.reset();
	maf_mag_x_.reset();
	maf_mag_y_.reset();
	maf_mag_z_.reset();

	// AHRS Deinit
	ahrs_param_deinit(&ahrs_param_);
	ahrs_param_.sample_freq  = ICM_20948_SAMPLE_FREQ;
	ahrs_param_.inv_sampfreq = ICM_20948_SAMPLE_PERIOD;

	return true;
}

bool icm20948_app::init_imu(bool minimal){
	bool success(true),initialized(false);
	printf("[INFO] IMU ID-%d INIT START.\r\n", imu_handle_._id);

	while (!initialized){
		imu_handle_.begin(imu_handle_._i2c_bus, imu_handle_._addr);
		if (imu_handle_.status != ICM_20948_Stat_Ok) {
			printf("[INFO] IMU ID-%d Status NOT OK! ERRID : %d\r\n", imu_handle_._id, imu_handle_.status);

			static int retry_count = 0;
			if (++retry_count >= 10) {
				return false;
			}

			osDelay(200);
		} else {
			initialized = true;
			printf("[INFO] IMU ID-%d Status OK!\r\n", imu_handle_._id);
		}
	}

	if(imu_handle_.startupDefault(minimal)!= ICM_20948_Stat_Ok){
		success = false;
		printf("[INFO] IMU ID-%d Default Init Failed!\r\n", imu_handle_._id);
	}
	return success;
}

void icm20948_app::loop(void){
	if (!imu_data_.data_ready) {
		return; 							// No new data available
	}
	else{
		imu_data_.data_ready = false; 	// Reset data ready flag

		// print raw data
		if(print_acc_raw){
			// transmit acc data in frame format
			uint8_t buf[21];
			buf[0] = 0xA5;		// frame header
			buf[1] = 0x5A;
			buf[2] = 0xFF;

			buf[3] = 0x00;		// addr
			buf[4] = 0x00;		// cmd

			memcpy(&buf[5], imu_data_.acc, sizeof(float)*3); //data

			// calculate checksum
			uint8_t checksum = 0;
			for(int i=0; i<17; i++){
				checksum += buf[i];
			}
			buf[17] = checksum;
			buf[18] = 0xFF;		// frame tail
			buf[19] = 0x5A;
			buf[20] = 0xA5;

			HAL_UART_Transmit(&huart1, buf, 21, 10);
		}

		if(print_mag_raw){
			printf("[IMU]Mag:%.3f,%.3f,%.3f,%.3f\r\n",
					imu_data_.mag[0], imu_data_.mag[1], imu_data_.mag[2],imu_data_.temp);
		}

		imu_data_.calib();				// Calibrate IMU data

		// print calibrated data
		// todo: filter acc
		// accel
		if(print_acc_calib){
			printf("[IMU]Accel:%.4f,%.4f,%.4f\r\n",
					imu_data_.acc[0], imu_data_.acc[1], imu_data_.acc[2]);
		}

		// gyro
		// Apply LPF to gyro data
		imu_data_.gyro[0] = lpf_gyro_x_.apply(imu_data_.gyro[0]);
		imu_data_.gyro[1] = lpf_gyro_y_.apply(imu_data_.gyro[1]);
		imu_data_.gyro[2] = lpf_gyro_z_.apply(imu_data_.gyro[2]);

		// cheat: dead zone for gyro data
#ifdef ICM_20948_GYRO_CHEAT
		if(fabs(imu_data_.gyro[0]) < 0.01f) imu_data_.gyro[0] = 0.0f;
		if(fabs(imu_data_.gyro[1]) < 0.01f) imu_data_.gyro[1] = 0.0f;
		if(fabs(imu_data_.gyro[2]) < 0.01f) imu_data_.gyro[2] = 0.0f;
#endif
		// todo is rotating?

		if(print_gyro_calib){
			printf("[IMU]Gyro:%.3f,%.3f,%.3f\r\n",
					imu_data_.gyro[0], imu_data_.gyro[1], imu_data_.gyro[2]);
		}

		// mag
		// Invert Y and Z axis for compatibility with icm20948
		imu_data_.mag[1] = -imu_data_.mag[1];
		imu_data_.mag[2] = -imu_data_.mag[2];
		imu_data_.mag[0] = maf_mag_x_.apply(imu_data_.mag[0]);
		imu_data_.mag[1] = maf_mag_y_.apply(imu_data_.mag[1]);
		imu_data_.mag[2] = maf_mag_z_.apply(imu_data_.mag[2]);

		if(print_mag_calib){	// mag after filter
			printf("[IMU]Mag:%.3f,%.3f,%.3f\r\n",
					imu_data_.mag[0], imu_data_.mag[1], imu_data_.mag[2]);
		}
		if(print_mag_norm){	// print normalized magnetometer data
			float mag_n_ = sqrtf(
					imu_data_.mag[0] * imu_data_.mag[0] +
					imu_data_.mag[1] * imu_data_.mag[1] +
					imu_data_.mag[2] * imu_data_.mag[2]);
			printf("[IMU]MagNorm:%.3f\r\n", mag_n_);
		}

		// print temperature
		if(print_temp){
			printf("[IMU]Temp:%.2f\r\n", imu_data_.temp);
		}

		// AHRS Update
		ahrs_update_mag_adapt(imu_data_.gyro, imu_data_.acc, imu_data_.mag, &ahrs_param_);

		// print AHRS Result
		if(print_ahrs){
			printf("imu_0:%.3f,%.3f,%.3f,%.3f\r\n",
							ahrs_param_.ori.quaternion[0],
							ahrs_param_.ori.quaternion[1],
							ahrs_param_.ori.quaternion[2],
							ahrs_param_.ori.quaternion[3]);
//			printf("imu_1:%.3f,%.3f,%.3f,%.3f\r\n",
//					g_system_state.rs485_sensor_data_1.imu_data_1.quat.w,
//					g_system_state.rs485_sensor_data_1.imu_data_1.quat.x,
//					g_system_state.rs485_sensor_data_1.imu_data_1.quat.y,
//					g_system_state.rs485_sensor_data_1.imu_data_1.quat.z);
//			printf("imu_2:%.3f,%.3f,%.3f,%.3f\r\n",
//					g_system_state.rs485_sensor_data_1.imu_data_2.quat.w,
//					g_system_state.rs485_sensor_data_1.imu_data_2.quat.x,
//					g_system_state.rs485_sensor_data_1.imu_data_2.quat.y,
//					g_system_state.rs485_sensor_data_1.imu_data_2.quat.z);
//			printf("imu_3:%.3f,%.3f,%.3f,%.3f\r\n",
//					g_system_state.rs485_sensor_data_2.imu_data_1.quat.w,
//					g_system_state.rs485_sensor_data_2.imu_data_1.quat.x,
//					g_system_state.rs485_sensor_data_2.imu_data_1.quat.y,
//					g_system_state.rs485_sensor_data_2.imu_data_1.quat.z);
//			printf("imu_4:%.3f,%.3f,%.3f,%.3f\r\n",
//					g_system_state.rs485_sensor_data_2.imu_data_2.quat.w,
//					g_system_state.rs485_sensor_data_2.imu_data_2.quat.x,
//					g_system_state.rs485_sensor_data_2.imu_data_2.quat.y,
//					g_system_state.rs485_sensor_data_2.imu_data_2.quat.z);
		}
	}
}

void icm20948_app::timer_cbk(void){
	imu_handle_.getAGMT();

	imu_data_.acc[0] = imu_handle_.accX() * 0.001f;				// mg to g
	imu_data_.acc[1] = imu_handle_.accY() * 0.001f;
	imu_data_.acc[2] = imu_handle_.accZ() * 0.001f;

	imu_data_.gyro[0] = imu_handle_.gyrX() * 0.01745329252f;	// dps to rad/s
	imu_data_.gyro[1] = imu_handle_.gyrY() * 0.01745329252f;
	imu_data_.gyro[2] = imu_handle_.gyrZ() * 0.01745329252f;

	imu_data_.mag[0] = imu_handle_.magX();	// uT
	imu_data_.mag[1] = imu_handle_.magY();
	imu_data_.mag[2] = imu_handle_.magZ();

	imu_data_.temp = imu_handle_.temp();	// ℃
	imu_data_.data_ready = true;
}

