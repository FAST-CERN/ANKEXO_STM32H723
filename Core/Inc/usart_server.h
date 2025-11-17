#ifndef __USART_SERVER_H__
#define __USART_SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h"
#include <stdbool.h>

// Buffer size for UART reception
#define UART_RX_BUFFER_SIZE 1024

// UART reception state structure
typedef struct {
    uint8_t RxBuffer[UART_RX_BUFFER_SIZE];
    uint16_t RxCount;
    volatile uint8_t frameReceived;
    bool headerFound;
    uint16_t lastHeaderPos;
    uint16_t searchOffset;  // Optimization to avoid re-searching previously checked data
} UART_RxState;

// State structures for different UARTs
extern UART_RxState uart2_rx_state;  // RS485 sensor 1
extern UART_RxState uart3_rx_state;  // RS485 sensor 2
extern UART_RxState uart6_rx_state;  // Additional UART service
extern UART_RxState uart10_rx_state; // Additional UART service

// Function declarations
void UART_Service_Init(void);
bool IsFrameHeaderFound(uint8_t *buffer, uint16_t startPos, const uint8_t* frameHeader, uint16_t headerSize);
bool IsFrameTailFound(uint8_t *buffer, uint16_t startPos, const uint8_t* frameTail, uint16_t tailSize);
uint8_t ProcessUartData(UART_RxState *state, const uint8_t* frameHeader, uint16_t headerSize,
                        const uint8_t* frameTail, uint16_t tailSize);
void ResetUartState(UART_RxState *state);

#ifdef __cplusplus
}
#endif

#endif // __USART_SERVER_H__
