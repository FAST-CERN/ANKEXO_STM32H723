/*
 * imu_icm_20948_app.hpp
 *
 *  Created on: Aug 6, 2025
 *      Author: FAST_CERN
 */

#ifndef APP_IMU_IMU_ICM20948_APP_H_
#define APP_IMU_IMU_ICM20948_APP_H_

#include "ICM_20948.h"
#include "sensor_data.hpp"
#include "imu_mahony.h"

#define ICM_20948_LPF_CFREQ   		15
#define ICM_20948_SAMPLE_FREQ 		100
#define ICM_20948_SAMPLE_PERIOD 	(1.0f/ICM_20948_SAMPLE_FREQ)
#define ICM_20948_MAF_WINDOW_SIZE 	10

#define ICM_20948_GYRO_CHEAT


typedef enum {
	ICM_ADDR_AD0 = ICM_20948_I2C_ADDR_AD0,
	ICM_ADDR_AD1 = ICM_20948_I2C_ADDR_AD1
} ICM_20948_I2C_Addr_e;

typedef enum {
	I2C_1 = I2C_BUS_1,
	I2C_2 = I2C_BUS_2
} I2C_BUS;

/**
 * @class ICM20948_App
 * @brief Encapsulates all functionality for the ICM20948 IMU sensor.
 *
 * This class handles the initialization, data acquisition, processing,
 * and attitude estimation (AHRS) for the ICM20948 sensor.
 */
class icm20948_app{
public:
	icm20948_app(uint8_t id = 0, uint8_t bus = I2C_BUS_1, uint8_t addr = ICM_20948_I2C_ADDR_AD1);
    ~icm20948_app();

    bool init(void);
    void loop(void);
    void timer_cbk(void);

    // print options
    bool print_ahrs = false;		// print AHRS result
	bool print_acc_raw = false;		// print raw accelerometer data

	bool print_mag_raw = false;		// print raw magnetometer data
	bool print_mag_calib = false;	// print calibrated magnetometer data
	bool print_mag_norm = false;	// print normalized magnetometer data

	bool print_acc_calib = false;	// print calibrated accelerometer data
	bool print_gyro_calib = false;	// print calibrated gyroscope data
	bool print_temp = false;		// print calibrated temperature data

private:
    using imu_handle = ICM_20948_I2C;

    void set_i2c(uint8_t id, uint8_t bus, uint8_t addr) {
    			imu_handle_._id = id;
    			imu_handle_._i2c_bus = bus;
    			imu_handle_._addr = addr;
    	}
    bool init_imu(bool minimal = false);

    imu_handle imu_handle_;
    sensor_data::imu imu_data_;
    AHRS_Param ahrs_param_;

    filter::lpf::lpf_1st lpf_gyro_x_;
    filter::lpf::lpf_1st lpf_gyro_y_;
    filter::lpf::lpf_1st lpf_gyro_z_;
    filter::lpf::mov_ave maf_mag_x_;
    filter::lpf::mov_ave maf_mag_y_;
    filter::lpf::mov_ave maf_mag_z_;
};
#endif /* APP_IMU_IMU_ICM20948_APP_H_ */
