# Ectoskeleton 外骨骼控制系统固件

## 一、项目简介

本项目基于 STM32H723VGTX 微控制器，面向外骨骼机器人控制应用，集成多电机驱动、IMU/传感器采集、通信协议、运动控制算法等功能。系统采用模块化设计，支持多任务并发、实时数据采集与控制，适用于康复、助力等外骨骼场景。

---

## 二、硬件环境

- **主控芯片**：STM32H723VGTX（ARM Cortex-M7，480MHz）
- **电机驱动**：多路伺服/步进电机
- **传感器**：IMU（如MPU6050/ICM20602）、编码器、力矩传感器等
- **通信接口**：CAN、RS485、UART、USB、BLE（可扩展）
- **其他**：LED指示、按键、蜂鸣器等

---

## 三、软件架构

### 1. 目录结构

```
ectoskeleton/
├── Core/           // 主工程代码
│   ├── Inc/        // 头文件
│   └── Src/        // 源文件
├── Drivers/        // STM32 HAL驱动
├── HardWare/       // 硬件抽象层（电机、IMU、通信等）
├── APP/            // 应用层（协议、控制算法、系统管理等）
├── Middlewares/    // 中间件（如FreeRTOS、USB等）
├── Startup/        // 启动文件
├── Debug/          // 调试输出
├── makefile        // GCC Makefile
├── ectoskeleton.ioc // CubeMX 工程文件
└── ...
```

### 2. 层次结构

- **硬件抽象层（HAL/HardWare）**：封装底层外设驱动，提供统一接口
- **中间件（Middlewares）**：如RTOS、USB协议栈等
- **应用层（APP）**：运动控制、协议解析、系统管理等
- **主循环/任务调度**：系统初始化、主循环或多任务调度

---

## 四、主要功能模块

### 1. 电机控制

- 多路电机独立驱动
- 支持速度、位置、力矩等多种控制模式
- 加减速参数设置
- 位置归零、限位保护
- 运动状态实时反馈

### 2. 传感器采集

- IMU数据采集与滤波
- 编码器/力矩传感器数据读取
- 多通道数据同步
- 数据校准与异常检测

### 3. 通信协议

- 支持RS485、CAN、UART、USB等多种通信方式
- 自定义协议帧格式，支持命令/数据分离
- CRC校验、异常处理
- 支持主从/广播模式

### 4. 控制算法

- 步态生成与切换
- 运动轨迹规划
- 力矩/速度/位置闭环控制
- 测试与自检功能

### 5. 系统管理

- LED/蜂鸣器状态指示
- 参数存储与恢复
- 错误检测与日志输出
- OTA/固件升级（可扩展）

---

## 五、接口说明

### 1. 电机控制接口（`HardWare/Motor.c/h`）

```c
void Motor_Init(void);                              // 电机初始化
void Motor_SetSpeed(uint8_t id, float speed);       // 设置电机速度
void Motor_SetPosition(uint8_t id, float pos);      // 设置电机位置
void Motor_SetTorque(uint8_t id, float torque);     // 设置电机力矩
void Motor_Stop(uint8_t id);                        // 停止电机
void Motor_Zero(uint8_t id);                        // 电机归零
```

### 2. IMU/传感器接口（`HardWare/IMU.c/h`）

```c
void IMU_Init(void);                                // IMU初始化
void IMU_ReadRaw(int16_t *acc, int16_t *gyro, int16_t *temp); // 读取原始数据
void IMU_GetData(float *acc, float *gyro, float *temp);       // 获取校准后数据
```

### 3. 通信接口（`HardWare/RS485.c/h`, `HardWare/CAN.c/h`）

```c
void RS485_Init(void);                              // RS485初始化
void RS485_Send(uint8_t *data, uint16_t len);       // 发送数据
uint16_t RS485_Receive(uint8_t *buf, uint16_t maxlen); // 接收数据
void CAN_Init(void);                                // CAN初始化
void CAN_Send(uint32_t id, uint8_t *data, uint8_t len); // CAN发送
```

### 4. 协议封装与解析（`APP/Protocol.c/h`）

```c
uint16_t Protocol_PackFrame(uint8_t *buf, uint8_t type, const uint8_t *data, uint16_t len); // 打包数据帧
int Protocol_ParseFrame(const uint8_t *buf, uint16_t len, ProtocolFrame_t *frame);          // 解析数据帧
void Protocol_HandleCommand(const ProtocolFrame_t *frame);                                  // 命令处理
```

### 5. 系统管理与调试（`APP/Sys.c/h`）

```c
void LED_Set(uint8_t id, uint8_t state);             // LED控制
void Debug_Printf(const char *fmt, ...);             // 串口调试输出
void System_Reset(void);                             // 系统复位
```

---

## 六、功能流程与逻辑

1. **系统上电初始化**
   - 时钟、外设、RTOS、各模块初始化
   - 读取参数，LED状态指示

2. **主循环/任务调度**
   - 定时采集传感器数据
   - 运动控制与状态更新
   - 通信协议收发与命令处理
   - 错误检测与系统管理

3. **异常与错误处理**
   - 外设异常检测与恢复
   - 通信超时/校验错误处理
   - 系统状态通过LED/串口输出

---

## 七、通信协议示例

| 字节 | 内容         | 说明           |
|------|--------------|----------------|
| 0    | 帧头(0xAA)   | 固定           |
| 1    | 帧类型       | 数据/命令      |
| 2~N  | 数据内容     | 依类型而定     |
| N+1  | CRC 校验     | 1字节          |

- 典型命令：
    - 0x01：读取状态
    - 0x02：设置参数
    - 0x03：启动/停止运动
    - 0x04：系统复位

---

## 八、使用方法

1. 使用 STM32CubeIDE 打开 `.ioc` 工程文件，生成代码并编译下载
2. 连接电机、传感器、通信线缆
3. 上电后，系统自动初始化并进入主循环
4. 通过上位机/手机APP发送协议命令，进行参数配置与控制
5. 通过串口/调试口查看系统日志和状态

---

## 九、常见问题与调试

- **电机无响应**：检查供电与驱动、参数设置
- **传感器无数据**：检查硬件连接与初始化流程
- **通信异常**：检查波特率、协议格式、收发方向
- **系统死机/复位**：检查电源、异常处理逻辑

---

## 十、联系方式

如需技术支持或有建议，请联系开发团队。

---

**版权所有 © 2024**