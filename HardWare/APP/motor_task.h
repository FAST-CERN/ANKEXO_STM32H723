#ifndef __MOTOR_TASK_H__
#define __MOTOR_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "HW_epos.h"
#include "sdo.h"
#include "epos.h"
#include <stdbool.h>
#include "global_param.h"

void app_motor_task_init();

void app_motor_task_loop();

void stop_motor_control(uint8_t motor_index);

void start_motor_control(uint8_t motor_index) ;

void stop_motor_monitoring() ;

void start_motor_monitoring() ;



// 初始化电机控制
void motor_control_init(void);

bool motor_send_command(MotorCommand_t* cmd);

// 设置电机模式
bool motor_set_mode(uint8_t motor_idx, MotorMode_t mode);

// 启动电机
bool motor_start(uint8_t motor_idx);

// 停止电机
bool motor_stop(uint8_t motor_idx);

// 设置电机位置
bool motor_set_position(uint8_t motor_idx, int32_t position);

// 设置电机速度
bool motor_set_speed(uint8_t motor_idx, int32_t speed);

// 设置电机力矩
bool motor_set_torque(uint8_t motor_idx, int16_t torque);

// 设置电机加速度
bool motor_set_acceleration(uint8_t motor_idx, uint32_t acceleration);

// 设置电机减速度
bool motor_set_deceleration(uint8_t motor_idx, uint32_t deceleration);

// 设置零位
bool motor_set_zero_position(uint8_t motor_idx);

// 获取电机数据
void motor_get_data(uint8_t motor_idx, MotorData* data);

// 电机控制主任务循环
void motor_control_task(void);


#ifdef __cplusplus
}
#endif

#endif

