#include "epos.h"
#include "main.h"
#include "sdo.h"
#include "cmsis_os.h"
/*
*输入：EPOS结构体，串口对象fd1.
*功能：启动EPOS，并且设置最大的跟踪误差，最大的速度、加速度、负加速度，以及快速停止的负加速度设定
*/
void Epos_ParamInit(Epos* epos)
{
    //SDO_Read(epos,OD_STATUS_WORD,0x00);                                 //Fault Status=0x0108  红灯闪烁

    SDO_Write(epos, OD_CTRL_WORD, 0x00, Fault_Reset);      //Fault_Reset command 控制字设置为0x80 第7位置1，参考固件手册 Fault reset figure3-3 事件15 驱动初始化完成

     //SDO_Read(epos,OD_STATUS_WORD,0x00);                               //Switch On    Status=0x0540/0140   绿灯闪烁

//    SDO_Write(epos, OD_Following_ERR_window, 0x00, MAX_F_ERR);     //最大误差设置

//    SDO_Write(epos, OD_MAX_P_VELOCITY, 0x00, MAX_P_V);   //最大速度

//    SDO_Write(epos, OD_P_ACCELERATION, 0x00, epos->acc); //加速度

//    SDO_Write(epos, OD_P_DECELERATION, 0x00, epos->dec); //负加速度

//    SDO_Write(epos, OD_QS_DECELERATION, 0x00, QDEC);     //快速停止负加速度

    //SDO_Write(epos,OD_CAN_BITRATE,0x00,0x00);              //set value = 0. set CAN bitrate 1M/s. 

    printf("Epos_ParamInit \r\n");
}


/*
*输入：EPOS结构体，CAN设备(节点)序号 CAN_ID，节点的ID：NODE_ID1，串口对象 fd1
*功能：启动EPOS，并且设置最大的跟踪误差，最大的速度、加速度、负加速度，以及快速停止的负加速度设定
*/
void Epos_Init(Epos* epos, Uint8 CAN_ID, Uint8 NODE_ID)
{
    // EPOS结构体初始化
    epos->b_init = 0;
    epos->CAN_Handles = NULL;
    epos->node_ID = NODE_ID;               //节点ID设置
    
    epos->b_init = 1;                      //EPOS 初始化标志
    epos->cur_mode = MODE_NONE;            //控制模式
    epos->opt = 0x001f;
    epos->acc = MAX_ACC;                   //最大加速度
    epos->dec = MAX_DEC;                   //最大负加速度
    epos->b_need_shutdown = 0;             //是否需要关闭控制器

    // 通过canopen设定EPOS控制器参数
    Epos_ParamInit(epos);

    printf("Epos_SInit\r\n");
}



/****** 设置EPOS工作模式 *****/
void Epos_setMode(Epos* epos, Uint16 mode){

    epos->cur_mode = mode;
    
    SDO_Write(epos,OP_MODE,0x00,mode); 
                
    switch(mode){
        
        case(CSP):
                //SDO_Write(epos,OD_MOTOR_DATA,0x01,15000);//nominal电流0-15000
                //SDO_Write(epos, OD_MOTOR_DATA, 0x05,0);  //扭矩常数 0-10 000 000
				
                //SDO_Write(epos, Current_control_parameter , 0x01, 1171880);//P                   
                //SDO_Write(epos, Current_control_parameter, 0x02, 3906250);  //I  
				
                //SDO_Write(epos,Pos_control_para,0x01,1500000);//P                  
                //SDO_Write(epos, Pos_control_para, 0x02,7800000);  //I              
                //SDO_Write(epos, Pos_control_para , 0x03, 16000);//D      
                //SDO_Write(epos, Pos_control_para, 0x04, 0);  //Position controller FF velocity gain		
                //SDO_Write(epos, Pos_control_para, 0x05, 0);  //Position controller FF acceleration gain
				
				
                SDO_Write(epos, OD_P_DECELERATION, 0x00,3000); 
                SDO_Write(epos, OD_QS_DECELERATION , 0x00, 5000);
				
				SDO_Write(epos, OD_Following_ERR_window , 0x00, 0);
				
                //SDO_Write(epos, Soft_P_Limit, 0x01, 0x80000000);                //-2147483648
                //SDO_Write(epos, Soft_P_Limit, 0x02, 0x7FFFFFFF);                //2147483647
				
                break;
								
		case(CST):
				SDO_Write(epos, Torque_offset, 0x00,0);
				break;
        
        case(Velocity_Mode):		//CONFIGURATION PARAMETERS
                //SDO_Write(epos,OP_MODE,0x00,Velocity_Mode); 
                SDO_Write(epos,OD_Max_Acceleration,0x00,1000);                                                  // set Max Acceleration
                SDO_Write(epos, OD_MAX_P_VELOCITY, 0x00, MAX_P_V);                                            // Maximal Profile Velocity 
                break;
                
        case(Current_Mode):
                SDO_Write(epos,OD_MOTOR_DATA,0x01,6000);         // set Continuous Current Limit 连续输出电流最大值 mA
                SDO_Write(epos, OD_MOTOR_DATA, 0x02, 12000);     //输出最大电流，推荐为两倍连续最大电流
                SDO_Write(epos, OD_MOTOR_DATA, 0x03, 1);        //Changes only in “Disable” state.Number of magnetic pole pairs 参考电机手册
                SDO_Write(epos, OD_MOTOR_DATA, 0x04, 12000);     //限制电机最大速度 rpm
                SDO_Write(epos, OD_MOTOR_DATA, 0x05, 1);         // thermal time constant
                break;
                
        case(Profile_Position_Mode):	//CONFIGURATION PARAMETERS
                //SDO_Write(epos, Soft_P_Limit, 0x01, 0x80000000);                //-2147483648
                //SDO_Write(epos, Soft_P_Limit, 0x02, 0x7FFFFFFF);                //2147483647
//                SDO_Write(epos, OD_MAX_P_VELOCITY, 0x00, MAX_P_V);              // Maximal Profile Velocity 
//								SDO_Write(epos, OD_MAX_MOTOR_SPEED, 0x00, 5000);								//参考电机手册
//								SDO_Write(epos, Max_gear_input_speed, 0x03,1000);
//                SDO_Write(epos, OD_QS_DECELERATION, 0x00, QDEC);                //快速停止负加速度
//                SDO_Write(epos,OD_Max_Acceleration,0x00,10000);   
                break;
                
        case(Profile_Velocity_Mode):		//CONFIGURATION PARAMETERS
                //SDO_Write(epos, Soft_P_Limit, 0x01, 0x80000000);                //-2147483648
                //SDO_Write(epos, Soft_P_Limit, 0x02, 0x7FFFFFFF);                //2147483647
//                SDO_Write(epos, OD_MAX_P_VELOCITY, 0x00,12000);                 //最大速度 Maximal Profile Velocity
//	              SDO_Write(epos, OD_QS_DECELERATION, 0x00, 50000);              //快速停止负加速度
//                SDO_Write(epos, OD_MAX_MOTOR_SPEED, 0x00, 7000);              // Maximal Profile Velocity 
//				        //SDO_Write(epos, Max_gear_input_speed, 0x03,7580);
//                SDO_Write(epos,OD_Max_Acceleration,0x00,10000);   
				
								break;
                
        case(Homing_Mode):      //可以将该功能配置给某个数字口
                //SDO_Write(epos, OD_Motion_Profile_Type, 0x00,1);                 //sin2  ramp 
                break;
                
        case(Interpolated_Position_Mode):
                SDO_Write(epos, OD_Interpolation_Sub_Mode, 0x00,(Uint32)(-1));//always -1 cubic spline interpolation (PVT)
                SDO_Write(epos, Interpolation_Time_Period, 0x01,1);//always 1
                SDO_Write(epos, Interpolation_Time_Period, 0x02,(Uint32)(-3));//always -3  插值周期 10^-3s
                SDO_Write(epos, Soft_P_Limit, 0x01, 0x80000000);                //-2147483648
                SDO_Write(epos, Soft_P_Limit, 0x02, 0x7FFFFFFF);                //2147483647
                //SDO_Write(epos,OD_Position_Window, 0x00,4294967295);             //关闭 position window
                SDO_Write(epos, Pos_Window_Time, 0x00, 0);
                break;
                
        default: 
                printf("\r\nerror mode\r\n");
        
    }
}

    
void Epos_OperEn(Epos* epos){
    SDO_Write(epos,OD_CTRL_WORD,0x00,0x06);                    // Shut down  驱动函数失能
    //osDelay(1);

    //SDO_Read(epos,OD_STATUS_WORD,0x00);                                                // Ready to Switch On    Status=0x0121   绿灯闪烁
    
    SDO_Write(epos,OD_CTRL_WORD,0x00,0x0F);                    // Switch on AND Enable Operation 驱动参数设定
    //osDelay(1);
    
    //SDO_Read(epos,OD_STATUS_WORD,0x00);                                                // Operation Enable      Status=0x0137   绿灯常亮
}


void Epos_ClorEn(Epos* epos){
	SDO_Write(epos,OD_CTRL_WORD,0x00,0x06);                    // Shut down  驱动函数失能
    //osDelay(1);
}


void Epos_Delay(Uint32 time){
    osDelay(time);
}

