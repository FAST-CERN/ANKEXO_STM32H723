#include "S485_master.h"
#include "usart_server.h"
#include "global_param.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* 三字节帧头和帧尾 */
const uint8_t FRAME_HEADER[FRAME_HEADER_SIZE] = {0xA5, 0xEA, 0xA7}; // 帧头: A5 EA A7
const uint8_t FRAME_TAIL[FRAME_TAIL_SIZE] = {0x5B, 0xAB, 0xCA};     // 帧尾: 5B AB CA

/* RS485通信状态结构体 */
RS485_State rs485_sensor_1 = {0};
RS485_State rs485_sensor_2 = {0};

/* 定时器句柄，用于100Hz的数据采集 */
osTimerId_t rs485DataTimerId;


/*自动采集标志位*/
uint8_t RS4815_1_AUTO_SEND_DATA = 0;
uint8_t RS4815_2_AUTO_SEND_DATA = 0;

/**
 * @brief 初始化RS485主机通信
 */
void RS485_Master_Init(void) {
    // 初始化UART服务
    UART_Service_Init();

    // 初始化RS485状态
    rs485_sensor_1.huart = &huart2;
    rs485_sensor_1.uartState = &uart2_rx_state;
    rs485_sensor_1.sensorId = RS485_SENSOR_1;
    rs485_sensor_1.initialized = false;

    rs485_sensor_2.huart = &huart3;
    rs485_sensor_2.uartState = &uart3_rx_state;
    rs485_sensor_2.sensorId = RS485_SENSOR_2;
    rs485_sensor_2.initialized = false;

    // 创建定时器，用于100Hz的数据采集
    osTimerAttr_t timer_attr = {
        .name = "RS485DataTimer"
    };
    rs485DataTimerId = osTimerNew(RS485_DataAcquisitionCallback, osTimerPeriodic, NULL, &timer_attr);
}

/**
 * @brief 切换到RS485接收模式
 * @param state RS485状态结构体
 */
void RS485_EnableRxMode(RS485_State *state) {
    // 当CUBEMX中配置了硬件流控制(RS485)时，STM32会自动控制DE/RE引脚
    // 如果需要手动控制，可以取消注释下面的代码
    /*
    if (state->sensorId == RS485_SENSOR_1) {
        HAL_GPIO_WritePin(RS485_DE1_GPIO_Port, RS485_DE1_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(RS485_DE2_GPIO_Port, RS485_DE2_Pin, GPIO_PIN_RESET);
    }
    */
}

/**
 * @brief 切换到RS485发送模式
 * @param state RS485状态结构体
 */
void RS485_EnableTxMode(RS485_State *state) {
    // 当CUBEMX中配置了硬件流控制(RS485)时，STM32会自动控制DE/RE引脚
    // 如果需要手动控制，可以取消注释下面的代码
    /*
    if (state->sensorId == RS485_SENSOR_1) {
        HAL_GPIO_WritePin(RS485_DE1_GPIO_Port, RS485_DE1_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(RS485_DE2_GPIO_Port, RS485_DE2_Pin, GPIO_PIN_SET);
    }
    */
}

/**
 * @brief 计算校验和
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return 校验和
 */
uint8_t CalculateChecksum(uint8_t *data, uint16_t length) {
    uint8_t sum = 0;
    for (uint16_t i = 0; i < length; i++) {
        sum += data[i];
    }
    return sum;
}

/**
 * @brief 验证接收到的数据的校验和
 * @param buffer 数据缓冲区
 * @param headerPos 帧头位置
 * @param tailPos 帧尾位置
 * @return 校验和是否正确
 */
bool VerifyChecksum(uint8_t *buffer, uint16_t headerPos, uint16_t tailPos) {
    if (tailPos <= headerPos + FRAME_HEADER_SIZE + 1) return false;

    uint8_t receivedChecksum = buffer[tailPos - 1];
    uint8_t calculatedChecksum = CalculateChecksum(&buffer[headerPos], tailPos - headerPos - 1);

    return (receivedChecksum == calculatedChecksum);
}

/**
 * @brief 通过RS485发送数据
 * @param state RS485状态结构体
 * @param data 要发送的数据
 * @param size 数据大小
 */
void RS485_Send(RS485_State *state, uint8_t *data, uint16_t size) {
    // 切换到发送模式
    RS485_EnableTxMode(state);

    // 发送数据
    HAL_UART_Transmit(state->huart, data, size, 100);

    // 切换回接收模式
    RS485_EnableRxMode(state);

    // 重新启动接收中断
    HAL_UART_Receive_IT(state->huart, &state->uartState->RxBuffer[state->uartState->RxCount], 1);
}

/**
 * @brief 发送命令到从机
 * @param state RS485状态结构体
 * @param cmd 命令码
 * @return 发送是否成功
 */
bool RS485_SendCommand(RS485_State *state, uint8_t cmd) {
    uint8_t txBuffer[RS485_TX_BUFFER_SIZE];
    uint16_t txIndex = 0;

    // 添加帧头
    for (int i = 0; i < FRAME_HEADER_SIZE; i++) {
        txBuffer[txIndex++] = FRAME_HEADER[i];
    }

    // 添加从机地址
    txBuffer[txIndex++] = SLAVE_ADDRESS;

    // 添加命令码
    txBuffer[txIndex++] = cmd;

    // 计算校验和
    txBuffer[txIndex] = CalculateChecksum(txBuffer, txIndex);
    txIndex++;

    // 添加帧尾
    for (int i = 0; i < FRAME_TAIL_SIZE; i++) {
        txBuffer[txIndex++] = FRAME_TAIL[i];
    }

    // 清空接收计数器，准备接收响应
    state->uartState->frameReceived = 0;
    state->uartState->RxCount = 0;
    state->uartState->lastHeaderPos = 0;
    state->uartState->headerFound = 0;
    state->uartState->searchOffset = 0;


    // 发送命令
    RS485_Send(state, txBuffer, txIndex);

    // 清空接收计数器，准备接收响应
//    state->uartState->frameReceived = 0;
//    state->uartState->RxCount = 0;

    return true;
}

/**
 * @brief 等待从机响应
 * @param state RS485状态结构体
 * @param timeout 超时时间（毫秒）
 * @return 是否接收到响应
 */
bool RS485_WaitForResponse(RS485_State *state, uint32_t timeout) {
    uint32_t startTime = HAL_GetTick();

    while ((HAL_GetTick() - startTime) < timeout) {
        if (state->uartState->frameReceived) {
            return true;
        }
        osDelay(1);
    }

    return false;
}

/**
 * @brief 查找完整帧的位置
 * @param state UART接收状态结构体
 * @param headerPos 帧头位置的输出参数
 * @param tailPos 帧尾位置的输出参数
 * @return 是否找到完整帧
 */
bool FindCompleteFrame(UART_RxState *state, uint16_t *headerPos, uint16_t *tailPos) {
    // 查找帧头
    bool headerFound = false;
    *headerPos = 0;

    for (uint16_t i = 0; i <= state->RxCount - FRAME_HEADER_SIZE; i++) {
        if (IsFrameHeaderFound(state->RxBuffer, i, FRAME_HEADER, FRAME_HEADER_SIZE)) {
            headerFound = true;
            *headerPos = i;
            break;
        }
    }

    if (!headerFound) {
        return false;
    }

    // 查找帧尾
    bool tailFound = false;
    *tailPos = 0;

    for (uint16_t i = *headerPos + FRAME_HEADER_SIZE; i <= state->RxCount - FRAME_TAIL_SIZE; i++) {
        if (IsFrameTailFound(state->RxBuffer, i, FRAME_TAIL, FRAME_TAIL_SIZE)) {
            tailFound = true;
            *tailPos = i;
            break;
        }
    }

    return tailFound;
}

/**
 * @brief 处理接收到的响应
 * @param state RS485状态结构体
 */
void ProcessResponse(RS485_State *state) {
    UART_RxState *uartState = state->uartState;


    if (!uartState->frameReceived) {
        return;
    }

    uint16_t headerPos = 0;
    uint16_t tailPos = 0;

    if (!FindCompleteFrame(uartState, &headerPos, &tailPos)) {
        // 没有找到完整帧，重置接收状态
        ResetUartState(uartState);
        printf("NOT FindCompleteFrame\r\n");
        return;
    }

    // 验证校验和
    if (!VerifyChecksum(uartState->RxBuffer, headerPos, tailPos)) {
        // 校验和错误，重置接收状态
        ResetUartState(uartState);
        printf("NOT VerifyChecksum\r\n");
        return;
    }

    // 验证从机地址
    if (uartState->RxBuffer[headerPos + FRAME_HEADER_SIZE] != SLAVE_ADDRESS) {
        // 不是发给本从机的，重置接收状态
        ResetUartState(uartState);
        printf("NOT SLAVE_ADDRESS\r\n");
        return;
    }

    // 获取命令码
    uint8_t cmd = uartState->RxBuffer[headerPos + FRAME_HEADER_SIZE + 1]; // 命令码在地址后面
    uint16_t rxIndex = headerPos + FRAME_HEADER_SIZE + 2; // 数据开始索引

    // 处理不同命令的响应
    switch (cmd) {
        case CMD_QUERY_STATUS: {
            // 解析设备状态
            RS485_Sensor_DeviceStatus *deviceStatus;

            if (state->sensorId == RS485_SENSOR_1) {
                deviceStatus = &g_device_status.rs485_sensor_devicestatus_1;
            } else {
                deviceStatus = &g_device_status.rs485_sensor_devicestatus_2;
            }

            deviceStatus->imu_1_init = uartState->RxBuffer[rxIndex++];
            deviceStatus->imu_2_init = uartState->RxBuffer[rxIndex++];
            deviceStatus->mt6835_cs1_init = uartState->RxBuffer[rxIndex++];

            // 解析错误信息
            uint8_t errLen = uartState->RxBuffer[rxIndex++];
            if (errLen > 0) {
                memcpy(deviceStatus->error_message, &uartState->RxBuffer[rxIndex], errLen);
                deviceStatus->error_message[errLen] = '\0'; // 添加字符串结束符
            } else {
                deviceStatus->error_message[0] = '\0'; // 无错误信息
            }

            // 更新初始化状态
            state->initialized = deviceStatus->imu_1_init &&
                                deviceStatus->imu_2_init &&
                                deviceStatus->mt6835_cs1_init;

            break;
        }
        case CMD_GET_ANGLE: {

            // 解析所有传感器数据
            RS485_Sensor_Data *sensorData;

            if (state->sensorId == RS485_SENSOR_1) {
                sensorData = &g_system_state.rs485_sensor_data_1;
            } else {
                sensorData = &g_system_state.rs485_sensor_data_2;
            }

            // 获取互斥量
            lock_system_state();

            // 角度
            memcpy(&sensorData->encoder_data.angle, &uartState->RxBuffer[rxIndex], sizeof(float));

            // 释放互斥量
            unlock_system_state();

            break;

        }

        case CMD_GET_ALL_DATA: {
            // 解析所有传感器数据
            RS485_Sensor_Data *sensorData;

            if (state->sensorId == RS485_SENSOR_1) {
                sensorData = &g_system_state.rs485_sensor_data_1;
            } else {
                sensorData = &g_system_state.rs485_sensor_data_2;
            }

            // 获取互斥量
            lock_system_state();

            // 角度
            memcpy(&sensorData->encoder_data.angle, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);

            // IMU1数据
            // 加速度
            memcpy(&sensorData->imu_data_1.acc.x, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);
            memcpy(&sensorData->imu_data_1.acc.y, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);
            memcpy(&sensorData->imu_data_1.acc.z, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);

            // 角速度
            memcpy(&sensorData->imu_data_1.gyro.x, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);
            memcpy(&sensorData->imu_data_1.gyro.y, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);
            memcpy(&sensorData->imu_data_1.gyro.z, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);

            // IMU2数据
            // 加速度
            memcpy(&sensorData->imu_data_2.acc.x, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);
            memcpy(&sensorData->imu_data_2.acc.y, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);
            memcpy(&sensorData->imu_data_2.acc.z, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);

            // 角速度
            memcpy(&sensorData->imu_data_2.gyro.x, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);
            memcpy(&sensorData->imu_data_2.gyro.y, &uartState->RxBuffer[rxIndex], sizeof(float));
            rxIndex += sizeof(float);
            memcpy(&sensorData->imu_data_2.gyro.z, &uartState->RxBuffer[rxIndex], sizeof(float));

            // 设置数据就绪标志
            sensorData->imu_data_1.dataReady = true;
            sensorData->imu_data_2.dataReady = true;

            // 释放互斥量
            unlock_system_state();


//    		printf("G1:%.3f,%.3f,%.3f;A1:%.3f,%.3f,%.3f;T1:%.2f \t",
//    				g_system_state.rs485_sensor_data_1.imu_data_1.gyro.x, g_system_state.rs485_sensor_data_1.imu_data_1.gyro.y, g_system_state.rs485_sensor_data_1.imu_data_1.gyro.z,
//    				g_system_state.rs485_sensor_data_1.imu_data_1.acc.x, g_system_state.rs485_sensor_data_1.imu_data_1.acc.y, g_system_state.rs485_sensor_data_1.imu_data_1.acc.z,
//    				g_system_state.rs485_sensor_data_1.encoder_data.angle);
//
//    		printf("G2:%.3f,%.3f,%.3f;A2:%.3f,%.3f,%.3f;T2:%.2f\r\n",
//    				g_system_state.rs485_sensor_data_2.imu_data_1.gyro.x, g_system_state.rs485_sensor_data_2.imu_data_1.gyro.y, g_system_state.rs485_sensor_data_2.imu_data_1.gyro.z,
//    				g_system_state.rs485_sensor_data_2.imu_data_1.acc.x, g_system_state.rs485_sensor_data_2.imu_data_1.acc.y, g_system_state.rs485_sensor_data_2.imu_data_1.acc.z,
//    				g_system_state.rs485_sensor_data_2.encoder_data.angle);

            //printf("GET ALL %d\r\n",state->sensorId);

            break;
        }
        case CMD_GET_ALL_AUTO_START: {
        	 // 解析设备状态
			RS485_Sensor_DeviceStatus *deviceStatus;

			if (state->sensorId == RS485_SENSOR_1) {
				deviceStatus = &g_device_status.rs485_sensor_devicestatus_1;
			} else {
				deviceStatus = &g_device_status.rs485_sensor_devicestatus_2;
			}

			deviceStatus->imu_1_init = uartState->RxBuffer[rxIndex++];
			deviceStatus->imu_2_init = uartState->RxBuffer[rxIndex++];
			deviceStatus->mt6835_cs1_init = uartState->RxBuffer[rxIndex++];

			// 解析错误信息
			uint8_t errLen = uartState->RxBuffer[rxIndex++];
			if (errLen > 0) {
				memcpy(deviceStatus->error_message, &uartState->RxBuffer[rxIndex], errLen);
				deviceStatus->error_message[errLen] = '\0'; // 添加字符串结束符
			} else {
				deviceStatus->error_message[0] = '\0'; // 无错误信息
			}

			// 更新初始化状态
			state->initialized = deviceStatus->imu_1_init &&
								deviceStatus->imu_2_init &&
								deviceStatus->mt6835_cs1_init;

        	break;
        }

        case CMD_GET_ALL_AUTO_STOP: {
        	 // 解析设备状态
			RS485_Sensor_DeviceStatus *deviceStatus;

			if (state->sensorId == RS485_SENSOR_1) {
				deviceStatus = &g_device_status.rs485_sensor_devicestatus_1;
			} else {
				deviceStatus = &g_device_status.rs485_sensor_devicestatus_2;
			}

			deviceStatus->imu_1_init = uartState->RxBuffer[rxIndex++];
			deviceStatus->imu_2_init = uartState->RxBuffer[rxIndex++];
			deviceStatus->mt6835_cs1_init = uartState->RxBuffer[rxIndex++];

			// 解析错误信息
			uint8_t errLen = uartState->RxBuffer[rxIndex++];
			if (errLen > 0) {
				memcpy(deviceStatus->error_message, &uartState->RxBuffer[rxIndex], errLen);
				deviceStatus->error_message[errLen] = '\0'; // 添加字符串结束符
			} else {
				deviceStatus->error_message[0] = '\0'; // 无错误信息
			}

			// 更新初始化状态
			state->initialized = deviceStatus->imu_1_init &&
								deviceStatus->imu_2_init &&
								deviceStatus->mt6835_cs1_init;


        	break;
        }

        default:
            break;
    }

    // 重置接收状态，准备接收下一帧
    ResetUartState(uartState);
}

/**
 * @brief 查询设备状态
 * @param state RS485状态结构体
 * @return 查询是否成功
 */
bool RS485_QueryStatus(RS485_State *state) {
    RS485_SendCommand(state, CMD_QUERY_STATUS);

    if (RS485_WaitForResponse(state, 10)) {
        ProcessResponse(state);
        return true;
    }

    return false;
}

/**
 * @brief 获取所有传感器数据
 * @param state RS485状态结构体
 * @return 获取是否成功
 */
bool RS485_GetAllData(RS485_State *state) {
    RS485_SendCommand(state, CMD_GET_ALL_DATA);

    if (RS485_WaitForResponse(state, 4)) { // 缩短超时时间，保证100Hz的采集速率
        //printf("ProcessResponse\r\n");
    	ProcessResponse(state);
        return true;
    }
    printf("WaitForResponse Failed %d\r\n", state->sensorId);

    return false;
}



/**
 * @brief 初始化RS485传感器
 * @return 初始化是否成功
 */
bool RS485_InitializeSensors(void) {
    bool success = true;

    // 初始化RS485主机
    RS485_Master_Init();

    // 查询RS485传感器1状态
    if (!RS485_QueryStatus(&rs485_sensor_1)) {
        //sprintf(g_device_status.error_message, "RS485 sensor 1 not responding");
        success = false;
    }

    // 查询RS485传感器2状态
    if (!RS485_QueryStatus(&rs485_sensor_2)) {
        //sprintf(g_device_status.error_message, "RS485 sensor 2 not responding");
        success = false;
    }

    // 检查是否所有设备都已初始化
    if (success && rs485_sensor_1.initialized && rs485_sensor_2.initialized) {
    	printf("Start rs485DataTimerId\r\n");
        // 启动100Hz数据采集定时器
         osTimerStart(rs485DataTimerId, 25); // 10ms = 100Hz

    	//RS485_AUTO_SendDataStart(&rs485_sensor_1);
    	//RS485_AUTO_SendDataStart(&rs485_sensor_2);

        return true;
    }

    return false;
}

//int counttt = 100;
/**
 * @brief 100Hz数据采集回调函数
 * @param argument 回调参数
 */
void RS485_DataAcquisitionCallback(void *argument) {
    // 读取RS485传感器1数据
    RS485_GetAllData(&rs485_sensor_1);

    // 读取RS485传感器2数据
    RS485_GetAllData(&rs485_sensor_2);

//    counttt--;
//
//	if(counttt <=0)
//	{
//	    printf("RS485_DataAcquisitionCallback\r\n");
//
//	    counttt = 100;
//	}

}


/**
 * @brief 下位机自动发送数据开始
 * @param state RS485状态结构体
 * @return void
 */
void RS485_AUTO_SendDataStart(RS485_State *state) {
    RS485_SendCommand(state, CMD_GET_ALL_AUTO_START);
}

/**
 * @brief 下位机自动发送数据结束
 * @param state RS485状态结构体
 * @return void
 */
void RS485_AUTO_SendDataStop(RS485_State *state) {
    RS485_SendCommand(state, CMD_GET_ALL_AUTO_STOP);
}


/**
 * @brief 自动处理数据函数
 * @param argument 回调参数
 */
void RS485_DataAutoProcess() {
    // 读取RS485传感器1数据
	ProcessResponse(&rs485_sensor_1);

    // 读取RS485传感器2数据
	ProcessResponse(&rs485_sensor_2);

}
///**
// * @brief RS485主机任务函数
// * @param argument 任务参数
// */
//void RS485_Master_Task(void *argument) {
//    // 初始化RS485传感器
//    if (!RS485_InitializeSensors()) {
//        printf("RS485 sensors initialization failed\r\n");
//    } else {
//        printf("RS485 sensors initialized successfully\r\n");
//    }
//
//    // 任务主循环
//    for (;;) {
//    	 printf("RS485 For cir\r\n");
//        // 监控设备状态
//        if (!rs485_sensor_1.initialized || !rs485_sensor_2.initialized) {
//            // 如果设备未初始化，尝试重新初始化
//            RS485_QueryStatus(&rs485_sensor_1);
//            osDelay(100);
//            RS485_QueryStatus(&rs485_sensor_2);
//            printf("RS485 sensors re-initialized ing\r\n");
//
//            // 检查是否所有设备都已初始化
//            if (rs485_sensor_1.initialized && rs485_sensor_2.initialized) {
//                printf("RS485 sensors re-initialized successfully\r\n");
//
//                // 启动100Hz数据采集定时器
//                osTimerStart(rs485DataTimerId, 10); // 10ms = 100Hz
//            }
//        }
//
//        // 延时1秒
//        osDelay(1000);
//    }
//}
