#include "epos.h"
#include "sdo.h"
#include "main.h"
#include "fdcan.h"
#include "can_bsp.h"
#include "cmsis_os.h"

/***** 
write parameter to Object Dictionary through CAN use SDO messages
******/

int NEST = 0;           //嵌套层数

void SDO_Write(Epos* epos, Uint16 Index, Uint8 SubIndex, Uint32 param)
{
    int n;
    uint8_t TxData[8] = {0};
    uint8_t RxData[8] = {0};
    uint16_t rec_id = 0;
    
    //printf("Send %d-%X-%X\r\n", epos->node_ID, Index, SubIndex);
    
    // 检查嵌套层数，防止无限递归
    if(++NEST < 6) {
        //printf("nest = %d\r\n", NEST);
        
        // 关闭FDCAN接收中断
        fdcan_disable_interrupt(&hfdcan1);
			
		// 清空FIFO0邮箱，防止旧消息干扰
        fdcan_clear_rx_fifo(&hfdcan1);
        
        // 准备发送数据
        uint32_t low_4byte = 0x22 + ((Uint32)Index << 8) + ((Uint32)SubIndex << 24);
        
        // 填充数据数组
        TxData[0] = (low_4byte) & 0xFF;
        TxData[1] = (low_4byte >> 8) & 0xFF;
        TxData[2] = (low_4byte >> 16) & 0xFF;
        TxData[3] = (low_4byte >> 24) & 0xFF;
        TxData[4] = (param) & 0xFF;
        TxData[5] = (param >> 8) & 0xFF;
        TxData[6] = (param >> 16) & 0xFF;
        TxData[7] = (param >> 24) & 0xFF;
        
        // 发送CAN消息
        uint16_t msgId = 0x600 + epos->node_ID;
        

        fdcanx_send_data(&hfdcan1, msgId, TxData, 8);


        // 等待接收回复
        n = 5;
        uint8_t res = 0;
        
        while(n-- > 0) {
            // 直接调用接收函数，使用轮询模式
            res = fdcanx_receive(&hfdcan1, &rec_id, RxData);
            
            if(res > 0 && rec_id == (0x580 + epos->node_ID)) {
                // 检查接收到的消息类型
                switch(RxData[0]) {
                    case 0x60:
                        // 检查索引和子索引是否匹配
                        if((Index == (((Uint16)RxData[2] << 8) + RxData[1])) && 
                           (SubIndex == RxData[3])) {
                            //printf("WoK\r\n");
                            
                            // 重新启用FDCAN接收中断
                            fdcan_enable_interrupt(&hfdcan1);
                            
                            NEST = 0;
                            return;  // 成功接收匹配的应答
                        } else {
                            // 重新启用FDCAN接收中断
                            fdcan_enable_interrupt(&hfdcan1);
                            
                            SDO_Write(epos, Index, SubIndex, param);
                            return;
                        }
                        
                    case 0x80:
                        // 错误应答
                        printf("Error code: %02X%02X%02X%02X\r\n",
                               RxData[7], RxData[6], RxData[5], RxData[4]);
                        
                        // 重新启用FDCAN接收中断
                        fdcan_enable_interrupt(&hfdcan1);
                        
                        SDO_Write(epos, Index, SubIndex, param);
                        return;
                        
                    default:
                        // 重新启用FDCAN接收中断
                        fdcan_enable_interrupt(&hfdcan1);
                        
                        SDO_Write(epos, Index, SubIndex, param);
                        return;
                }
            }
            osDelay(2);  // 延时等待接收
        }
        
        // 如果超时未收到消息
        if(n == -1) {
            // 重新启用FDCAN接收中断
            fdcan_enable_interrupt(&hfdcan1);
            
            SDO_Write(epos, Index, SubIndex, param);
            printf("No Received\r\n");
            return;
        }
    } else {
        printf("\r\nwrong--%d-%X-%X \r\n", epos->node_ID, Index, SubIndex);
        printf("Too many retries\r\n");
    }
    
    // 确保在所有情况下都重新启用中断
    fdcan_enable_interrupt(&hfdcan1);
    
    NEST = 0;
}



/**** 发送CAN SDO读报文，并接受返回值 ***/
/**** 发送CAN SDO读报文，并接受返回值 ***/
Uint32 SDO_Read(Epos* epos, Uint16 Index, Uint8 SubIndex)
{
    int n;
    Uint32 data = 0;
    uint8_t TxData[8] = {0};
    uint8_t RxData[8] = {0};
    uint16_t rec_id = 0;
    
    if(++NEST < 6) {
        // 关闭FDCAN接收中断
        fdcan_disable_interrupt(&hfdcan1);
			
			  // 清空FIFO0邮箱，防止旧消息干扰
        fdcan_clear_rx_fifo(&hfdcan1);
        
        // 准备发送数据
        uint32_t low_4byte = 0x40 + ((Uint32)Index << 8) + ((Uint32)SubIndex << 24);
        
        // 填充数据数组
        TxData[0] = (low_4byte) & 0xFF;
        TxData[1] = (low_4byte >> 8) & 0xFF;
        TxData[2] = (low_4byte >> 16) & 0xFF;
        TxData[3] = (low_4byte >> 24) & 0xFF;
        TxData[4] = 0;
        TxData[5] = 0;
        TxData[6] = 0;
        TxData[7] = 0;
        
        // 发送CAN消息
        uint16_t msgId = 0x600 + epos->node_ID;
        fdcanx_send_data(&hfdcan1, msgId, TxData, 8);
        
        // 等待接收回复
        n = 5;
        uint8_t res = 0;
        
        while(n-- > 0) {
            // 直接调用接收函数，使用轮询模式
            res = fdcanx_receive(&hfdcan1, &rec_id, RxData);
            
            if(res > 0 && rec_id == (0x580 + epos->node_ID)) {
                switch(RxData[0]) {
                    case 0x4F:  // 1字节数据
                    case 0x4B:  // 2字节数据
                    case 0x47:  // 3字节数据
                    case 0x43:  // 4字节数据
                        if(Index == (((Uint16)RxData[2] << 8) + RxData[1]) && 
                           SubIndex == RxData[3]) {
                            data = ((Uint32)RxData[7] << 24) + 
                                   ((Uint32)RxData[6] << 16) + 
                                   (RxData[5] << 8) + 
                                    RxData[4];
                            
                            // 重新启用FDCAN接收中断
                            fdcan_enable_interrupt(&hfdcan1);
                            
                            NEST = 0;
                            return data;
                        } else {
                            // 重新启用FDCAN接收中断
                            fdcan_enable_interrupt(&hfdcan1);
                            
                            data = SDO_Read(epos, Index, SubIndex);
                            return data;
                        }
                        
                    case 0x80:  // 错误应答
                        printf("Error code: %02X%02X%02X%02X\r\n",
                               RxData[7], RxData[6], RxData[5], RxData[4]);
                        
                        // 重新启用FDCAN接收中断
                        fdcan_enable_interrupt(&hfdcan1);
                        
                        data = SDO_Read(epos, Index, SubIndex);
                        return data;
                        
                    default:
                        printf("Unexpected response: %02X\r\n", RxData[0]);
                        
                        // 重新启用FDCAN接收中断
                        fdcan_enable_interrupt(&hfdcan1);
                        
                        data = SDO_Read(epos, Index, SubIndex);
                        return data;
                }
            }
            osDelay(2);  // 延时等待接收
        }
        
        // 如果超时未收到消息
        if(n == -1) {
            printf("Read timeout for index %04X:%02X\r\n", Index, SubIndex);
            
            // 重新启用FDCAN接收中断
            fdcan_enable_interrupt(&hfdcan1);
            
            data = SDO_Read(epos, Index, SubIndex);
            return data;
        }
    }
    
    if(NEST == 6) {
        printf("wrong read--%X-%X\r\n", Index, SubIndex);
        printf("Too many retries\r\n");
        
        // 确保在达到最大重试次数时也重新启用中断
        fdcan_enable_interrupt(&hfdcan1);
        
        NEST = 0;
        return 0;
    }
    
    // 确保在所有情况下都重新启用中断
    fdcan_enable_interrupt(&hfdcan1);
    
    NEST = 0;
    return data;
}
