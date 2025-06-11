#include "can_bsp.h"
#include "main.h"
#include <stdio.h>

/**
************************************************************************
* @brief:      	can_bsp_init(void)
* @param:       void
* @retval:     	void
* @details:    	CAN 使能
************************************************************************
**/
void can_bsp_init(void)
{
	can_filter_init();
	HAL_FDCAN_Start(&hfdcan1);                               //开启FDCAN
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}
/**
************************************************************************
* @brief:      	can_filter_init(void)
* @param:       void
* @retval:     	void
* @details:    	CAN滤波器初始化
************************************************************************
**/
void can_filter_init(void)
{
	FDCAN_FilterTypeDef fdcan_filter;
	
	fdcan_filter.IdType = FDCAN_STANDARD_ID;                       //标准ID
	fdcan_filter.FilterIndex = 0;                                  //滤波器索引
	fdcan_filter.FilterType = FDCAN_FILTER_RANGE;                  //滤波器类型
	fdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;           //过滤器0关联到FIFO0
	fdcan_filter.FilterID1 = 0x0580;                               //32位ID
	fdcan_filter.FilterID2 = 0x0583;                               //如果FDCAN配置为传统模式的话，这里是32位掩码
	if(HAL_FDCAN_ConfigFilter(&hfdcan1,&fdcan_filter)!=HAL_OK) 		 //滤波器初始化
	{
		Error_Handler();
	}
	//HAL_FDCAN_ConfigFifoWatermark(&hfdcan1, FDCAN_RX_FIFO0, 1);
}
/**
************************************************************************
* @brief:      	fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len)
* @param:       hfdcan：FDCAN句柄
* @param:       id：CAN设备ID
* @param:       data：发送的数据
* @param:       len：发送的数据长度
* @retval:     	void
* @details:    	发送数据
************************************************************************
**/
uint8_t fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len)
{	
	FDCAN_TxHeaderTypeDef TxHeader;
	
  TxHeader.Identifier = id;
  TxHeader.IdType = FDCAN_STANDARD_ID;																// 标准ID
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;														// 数据帧
  TxHeader.DataLength = len << 16;																		// 发送数据长度
	
	if(len<=8)
		TxHeader.DataLength = len;
	if(len==12)
		TxHeader.DataLength = FDCAN_DLC_BYTES_12;
	if(len==16)
		TxHeader.DataLength = FDCAN_DLC_BYTES_16;
	if(len==20)
		TxHeader.DataLength = FDCAN_DLC_BYTES_20;
	if(len==24)
		TxHeader.DataLength = FDCAN_DLC_BYTES_24;
	if(len==32)
		TxHeader.DataLength = FDCAN_DLC_BYTES_32;
	if(len==48)
		TxHeader.DataLength = FDCAN_DLC_BYTES_48;
	if(len==64)
		TxHeader.DataLength = FDCAN_DLC_BYTES_64;
	
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;										// 设置错误状态指示
  TxHeader.BitRateSwitch = FDCAN_BRS_ON;															// 开启可变波特率
  TxHeader.FDFormat = FDCAN_FD_CAN;															// 普通CAN格式
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;										// 用于发送事件FIFO控制, 不存储
  TxHeader.MessageMarker = 0x00; 			// 用于复制到TX EVENT FIFO的消息Maker来识别消息状态，范围0到0xFF
    
  if(HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, data)!=HAL_OK){
	  printf("Send Failed!\r\n");
	  return 1;//发送
  }
	return 0;	
}
/**
************************************************************************
* @brief:      	fdcanx_receive(FDCAN_HandleTypeDef *hfdcan, uint8_t *buf)
* @param:       hfdcan：FDCAN句柄
* @param:       buf：接收数据缓存
* @retval:     	接收的数据长度
* @details:    	接收数据
************************************************************************
**/
uint8_t fdcanx_receive(FDCAN_HandleTypeDef *hfdcan, uint16_t *rec_id, uint8_t *buf)
{    
    FDCAN_RxHeaderTypeDef pRxHeader;
    uint8_t len = 0;
    
    // 检查FIFO0中是否有消息
    if(HAL_FDCAN_GetRxFifoFillLevel(hfdcan, FDCAN_RX_FIFO0) > 0)
    {
        if(HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &pRxHeader, buf) == HAL_OK)
        {
            *rec_id = pRxHeader.Identifier;
            
            // 根据数据长度代码转换为实际字节数
            if(pRxHeader.DataLength <= FDCAN_DLC_BYTES_8)
                len = pRxHeader.DataLength;
            else if(pRxHeader.DataLength == FDCAN_DLC_BYTES_12)
                len = 12;
            else if(pRxHeader.DataLength == FDCAN_DLC_BYTES_16)
                len = 16;
            else if(pRxHeader.DataLength == FDCAN_DLC_BYTES_20)
                len = 20;
            else if(pRxHeader.DataLength == FDCAN_DLC_BYTES_24)
                len = 24;
            else if(pRxHeader.DataLength == FDCAN_DLC_BYTES_32)
                len = 32;
            else if(pRxHeader.DataLength == FDCAN_DLC_BYTES_48)
                len = 48;
            else if(pRxHeader.DataLength == FDCAN_DLC_BYTES_64)
                len = 64;
            
            return len; // 返回接收到的数据长度
        }
    }
    return 0; // 没有接收到数据
}


/**
* @brief 清空FDCAN接收FIFO
* @param hfdcan FDCAN句柄
*/
void fdcan_clear_rx_fifo(FDCAN_HandleTypeDef *hfdcan)
{
    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[64];  // 最大可能的FDCAN数据长度
    
    // 循环读取FIFO中的所有消息，直到清空
    while(HAL_FDCAN_GetRxFifoFillLevel(hfdcan, FDCAN_RX_FIFO0) > 0)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData);
        HAL_Delay(1);  // 短暂延时，避免过于频繁的硬件访问
    }
}


/**
************************************************************************
* @brief:      	HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
* @param:       hfdcan；FDCAN句柄
* @param:       RxFifo0ITs：中断标志位
* @retval:     	void
* @details:    	HAL库的FDCAN中断回调函数
************************************************************************
**/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
		if(hfdcan == &hfdcan1)
		{
			fdcan1_rx_callback();
		}
	}
}

/**
* @brief 关闭FDCAN接收中断
* @param hfdcan FDCAN句柄
*/
void fdcan_disable_interrupt(FDCAN_HandleTypeDef *hfdcan)
{
    HAL_FDCAN_DeactivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
}

/**
* @brief 重新启用FDCAN接收中断
* @param hfdcan FDCAN句柄
*/
void fdcan_enable_interrupt(FDCAN_HandleTypeDef *hfdcan)
{
    HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}




/**
************************************************************************
* @brief:      	fdcan_rx_callback(void)
* @param:       void
* @retval:     	void
* @details:    	供用户调用的接收弱函数
************************************************************************
**/

uint8_t rx_data1[8] = {0};
uint16_t rec_id1;

void fdcan1_rx_callback(void)
{
	uint8_t res;
	fdcanx_receive(&hfdcan1, &rec_id1, rx_data1);
//	if(res == 0){
//		usb_printf("接收失败\r\n");
//	}
//	else{
//		print_rx_data_with_message("接收成功", rx_data1, res);
//	}
	//print_rx_data_with_message("测试", rx_data1, res);
}




/* jihuo caozuo mode*/
void can_run(void)
{
	uint8_t canbufcanrun[]={0x01,0x00};
	uint8_t res = fdcanx_send_data(&hfdcan1, 0X00, canbufcanrun, 2);
}

/* jihuo lunkuo caozuo mode */
uint8_t can_run_outline_speed(void)
{
	uint16_t res=1;
	uint8_t canbufrunotlspd[]={0x2F,0x60,0x60,0x00,0x03};
	res = fdcanx_send_data(&hfdcan1, 0X601, canbufrunotlspd, 5);
//	res = can_normal_command(0X601, canbufrunotlspd,5,buf);
	if (res==0)
	{
		printf("can run otl spd\r\n");
		return 1;
	}
	return 0;
}

/* set lunkuo speed */
uint8_t can_set_outline_speed(int speed)
{
	uint16_t res=1;
	uint8_t canbufotlspd[]={0x23,0x81,0x60,0x00,0x00,0x00,0x00,0x00};
	canbufotlspd[7] = (uint8_t)((speed >> 24) & 0xFF); 
	canbufotlspd[6] = (uint8_t)((speed >> 16) & 0xFF);  
	canbufotlspd[5] = (uint8_t)((speed >> 8) & 0xFF);  
	canbufotlspd[4] = (uint8_t)(speed & 0xFF);
	res = fdcanx_send_data(&hfdcan1,0X601, canbufotlspd, 8);
	if (res==0)
	{
		printf("can set otl spd\r\n");
		return 1;
	}
	return 0;
}

/* unenable */
uint8_t can_exit(void)
{
	uint16_t res=1;
	uint8_t canbufexit[]={0x2B,0x40,0x60,0x00,0x06,0x00};
	res= fdcanx_send_data(&hfdcan1,0X601, canbufexit, 6);
	if (res==0)
	{
		printf("can exit\r\n");
		return 1;
	}
	return 0;
}
/* enable */
uint8_t can_enable(void)
{
	uint16_t res=1;
	uint8_t canbufenable[]={0x2B,0x40,0x60,0x00,0x0F,0x00};
	res= fdcanx_send_data(&hfdcan1, 0X601, canbufenable, 6);
	if (res==0)
	{
		printf("can enable\r\n");
		return 1;
	}
	return 0;
}
/*set position*/
uint8_t can_set_position(int position)
{
	uint16_t res=1;
	uint8_t canbufpst[]={0x23,0xFF,0x60,0x00,0x00,0x00,0x00,0x00};
	canbufpst[7] = (uint8_t)((position >> 24) & 0xFF); 
	canbufpst[6] = (uint8_t)((position >> 16) & 0xFF);  
	canbufpst[5] = (uint8_t)((position >> 8) & 0xFF);  
	canbufpst[4] = (uint8_t)(position & 0xFF);
	res= fdcanx_send_data(&hfdcan1, 0X601, canbufpst, 8);
	if (res==0)
	{
		printf("can set position\r\n");
		return 1;
	}
	return 0;
}
/* run */
uint8_t can_run_imm(void)
{
	uint16_t res=1;
	uint8_t canbufrun[]={0x2B,0x40,0x60,0x00,0x0F,0x00};
	res= fdcanx_send_data(&hfdcan1, 0X601, canbufrun, 6);
	if (res==0)
	{
		printf("can run_imm\r\n");
		return 1;
	}
	return 0;
}
uint8_t can_read_pos(void)
{
	uint16_t res=1;
	uint8_t canbufreadpos[]={0x40,0xD3,0x30,0x01};
	res=fdcanx_send_data(&hfdcan1, 0X601, canbufreadpos, 4);
	if (res==0)
	{
		printf("can read_pos\r\n");
		return 1;
	}
	return 0;
}

//int can_recv_pos(void)
//{
//	uint16_t res=1;
//	int position=0;
//	uint8_t read_position[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//	res=fdcanx_receive(&hfdcan1, read_position);
//	if (res==0X581 && read_position[0]==0x43 && read_position[1]==0x64 && read_position[2]==0x60 && read_position[3]==0x00)
//	{
//		position=(int)(read_position[4] & 0xFF)+(int)(read_position[5] & 0xFF)*256+(int)(read_position[6] & 0xFF)*256*256+(int)(read_position[6] & 0xFF)*256*256*256;
//		printf("can read_recv_pos\r\n");
//		return position;
//	}

//	return 0;
//}


