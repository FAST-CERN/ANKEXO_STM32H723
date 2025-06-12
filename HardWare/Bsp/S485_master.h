#ifndef __S485_MASTER_H__
#define __S485_MASTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h"
#include "usart_server.h"
#include <stdbool.h>
#include "cmsis_os.h"

extern osTimerId_t rs485DataTimerId;

extern uint8_t RS4815_1_AUTO_SEND_DATA;
extern uint8_t RS4815_2_AUTO_SEND_DATA;

/* 定义RS485通信相关宏 */
#define RS485_TX_BUFFER_SIZE     256

/* 定义命令码 */
#define CMD_QUERY_STATUS         0x01    // 查询设备状态
#define CMD_GET_ANGLE            0x02    // 获取角度传感器数据
#define CMD_GET_IMU1_QUAT        0x03    // 获取IMU1四元数
#define CMD_GET_IMU1_ACCEL       0x04    // 获取IMU1加速度
#define CMD_GET_IMU1_GYRO        0x05    // 获取IMU1角速度
#define CMD_GET_IMU2_QUAT        0x06    // 获取IMU2四元数
#define CMD_GET_IMU2_ACCEL       0x07    // 获取IMU2加速度
#define CMD_GET_IMU2_GYRO        0x08    // 获取IMU2角速度
#define CMD_GET_IMU1_ALL         0x09    // 获取IMU1全部数据
#define CMD_GET_IMU2_ALL         0x0A    // 获取IMU2全部数据
#define CMD_GET_ALL_DATA         0x0B    // 获取所有传感器数据
#define CMD_GET_ALL_AUTO_START   0x0C    // 查询设备状态
#define CMD_GET_ALL_AUTO_STOP    0x0D    // 查询设备状态

/* 定义通信协议帧格式 - 3字节帧头和帧尾 */
#define FRAME_HEADER_SIZE        3       // 帧头大小
#define FRAME_TAIL_SIZE          3       // 帧尾大小
#define SLAVE_ADDRESS            0x01    // 从机地址

/* 传感器ID */
typedef enum {
    RS485_SENSOR_1 = 0,
    RS485_SENSOR_2 = 1
} RS485_SensorId;

/* RS485状态结构体 */
typedef struct {
    UART_HandleTypeDef *huart;      // UART句柄
    UART_RxState *uartState;        // UART接收状态
    RS485_SensorId sensorId;        // 传感器ID
    bool initialized;               // 是否初始化完成
} RS485_State;

/* 帧头和帧尾 */
extern const uint8_t FRAME_HEADER[FRAME_HEADER_SIZE];
extern const uint8_t FRAME_TAIL[FRAME_TAIL_SIZE];

/* RS485状态 */
extern RS485_State rs485_sensor_1;
extern RS485_State rs485_sensor_2;

/* 函数声明 */
void RS485_Master_Init(void);
void RS485_EnableRxMode(RS485_State *state);
void RS485_EnableTxMode(RS485_State *state);
void RS485_Send(RS485_State *state, uint8_t *data, uint16_t size);
bool RS485_SendCommand(RS485_State *state, uint8_t cmd);
bool RS485_WaitForResponse(RS485_State *state, uint32_t timeout);
void ProcessResponse(RS485_State *state);
uint8_t CalculateChecksum(uint8_t *data, uint16_t length);
bool VerifyChecksum(uint8_t *buffer, uint16_t headerPos, uint16_t tailPos);
bool FindCompleteFrame(UART_RxState *state, uint16_t *headerPos, uint16_t *tailPos);
bool RS485_QueryStatus(RS485_State *state);
bool RS485_GetAllData(RS485_State *state);
bool RS485_InitializeSensors(void);
void RS485_DataAcquisitionCallback(void *argument);
void RS485_Master_Task(void *argument);


void RS485_DataAutoProcess();
void RS485_AUTO_SendDataStop(RS485_State *state) ;
void RS485_AUTO_SendDataStart(RS485_State *state) ;



#ifdef __cplusplus
}
#endif

#endif // __S485_MASTER_H__
