#include <imu_bmi088_app.h>

// global
#include "main.h"
#include "global_param.h"
#include "tim.h"

// OS Include
#include "cmsis_os.h"
#include "FreeRTOS.h"

// Hardware include
#include "BMI088driver.h"			// BMI088 Drivers

// Alogrithm include
#include <pid.h>
#include <imu_mahony.h>

// Global Variables
AHRS_Param ahrs_param;
pid_type_def temp_pid;


void imu_bmi_init(void)
{
	// 初始化BMI088传感器
	while(BMI088_init())
	{
		  printf("[INFO] IMU: BMI088 initialized.\r\n");
		  osDelay(100);
	}
	g_device_status.bmi088_pcb_devicestatus = true;

	// 初始化AHRS参数
	ahrs_param_deinit(&ahrs_param);

	// 打印AHRS参数
//	printf("[INFO] IMU: AHRS parameters initialized.\r\n");
//	printf("[INFO] IMU: AHRS sample frequency: %d Hz\r\n", ahrs_param.sample_freq);
//	printf("[INFO] IMU: AHRS proportional gain: %.2f\r\n", ahrs_param.k_p);
//	printf("[INFO] IMU: AHRS integral gain: %.2f\r\n", ahrs_param.k_i);

	// 初始化PID控制器
	PID_init(&temp_pid, PID_POSITION, (fp32[]){160.0f, 5.0f, 4.0f}, 400.0f, 100.0f);

	// 开启温控PWM输出
	if(TIM3_PWM_START())
	{
		Error_Handler();
	}
	TIM3_PWM_SET_PULSE(0);
}


void imu_bmi_loop(void)
{
	if(g_system_state.bmi088_imu_data.dataReady){
		float gyro[3], accel[3], temp;

		gyro[0] = g_system_state.bmi088_imu_data.gyro.x;
		gyro[1] = g_system_state.bmi088_imu_data.gyro.y;
		gyro[2] = g_system_state.bmi088_imu_data.gyro.z;
		accel[0] = g_system_state.bmi088_imu_data.acc.x;
		accel[1] = g_system_state.bmi088_imu_data.acc.y;
		accel[2] = g_system_state.bmi088_imu_data.acc.z;
		temp = g_system_state.bmi088_imu_data.temp;

		g_system_state.bmi088_imu_data.dataReady = false;

		// IMU Data Handling
//		printf("main_imu:%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\n",
//				gyro[0],gyro[1],gyro[2],
//				accel[0],accel[1],accel[2]);
//		printf("imu_temp:%.2f\r\n", temp);

		// PID Control
		PID_calc(&temp_pid, temp, 37.0f);		// 30.0f is the target temperature
		TIM3_PWM_SET_PULSE(temp_pid.out + 500);	// Set PWM pulse width (0.5*1000 + PID output)
		// Update system state

		// Mahony AHRS
		ahrs_update(gyro, accel, &ahrs_param);

		// load to g_systemstate
		g_system_state.bmi088_imu_data.quat.w = ahrs_param.ori.quaternion[0];
		g_system_state.bmi088_imu_data.quat.x = ahrs_param.ori.quaternion[1];
		g_system_state.bmi088_imu_data.quat.y = ahrs_param.ori.quaternion[2];
		g_system_state.bmi088_imu_data.quat.z = ahrs_param.ori.quaternion[3];

		// calculate inc angle
		float w = ahrs_param.ori.quaternion[0];
		float x = ahrs_param.ori.quaternion[1];
		float y = ahrs_param.ori.quaternion[2];
		float z = ahrs_param.ori.quaternion[3];

		uint8_t axis = 0; // 0: x  1: y  2: z
		float inc_ang;
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
		g_system_state.bmi088_imu_data.inc_angle = inc_ang;		// load to g_system_state

		// print AHRS Result
//		printf("main_imu:%.3f,%.3f,%.3f,%.3f\r\n",
//				ahrs_param.ori.quaternion[0],
//				ahrs_param.ori.quaternion[1],
//				ahrs_param.ori.quaternion[2],
//				ahrs_param.ori.quaternion[3]);
	}
}


void imu_bmi_time(void)
{
	// 读取BMI088传感器数据
	float gyro[3], accel[3], temp;
	BMI088_read(gyro, accel, &temp);

	g_system_state.bmi088_imu_data.acc.x = accel[0];
	g_system_state.bmi088_imu_data.acc.y = accel[1];
	g_system_state.bmi088_imu_data.acc.z = accel[2];
	g_system_state.bmi088_imu_data.gyro.x = gyro[0];
	g_system_state.bmi088_imu_data.gyro.y = gyro[1];
	g_system_state.bmi088_imu_data.gyro.z = gyro[2];
	g_system_state.bmi088_imu_data.temp = temp;
	g_system_state.bmi088_imu_data.dataReady = true;
}


//// IMU Timer Callback
//void callback_IMUTimer(void *argument)
//{
//	// 读取BMI088传感器数据
//	float gyro[3], accel[3], temp;
//	BMI088_read(gyro, accel, &temp);
//
//	g_system_state.bmi088_imu_data.acc.x = accel[0];
//	g_system_state.bmi088_imu_data.acc.y = accel[1];
//	g_system_state.bmi088_imu_data.acc.z = accel[2];
//	g_system_state.bmi088_imu_data.gyro.x = gyro[0];
//	g_system_state.bmi088_imu_data.gyro.y = gyro[1];
//	g_system_state.bmi088_imu_data.gyro.z = gyro[2];
//	g_system_state.bmi088_imu_data.temp = temp;
//	g_system_state.bmi088_imu_data.dataReady = true;
//
//	// 读取ICM20948传感器数据
//	imu_cbk_timer();
//}
//
///**
// * @brief 初始化并启动20msIMU周期定时器
// * @param None
// * @retval osStatus_t: 返回操作的状态
// */
//osStatus_t start_IMUTimer(void)
//{
//	// create a periodic timer
//	IMUTimerId = osTimerNew(callback_IMUTimer, osTimerPeriodic, NULL, NULL);
//    if (IMUTimerId == NULL) {
//        return osError;
//    }
//
//    // start timer
//    osStatus_t status = osTimerStart(IMUTimerId, 20U); 	// T = 20Ticks
//    return status;
//}


// ====================================================================//
// =====================ICM 20948 i2c Interface========================//
//ICM_20948_Device_t myICM_device;
//ICM_20948_Serif_t mySerif = {
//	.write = ICM_20948_i2c_write,
//	.read = ICM_20948_i2c_read,
//	.user = &hi2c1
//};
//
//ICM_20948_Status_e ICM_20948_i2c_write(uint8_t regaddr, uint8_t *pdata, uint32_t len, void *user) {
//     I2C_HandleTypeDef* hi2c = (I2C_HandleTypeDef*)user;
//     HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, ICM_20948_I2C_ADDR_AD1 << 1, regaddr, I2C_MEMADD_SIZE_8BIT, pdata, len, HAL_MAX_DELAY);
//     return (status == HAL_OK) ? ICM_20948_Stat_Ok : ICM_20948_Stat_Err;
//}
//
//ICM_20948_Status_e ICM_20948_i2c_read(uint8_t regaddr, uint8_t *pdata, uint32_t len, void *user) {
//     I2C_HandleTypeDef* hi2c = (I2C_HandleTypeDef*)user;
//     HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, ICM_20948_I2C_ADDR_AD1 << 1, regaddr, I2C_MEMADD_SIZE_8BIT, pdata, len, HAL_MAX_DELAY);
//     return (status == HAL_OK) ? ICM_20948_Stat_Ok : ICM_20948_Stat_Err;
//}
//
//// =========  NOT FINISHED YET ===========//
//bool ICM_20948_init(ICM_20948_Device_t *device, ICM_20948_Serif_t *serif, icm_20948_mode_t mode) {
//	ICM_20948_init_struct(device);
//	ICM_20948_link_serif(device, serif);
//
//	ICM_20948_check_id(device);
//	ICM_20948_sw_reset(device);
//	HAL_Delay(50);
//
//	ICM_20948_sleep(device, false);
//	ICM_20948_set_clock_source(device, ICM_20948_Clock_Auto);
//	ICM_20948_firmware_load(device);
//	ICM_20948_set_dmp_start_address(device, DMP_START_ADDRESS);
//
//	ICM_20948_fss_t fss;
//	fss.a = gpm4;
//	fss.g = dps2000;
//	ICM_20948_set_full_scale(device, (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), fss);
//
//	switch (mode) {
//		case IMU_6DOF_RAW:
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_RAW_ACCELEROMETER, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_RAW_GYROSCOPE, 1);
//			break;
//		case IMU_9DOF_RAW:
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_RAW_ACCELEROMETER, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_RAW_GYROSCOPE, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_RAW_MAGNETOMETER, 1);
//			break;
//
//		case IMU_6DOF_CAL:
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_ACCELEROMETER, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_GYROSCOPE, 1);
//			break;
//		case IMU_9DOF_CAL:
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_ACCELEROMETER, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_GYROSCOPE, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD, 1);
//			break;
//
//		case IMU_6DOF_DMP:
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_ACCELEROMETER, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_GYROSCOPE, 1);
//			break;
//		case IMU_9DOF_DMP:
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_ROTATION_VECTOR, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_ACCELEROMETER, 1);
//			inv_icm20948_enable_dmp_sensor(device, INV_ICM20948_SENSOR_GYROSCOPE, 1);
//			break;
//		default:
//			break;
//	}
//
////	switch(mode){
////		// set ODR for each sensor
////	}
//
//	ICM_20948_enable_FIFO(&myICM_device, true);
//	ICM_20948_enable_DMP(&myICM_device, true);
//	ICM_20948_reset_FIFO(&myICM_device);
//	ICM_20948_reset_DMP(&myICM_device);
//
//	printf("[INFO] IMU: ICM20948 initialization successful.\n");
//	return true;
//}
//
//bool ICM_20948_init_6dof(void) {
//    ICM_20948_init_struct(&myICM_device);
//    ICM_20948_link_serif(&myICM_device, &mySerif);
//
//    ICM_20948_check_id(&myICM_device);
//    ICM_20948_sw_reset(&myICM_device);
//    HAL_Delay(50);
//
//    ICM_20948_sleep(&myICM_device, false);
//    ICM_20948_set_clock_source(&myICM_device, ICM_20948_Clock_Auto);
//    ICM_20948_firmware_load(&myICM_device);
//    ICM_20948_set_dmp_start_address(&myICM_device, DMP_START_ADDRESS);
//
//    ICM_20948_fss_t fss;
//    fss.a = gpm4;
//    fss.g = dps2000;
//    ICM_20948_set_full_scale(&myICM_device, (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), fss);
//    inv_icm20948_enable_dmp_sensor(&myICM_device, INV_ICM20948_SENSOR_RAW_ACCELEROMETER, 1);
//    inv_icm20948_enable_dmp_sensor(&myICM_device, INV_ICM20948_SENSOR_RAW_GYROSCOPE, 1);
//    inv_icm20948_set_dmp_sensor_period(&myICM_device, DMP_ODR_Reg_Accel, 4);
//    inv_icm20948_set_dmp_sensor_period(&myICM_device, DMP_ODR_Reg_Gyro, 4);
//
//    ICM_20948_enable_FIFO(&myICM_device, true);
//    ICM_20948_enable_DMP(&myICM_device, true);
//    ICM_20948_reset_FIFO(&myICM_device);
//    ICM_20948_reset_DMP(&myICM_device);
//
//    printf("[INFO] IMU: ICM20948 initialization successful.\n");
//    return true;
//}
//
//bool ICM_20948_init_9dof_dmp(void) {
//    ICM_20948_init_struct(&myICM_device);
//    ICM_20948_link_serif(&myICM_device, &mySerif);
//
//    ICM_20948_check_id(&myICM_device);
//    ICM_20948_sw_reset(&myICM_device);
//    HAL_Delay(50);
//
//    ICM_20948_sleep(&myICM_device, false);
//    ICM_20948_set_clock_source(&myICM_device, ICM_20948_Clock_Auto);
//    ICM_20948_firmware_load(&myICM_device);
//    ICM_20948_set_dmp_start_address(&myICM_device, DMP_START_ADDRESS);
//
//    ICM_20948_fss_t fss;
//    fss.a = gpm4;
//    fss.g = dps2000;
//    ICM_20948_set_full_scale(&myICM_device, (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), fss);
//    inv_icm20948_enable_dmp_sensor(&myICM_device, INV_ICM20948_SENSOR_ROTATION_VECTOR, 1);
//    inv_icm20948_set_dmp_sensor_period(&myICM_device, DMP_ODR_Reg_Quat9, 4);		// 225Hz
//
//    ICM_20948_enable_FIFO(&myICM_device, true);
//    ICM_20948_enable_DMP(&myICM_device, true);
//    ICM_20948_reset_FIFO(&myICM_device);
//    ICM_20948_reset_DMP(&myICM_device);
//
//    printf("[INFO] IMU: ICM20948 initialization successful.\n");
//    return true;
//}
//
//void ICM_20948_read_dmp(void){
//	icm_20948_DMP_data_t data;
//	ICM_20948_Status_e status = inv_icm20948_read_dmp_data(&myICM_device, &data);
//
//	if (status == ICM_20948_Stat_Ok || status == ICM_20948_Stat_FIFOMoreDataAvail){
//		if ((data.header & DMP_header_bitmap_Quat9) > 0) {
//			int32_t q1_raw = data.Quat9.Data.Q1;
//			int32_t q2_raw = data.Quat9.Data.Q2;
//			int32_t q3_raw = data.Quat9.Data.Q3;
//
//			// Q30 format to float
//			float q_data[4];
//			q_data[1] = (float)q1_raw / 1073741824.0f;
//			q_data[2] = (float)q2_raw / 1073741824.0f;
//			q_data[3] = (float)q3_raw / 1073741824.0f;
//
//			float q_norm_sq = q_data[1]*q_data[1] + q_data[2]*q_data[2] + q_data[3]*q_data[3];
//			if (q_norm_sq < 1.0f) {
//				q_data[0] = sqrtf(1.0f - q_norm_sq);
//			} else {
//				q_data[0] = 0.0f;
//			}
//
//			// Update global state
//			g_system_state.icm20948_imu_data.quat.w = q_data[0];
//			g_system_state.icm20948_imu_data.quat.x = q_data[1];
//			g_system_state.icm20948_imu_data.quat.y = q_data[2];
//			g_system_state.icm20948_imu_data.quat.z = q_data[3];
//		}
//
//		if ((data.header & DMP_header_bitmap_Accel) > 0) {
//			g_system_state.icm20948_imu_data.acc.x = (float)data.Raw_Accel.Data.X;
//			g_system_state.icm20948_imu_data.acc.y = (float)data.Raw_Accel.Data.Y;
//			g_system_state.icm20948_imu_data.acc.z = (float)data.Raw_Accel.Data.Z;
//		}
//
//		if ((data.header & DMP_header_bitmap_Gyro) > 0) {
//			g_system_state.icm20948_imu_data.gyro.x = (float)data.Raw_Gyro.Data.X;
//			g_system_state.icm20948_imu_data.gyro.y = (float)data.Raw_Gyro.Data.Y;
//			g_system_state.icm20948_imu_data.gyro.z = (float)data.Raw_Gyro.Data.Z;
//		}
//		g_system_state.icm20948_imu_data.dataReady = true;
//	}
//}
