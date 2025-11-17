#include "ICM_20948_APP.h"

ICM_20948_I2C IMU_LEFT_UP_I2C;
ICM_20948_I2C IMU_LEFT_DOWN_I2C;
ICM_20948_I2C IMU_RIGHT_UP_I2C;
ICM_20948_I2C IMU_RIGHT_DOWN_I2C;

Quaternion_t quat_left_up;
Quaternion_t quat_left_down;
Quaternion_t quat_right_up;
Quaternion_t quat_right_down;

Acc_t acc_left_up  = {0.0f, 0.0f, 0.0f};
Acc_t acc_left_down  = {0.0f, 0.0f, 0.0f};
Acc_t acc_right_up  = {0.0f, 0.0f, 0.0f};
Acc_t acc_right_down  = {0.0f, 0.0f, 0.0f};

Gryo_t gryo_left_up  = {0.0f, 0.0f, 0.0f};
Gryo_t gryo_left_down  = {0.0f, 0.0f, 0.0f};
Gryo_t gryo_right_up  = {0.0f, 0.0f, 0.0f};
Gryo_t gryo_right_down  = {0.0f, 0.0f, 0.0f};

Meg_t meg_left_up  = {0.0f, 0.0f, 0.0f};
Meg_t meg_left_down  = {0.0f, 0.0f, 0.0f};
Meg_t meg_right_up  = {0.0f, 0.0f, 0.0f};
Meg_t meg_right_down  = {0.0f, 0.0f, 0.0f};

IMU_Data_t imu_data[IMU_COUNT];


/*
 * VIN ��ɫ
 * GND ��ɫ
 * SCL ��ɫ
 * SDA ��ɫ
 *
 * */


void updateIMUStatus(const char* imuName, bool status) {
    if (strcmp(imuName, "IMU_LEFT_UP") == 0) {
//        g_device_status.imu_left_up_init = status;
    } else if (strcmp(imuName, "IMU_LEFT_DOWN") == 0) {
//        g_device_status.imu_left_down_init = status;
    } else if (strcmp(imuName, "IMU_RIGHT_UP") == 0) {
//        g_device_status.imu_right_up_init = status;
    } else if (strcmp(imuName, "IMU_RIGHT_DOWN") == 0) {
//        g_device_status.imu_right_down_init = status;
    }

    osSemaphoreRelease(initSemaphore);
}


bool initializeIMU(ICM_20948_I2C& imu, uint8_t i2cBus, uint8_t i2cAddress, const char* imuName) {
    bool initialized = false;
    bool success = true;

    printf("%s INIT START\r\n", imuName);

    // ��ʼ�� IMU
    while (!initialized) {
        imu.begin(i2cBus, i2cAddress);
        if (imu.status != ICM_20948_Stat_Ok) {
            printf("%s Status NOT OK! ERRID : %d\r\n", imuName, imu.status);

            snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                    "%s initialization failed. Status: %d, Bus: %d, Addr: 0x%x",
                    imuName, imu.status, i2cBus, i2cAddress);

            static int retry_count = 0;
            if (++retry_count >= 10) {
                updateIMUStatus(imuName, false);
                return false;
            }

            osDelay(200);
        } else {
            initialized = true;
            printf("%s Status OK!\r\n", imuName);
        }
    }

    if (imu.initializeDMP() != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s DMP initialization failed", imuName);
    }
    osDelay(2);


    if (imu.enableDMPSensor(INV_ICM20948_SENSOR_RAW_GYROSCOPE) != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s failed to enable Game Rotation Vector sensor", imuName);
    }
    osDelay(2);

    if (imu.enableDMPSensor(INV_ICM20948_SENSOR_RAW_ACCELEROMETER) != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s failed to enable Game Rotation Vector sensor", imuName);
    }
    osDelay(2);

//    if (imu.enableDMPSensor(INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR) != ICM_20948_Stat_Ok) {
//        success = false;
//        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
//                "%s failed to enable Game Rotation Vector sensor", imuName);
//    }
//    osDelay(2);

//    if (imu.setDMPODRrate(DMP_ODR_Reg_Quat6, 1) != ICM_20948_Stat_Ok) {
//        success = false;
//        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
//                "%s failed to set DMP ODR rate", imuName);
//    }
//    osDelay(2);

    if (imu.setDMPODRrate(DMP_ODR_Reg_Accel, 1) != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s failed to set DMP ODR rate", imuName);
    }
    osDelay(2);
    if (imu.setDMPODRrate(DMP_ODR_Reg_Gyro, 1) != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s failed to set DMP ODR rate", imuName);
    }
    osDelay(2);

    if (imu.enableFIFO() != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s failed to enable FIFO", imuName);
    }
    osDelay(2);

    if (imu.enableDMP() != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s failed to enable DMP", imuName);
    }
    osDelay(2);

    if (imu.resetDMP() != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s failed to reset DMP", imuName);
    }
    osDelay(2);

    if (imu.resetFIFO() != ICM_20948_Stat_Ok) {
        success = false;
        snprintf(g_device_status.error_message, sizeof(g_device_status.error_message),
                "%s failed to reset FIFO", imuName);
    }
    osDelay(2);

    if (success) {
        printf("%s DMP INIT OK!\r\n", imuName);
    } else {
        printf("%s DMP INIT FAILED!\r\n", imuName);
    }

    updateIMUStatus(imuName, success);
    return success;
}



bool readQuaternionFromDMP(ICM_20948_I2C* imu, Quaternion_t* quaternion, Acc_t* acc, Gryo_t* gryo, Meg_t* meg) {
    icm_20948_DMP_data_t dmpData;

    ICM_20948_Status_e result = imu->readDMPdataFromFIFO(&dmpData);

    if ((result == ICM_20948_Stat_Ok) || (result == ICM_20948_Stat_FIFOMoreDataAvail)) {

        if (1){
			quaternion->w = 0;
			quaternion->x = 0;
			quaternion->y = 0;
			quaternion->z = 0;

			if ((dmpData.header & DMP_header_bitmap_Accel) > 0) // Check for Accel
			{
				acc->x = (float)dmpData.Raw_Accel.Data.X; // Extract the raw accelerometer data
				acc->y = (float)dmpData.Raw_Accel.Data.Y;
				acc->z = (float)dmpData.Raw_Accel.Data.Z;
			}

			if ( (dmpData.header & DMP_header_bitmap_Gyro) > 0 ) // Check for Gyro
			{
				gryo->x = (float)dmpData.Raw_Gyro.Data.X; // Extract the raw gyro data
				gryo->y = (float)dmpData.Raw_Gyro.Data.Y;
				gryo->z = (float)dmpData.Raw_Gyro.Data.Z;
			}
			return true;
        }

        if ((dmpData.header & DMP_header_bitmap_Quat9) > 0) {

        	double q0, q1, q2, q3;
	        q1 = ((double)dmpData.Quat9.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
	        q2 = ((double)dmpData.Quat9.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
	        q3 = ((double)dmpData.Quat9.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30
	        q0 = 0;// sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));


	        quaternion->w = q0;
	        quaternion->x = q1;
	        quaternion->y = q2;
	        quaternion->z = q3;


//	        if ((dmpData.header & DMP_header_bitmap_Accel) > 0) // Check for Accel
//			  {
//				acc->x = (float)dmpData.Raw_Accel.Data.X; // Extract the raw accelerometer data
//				acc->y = (float)dmpData.Raw_Accel.Data.Y;
//				acc->z = (float)dmpData.Raw_Accel.Data.Z;
//			  }
//
//			  if ( (dmpData.header & DMP_header_bitmap_Gyro) > 0 ) // Check for Gyro
//			  {
//				gryo->x = (float)dmpData.Raw_Gyro.Data.X; // Extract the raw gyro data
//				gryo->y = (float)dmpData.Raw_Gyro.Data.Y;
//				gryo->z = (float)dmpData.Raw_Gyro.Data.Z;
//			  }
//
//			  if ( (dmpData.header & DMP_header_bitmap_Compass) > 0 ) // Check for Compass
//			  {
//				meg->x = (float)dmpData.Compass.Data.X; // Extract the compass data
//				meg->y = (float)dmpData.Compass.Data.Y;
//				meg->z = (float)dmpData.Compass.Data.Z;
//			  }

            return true;
        }
    }
    return false;
}


void IMU_ICM_INIT(void)
{
    printf("Starting IMU initialization...\r\n");

    // IMU_LEFT_UP, IMU_LEFT_DOWN, IMU_RIGHT_UP, IMU_RIGHT_DOWN
    bool successLeftUp = initializeIMU(IMU_LEFT_UP_I2C, I2C_BUS_1, ICM_20948_I2C_ADDR_AD1, "IMU_LEFT_UP");
    bool successLeftDown = initializeIMU(IMU_LEFT_DOWN_I2C, I2C_BUS_1, ICM_20948_I2C_ADDR_AD0, "IMU_LEFT_DOWN");
    bool successRightUp = initializeIMU(IMU_RIGHT_UP_I2C, I2C_BUS_2, ICM_20948_I2C_ADDR_AD1, "IMU_RIGHT_UP");
    bool successRightDown = initializeIMU(IMU_RIGHT_DOWN_I2C, I2C_BUS_2, ICM_20948_I2C_ADDR_AD0, "IMU_RIGHT_DOWN");

    if (!successLeftUp || !successLeftDown || !successRightUp || !successRightDown){
        printf("One or more IMUs failed to initialize.\r\n");
    } else {
        printf("All IMUs initialized successfully!\r\n");
    }

    osDelay(100);


    int i = 100;
    while(i--) {

        bool new_data_left_up = readQuaternionFromDMP(&IMU_LEFT_UP_I2C, &quat_left_up, &acc_left_up, &gryo_left_up, &meg_left_up);
        if (new_data_left_up) {
            printf("LEFT_UP Quaternion: w=%.4f, x=%.4f, y=%.4f, z=%.4f\n",
                    quat_left_up.w, quat_left_up.x, quat_left_up.y, quat_left_up.z);
        }

        // ���� IMU ���ݶ�ȡ
        bool new_data_left_down = readQuaternionFromDMP(&IMU_LEFT_DOWN_I2C, &quat_left_down, &acc_left_down, &gryo_left_down, &meg_left_down);
        if (new_data_left_down) {
            printf("LEFT_DOWN Quaternion: w=%.4f, x=%.4f, y=%.4f, z=%.4f\n",
                    quat_left_down.w, quat_left_down.x, quat_left_down.y, quat_left_down.z);
        }

        // ���� IMU ���ݶ�ȡ
        bool new_data_right_up = readQuaternionFromDMP(&IMU_RIGHT_UP_I2C, &quat_right_up, &acc_right_up, &gryo_right_up, &meg_right_up);
        if (new_data_right_up) {
            printf("RIGHT_UP Quaternion: w=%.4f, x=%.4f, y=%.4f, z=%.4f\n",
                    quat_right_up.w, quat_right_up.x, quat_right_up.y, quat_right_up.z);
        }

        // ���� IMU ���ݶ�ȡ
        bool new_data_right_down = readQuaternionFromDMP(&IMU_RIGHT_DOWN_I2C, &quat_right_down, &acc_right_down, &gryo_right_down, &meg_right_down);
        if (new_data_right_down) {
            printf("RIGHT_DOWN Quaternion: w=%.4f, x=%.4f, y=%.4f, z=%.4f\n",
                    quat_right_down.w, quat_right_down.x, quat_right_down.y, quat_right_down.z);
        }

        osDelay(4); // �ȶ� IMU ���ݶ�ȡ
    }

    // �ȴ������豸��ʼ�����
    while (!all_devices_initialized()) {
        osDelay(100);
    }

    printf("All devices initialized, entering IMU main loop.\r\n");
}


// ��ȫ������������±���
typedef struct {
    uint32_t sample_count;          // ����������
    uint32_t last_sample_time;      // �ϴμ���Ƶ�ʵ�ʱ���(ms)
    uint32_t last_calculation_time; // �ϴμ���Ƶ�ʵ�ʱ��
    float actual_frequency;         // ʵ�ʵĲ���Ƶ��(Hz)
} IMU_Frequency_t;


IMU_Frequency_t imu_frequencies[IMU_COUNT] = {0};

#define FREQUENCY_CALC_INTERVAL 500


void IMU_Frequency_Init(void)
{
    for (int i = 0; i < IMU_COUNT; i++) {
        imu_frequencies[i].sample_count = 0;
        imu_frequencies[i].last_sample_time = HAL_GetTick();
        imu_frequencies[i].last_calculation_time = HAL_GetTick();
        imu_frequencies[i].actual_frequency = 0.0f;
    }
}



void UpdateIMUSamplingFrequency(uint8_t imu_index)
{
    imu_frequencies[imu_index].sample_count++;

    // ÿ�ɼ�FREQUENCY_CALC_INTERVAL�κ����һ��ʵ��Ƶ��
    if (imu_frequencies[imu_index].sample_count >= FREQUENCY_CALC_INTERVAL) {
        uint32_t current_time = HAL_GetTick();
        uint32_t time_elapsed = current_time - imu_frequencies[imu_index].last_calculation_time;

        // ��ֹ������
        if (time_elapsed > 0) {
            imu_frequencies[imu_index].actual_frequency = (float)(FREQUENCY_CALC_INTERVAL * 1000.0f) / time_elapsed;
            printf("IMU %d Frequency: %.2f Hz\r\n", imu_index, imu_frequencies[imu_index].actual_frequency);
        }

        imu_frequencies[imu_index].sample_count = 0;
        imu_frequencies[imu_index].last_calculation_time = current_time;
    }
}

void IMU_ICM_LOOP(void)
{

    bool new_data_left_up = readQuaternionFromDMP(&IMU_LEFT_UP_I2C, &quat_left_up, &acc_left_up, &gryo_left_up, &meg_left_up);
    if (new_data_left_up) {
        imu_data[IMU_LEFT_UP].acc = acc_left_up;
        imu_data[IMU_LEFT_UP].gyro = gryo_left_up;
        imu_data[IMU_LEFT_UP].mag = meg_left_up;
        imu_data[IMU_LEFT_UP].quat = quat_left_up;
        //printf("IMU_LOOP IMU_LEFT_UP\r\n");
        //AddIMUDataToBuffer(IMU_LEFT_UP, imu_data[IMU_LEFT_UP].acc, imu_data[IMU_LEFT_UP].gyro, imu_data[IMU_LEFT_UP].mag, imu_data[IMU_LEFT_UP].quat);
//        printf("LEFT_UP ACC: x=%.4f, y=%.4f, z=%.4f\n",
//        		imu_data[IMU_LEFT_UP].acc.x, imu_data[IMU_LEFT_UP].acc.y, imu_data[IMU_LEFT_UP].acc.z);
//        set_imu_data(&imu_data[IMU_LEFT_UP], IMU_LEFT_UP);
        // ���²���Ƶ�ʼ���
		UpdateIMUSamplingFrequency(IMU_LEFT_UP);

    }

    bool new_data_left_down = readQuaternionFromDMP(&IMU_LEFT_DOWN_I2C, &quat_left_down, &acc_left_down, &gryo_left_down, &meg_left_down);
    if (new_data_left_down) {
        imu_data[IMU_LEFT_DOWN].acc = acc_left_down;
        imu_data[IMU_LEFT_DOWN].gyro = gryo_left_down;
        imu_data[IMU_LEFT_DOWN].mag = meg_left_down;
        imu_data[IMU_LEFT_DOWN].quat = quat_left_down;
//        printf("LEFT_DOWN Quaternion: w=%.4f, x=%.4f, y=%.4f, z=%.4f\n",
//        		imu_data[IMU_LEFT_DOWN].quat.w,imu_data[IMU_LEFT_DOWN].quat.x, imu_data[IMU_LEFT_DOWN].quat.y, imu_data[IMU_LEFT_DOWN].quat.z);
        //printf("IMU_LOOP IMU_LEFT_DOWN\r\n");
       // AddIMUDataToBuffer(IMU_LEFT_DOWN, imu_data[IMU_LEFT_DOWN].acc, imu_data[IMU_LEFT_DOWN].gyro, imu_data[IMU_LEFT_DOWN].mag, imu_data[IMU_LEFT_DOWN].quat);
//        set_imu_data(&imu_data[IMU_LEFT_DOWN], IMU_LEFT_DOWN);
        // ���²���Ƶ�ʼ���
        UpdateIMUSamplingFrequency(IMU_LEFT_DOWN);
    }

    bool new_data_right_up = readQuaternionFromDMP(&IMU_RIGHT_UP_I2C, &quat_right_up, &acc_right_up, &gryo_right_up, &meg_right_up);
    if (new_data_right_up) {
        imu_data[IMU_RIGHT_UP].acc = acc_right_up;
        imu_data[IMU_RIGHT_UP].gyro = gryo_right_up;
        imu_data[IMU_RIGHT_UP].mag = meg_right_up;
        imu_data[IMU_RIGHT_UP].quat = quat_right_up;
        //printf("IMU_LOOP IMU_RIGHT_UP\r\n");
        //AddIMUDataToBuffer(IMU_RIGHT_UP, imu_data[IMU_RIGHT_UP].acc, imu_data[IMU_RIGHT_UP].gyro, imu_data[IMU_RIGHT_UP].mag, imu_data[IMU_RIGHT_UP].quat);
//        set_imu_data(&imu_data[IMU_RIGHT_UP], IMU_RIGHT_UP);
        // ���²���Ƶ�ʼ���
	    UpdateIMUSamplingFrequency(IMU_RIGHT_UP);
    }

    bool new_data_right_down = readQuaternionFromDMP(&IMU_RIGHT_DOWN_I2C, &quat_right_down, &acc_right_down, &gryo_right_down, &meg_right_down);
    if (new_data_right_down) {
        imu_data[IMU_RIGHT_DOWN].acc = acc_right_down;
        imu_data[IMU_RIGHT_DOWN].gyro = gryo_right_down;
        imu_data[IMU_RIGHT_DOWN].mag = meg_right_down;
        imu_data[IMU_RIGHT_DOWN].quat = quat_right_down;
        //printf("IMU_LOOP IMU_RIGHT_DOWN\r\n");
        //AddIMUDataToBuffer(IMU_RIGHT_DOWN, imu_data[IMU_RIGHT_DOWN].acc, imu_data[IMU_RIGHT_DOWN].gyro, imu_data[IMU_RIGHT_DOWN].mag, imu_data[IMU_RIGHT_DOWN].quat);
//        set_imu_data(&imu_data[IMU_RIGHT_DOWN], IMU_RIGHT_DOWN);
        // ���²���Ƶ�ʼ���
        UpdateIMUSamplingFrequency(IMU_RIGHT_DOWN);
    }
    //printf("IMU_LOOP\r\n");
}


