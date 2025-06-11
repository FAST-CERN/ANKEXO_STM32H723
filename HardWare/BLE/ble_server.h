#ifndef __BLE_SERVER_H__
#define __BLE_SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


typedef enum
{
    BLE_DISCONNECTED = 0,
    BLE_CONNECTED,
    BLE_NOTIFICATION_ENABLED,
    BLE_NOTIFICATION_DISABLED
} BLE_Status_t;

#define BLE_RX_BUFFER_SIZE 256
#define BLE_TX_BUFFER_SIZE 256


void ble_command_progress();
void UART6_SendData(const char *prefix, const char *data);
void UART6_StartReceive(void);


#ifdef __cplusplus
}
#endif

#endif

