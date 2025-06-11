#include "main.h"
#include "ble_server.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "usart.h"
#include "global_param.h"
#include "motor_task.h"


int motor_idx, value;
extern osMessageQueueId_t motorStartQueue;
extern osMessageQueueId_t motorStopQueue;
extern osMessageQueueId_t motorZeroQueue;

/*
 * NRF52832蓝牙透传功能
 * */

uint8_t ble_rxBuffer[BLE_RX_BUFFER_SIZE];
uint8_t ble_txBuffer[BLE_TX_BUFFER_SIZE];
uint8_t ble_rxData;
BLE_Status_t current_ble_status = BLE_DISCONNECTED;
volatile uint16_t ble_rxIndex = 0;


// (根据前缀和数据发送给 NRF52832)
void UART6_SendData(const char *prefix, const char *data)
{
    if (strncmp(prefix, "TX", 2) == 0)
    {
        if (current_ble_status != BLE_NOTIFICATION_ENABLED)
        {
            printf("BLE not connected or notification not enabled, TX data not sent.\r\n");
            return;
        }
    }
    uint8_t buffer[BLE_TX_BUFFER_SIZE];
    snprintf((char *)buffer, BLE_TX_BUFFER_SIZE, "%s%s\r\n", prefix, data);
    HAL_UART_Transmit(&huart6, buffer, strlen((char *)buffer), HAL_MAX_DELAY);
}

// 初始 UART5 接收中断
void UART6_StartReceive(void)
{
    HAL_UART_Receive_IT(&huart6, &ble_rxData, 1);  // ??启接收中??
}


// 查找字符串中的命令模式
char* findCommand(const char* buffer, const char* cmd) {
    return strstr(buffer, cmd);
}
//
//
//
//void ble_command_progress(){
//
//	if (ble_rxIndex < BLE_RX_BUFFER_SIZE - 1)
//	{
//		if (ble_rxData == '\n')//|| ble_rxData == '\r')
//		{
//			ble_rxBuffer[ble_rxIndex] = '\0';  // 添加字符串结束符
//			printf("Received data from NRF: %s\r\n", ble_rxBuffer);  // 调试输出接收数据
//
//			// 使用查找函数来定位命令，而不是依赖固定位置
//			char *cmdPtr;
//
//			// 解析 NRF52810 的回复或透传数据
//			if ((cmdPtr = findCommand((char *)ble_rxBuffer, "BLCONNECT")) != NULL)
//			{
//				current_ble_status = BLE_CONNECTED;
//				printf("BLE Status: Connected\r\n");
//				//LED2_ON();
//			}
//			else if ((cmdPtr = findCommand((char *)ble_rxBuffer, "BLDISCONNECT")) != NULL)
//			{
//				current_ble_status = BLE_DISCONNECTED;
//				printf("BLE Status: Disconnected\r\n");
//				//LED2_OFF();
//			}
//			else if ((cmdPtr = findCommand((char *)ble_rxBuffer, "BLNUSEN")) != NULL)
//			{
//				current_ble_status = BLE_NOTIFICATION_ENABLED;
//				printf("BLE Status: Notification Enabled\r\n");
//				// LED2ON();
//			}
//			else if ((cmdPtr = findCommand((char *)ble_rxBuffer, "BLNUSDIS")) != NULL)
//			{
//				current_ble_status = BLE_NOTIFICATION_DISABLED;
//				printf("BLE Status: Notification Disabled\r\n");
//				// LED2OFF();
//			}
//			// 在HAL_UART_RxCpltCallback函数中的数据透传部分添加以下代码
//			else if ((cmdPtr = findCommand((char *)ble_rxBuffer, "RX")) != NULL)
//			{
//				// 获取RX之后的命令内容
//				char* dataPtr = cmdPtr + 2;
//				printf("Transparent Data Received: %s\r\n", dataPtr);
//
//
//				// 对透传命令进行分析，找到命令类型
//				char cmd_type = '\0';
//				char sub_cmd = '\0';
//				int motor_idx, value;
//
//				// 寻找电机命令 'M'
//				char* mCmdPtr = strstr(dataPtr, "M");
//				if (mCmdPtr != NULL && mCmdPtr < dataPtr + 10) { // 在合理范围内查找
//					cmd_type = 'M';
//					if (strlen(mCmdPtr) > 1) {
//						sub_cmd = mCmdPtr[1];
//					}
//				}
//
//				// 寻找数据回传控制命令 'D'
//				char* dCmdPtr = strstr(dataPtr, "D");
//				if (dCmdPtr != NULL && dCmdPtr < dataPtr + 10) {
//					cmd_type = 'D';
//					if (strlen(dCmdPtr) > 1) {
//						sub_cmd = dCmdPtr[1];
//					}
//				}
//
//				// 寻找数据回传控制命令 'K'
//				char* kCmdPtr = strstr(dataPtr, "K");
//				if (kCmdPtr != NULL && kCmdPtr < dataPtr + 10) {
//					cmd_type = 'K';
//					if (strlen(kCmdPtr) > 1) {
//						sub_cmd = kCmdPtr[1];
//					}
//				}
//
//				if (cmd_type == 'M')  // 电机相关命令
//				{
//					MotorControlParams params;
//
//					switch(sub_cmd)
//					{
//						case 'V':  // 速度控制
//							if (strstr(mCmdPtr + 2, "S") != NULL)  // 启动
//							{
//								char* motorIdxPtr = strstr(mCmdPtr + 3, "1") ? "1" :
//													strstr(mCmdPtr + 3, "2") ? "2" : NULL;
//								if (motorIdxPtr) {
//									motor_idx = *motorIdxPtr - '1';
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										// 设置模式并启动
//										//set_motor_control_mode(motor_idx, MOTOR_CTRL_SPEED);
//										//start_motor_control(motor_idx);
//										if(osMessageQueuePut(motorStartQueue, &motor_idx, 0, 0) == osOK){
//											printf("osMessageQueuePut OK\r\n");
//										}
//										printf("Motor %d speed mode started\r\n", motor_idx + 1);
//
//										UART6_SendData("TX", "MVS");
//									}
//								}
//							}
//							else if (strstr(mCmdPtr + 2, "P") != NULL)  // 停止
//							{
//								char* motorIdxPtr = strstr(mCmdPtr + 3, "1") ? "1" :
//													strstr(mCmdPtr + 3, "2") ? "2" : NULL;
//								if (motorIdxPtr) {
//									motor_idx = *motorIdxPtr - '1';
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										//stop_motor_control(motor_idx);
//										osMessageQueuePut(motorStopQueue, &motor_idx, 0, 0);
//										printf("Motor %d stopped\r\n", motor_idx + 1);
//
//										UART6_SendData("TX", "MVP");
//									}
//								}
//							}
//							else  // 设置速度 - 使用sscanf提取参数
//							{
//								// 查找MV命令后的数字
//								char* numPtr = mCmdPtr + 2;
//								while (*numPtr && !isdigit(*numPtr)) numPtr++;
//
//								if (sscanf(numPtr, "%d,%d", &motor_idx, &value) == 2)
//								{
//									motor_idx--; // 转换为0基索引
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										// 获取当前参数
//										if (motor_idx == 0) {
//											memcpy(&params, &g_system_state.motor1_params, sizeof(MotorControlParams));
//										} else {
//											memcpy(&params, &g_system_state.motor2_params, sizeof(MotorControlParams));
//										}
//
//										// 更新速度参数与限幅
//										if(value >= 15000){
//											value = 15000;
//											params.target_speed = value;
//										}
//										else if (value <= -15000)
//										{
//											value = -15000;
//											params.target_speed = value;
//										}
//										else {
//											params.target_speed = value;
//										}
//
//										set_motor_parameters(motor_idx, &params);
//										printf("Set motor %d speed to %d\r\n", motor_idx + 1, value);
//
//										UART6_SendData("TX", "MV");
//									}
//								}
//							}
//							break;
//
//						case 'P':  // 位置控制
//							if (strstr(mCmdPtr + 2, "S") != NULL)  // 启动
//							{
//								char* motorIdxPtr = strstr(mCmdPtr + 3, "1") ? "1" :
//													strstr(mCmdPtr + 3, "2") ? "2" : NULL;
//								if (motorIdxPtr) {
//									motor_idx = *motorIdxPtr - '1';
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										//set_motor_control_mode(motor_idx, MOTOR_CTRL_POSITION);
//										//start_motor_control(motor_idx);
//										osMessageQueuePut(motorStartQueue, &motor_idx, 0, 0);
//										printf("Motor %d position mode started\r\n", motor_idx + 1);
//
//										UART6_SendData("TX", "MPS");
//									}
//								}
//							}
//							else if (strstr(mCmdPtr + 2, "P") != NULL)  // 停止
//							{
//								char* motorIdxPtr = strstr(mCmdPtr + 3, "1") ? "1" :
//													strstr(mCmdPtr + 3, "2") ? "2" : NULL;
//								if (motorIdxPtr) {
//									motor_idx = *motorIdxPtr - '1';
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										//stop_motor_control(motor_idx);
//										osMessageQueuePut(motorStopQueue, &motor_idx, 0, 0);
//										printf("Motor %d stopped\r\n", motor_idx + 1);
//
//										UART6_SendData("TX", "MPP");
//									}
//								}
//							}
//							else  // 设置位置
//							{
//								// 查找MP命令后的数字
//								char* numPtr = mCmdPtr + 2;
//								while (*numPtr && !isdigit(*numPtr)) numPtr++;
//
//								if (sscanf(numPtr, "%d,%d", &motor_idx, &value) == 2)
//								{
//									motor_idx--; // 转换为0基索引
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										// 获取当前参数
//										if (motor_idx == 0) {
//											memcpy(&params, &g_system_state.motor1_params, sizeof(MotorControlParams));
//										} else {
//											memcpy(&params, &g_system_state.motor2_params, sizeof(MotorControlParams));
//										}
//										// 更新位置参数
//										params.target_position = value;
//										set_motor_parameters(motor_idx, &params);
//
//										UART6_SendData("TX", "MP");
//									}
//								}
//							}
//							break;
//
//						case 'T':  // 力矩控制
//							if (strstr(mCmdPtr + 2, "S") != NULL)  // 启动
//							{
//								char* motorIdxPtr = strstr(mCmdPtr + 3, "1") ? "1" :
//													strstr(mCmdPtr + 3, "2") ? "2" : NULL;
//								if (motorIdxPtr) {
//									motor_idx = *motorIdxPtr - '1';
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										//set_motor_control_mode(motor_idx, MOTOR_CTRL_TORQUE);
//										//start_motor_control(motor_idx);
//										osMessageQueuePut(motorStartQueue, &motor_idx, 0, 0);
//
//										printf("Motor %d torque mode started\r\n", motor_idx + 1);
//
//										UART6_SendData("TX", "MTS");
//									}
//								}
//							}
//							else if (strstr(mCmdPtr + 2, "P") != NULL)  // 停止
//							{
//								char* motorIdxPtr = strstr(mCmdPtr + 3, "1") ? "1" :
//													strstr(mCmdPtr + 3, "2") ? "2" : NULL;
//								if (motorIdxPtr) {
//									motor_idx = *motorIdxPtr - '1';
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										//stop_motor_control(motor_idx);
//										osMessageQueuePut(motorStopQueue, &motor_idx, 0, 0);
//										printf("Motor %d stopped\r\n", motor_idx + 1);
//
//										UART6_SendData("TX", "MTP");
//									}
//								}
//							}
//							else  // 设置力矩
//							{
//								// 查找MT命令后的数字
//								char* numPtr = mCmdPtr + 2;
//								while (*numPtr && !isdigit(*numPtr)) numPtr++;
//
//								if (sscanf(numPtr, "%d,%d", &motor_idx, &value) == 2)
//								{
//									motor_idx--; // 转换为0基索引
//									if (motor_idx >= 0 && motor_idx < 2)
//									{
//										// 获取当前参数
//										if (motor_idx == 0) {
//											memcpy(&params, &g_system_state.motor1_params, sizeof(MotorControlParams));
//										} else {
//											memcpy(&params, &g_system_state.motor2_params, sizeof(MotorControlParams));
//										}
//
//										// 更新力矩参数
//										if(value >= 1000){
//											value = 1000;
//										}
//										else if(value <=-1000){
//											value = -1000;
//										}
//										params.target_torque = value;
//										set_motor_parameters(motor_idx, &params);
//
//										UART6_SendData("TX", "MT");
//									}
//								}
//							}
//							break;
//
//						case 'A':  // 设置加速度
//							// 查找MA命令后的数字
//							char* numPtr = mCmdPtr + 2;
//							while (*numPtr && !isdigit(*numPtr)) numPtr++;
//
//							if (sscanf(numPtr, "%d,%d", &motor_idx, &value) == 2)
//							{
//								motor_idx--; // 转换为0基索引
//								if (motor_idx >= 0 && motor_idx < 2)
//								{
//									// 获取当前参数
//									if (motor_idx == 0) {
//										memcpy(&params, &g_system_state.motor1_params, sizeof(MotorControlParams));
//									} else {
//										memcpy(&params, &g_system_state.motor2_params, sizeof(MotorControlParams));
//									}
//
//									// 更新加速度参数
//									params.acceleration = value;
//									set_motor_parameters(motor_idx, &params);
//								}
//							}
//							break;
//
//						case 'D':  // 设置减速度
//							// 查找MD命令后的数字
//							char* numPtr2 = mCmdPtr + 2;
//							while (*numPtr2 && !isdigit(*numPtr2)) numPtr2++;
//
//							if (sscanf(numPtr2, "%d,%d", &motor_idx, &value) == 2)
//							{
//								motor_idx--; // 转换为0基索引
//								if (motor_idx >= 0 && motor_idx < 2)
//								{
//									// 获取当前参数
//									if (motor_idx == 0) {
//										memcpy(&params, &g_system_state.motor1_params, sizeof(MotorControlParams));
//									} else {
//										memcpy(&params, &g_system_state.motor2_params, sizeof(MotorControlParams));
//									}
//
//									// 更新减速度参数
//									params.deceleration = value;
//									set_motor_parameters(motor_idx, &params);
//								}
//							}
//							break;
//
//						case 'Z':  // 零位置设置
//							char* motorIdxPtr = strstr(mCmdPtr + 2, "1") ? "1" :
//												strstr(mCmdPtr + 2, "2") ? "2" : NULL;
//							if (motorIdxPtr) {
//								motor_idx = *motorIdxPtr - '1';
//								if (motor_idx >= 0 && motor_idx < 2)
//								{
//									//stop_motor_control(motor_idx);
//									osMessageQueuePut(motorZeroQueue, &motor_idx, 0, 0);
//									printf("Motor %d HMMZero\r\n", motor_idx + 1);
//
//									UART6_SendData("TX", "MZ");
//								}
//							}
//							break;
//					}
//				}
//				else if (cmd_type == 'D')  // 数据回传控制
//				{
//					int enable;
//
//					// 寻找启用/禁用状态
//					char* statusPtr = dCmdPtr + 2;
//					while (*statusPtr && !isdigit(*statusPtr)) statusPtr++;
//
//					if (*statusPtr) {
//						enable = *statusPtr - '0';
//
//						switch(sub_cmd)
//						{
//							case 'I':  // IMU数据回传
//								g_system_state.usb_settings.imu_data_enabled = enable;
//								printf("IMU data transmission %s\r\n", enable ? "enabled" : "disabled");
//								UART6_SendData("TX", "DI");
//								break;
//
//							case 'E':  // 编码器数据回传
//								g_system_state.usb_settings.encoder_data_enabled = enable;
//								printf("Encoder data transmission %s\r\n", enable ? "enabled" : "disabled");
//								UART6_SendData("TX", "DE");
//								break;
//
//							case 'M':  // 电机数据回传
//								g_system_state.usb_settings.motor_data_enabled = enable;
//								printf("Motor data transmission %s\r\n", enable ? "enabled" : "disabled");
//								UART6_SendData("TX", "DM");
//								break;
//						}
//					}
//				}
////				else if(cmd_type == 'K') //外骨骼步态控制
////				{
////					printf("KEXO CONTROL\r\n");
////					switch(sub_cmd)
////					{
////						case 'N':  // 启动步态
////						g_system_state.kexo_control_mode = KEXO_MODE_NONE;
////						//osMessageQueuePut(motorModelQueue, &g_system_state.kexo_control_mode, 0, 0);
////						break;
////
////						case 'Z':  // 初始模式
////						g_system_state.kexo_control_mode = KEXO_MODE_INIT;
////						//osMessageQueuePut(motorModelQueue, &g_system_state.kexo_control_mode, 0, 0);
////
////						break;
////						case 'A':  // 零力模式
////						g_system_state.kexo_control_mode = KEXO_MODE_IDLE;
////						//osMessageQueuePut(motorModelQueue, &g_system_state.kexo_control_mode, 0, 0);
////
////						break;
////						case 'B':  // 持续助力
////						g_system_state.kexo_control_mode = KEXO_MODE_CONINTING;
////					   // osMessageQueuePut(motorModelQueue, &g_system_state.kexo_control_mode, 0, 0);
////
////						break;
////						case 'C':  // 坐到站
////						g_system_state.kexo_control_mode = KEXO_MODE_SITTOSTAND;
////						//osMessageQueuePut(motorModelQueue, &g_system_state.kexo_control_mode, 0, 0);
////
////						break;
////						case 'D':  // 站到坐
////						g_system_state.kexo_control_mode = KEXO_MODE_STANDTOSIT;
////						//osMessageQueuePut(motorModelQueue, &g_system_state.kexo_control_mode, 0, 0);
////
////						break;
////						case 'E':  // 行走
////						g_system_state.kexo_control_mode = KEXO_MODE_WALKING;
////						//osMessageQueuePut(motorModelQueue, &g_system_state.kexo_control_mode, 0, 0);
////						break;
////					}
////				}
//			}
//			else
//			{
//				printf("Unknown Command: %s\r\n", ble_rxBuffer);
//			}
//
//			ble_rxIndex = 0;
//			memset(ble_rxBuffer, 0, BLE_RX_BUFFER_SIZE);
//		}
//		else if(ble_rxData == 0x20){
//			// 空格字符处理，保持不变
//		}
//		else
//		{
//			ble_rxBuffer[ble_rxIndex++] = ble_rxData;
//		}
//	}
//	else
//	{
//		printf("RX Buffer Overflow\r\n");
//		ble_rxIndex = 0;
//		memset(ble_rxBuffer, 0, BLE_RX_BUFFER_SIZE);
//	}
//}
//
//









// 处理电机速度命令 (MV)
static void handle_motor_velocity_command(const char* cmd_ptr) {
    int motor_idx, value;
    MotorCommand_t motor_cmd;

    // 检查是否是启动命令
    if (findCommand(cmd_ptr + 2, "S") != NULL) {
        char* motor_id_ptr = findCommand(cmd_ptr + 3, "1") ? "1" :
        		findCommand(cmd_ptr + 3, "2") ? "2" : NULL;

        if (motor_id_ptr) {
            motor_idx = *motor_id_ptr - '1';

            if (motor_idx >= 0 && motor_idx < 2) {
                // 设置速度模式
                motor_cmd.cmd_type = MOTOR_CMD_SET_MODE;
                motor_cmd.motor_idx = motor_idx;
                motor_cmd.params.mode = MOTOR_MODE_SPEED;
                motor_send_command(&motor_cmd);

                // 启动电机
                motor_cmd.cmd_type = MOTOR_CMD_START;
                motor_send_command(&motor_cmd);

                printf("Motor %d speed mode command sent\r\n", motor_idx + 1);
                UART6_SendData("TX", "MVS");
            }
        }
    }
    // 检查是否是停止命令
    else if (findCommand(cmd_ptr + 2, "P") != NULL) {
        char* motor_id_ptr = findCommand(cmd_ptr + 3, "1") ? "1" :
        		findCommand(cmd_ptr + 3, "2") ? "2" : NULL;

        if (motor_id_ptr) {
            motor_idx = *motor_id_ptr - '1';

            if (motor_idx >= 0 && motor_idx < 2) {
                // 停止电机
                motor_cmd.cmd_type = MOTOR_CMD_STOP;
                motor_cmd.motor_idx = motor_idx;
                motor_send_command(&motor_cmd);

                printf("Motor %d stop command sent\r\n", motor_idx + 1);
                UART6_SendData("TX", "MVP");
            }
        }
    }
    // 否则是设置速度值
    else {
        // 查找MV命令后的数字
        char* num_ptr = cmd_ptr + 2;
        while (*num_ptr && !isdigit(*num_ptr)) num_ptr++;

        if (sscanf(num_ptr, "%d,%d", &motor_idx, &value) == 2) {
            motor_idx--; // 转换为0基索引

            if (motor_idx >= 0 && motor_idx < 2) {
                // 设置速度
                motor_cmd.cmd_type = MOTOR_CMD_SET_SPEED;
                motor_cmd.motor_idx = motor_idx;
                motor_cmd.params.speed = value;
                motor_send_command(&motor_cmd);

                printf("Motor %d speed set command sent: %d\r\n", motor_idx + 1, value);
                UART6_SendData("TX", "MV");
            }
        }
    }
}

// 处理电机位置命令 (MP)
static void handle_motor_position_command(const char* cmd_ptr) {
    int motor_idx, value;
    MotorCommand_t motor_cmd;

    // 检查是否是启动命令
    if (findCommand(cmd_ptr + 2, "S") != NULL) {
        char* motor_id_ptr = findCommand(cmd_ptr + 3, "1") ? "1" :
        		findCommand(cmd_ptr + 3, "2") ? "2" : NULL;

        if (motor_id_ptr) {
            motor_idx = *motor_id_ptr - '1';

            if (motor_idx >= 0 && motor_idx < 2) {
                // 设置位置模式
                motor_cmd.cmd_type = MOTOR_CMD_SET_MODE;
                motor_cmd.motor_idx = motor_idx;
                motor_cmd.params.mode = MOTOR_MODE_POSITION;
                motor_send_command(&motor_cmd);

                // 启动电机
                motor_cmd.cmd_type = MOTOR_CMD_START;
                motor_send_command(&motor_cmd);

                printf("Motor %d position mode command sent\r\n", motor_idx + 1);
                UART6_SendData("TX", "MPS");
            }
        }
    }
    // 检查是否是停止命令
    else if (findCommand(cmd_ptr + 2, "P") != NULL) {
        char* motor_id_ptr = findCommand(cmd_ptr + 3, "1") ? "1" :
        		findCommand(cmd_ptr + 3, "2") ? "2" : NULL;

        if (motor_id_ptr) {
            motor_idx = *motor_id_ptr - '1';

            if (motor_idx >= 0 && motor_idx < 2) {
                // 停止电机
                motor_cmd.cmd_type = MOTOR_CMD_STOP;
                motor_cmd.motor_idx = motor_idx;
                motor_send_command(&motor_cmd);

                printf("Motor %d stop command sent\r\n", motor_idx + 1);
                UART6_SendData("TX", "MPP");
            }
        }
    }
    // 否则是设置位置值
    else {
        // 查找MP命令后的数字
        char* num_ptr = cmd_ptr + 2;
        while (*num_ptr && !isdigit(*num_ptr)) num_ptr++;

        if (sscanf(num_ptr, "%d,%d", &motor_idx, &value) == 2) {
            motor_idx--; // 转换为0基索引

            if (motor_idx >= 0 && motor_idx < 2) {
                // 设置位置
                motor_cmd.cmd_type = MOTOR_CMD_SET_POSITION;
                motor_cmd.motor_idx = motor_idx;
                motor_cmd.params.position = value;
                motor_send_command(&motor_cmd);

                printf("Motor %d position set command sent: %d\r\n", motor_idx + 1, value);
                UART6_SendData("TX", "MP");
            }
        }
    }
}

// 处理电机力矩命令 (MT)
static void handle_motor_torque_command(const char* cmd_ptr) {
    int motor_idx, value;
    MotorCommand_t motor_cmd;

    // 检查是否是启动命令
    if (findCommand(cmd_ptr + 2, "S") != NULL) {
        char* motor_id_ptr = findCommand(cmd_ptr + 3, "1") ? "1" :
        		findCommand(cmd_ptr + 3, "2") ? "2" : NULL;

        if (motor_id_ptr) {
            motor_idx = *motor_id_ptr - '1';

            if (motor_idx >= 0 && motor_idx < 2) {
                // 设置力矩模式
                motor_cmd.cmd_type = MOTOR_CMD_SET_MODE;
                motor_cmd.motor_idx = motor_idx;
                motor_cmd.params.mode = MOTOR_MODE_TORQUE;
                motor_send_command(&motor_cmd);

                // 启动电机
                motor_cmd.cmd_type = MOTOR_CMD_START;
                motor_send_command(&motor_cmd);

                printf("Motor %d torque mode command sent\r\n", motor_idx + 1);
                UART6_SendData("TX", "MTS");
            }
        }
    }
    // 检查是否是停止命令
    else if (findCommand(cmd_ptr + 2, "P") != NULL) {
        char* motor_id_ptr = findCommand(cmd_ptr + 3, "1") ? "1" :
        		findCommand(cmd_ptr + 3, "2") ? "2" : NULL;

        if (motor_id_ptr) {
            motor_idx = *motor_id_ptr - '1';

            if (motor_idx >= 0 && motor_idx < 2) {
                // 停止电机
                motor_cmd.cmd_type = MOTOR_CMD_STOP;
                motor_cmd.motor_idx = motor_idx;
                motor_send_command(&motor_cmd);

                printf("Motor %d stop command sent\r\n", motor_idx + 1);
                UART6_SendData("TX", "MTP");
            }
        }
    }
    // 否则是设置力矩值
    else {
        // 查找MT命令后的数字
        char* num_ptr = cmd_ptr + 2;
        while (*num_ptr && !isdigit(*num_ptr)) num_ptr++;

        if (sscanf(num_ptr, "%d,%d", &motor_idx, &value) == 2) {
            motor_idx--; // 转换为0基索引

            if (motor_idx >= 0 && motor_idx < 2) {
                // 设置力矩
                motor_cmd.cmd_type = MOTOR_CMD_SET_TORQUE;
                motor_cmd.motor_idx = motor_idx;
                motor_cmd.params.torque = value;
                motor_send_command(&motor_cmd);

                printf("Motor %d torque set command sent: %d\r\n", motor_idx + 1, value);
                UART6_SendData("TX", "MT");
            }
        }
    }
}

// 处理电机加速度命令 (MA)
static void handle_motor_acceleration_command(const char* cmd_ptr) {
    int motor_idx, value;
    MotorCommand_t motor_cmd;

    // 查找MA命令后的数字
    char* num_ptr = cmd_ptr + 2;
    while (*num_ptr && !isdigit(*num_ptr)) num_ptr++;

    if (sscanf(num_ptr, "%d,%d", &motor_idx, &value) == 2) {
        motor_idx--; // 转换为0基索引

        if (motor_idx >= 0 && motor_idx < 2) {
            // 设置加速度
            motor_cmd.cmd_type = MOTOR_CMD_SET_ACCELERATION;
            motor_cmd.motor_idx = motor_idx;
            motor_cmd.params.acceleration = value;
            motor_send_command(&motor_cmd);

            printf("Motor %d acceleration set command sent: %d\r\n", motor_idx + 1, value);
            UART6_SendData("TX", "MA");
        }
    }
}

// 处理电机减速度命令 (MD)
static void handle_motor_deceleration_command(const char* cmd_ptr) {
    int motor_idx, value;
    MotorCommand_t motor_cmd;

    // 查找MD命令后的数字
    char* num_ptr = cmd_ptr + 2;
    while (*num_ptr && !isdigit(*num_ptr)) num_ptr++;

    if (sscanf(num_ptr, "%d,%d", &motor_idx, &value) == 2) {
        motor_idx--; // 转换为0基索引

        if (motor_idx >= 0 && motor_idx < 2) {
            // 设置减速度
            motor_cmd.cmd_type = MOTOR_CMD_SET_DECELERATION;
            motor_cmd.motor_idx = motor_idx;
            motor_cmd.params.deceleration = value;
            motor_send_command(&motor_cmd);

            printf("Motor %d deceleration set command sent: %d\r\n", motor_idx + 1, value);
            UART6_SendData("TX", "MD");
        }
    }
}

// 处理电机零位命令 (MZ)
static void handle_motor_zero_command(const char* cmd_ptr) {
    int motor_idx;
    MotorCommand_t motor_cmd;

    char* motor_id_ptr = findCommand(cmd_ptr + 2, "1") ? "1" :
    		findCommand(cmd_ptr + 2, "2") ? "2" : NULL;

    if (motor_id_ptr) {
        motor_idx = *motor_id_ptr - '1';

        if (motor_idx >= 0 && motor_idx < 2) {
            // 设置零位
            motor_cmd.cmd_type = MOTOR_CMD_SET_ZERO;
            motor_cmd.motor_idx = motor_idx;
            motor_send_command(&motor_cmd);

            printf("Motor %d zero position command sent\r\n", motor_idx + 1);
            UART6_SendData("TX", "MZ");
        }
    }
}

// 处理数据回传控制命令 (D)
static void handle_data_transmission_command(const char* cmd_ptr) {
    char sub_cmd = cmd_ptr[1];
    int enable;

    // 寻找启用/禁用状态
    char* status_ptr = cmd_ptr + 2;
    while (*status_ptr && !isdigit(*status_ptr)) status_ptr++;

    if (*status_ptr) {
        enable = *status_ptr - '0';

        switch(sub_cmd) {
            case 'I':  // IMU数据回传
                g_system_state.usb_settings.imu_data_enabled = enable;
                printf("IMU data transmission %s\r\n", enable ? "enabled" : "disabled");
                UART6_SendData("TX", "DI");
                break;

            case 'E':  // 编码器数据回传
                g_system_state.usb_settings.encoder_data_enabled = enable;
                printf("Encoder data transmission %s\r\n", enable ? "enabled" : "disabled");
                UART6_SendData("TX", "DE");
                break;

            case 'M':  // 电机数据回传
                g_system_state.usb_settings.motor_data_enabled = enable;
                printf("Motor data transmission %s\r\n", enable ? "enabled" : "disabled");
                UART6_SendData("TX", "DM");
                break;
        }
    }
}

// 处理外骨骼步态控制命令 (K)
//static void handle_exoskeleton_command(const char* cmd_ptr) {
//    char sub_cmd = cmd_ptr[1];
//
//    switch(sub_cmd) {
//        case 'N':  // 无模式
//            g_system_state.kexo_control_mode = KEXO_MODE_NONE;
//            printf("Exoskeleton mode: NONE\r\n");
//            UART6_SendData("TX", "KN");
//            break;
//
//        case 'Z':  // 初始模式
//            g_system_state.kexo_control_mode = KEXO_MODE_INIT;
//            printf("Exoskeleton mode: INIT\r\n");
//            UART6_SendData("TX", "KZ");
//            break;
//
//        case 'A':  // 零力模式
//            g_system_state.kexo_control_mode = KEXO_MODE_IDLE;
//            printf("Exoskeleton mode: IDLE\r\n");
//            UART6_SendData("TX", "KA");
//            break;
//
//        case 'B':  // 持续助力
//            g_system_state.kexo_control_mode = KEXO_MODE_CONINTING;
//            printf("Exoskeleton mode: CONTINUOUS ASSIST\r\n");
//            UART6_SendData("TX", "KB");
//            break;
//
//        case 'C':  // 坐到站
//            g_system_state.kexo_control_mode = KEXO_MODE_SITTOSTAND;
//            printf("Exoskeleton mode: SIT TO STAND\r\n");
//            UART6_SendData("TX", "KC");
//            break;
//
//        case 'D':  // 站到坐
//            g_system_state.kexo_control_mode = KEXO_MODE_STANDTOSIT;
//            printf("Exoskeleton mode: STAND TO SIT\r\n");
//            UART6_SendData("TX", "KD");
//            break;
//
//        case 'E':  // 行走
//            g_system_state.kexo_control_mode = KEXO_MODE_WALKING;
//            printf("Exoskeleton mode: WALKING\r\n");
//            UART6_SendData("TX", "KE");
//            break;
//    }
//}

// 处理蓝牙状态命令
static void handle_ble_status_command(const char* command) {
    if (findCommand(command, "BLCONNECT") != NULL) {
        current_ble_status = BLE_CONNECTED;
        printf("BLE Status: Connected\r\n");
    }
    else if (findCommand(command, "BLDISCONNECT") != NULL) {
        current_ble_status = BLE_DISCONNECTED;
        printf("BLE Status: Disconnected\r\n");
    }
    else if (findCommand(command, "BLNUSEN") != NULL) {
        current_ble_status = BLE_NOTIFICATION_ENABLED;
        printf("BLE Status: Notification Enabled\r\n");
    }
    else if (findCommand(command, "BLNUSDIS") != NULL) {
        current_ble_status = BLE_NOTIFICATION_DISABLED;
        printf("BLE Status: Notification Disabled\r\n");
    }
}

// 处理蓝牙命令
void ble_handle_command(const char* command) {
    // 处理蓝牙状态命令
    if (findCommand(command, "BLCONNECT") != NULL ||
        findCommand(command, "BLDISCONNECT") != NULL ||
        findCommand(command, "BLNUSEN") != NULL ||
        findCommand(command, "BLNUSDIS") != NULL) {
        handle_ble_status_command(command);
        return;
    }
    char *cmdPtr;
    // 检查是否是透传数据
    if ((cmdPtr = findCommand((char *)ble_rxBuffer, "RX")) != NULL) {
        // 获取RX之后的命令内容
        const char* data_ptr = cmdPtr + 2;
        printf("Transparent Data Received: %s\r\n", data_ptr);

        // 查找电机命令 'M'
        char* m_cmd_ptr = findCommand(data_ptr, "M");
        if (m_cmd_ptr != NULL && m_cmd_ptr < data_ptr + 10) {
            char sub_cmd = '\0';
            if (strlen(m_cmd_ptr) > 1) {
                sub_cmd = m_cmd_ptr[1];
            }

            switch (sub_cmd) {
                case 'V':  // 速度控制
                    handle_motor_velocity_command(m_cmd_ptr);
                    break;

                case 'P':  // 位置控制
                    handle_motor_position_command(m_cmd_ptr);
                    break;

                case 'T':  // 力矩控制
                    handle_motor_torque_command(m_cmd_ptr);
                    break;

                case 'A':  // 设置加速度
                    handle_motor_acceleration_command(m_cmd_ptr);
                    break;

                case 'D':  // 设置减速度
                    handle_motor_deceleration_command(m_cmd_ptr);
                    break;

                case 'Z':  // 零位置设置
                    handle_motor_zero_command(m_cmd_ptr);
                    break;
            }
        }

        // 查找数据回传控制命令 'D'
        char* d_cmd_ptr = findCommand(data_ptr, "D");
        if (d_cmd_ptr != NULL && d_cmd_ptr < data_ptr + 10) {
            handle_data_transmission_command(d_cmd_ptr);
        }

        // 查找外骨骼控制命令 'K'
        char* k_cmd_ptr = findCommand(data_ptr, "K");
        if (k_cmd_ptr != NULL && k_cmd_ptr < data_ptr + 10) {
            //handle_exoskeleton_command(k_cmd_ptr);
        }
    }
    else {
        printf("Unknown Command: %s\r\n", command);
    }
}

void ble_command_progress() {
    if (ble_rxIndex < BLE_RX_BUFFER_SIZE - 1) {
        if (ble_rxData == '\n') {
            ble_rxBuffer[ble_rxIndex] = '\0';  // 添加字符串结束符
            printf("Received data from NRF: %s\r\n", ble_rxBuffer);  // 调试输出接收数据

            // 调用新的命令处理函数，而不是直接在这里处理命令
            ble_handle_command((char *)ble_rxBuffer);

            // 重置接收缓冲区
            ble_rxIndex = 0;
            memset(ble_rxBuffer, 0, BLE_RX_BUFFER_SIZE);
        }
        else if (ble_rxData == 0x20) {
            // 空格字符处理，保持不变
        }
        else {
            ble_rxBuffer[ble_rxIndex++] = ble_rxData;
        }
    }
    else {
        printf("RX Buffer Overflow\r\n");
        ble_rxIndex = 0;
        memset(ble_rxBuffer, 0, BLE_RX_BUFFER_SIZE);
    }
}
