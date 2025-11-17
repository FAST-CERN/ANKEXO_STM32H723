#include "main.h"
#include "string.h"
#include "global_param.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdbool.h>

//#include "usbd_cdc_if.h"

bool success = 1;

// Initialization status semaphore for thread synchronization
osSemaphoreId_t initSemaphore;

// Device initialization status global variable
DeviceInitStatus g_device_status = {
    .rs485_sensor_devicestatus_1 = {
        .imu_1_init = false,
        .imu_2_init = false,
        .mt6835_cs1_init = false,
        .error_message = ""
    },
    .rs485_sensor_devicestatus_2 = {
        .imu_1_init = false,
        .imu_2_init = false,
        .mt6835_cs1_init = false,
        .error_message = ""
    },
    .bmi088_pcb_devicestatus = false,
    .motor_devicestatus_1 = false,
    .motor_devicestatus_2 = false,
    .error_message = ""
};

// Check if all devices are initialized
bool all_devices_initialized() {
    return (g_device_status.motor_devicestatus_1 &&
            g_device_status.motor_devicestatus_2 &&
            g_device_status.rs485_sensor_devicestatus_1.mt6835_cs1_init &&
            g_device_status.rs485_sensor_devicestatus_1.imu_1_init &&
            g_device_status.rs485_sensor_devicestatus_1.imu_2_init &&
            g_device_status.rs485_sensor_devicestatus_2.mt6835_cs1_init &&
			g_device_status.rs485_sensor_devicestatus_2.imu_1_init &&
			g_device_status.rs485_sensor_devicestatus_2.imu_2_init);
}


// Initialize system state
SystemState g_system_state = {
    // Initialize with default USB settings
    .usb_settings = {
        .imu_data_enabled = false,
        .encoder_data_enabled = false,
        .motor_data_enabled = false
    },

    // Initialize with default motor parameters
//    .motor1_params = {
//        .max_current = 6000,     // 6A default (mA)
//        .acceleration = 12000,   // 12000 rpm/s default
//        .deceleration = 12000,   // 12000 rpm/s default
//        .target_speed = 0,
//        .target_position = 0,
//        .target_torque = 0,
//        .execution_time = 0,
//        .is_running = false
//    },
//
//    .motor2_params = {
//        .max_current = 6000,     // 6A default
//        .acceleration = 12000,   // 12000 rpm/s default
//        .deceleration = 12000,   // 12000 rpm/s default
//        .target_speed = 0,
//        .target_position = 0,
//        .target_torque = 0,
//        .execution_time = 0,
//        .is_running = false
//    },

    // Initialize motor data
    .motor_data_1 = {0},
    .motor_data_2 = {0},

    // Initialize RS485 sensor data
    .rs485_sensor_data_1 = {0},
    .rs485_sensor_data_2 = {0},

    // Initialize Main IMU data
    .bmi088_imu_data = {0},
	.icm20948_imu_data = {0},

    // Initialize system mutex
    .system_mutex = NULL
};



// Mutex definition for system state access
osMutexId_t system_state_mutex_id = NULL;
const osMutexAttr_t system_state_mutex_attributes = {
  .name = "system_state_mutex"
};

// Initialize the system mutex
void init_system_mutex(void) {
    if (system_state_mutex_id == NULL) {
        system_state_mutex_id = osMutexNew(&system_state_mutex_attributes);
    }
}

// Lock the system state mutex
void lock_system_state(void) {
//    if (system_state_mutex_id != NULL) {
//    	osMutexAcquire(system_state_mutex_id, osWaitForever);
//    }
}

// Unlock the system state mutex
void unlock_system_state(void) {
    if (system_state_mutex_id != NULL) {
        osMutexRelease(system_state_mutex_id);
    }
}




//// Start motor operation
//void start_motor_operation(uint8_t motor_index) {
//    lock_system_state();
//
//    if (motor_index == 0) {
//        g_system_state.motor1_params.is_running = true;
//    } else if (motor_index == 1) {
//        g_system_state.motor2_params.is_running = true;
//    }
//
//    unlock_system_state();
//}
//
//// Stop motor operation
//void stop_motor_operation(uint8_t motor_index) {
//    lock_system_state();
//
//    if (motor_index == 0) {
//        g_system_state.motor1_params.is_running = false;
//    } else if (motor_index == 1) {
//        g_system_state.motor2_params.is_running = false;
//    }
//
//    unlock_system_state();
//}
//
//// Set motor parameters
//void set_motor_parameters(uint8_t motor_index, const MotorControlParams* params) {
//    if (params == NULL) return;
//
//    lock_system_state();
//
//    if (motor_index == 0) {
//        memcpy(&g_system_state.motor1_params, params, sizeof(MotorControlParams));
//    } else if (motor_index == 1) {
//        memcpy(&g_system_state.motor2_params, params, sizeof(MotorControlParams));
//    }
//
//    unlock_system_state();
//}

//// Set motor control mode
//void set_motor_control_mode(uint8_t motor_index, MotorControlMode mode) {
//    lock_system_state();
//
//    g_system_state.motor_control_mode = mode;
//
//   // printf("set_motor_control_mode %d\r\n",motor_index);
//
//    unlock_system_state();
//}





// Helper function to send data over USB CDC
static void send_over_usb(const char *data, uint16_t len) {
    // Transmit using USB CDC
    //CDC_Transmit_FS((uint8_t *)data, len);
}


// Set USB transmission settings
void set_usb_transmission(bool imu_enabled, bool encoder_enabled, bool motor_enabled) {
    lock_system_state();

    g_system_state.usb_settings.imu_data_enabled = imu_enabled;
    g_system_state.usb_settings.encoder_data_enabled = encoder_enabled;
    g_system_state.usb_settings.motor_data_enabled = motor_enabled;

    unlock_system_state();
}


// Transmit data over USB
void transmit_data_over_usb(void) {
    // Get current settings
    bool send_imu = false;
    bool send_encoder = false;
    bool send_motor = false;

    // Lock while reading settings
    lock_system_state();
    send_imu = g_system_state.usb_settings.imu_data_enabled;
    send_encoder = g_system_state.usb_settings.encoder_data_enabled;
    send_motor = g_system_state.usb_settings.motor_data_enabled;
    unlock_system_state();

    char tx_buffer[1024]; // Buffer to store formatted data
    int offset = 0;       // Length of formatted string

    // Send IMU data if enabled
    if (send_imu) {
        // Send IMU data from RS485 sensors and BMI088
        IMU_Data_t imu_data[5]; // 4 IMUs from RS485 sensors + 1 BMI088

        // Get data from RS485 sensors
        lock_system_state();
        imu_data[0] = g_system_state.rs485_sensor_data_1.imu_data_1;
        imu_data[1] = g_system_state.rs485_sensor_data_1.imu_data_2;
        imu_data[2] = g_system_state.rs485_sensor_data_2.imu_data_1;
        imu_data[3] = g_system_state.rs485_sensor_data_2.imu_data_2;
        imu_data[4] = g_system_state.bmi088_imu_data;
        unlock_system_state();

        // Format quaternions
        for (int i = 0; i < 5; i++) {
            offset += snprintf(tx_buffer + offset, sizeof(tx_buffer) - offset,
                "IMU%d,QUAT:(%.2f,%.2f,%.2f,%.2f);",
                i, imu_data[i].quat.w, imu_data[i].quat.x,
                imu_data[i].quat.y, imu_data[i].quat.z);

            // Ensure no buffer overflow
            if (offset >= sizeof(tx_buffer)) {
                break;
            }
        }

        // Format accelerometer data
        for (int i = 0; i < 5; i++) {
            offset += snprintf(tx_buffer + offset, sizeof(tx_buffer) - offset,
                "IMU%d,ACC:(%.2f,%.2f,%.2f);",
                i, imu_data[i].acc.x, imu_data[i].acc.y, imu_data[i].acc.z);

            // Ensure no buffer overflow
            if (offset >= sizeof(tx_buffer)) {
                break;
            }
        }

        // Format gyroscope data
        for (int i = 0; i < 5; i++) {
            offset += snprintf(tx_buffer + offset, sizeof(tx_buffer) - offset,
                "IMU%d,GRY:(%.2f,%.2f,%.2f);",
                i, imu_data[i].gyro.x, imu_data[i].gyro.y, imu_data[i].gyro.z);

            // Ensure no buffer overflow
            if (offset >= sizeof(tx_buffer)) {
                break;
            }
        }
    }

    // Send encoder data if enabled
    if (send_encoder) {
        EncoderData encoder_data[2];
        get_encoder_data(&encoder_data[0], 0);
        get_encoder_data(&encoder_data[1], 1);

        for (int i = 0; i < 2; i++) {
            offset += snprintf(tx_buffer + offset, sizeof(tx_buffer) - offset,
                "ENC%d,ANGLE:%.2f;",
                i + 1, encoder_data[i].angle);

            // Ensure no buffer overflow
            if (offset >= sizeof(tx_buffer)) {
                break;
            }
        }
    }

    // Send motor data if enabled
    if (send_motor) {
        MotorData motor_data[2];
        get_motor_data(&motor_data[0], 0);
        get_motor_data(&motor_data[1], 1);

        for (int i = 0; i < 2; i++) {
            offset += snprintf(tx_buffer + offset, sizeof(tx_buffer) - offset,
                "M%d,POS:%ld,VEL:%ld,TOR:%d,CUR:%.2f,STATUS:%u;",
                i + 1,
                motor_data[i].position,
                motor_data[i].velocity,
                motor_data[i].torque,
                motor_data[i].current,
                motor_data[i].status);

            // Ensure no buffer overflow
            if (offset >= sizeof(tx_buffer)) {
                break;
            }
        }
    }

    if (offset != 0) {
        // Add a newline at the end to indicate end of message
        if (offset < sizeof(tx_buffer)) {
            snprintf(tx_buffer + offset, sizeof(tx_buffer) - offset, "\r\n");
        }
        // Transmit the entire buffer
        send_over_usb(tx_buffer, strlen(tx_buffer));
    }
}
