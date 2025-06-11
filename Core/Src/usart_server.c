#include "usart_server.h"
#include "usart.h"
#include "global_param.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "S485_master.h"
#include "ble_server.h"

// State structures for different UARTs
UART_RxState uart2_rx_state = {0};  // RS485 sensor 1
UART_RxState uart3_rx_state = {0};  // RS485 sensor 2
UART_RxState uart6_rx_state = {0};  // Additional UART service
UART_RxState uart10_rx_state = {0}; // Additional UART service



/**
 * @brief Initialize UART service for all UART peripherals
 */
void UART_Service_Init(void) {
    // Initialize UART2 (RS485 sensor 1)
    uart2_rx_state.RxCount = 0;
    uart2_rx_state.frameReceived = 0;
    uart2_rx_state.headerFound = false;
    uart2_rx_state.lastHeaderPos = 0;
    uart2_rx_state.searchOffset = 0;
    memset(uart2_rx_state.RxBuffer, 0, UART_RX_BUFFER_SIZE);

    // Initialize UART3 (RS485 sensor 2)
    uart3_rx_state.RxCount = 0;
    uart3_rx_state.frameReceived = 0;
    uart3_rx_state.headerFound = false;
    uart3_rx_state.lastHeaderPos = 0;
    uart3_rx_state.searchOffset = 0;
    memset(uart3_rx_state.RxBuffer, 0, UART_RX_BUFFER_SIZE);

    // Initialize UART6 (if needed)
    uart6_rx_state.RxCount = 0;
    uart6_rx_state.frameReceived = 0;
    uart6_rx_state.headerFound = false;
    uart6_rx_state.lastHeaderPos = 0;
    uart6_rx_state.searchOffset = 0;
    memset(uart6_rx_state.RxBuffer, 0, UART_RX_BUFFER_SIZE);

    // Initialize UART10 (if needed)
    uart10_rx_state.RxCount = 0;
    uart10_rx_state.frameReceived = 0;
    uart10_rx_state.headerFound = false;
    uart10_rx_state.lastHeaderPos = 0;
    uart10_rx_state.searchOffset = 0;
    memset(uart10_rx_state.RxBuffer, 0, UART_RX_BUFFER_SIZE);

    // Start initial receive on required UARTs
    HAL_UART_Receive_IT(&huart2, &uart2_rx_state.RxBuffer[0], 1);
    HAL_UART_Receive_IT(&huart3, &uart3_rx_state.RxBuffer[0], 1);

    // Enable these if you need them
    // HAL_UART_Receive_IT(&huart6, &uart6_rx_state.RxBuffer[0], 1);
    // HAL_UART_Receive_IT(&huart10, &uart10_rx_state.RxBuffer[0], 1);
}

/**
 * @brief Check if frame header is found
 * @param buffer Data buffer
 * @param startPos Start position
 * @param frameHeader Frame header pattern
 * @param headerSize Size of frame header
 * @return true if frame header found
 */
bool IsFrameHeaderFound(uint8_t *buffer, uint16_t startPos, const uint8_t* frameHeader, uint16_t headerSize) {
    for (uint16_t i = 0; i < headerSize; i++) {
        if (buffer[startPos + i] != frameHeader[i]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Check if frame tail is found
 * @param buffer Data buffer
 * @param startPos Start position
 * @param frameTail Frame tail pattern
 * @param tailSize Size of frame tail
 * @return true if frame tail found
 */
bool IsFrameTailFound(uint8_t *buffer, uint16_t startPos, const uint8_t* frameTail, uint16_t tailSize) {
    for (uint16_t i = 0; i < tailSize; i++) {
        if (buffer[startPos + i] != frameTail[i]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Process received UART data - optimized version
 * @param state UART_RxState structure
 * @param frameHeader Frame header pattern
 * @param headerSize Size of frame header
 * @param frameTail Frame tail pattern
 * @param tailSize Size of frame tail
 * @return 1 if complete frame received, 0 otherwise
 */
uint8_t ProcessUartData(UART_RxState *state, const uint8_t* frameHeader, uint16_t headerSize,
                       const uint8_t* frameTail, uint16_t tailSize) {
    uint8_t result = 0;

    // 1. 如果已经找到帧头，直接检查帧尾
    if (state->headerFound) {
        // 只有当收到足够的数据才检查帧尾
        if (state->RxCount >= state->lastHeaderPos + headerSize + tailSize) {
            // 只检查最新收到的字节是否可能形成帧尾
            uint16_t checkPos = state->RxCount - tailSize;

            // 如果新字节可能是帧尾的一部分，检查是否匹配
            if (state->RxBuffer[state->RxCount-1] == frameTail[tailSize-1]) {
                // 验证完整帧尾
                if (IsFrameTailFound(state->RxBuffer, checkPos, frameTail, tailSize)) {
                    state->frameReceived = 1;
                    result = 1;
                    //printf("frameReceived\r\n");
                }
            }
        }
    }
    // 2. 否则，尝试查找帧头，但只在可能的位置
    else {
        // 只检查最新收到的字节是否可能形成帧头
        if (state->RxCount >= headerSize) {
            uint16_t checkPos = state->RxCount - headerSize;

            // 如果新字节与帧头最后一个字节匹配，检查是否是完整帧头
            if (state->RxBuffer[state->RxCount-1] == frameHeader[headerSize-1]) {
                if (IsFrameHeaderFound(state->RxBuffer, checkPos, frameHeader, headerSize)) {
                    state->headerFound = true;
                    state->lastHeaderPos = checkPos;
                    //printf("HeadFound\r\n");
                }
            }
        }
    }

    // 3. 处理缓冲区溢出保护 (当接近缓冲区上限时)
    if (state->RxCount >= UART_RX_BUFFER_SIZE - 10) {
        if (state->headerFound && !state->frameReceived) {
            // 找到帧头但没有帧尾，保留从帧头开始的数据
            uint16_t moveSize = state->RxCount - state->lastHeaderPos;

            if (moveSize < UART_RX_BUFFER_SIZE / 2) { // 只在数据量不太大时移动
                memmove(state->RxBuffer, &state->RxBuffer[state->lastHeaderPos], moveSize);
                state->RxCount = moveSize;
                state->lastHeaderPos = 0;
            } else {
                // 数据量太大，可能不是有效帧，重置
                state->RxCount = 0;
                state->headerFound = false;
            }
        } else if (!state->headerFound) {
            // 没有找到帧头，直接清空缓冲区
            state->RxCount = 0;
        }
    }

    return result;
}
/**
 * @brief Reset UART state after processing a frame
 * @param state UART_RxState structure
 */
void ResetUartState(UART_RxState *state) {
    // If we processed a frame, reset the state
    if (state->frameReceived) {
        state->RxCount = 0;
        state->frameReceived = 0;
        state->headerFound = false;
        state->searchOffset = 0;
    }
}

/**
 * @brief UART receive interrupt callback
 * @param huart UART handle
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    UART_RxState *state = NULL;
    extern const uint8_t FRAME_HEADER[];
    extern const uint8_t FRAME_TAIL[];

    // Determine which UART triggered the callback
    if (huart->Instance == USART2) {
        state = &uart2_rx_state;
    } else if (huart->Instance == USART3) {
        state = &uart3_rx_state;
    } else if (huart->Instance == USART6) {
        //state = &uart6_rx_state;
        // Handle USART6 specific processing here if needed
    	ble_command_progress();
    	UART6_StartReceive();
    } else if (huart->Instance == USART10) {
        state = &uart10_rx_state;
        // Handle USART10 specific processing here if needed
    } else {
        // Unknown UART, exit
        return;
    }

    // Increment received data count
    state->RxCount++;

    // Process received data for RS485 sensors
    if (huart->Instance == USART2 || huart->Instance == USART3) {

        ProcessUartData(state, FRAME_HEADER, FRAME_HEADER_SIZE, FRAME_TAIL, FRAME_TAIL_SIZE);
    }
    // For USART6 and USART10, you can add specific processing here

    // Continue receiving
    if (state->RxCount < UART_RX_BUFFER_SIZE) {
        HAL_UART_Receive_IT(huart, &state->RxBuffer[state->RxCount], 1);
    } else {
        // Buffer full, reset and restart
        state->RxCount = 0;
        state->headerFound = false;
        state->searchOffset = 0;
        HAL_UART_Receive_IT(huart, &state->RxBuffer[0], 1);
    }
}
