#ifndef __CAN_BSP_H__
#define __CAN_BSP_H__
#include "main.h"
#include "fdcan.h"
#define hcan_t FDCAN_HandleTypeDef


void can_bsp_init(void);
void can_filter_init(void);
uint8_t fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len);
uint8_t fdcanx_receive(hcan_t *hfdcan, uint16_t *rec_id, uint8_t *buf);
void fdcan1_rx_callback(void);
void fdcan2_rx_callback(void);
void fdcan3_rx_callback(void);


void fdcan_disable_interrupt(FDCAN_HandleTypeDef *hfdcan);
void fdcan_enable_interrupt(FDCAN_HandleTypeDef *hfdcan);	
void fdcan_clear_rx_fifo(FDCAN_HandleTypeDef *hfdcan);

extern uint8_t rx_data1[8];
extern uint16_t rec_id1;
extern uint8_t rx_data2[8];
extern uint16_t rec_id2;
extern uint8_t rx_data3[8];
extern uint16_t rec_id3;


void can_run(void);
uint8_t can_run_outline_speed(void);
uint8_t can_set_outline_speed(int speed);
uint8_t can_exit(void);
uint8_t can_enable(void);
uint8_t can_set_position(int position);
uint8_t can_run_imm(void);
uint8_t can_read_pos(void);
int can_recv_pos(void);

#endif /* __CAN_BSP_H_ */

