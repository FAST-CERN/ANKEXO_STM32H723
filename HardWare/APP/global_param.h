#ifndef __GLOBAL_PARAM_H__
#define __GLOBAL_PARAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "cmsis_os.h"
#include <stdbool.h>
#include <string.h>



extern bool success;
extern osSemaphoreId_t initSemaphore;


bool all_devices_initialized();

void update_device_status(void);               // Update device initialization status
void update_init_progress(void);               // Update initialization progress


#define ANGLE_FILTER_SIZE 5

#define GAIT_PHASE_UNKNOWN     0
#define GAIT_PHASE_STANCE      1
#define GAIT_PHASE_EARLY_SWING 2
#define GAIT_PHASE_LATE_SWING  3

#define GAIT_LEARNING_INIT      0
#define GAIT_LEARNING_COMPLETE  1


/*
 * 描述：设备状态/初始化结果监控变量
 *
 *
 *
 *
 * */


/* 设备状态结构体 */
typedef struct {
    bool imu_1_init;
    bool imu_2_init;
    bool mt6835_cs1_init;
    char error_message[256];
} RS485_Sensor_DeviceStatus;


typedef struct {
	RS485_Sensor_DeviceStatus rs485_sensor_devicestatus_1;
	RS485_Sensor_DeviceStatus rs485_sensor_devicestatus_2;
    bool bmi088_pcb_devicestatus;
    bool motor_devicestatus_1;
    bool motor_devicestatus_2;
    char error_message[256];
} DeviceInitStatus;

extern DeviceInitStatus g_device_status;

/*
 * 描述：数据USB数据传输控制变量
 *
 *
 *
 *
 * */


// USB data transmission settings
typedef struct {
    bool imu_data_enabled;       // Transmit IMU data over USB
    bool encoder_data_enabled;   // Transmit encoder data over USB
    bool motor_data_enabled;     // Transmit motor data over USB
} UsbTransmitSettings;


/*
 * 描述：电机模式/参数变量
 *
 *
 *
 *
 * */


// 电机控制模式
typedef enum {
    MOTOR_MODE_IDLE,       // 空闲模式
    MOTOR_MODE_POSITION,   // 位置控制模式
    MOTOR_MODE_SPEED,      // 速度控制模式
    MOTOR_MODE_TORQUE      // 力矩控制模式
} MotorMode_t;


// 电机命令类型
typedef enum {
    MOTOR_CMD_SET_MODE,           // 设置电机模式
    MOTOR_CMD_START,              // 启动电机
    MOTOR_CMD_STOP,               // 停止电机
    MOTOR_CMD_SET_POSITION,       // 设置位置
    MOTOR_CMD_SET_SPEED,          // 设置速度
    MOTOR_CMD_SET_TORQUE,         // 设置力矩
    MOTOR_CMD_SET_ACCELERATION,   // 设置加速度
    MOTOR_CMD_SET_DECELERATION,   // 设置减速度
    MOTOR_CMD_SET_ZERO            // 设置零位
} MotorCmdType_t;


// 电机命令参数
typedef struct {
    MotorCmdType_t cmd_type;      // 命令类型
    uint8_t motor_idx;            // 电机索引 (0-1)
    union {
        MotorMode_t mode;         // 电机模式
        int32_t position;         // 位置值
        int32_t speed;            // 速度值
        int16_t torque;           // 力矩值
        uint32_t acceleration;    // 加速度值
        uint32_t deceleration;    // 减速度值
    } params;
} MotorCommand_t;

//// Motor control parameters
//typedef struct {
//    uint16_t max_current;        // Maximum current in mA
//    uint16_t acceleration;       // Acceleration in rpm/s
//    uint16_t deceleration;       // Deceleration in rpm/s
//    float target_speed;          // Target speed in rpm
//    float target_position;       // Target position in counts
//    float target_torque;         // Target torque in mNm
//    uint32_t execution_time;     // Execution time in ms (0 = infinite/until completion)
//    bool is_running;             // Whether the motor is currently running
//    MotorControlMode motor_control_mode;
//} MotorControlParams;


// 电机状态数组
typedef struct {
    uint8_t index;               // 电机索引 (0-1)
    MotorMode_t mode;            // 当前控制模式
    bool is_running;             // 运行状态
    int32_t target_position;     // 目标位置
    int32_t target_speed;        // 目标速度
    int16_t target_torque;       // 目标力矩
    uint32_t acceleration;       // 加速度
    uint32_t deceleration;       // 减速度
    uint16_t max_current;        // 最大电流
    bool params_changed;         // 参数是否已更改
} MotorState_t;


// Motor data structure
typedef struct {
    int32_t position;              // Current position
    int32_t velocity;              // Current velocity
    int16_t torque;                // Current torque
    float current;               // Current current draw
    uint8_t status;              // Status flags
} MotorData;


/*
 * 描述：RS485传感器变量
 *
 *
 *
 *
 * */


/* 传感器数据结构体，与从机一致 */
typedef struct {
    float w;
    float x;
    float y;
    float z;
} Quaternion_t;

typedef struct {
    float x;
    float y;
    float z;
} Acc_t;

typedef struct {
    float x;
    float y;
    float z;
} Gryo_t;

typedef struct {
    float x;
    float y;
    float z;
} Meg_t;

typedef struct {
    Acc_t acc;
    Gryo_t gyro;
    Meg_t mag;
    Quaternion_t quat;
    bool dataReady;
} IMU_Data_t;

typedef struct {
    float angle;            // Current position in counts
} EncoderData;


// Encoder data structure
typedef struct {
	EncoderData encoder_data;
	IMU_Data_t imu_data_1;
	IMU_Data_t imu_data_2;
} RS485_Sensor_Data;


/*
 * 描述：步态算法的相关变量
 *
 *
 *
 *
 * */


//Kexo Control mode
typedef enum {
    KEXO_MODE_NONE = 0,       // No mode
    KEXO_MODE_INIT,           // Idle mode
    KEXO_MODE_IDLE,           // Idle mode
    KEXO_MODE_CONINTING,      // Training mode
    KEXO_MODE_SITTOSTAND,     // Sitting mode
    KEXO_MODE_STANDTOSIT,     // Standing mode
    KEXO_MODE_WALKING         // Walking mode
} KexoControlMode;


//kexo CONINTING mode LEVEL
typedef enum {
    KEXO_LEVEL_1,            // Level 1
    KEXO_LEVEL_2,            // Level 2
    KEXO_LEVEL_3,            // Level 3
    KEXO_LEVEL_4,            // Level 4
    KEXO_LEVEL_5,            // Level 5
    KEXO_LEVEL_6,            // Level 5
} KexoControl_CONINTING_Level;



/*
 * 描述：全局变量
 *
 *
 *
 *
 * */


// UI and application state
typedef struct {

    // USB transmission settings
    UsbTransmitSettings usb_settings;          // USB transmission settings

    // Motor specific parameters
    //MotorControlParams motor1_params;          // Motor 1 control parameters
    //MotorControlParams motor2_params;          // Motor 2 control parameters
    // Motor data
    MotorData motor_data_1;                          // Motor 1 data
    MotorData motor_data_2;                          // Motor 2 data

    //RS485 SENSOR DATA
    RS485_Sensor_Data rs485_sensor_data_1;
    RS485_Sensor_Data rs485_sensor_data_2;

    //BMI088 PCB IMU
    IMU_Data_t bmi088_imu_data;

    // System mutex for accessing shared data
    osMutexId_t system_mutex;                  // Mutex for accessing system data


} SystemState;


extern SystemState g_system_state;             // System state

void init_system_mutex(void);

// Signal that a device is initialized
void signal_device_initialized(const char* device_name, bool status);

// Mutex management
void lock_system_state(void);
void unlock_system_state(void);

// Data access functions with mutex protection

void get_encoder_data(EncoderData* data, uint8_t encoder_index);
void set_encoder_data(const EncoderData* data, uint8_t encoder_index);

void get_motor_data(MotorData* data, uint8_t motor_index);
void set_motor_data(const MotorData* data, uint8_t motor_index);

// Motion control functions   motor_index = 1/2
//void set_motor_control_mode(uint8_t motor_index, MotorControlMode mode);
//void start_motor_operation(uint8_t motor_index);
//void stop_motor_operation(uint8_t motor_index);
//void set_motor_parameters(uint8_t motor_index, const MotorControlParams* params);


// USB transmission control
void set_usb_transmission(bool imu_enabled, bool encoder_enabled, bool motor_enabled);
void transmit_data_over_usb(void);



#ifdef __cplusplus
}
#endif

#endif
