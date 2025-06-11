#include "motor_task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "HW_epos.h"
#include "sdo.h"
#include "epos.h"
#include "global_param.h"
//#include "usbd_cdc_if.h"
#include "cmsis_os.h"


osMessageQueueId_t motorStartQueue;
osMessageQueueId_t motorStopQueue;
osMessageQueueId_t motorZeroQueue;
osMessageQueueId_t motorModelQueue;

void app_motor_task_init(){

	motorStartQueue = osMessageQueueNew(10, sizeof(uint32_t), NULL);
	motorStopQueue  = osMessageQueueNew(10, sizeof(uint32_t), NULL);
	motorZeroQueue  = osMessageQueueNew(10, sizeof(uint32_t), NULL);
    motorModelQueue = osMessageQueueNew(10, sizeof(uint32_t), NULL);

	osDelay(1000);

	MOTOR1_EN_ON();
	MOTOR2_EN_ON();

	osDelay(1000);
	osDelay(2000);

	Epos_INIT_Controller1();
	Epos_INIT_Controller2();


	printf(".\r\n");
    while (!all_devices_initialized()) {
        osDelay(100);
    }
    printf("All devices initialized, entering Motor main loop.\r\n");

}

void app_motor_task_loop(){
	int received_motor_idx;

	int send_motor_data_to_usb_count = 1000;

	for(;;){
		//printf("app_motor_task_loop");

        // ����Ƿ����������������ʽ��
        if (osMessageQueueGet(motorStartQueue, &received_motor_idx, NULL, 0) == osOK) {
        	//printf("motorStartQueue:%d\r\n",received_motor_idx);
            // ���������������
            start_motor_control(received_motor_idx);

        }
        // ����Ƿ����������������ʽ��
        if (osMessageQueueGet(motorStopQueue, &received_motor_idx, NULL, 0) == osOK) {
        	//printf("motorStopQueue:%d\r\n",received_motor_idx);
            // ���������������
            stop_motor_control(received_motor_idx);
        }

        // ����Ƿ����������������ʽ��
        if (osMessageQueueGet(motorZeroQueue, &received_motor_idx, NULL, 0) == osOK) {
        	//printf("motorZeroQueue:%d\r\n",received_motor_idx);
            // ���������������
            stop_motor_control(received_motor_idx);
        }
        

		if(send_motor_data_to_usb_count <= 0){

	        // Read motor data for monitoring
	        MotorData motor_data1;
	        MotorData motor_data2;

	        Epos_ReadPosition(&Controller1, &motor_data1.position);
	        Epos_ReadVelocity(&Controller1, &motor_data1.velocity);
			Epos_ReadTorque(&Controller1, &motor_data1.torque);
			//Epos_ReadCurrent(&Controller1, &motor_data1.current);
			//Epos_ReadStatus(&Controller1, &motor_data1.status);


	        Epos_ReadPosition(&Controller2, &motor_data2.position);
	        Epos_ReadVelocity(&Controller2, &motor_data2.velocity);
			Epos_ReadTorque(&Controller2, &motor_data2.torque);
			//Epos_ReadCurrent(&Controller2, &motor_data2.current);
			//Epos_ReadStatus(&Controller2, &motor_data2.status);


	        // Update global state
	        set_motor_data(&motor_data1, 0);
	        set_motor_data(&motor_data2, 1);

	        // Transmit data if USB transmission is enabled
//			bool should_transmit = false;
//
//			lock_system_state();
//			should_transmit = g_system_state.usb_settings.motor_data_enabled;
//			unlock_system_state();

        
            char tx_buffer[128];

            // ��ʽ�����1������
            sprintf(tx_buffer,
                "M1,POS:%ld,VEL:%ld,TOR:%d\r\n",
                motor_data1.position,
                motor_data1.velocity,
                motor_data1.torque);


            UART6_SendData("TX", tx_buffer);


            sprintf(tx_buffer,
                "M2,POS:%ld,VEL:%ld,TOR:%d\r\n",
                motor_data2.position,
                motor_data2.velocity,
                motor_data2.torque);

            UART6_SendData("TX", tx_buffer);


            send_motor_data_to_usb_count = 50;

            }

            send_motor_data_to_usb_count  = send_motor_data_to_usb_count - 1;

            osDelay(2);
		}

}



// 电机状态数组
static MotorState_t motor_states[2];

// 电机命令队列
osMessageQueueId_t motorCommandQueue;


// 初始化电机控制
void motor_control_init(void) {

    // 创建电机命令队列
    motorCommandQueue = osMessageQueueNew(20, sizeof(MotorCommand_t), NULL);


    // 初始化电机状态
    for (uint8_t i = 0; i < 2; i++) {
        motor_states[i].index = i;
        motor_states[i].mode = MOTOR_MODE_IDLE;
        motor_states[i].is_running = false;
        motor_states[i].target_position = 0;
        motor_states[i].target_speed = 0;
        motor_states[i].target_torque = 0;
        motor_states[i].acceleration = 300000;  // 默认加速度
        motor_states[i].deceleration = 300000;  // 默认减速度
        motor_states[i].max_current = 7000;    // 默认最大电流
    }

    // 初始化硬件
    MOTOR1_EN_ON();
    MOTOR2_EN_ON();

    osDelay(4000);

    // 初始化EPOS控制器
    Epos_INIT_Controller1();
    Epos_INIT_Controller2();

    printf("Motor control module initialized\r\n");

//	while (!all_devices_initialized()) {
//		osDelay(100);
//	}
//	printf("All devices initialized, entering Motor main loop.\r\n");
}


// 发送电机命令（通过消息队列）
bool motor_send_command(MotorCommand_t* cmd) {
    if (cmd == NULL || cmd->motor_idx > 1) return false;

    // 发送命令到队列
    osStatus_t status = osMessageQueuePut(motorCommandQueue, cmd, 0, 0);
    return (status == osOK);
}


// 设置电机模式
bool motor_set_mode(uint8_t motor_idx, MotorMode_t mode) {
    if (motor_idx > 1) return false;

    // 保存新模式
    motor_states[motor_idx].mode = mode;

    // 将内部模式映射到EPOS模式
    Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;

    switch (mode) {
        case MOTOR_MODE_POSITION:
            Epos_setMode(controller, Profile_Position_Mode);
            break;

        case MOTOR_MODE_SPEED:
            Epos_setMode(controller, Profile_Velocity_Mode);
            break;

        case MOTOR_MODE_TORQUE:
            Epos_setMode(controller, CST);
            break;

        default:
            return false;
    }

    printf("Motor %d mode set to %d\r\n", motor_idx + 1, mode);
    return true;
}


// 启动电机
bool motor_start(uint8_t motor_idx) {
    if (motor_idx > 1) return false;

    Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;

    // 应用当前的加速度和减速度参数
    SDO_Write(controller, Profile_Acceleration, 0x00, motor_states[motor_idx].acceleration);
    SDO_Write(controller, Profile_Deceleration, 0x00, motor_states[motor_idx].deceleration);

    // 根据当前模式启动电机
    switch (motor_states[motor_idx].mode) {
        case MOTOR_MODE_POSITION:
            Epos_OperEn(controller);
            Epos_PosSet(controller, motor_states[motor_idx].target_position);
            break;

        case MOTOR_MODE_SPEED:
            Epos_OperEn(controller);
            Epos_SpeedSet(controller, motor_states[motor_idx].target_speed);
            break;

        case MOTOR_MODE_TORQUE:
            Epos_OperEn(controller);
            Epos_CSTSet(controller, motor_states[motor_idx].target_torque, 0);
            break;

        default:
            return false;
    }

    motor_states[motor_idx].is_running = true;
    printf("Motor %d started in mode %d\r\n", motor_idx + 1, motor_states[motor_idx].mode);
    return true;
}



// 停止电机
bool motor_stop(uint8_t motor_idx) {
    if (motor_idx > 1) return false;

    Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;
    Epos_ClorEn(controller);

    motor_states[motor_idx].is_running = false;
    printf("Motor %d stopped\r\n", motor_idx + 1);
    return true;
}



// 设置电机位置
bool motor_set_position(uint8_t motor_idx, int32_t position) {
    if (motor_idx > 1) return false;

    motor_states[motor_idx].target_position = position;

    // 如果当前处于位置模式且电机正在运行，则立即应用新的位置
    if (motor_states[motor_idx].mode == MOTOR_MODE_POSITION && motor_states[motor_idx].is_running) {
        Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;
        Epos_PosSet(controller, position);
    }

    printf("Motor %d target position set to %ld\r\n", motor_idx + 1, position);
    return true;
}


// 设置电机速度
bool motor_set_speed(uint8_t motor_idx, int32_t speed) {
    if (motor_idx > 1) return false;

    // 应用速度限制
    if (speed > 15000) speed = 15000;
    else if (speed < -15000) speed = -15000;

    motor_states[motor_idx].target_speed = speed;

    // 如果当前处于速度模式且电机正在运行，则立即应用新的速度
    if (motor_states[motor_idx].mode == MOTOR_MODE_SPEED && motor_states[motor_idx].is_running) {
        Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;
        Epos_SpeedSet(controller, speed);
    }

    printf("Motor %d target speed set to %ld\r\n", motor_idx + 1, speed);
    return true;
}


// 设置电机力矩
bool motor_set_torque(uint8_t motor_idx, int16_t torque) {
    if (motor_idx > 1) return false;

    // 应用力矩限制
    if (torque > 1000) torque = 1000;
    else if (torque < -1000) torque = -1000;

    motor_states[motor_idx].target_torque = torque;

    // 如果当前处于力矩模式且电机正在运行，则立即应用新的力矩
    if (motor_states[motor_idx].mode == MOTOR_MODE_TORQUE && motor_states[motor_idx].is_running) {
        Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;
        Epos_CSTSet(controller, torque, 0);
    }

    printf("Motor %d target torque set to %d\r\n", motor_idx + 1, torque);
    return true;
}

// 设置电机加速度
bool motor_set_acceleration(uint8_t motor_idx, uint32_t acceleration) {
    if (motor_idx > 1) return false;

    motor_states[motor_idx].acceleration = acceleration;

    // 如果电机正在运行，立即应用新的加速度
    if (motor_states[motor_idx].is_running) {
        Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;
        SDO_Write(controller, Profile_Acceleration, 0x00, acceleration);
    }

    printf("Motor %d acceleration set to %lu\r\n", motor_idx + 1, acceleration);
    return true;
}

// 设置电机减速度
bool motor_set_deceleration(uint8_t motor_idx, uint32_t deceleration) {
    if (motor_idx > 1) return false;

    motor_states[motor_idx].deceleration = deceleration;

    // 如果电机正在运行，立即应用新的减速度
    if (motor_states[motor_idx].is_running) {
        Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;
        SDO_Write(controller, Profile_Deceleration, 0x00, deceleration);
    }

    printf("Motor %d deceleration set to %lu\r\n", motor_idx + 1, deceleration);
    return true;
}

// 设置零位
bool motor_set_zero_position(uint8_t motor_idx) {
    if (motor_idx > 1) return false;

    // 停止电机
    motor_stop(motor_idx);

    // 设置零位
    Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;
    Epos_HMMSetNowZero(controller, 0, 0);

    printf("Motor %d zero position set\r\n", motor_idx + 1);
    return true;
}

// 获取电机数据
void motor_get_data(uint8_t motor_idx, MotorData* data) {
    if (motor_idx > 1 || data == NULL) return;

    Epos* controller = (motor_idx == 0) ? &Controller1 : &Controller2;

    Epos_ReadPosition(controller, &data->position);
    Epos_ReadVelocity(controller, &data->velocity);
    Epos_ReadTorque(controller, &data->torque);
    // 注意：current和status在原代码中未实现读取
    data->current = 0;
    data->status = 0;
}


// 发送电机数据到蓝牙客户端
static void send_motor_data_to_ble(void) {
    static uint16_t counter = 0;

    // 每50个周期发送一次数据
    if (++counter >= 500) {
        counter = 0;

        MotorData motor_data1, motor_data2;
        motor_get_data(0, &motor_data1);
        motor_get_data(1, &motor_data2);

        char tx_buffer[128];

        // 发送电机1数据
        sprintf(tx_buffer, "M1,POS:%ld,VEL:%ld,TOR:%d",
                motor_data1.position,
                motor_data1.velocity,
                motor_data1.torque);
        UART6_SendData("TX", tx_buffer);

        // 发送电机2数据
        sprintf(tx_buffer, "M2,POS:%ld,VEL:%ld,TOR:%d",
                motor_data2.position,
                motor_data2.velocity,
                motor_data2.torque);
        UART6_SendData("TX", tx_buffer);
    }
}


// 电机控制主任务循环
void motor_control_task(void) {
    MotorCommand_t cmd;

    // 检查并处理命令队列中的命令
    if (osMessageQueueGet(motorCommandQueue, &cmd, NULL, 0) == osOK) {
        switch (cmd.cmd_type) {
            case MOTOR_CMD_SET_MODE:
                motor_set_mode(cmd.motor_idx, cmd.params.mode);
                break;

            case MOTOR_CMD_START:
                motor_start(cmd.motor_idx);
                break;

            case MOTOR_CMD_STOP:
                motor_stop(cmd.motor_idx);
                break;

            case MOTOR_CMD_SET_POSITION:
                motor_set_position(cmd.motor_idx, cmd.params.position);
                break;

            case MOTOR_CMD_SET_SPEED:
                motor_set_speed(cmd.motor_idx, cmd.params.speed);
                break;

            case MOTOR_CMD_SET_TORQUE:
                motor_set_torque(cmd.motor_idx, cmd.params.torque);
                break;

            case MOTOR_CMD_SET_ACCELERATION:
                motor_set_acceleration(cmd.motor_idx, cmd.params.acceleration);
                break;

            case MOTOR_CMD_SET_DECELERATION:
                motor_set_deceleration(cmd.motor_idx, cmd.params.deceleration);
                break;

            case MOTOR_CMD_SET_ZERO:
                motor_set_zero_position(cmd.motor_idx);
                break;
        }
    }

    // 处理老的消息队列（兼容原有代码）
    uint8_t motor_idx;
    if (osMessageQueueGet(motorStartQueue, &motor_idx, NULL, 0) == osOK) {
        motor_start(motor_idx);
    }

    if (osMessageQueueGet(motorStopQueue, &motor_idx, NULL, 0) == osOK) {
        motor_stop(motor_idx);
    }

    if (osMessageQueueGet(motorZeroQueue, &motor_idx, NULL, 0) == osOK) {
        motor_set_zero_position(motor_idx);
    }

    // 发送电机数据到蓝牙
    send_motor_data_to_ble();

    // 短暂延时
    osDelay(2);
}
