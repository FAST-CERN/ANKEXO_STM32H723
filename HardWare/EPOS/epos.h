#ifndef EPOS_H
#define EPOS_H

#define SDO

#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"



//demand value ֻ�Ǳ�ʾ�������������ֵ��ֻ�Ǿ����滮���滮���ѣ�������ʵ�ʵ���ֵ


//------------------------------------------------------------------------
//define variables  �����������
//------------------------------------------------------------------------

typedef char                    int8;
typedef int                     int16;
typedef long                    int32;
typedef uint8_t                 Uint8;
typedef uint16_t                Uint16;
typedef uint32_t                Uint32;
typedef uint8_t                 BOOL;
typedef uint16_t                stcCAN;

#define NOT_USED 0

extern  Uint32  epos_err_LK;
extern  Uint32  epos_err_LH;
extern  Uint32  epos_err_RK;
extern  Uint32  epos_err_RH;

extern Uint32   buffer_size_LK;    
extern Uint32   buffer_size_LH;
extern Uint32   buffer_size_RK;
extern Uint32   buffer_size_RH;

extern Uint32   status_LK;          //0Ϊδʹ�ܣ�1ΪPPM

extern Uint32 pos_value_LK;
extern Uint32 pos_value_LH;
extern Uint32 pos_value_RK;
extern Uint32 pos_value_RH;

#define NODE_LK (1) /* epos 24/5�Ľڵ�ID */
#define NODE_LH (2) /* epos 70/10�Ľڵ�ID */
#define NODE_RK (3) /* epos 24/5�Ľڵ�ID */
#define NODE_RH (4) /* epos 70/10�Ľڵ�ID */

/************����*************/

#define PPMODE 0x01
#define MAX_ACC 10000
#define MAX_DEC 10000
#define MODE_NONE 0x00
#define MAX_F_ERR 5000
#define MAX_P_V 7000//4000
#define QDEC 100000


#define ALL 0

//ģʽ
enum E_OBJ_MODE
{
    OP_MODE = 0x6060,           //Modes of operation, Sub-index 0x00, INTEGER8   ����ģʽ���� 1PPM 3PVM 6HMM 8CSP 9CSV 10CST
	  OP_MODE_Read = 0x6061,      //Modes of operation display, Subindex 0x00 ,INTEGER8  RO
    
    CSP = (uint8_t)8,
        //in
        Target_pos = 0x607A,
	      Position_offset = 0x60B0,
	     
        //out
				Pos_Actual_Value = 0x6064,     //��ȡʵ��λ�ã������õ�λΪ��λ
				Velocity_actual_value = 0x606C,//��ȡʵ���ٶȣ������õ�λΪ��λ
				Torque_actual_value = 0x6077,  //��ȡʵ��Ť�أ��Ե��ÿǧ�Ť��Ϊ��λ
	
	  CST = (uint8_t)10,
				
				//in
				Target_torque = 0x6071,
				Torque_offset = 0x60B2,
	
				//out
        
    Velocity_Mode = 0xFE,
        //in
        VM_SET_VALUE = 0x206B,
        //out
        Velocity_Demand_Value = 0x606B,

    Current_Mode  = 0xFD,
        //in
        CM_SET_VALUE = 0x2030,
        //out
        Current_Demand_Value = 0x606B,

    Diagnostic_Mode = 0xFC,
    Master_Encoder_Mode = 0xFB,
    Step_Direction_Mode = 0xFA,
    
    Interpolated_Position_Mode = (uint8_t)7,
        //IN
        //Controlword = 0x6040,
        Interpolation_Data_Record = 0x20C1, //a FIFO ����ǰ������������������ݵ㣬�����޷��滮�켣�����ʹ��PDO�������ݴ���
        
        //OUT
        //Statusword = 0x6041, //�Ƿ�λ
        Interpolation_Buffer = 0x20C4,  //FIFO ������ ��״̬
        //Pos_Demand_Value = 0x6062,
    
    Homing_Mode = (uint8_t)6,
        //IN
        //Controlword = 0x6040,
        Homing_Method = 0x6098,
        Homing_Speeds = 0x6099,
        Homing_Acceleration = 0x609A,
        Home_Offset = 0x607C,
        Current_Threshold = 0x2080,
        Home_Pos = 0x2081,
				
				Home_SetNowPos = 0x30B0,
				Home_SetNowOffSet = 0x30B1,
				
        //OUT
        //Statusword = 0x6041,          //�Ƿ�λ
    
    Profile_Velocity_Mode = (uint8_t)3,//Ŀ��λ�����ٶ�
        //IN
        //Controlword = 0x6040,
        Target_Velocity = 0x60FF,          //Ŀ��λ�õ��ٶ�
        //Profile_Acceleration = 0x6083,  //max Acceleration
        //Profile_Deceleration = 0x6084,  //min Deceleration
        //Motion_Profile_Type = 0x6086,
        
        //OUT
        //Velocity_Demand_Value = 0x606B,
        //Statusword = 0x6041,
        
    Profile_Position_Mode = (uint8_t)(1),//Ŀ��λ��Ϊ��
        //in 
        //Controlword = 0x6040,         //��������趨�ȹ���
        Target_Position = 0x607A,
        Profile_Velocity = 0x6081,      //�滮·��������ٶ�
        Profile_Acceleration = 0x6083,  //�滮·��max Acceleration
        Profile_Deceleration = 0x6084,  //min Deceleration
        Motion_Profile_Type = 0x6086
        
        //out
        //Pos_Demand_Value = 0x6062,
        //Statusword = 0x6041,
};



// E_OBJ_DICTIONARY ���͵�ö�٣�������define. �ο��ֲ�� EPOS2 Firmware Specification.pdf P110
enum E_OBJ_DICTIONARY
{
    Fault_Reset = 0x0080,   //���ô���
    
    OD_ERR_REG = 0x1001,    //error register, Index 0x1001, Sub-index 0x00, UNSIGNED8

    OD_ERR_HIS = 0x1003,    //error history, Index 0x1003,  number of entries 0x05
        SI_ERR_CNT = 0x00,  //number of errors
        SI_ERR_HIS1 = 0x01, //error history [1],Index 0x1003,Sub-index 0x01, UNSIGNED32
        SI_ERR_HIS2 = 0x02, //error history [2],Index 0x1003,Sub-index 0x02, UNSIGNED32
        SI_ERR_HIS3 = 0x03, //error history [3],Index 0x1003,Sub-index 0x03, UNSIGNED32
        SI_ERR_HIS4 = 0x04, //error history [4],Index 0x1003,Sub-index 0x04, UNSIGNED32
        SI_ERR_HIS5 = 0x05, //error history [5],Index 0x1003,Sub-index 0x05, UNSIGNED32

    OD_STORE = 0x1010,      //Save all Parameters, Subindex 0x01, Type UNSIGNED32
        SAVE_Code = 0x73617665,

    OD_CAN_BITRATE = 0x2001, //Subindex 0x00, Type UNSIGNED16

    OD_CURRENT_AVG = 0x2027, //Current Actual Value Averaged, Subindex 0x00,Type INTEGER16

    OD_CM_SET = 0x2030, //Setting value of current regulator in current mode [mA], Sub-index 0x00, INTEGER16

    OD_ANALOG_IN = 0x207C,//analog inputs, number of entries 2
        SI_ANALOG1 = 0x01,  //analog input 1, Index 0x207C, Sub-index 0x01,Type INTEGER16, analog input 1 [mV].
        SI_ANALOG2 = 0x02,  //analog input 2, Index 0x207C, Sub-index 0x02,Type INTEGER16,The voltage measured at analog input 2 [mV].

    Max_gear_input_speed = 0x3003,//Subindex 0x03, UNSIGNED32
		
		OD_CTRL_WORD = 0x6040,      /*controlword, UNSIGNED16,
                                  bit6 Operation mode specific: 0: Abs;  1: rel
                                 */
    OD_STATUS_WORD = 0x6041, //Statusword, Sub-index 0x00, UNSIGNED16

    OD_ACTUAL_POS = 0x6064,     //actual position, Sub-index 0x00, INTEGER32,

    OD_Following_ERR_window = 0x6065,   //Following error window, Sub-index 0x00, UNSIGNED32
		
    OD_Position_Window = 0x6067,//ʵ������ο� firmware pdf
    Pos_Window_Time = 0x6068,   //����Position_Window��ʱ�䣬��λms
    
    
    OD_CURRENT_VAL = 0x6078,    //Current Actual Value, Index 0x6078, Subindex 0x00,Type INTEGER16
    OD_TARGET_POS = 0x607A,     /*target position, Sub-index 0x00, INTEGER32,
                                  units steps (quadcounts = 4*Encoder Counts / Revolution)*/
                                  
    Soft_P_Limit = 0x607D,  //�������
    
    OD_MAX_P_VELOCITY = 0x607F, //UNSIGNED32, [1, 25000]

	  OD_MAX_MOTOR_SPEED = 0x6080, //0x00, UNSIGNED32,

    OD_P_VELOCITY = 0x6081,     //Profile velocity, Sub-index 0x00, UNSIGNED32, Unit:rpm
    OD_P_ACCELERATION = 0x6083, //profile acceleration, Sub-index 0x00,UNSIGNED32
    OD_P_DECELERATION = 0x6084, //profile deceleration, Sub-index 0x00,UNSIGNED32
    OD_QS_DECELERATION =  0x6085, //quick stop deceleration, Sub-index 0x00, UINT32
    
    OD_Motion_Profile_Type = 0x6086,
    
    OD_Interpolation_Sub_Mode = 0x60C0,
    Interpolation_Time_Period = 0x60C2,
    Interpolation_Data_Configuration = 0x60C4,
    
    OD_Max_Acceleration = 0x60C5,

    OD_TARGET_VELOCITY = 0x60FF,  //target velocity, Sub-index 0x00, INTEGER32
    OD_MOTOR_DATA = 0x3001  //motor data, number of entries 0x06,���庬�����firmware
};


#define EPOS_DELAY1  5          //5ms
#define EPOS_DELAY2  50
#define EPOS_DELAY3  62.5


// CAN ����
struct  CANMSGID_BITS {        // bits  description
   Uint16      EXTMSGID_L:16;  // 0:15
   Uint16      EXTMSGID_H:2;   // 16:17
   Uint16      STDMSGID:11;    // 18:28
   Uint16      AAM:1;          // 29, The auto answer mode bit
   Uint16      AME:1;          // 30, The acceptance mask enable bit
   Uint16      IDE:1;          // 31, The identifier extension bit

};

/* Allow access to the bit fields or entire register */
union CANMSGID_REG {
   Uint32                all;
   struct CANMSGID_BITS  bit;
};


typedef struct CANFrame_t
{
    union CANMSGID_REG  msg_id;
    Uint32  low_4byte;
    Uint32  high_4byte;
    Uint8   DataLen;
    BOOL    IsExtend;
    BOOL    IsRemote;
}stcCANFrame;


//EPOS����������
typedef struct __Epos
{
    stcCAN* CAN_Handles;//EPOS
    
    stcCANFrame buf;        //EPOSһ֡���ݵĻ�����
    
    BOOL b_init;                //Epos�Ƿ��ʼ���ı�־
    
    Uint8 node_ID;          //CANOPEN��������EPOS��������ID
    Uint8 cur_mode;         //EPOS�Ĺ���ģʽ
    
    int32 opt;              //EPOS���ڱ���PPMODE�µ�opt����
    int32 acc;                  //EPOS�ļ��ٶ�
    int32 dec;                  //EPOS�ĸ����ٶ�
    
    BOOL b_need_shutdown;   //�Ƿ���Ҫ�ض�EPOS�Եı�־
    
}Epos;



//------------------------------------------------------------------------
//define functions
//------------------------------------------------------------------------


       
//EPOS

//extern BOOL Epos_start_Read(Epos* epos,Uint16 Index,Uint8 SubIndex, int fd1);

//extern BOOL Epos_end_Read(Epos* epos, Uint32 *p_param, int fd1);

extern void Epos_Init(Epos* epos1, Uint8 CAN_ID, Uint8 NODE_ID);

extern void Epos_ParamInit(Epos* epos);

extern void Epos_Delay(Uint32 time);

//extern Uint32 changeIntOrder(Uint32 param);

void Epos_setMode(Epos* epos, Uint16 mode);        //�趨�������Ĺ���ģʽ

void Epos_OperEn(Epos* epos);                   //�� Switch On Disable �� Operation Enable


void Epos_ClorEn(Epos* epos);

//Control Funtional
void PM_SetAngle(Epos* epos, Uint32 angle);//Postion Mode
//extern void Epos_Write_PVT(Epos* epos, Uint16 Index, Uint8 SubIndex, Uint32 param1, Uint32 param2, Uint8 param3);
//------------------------------------------------------------------------
#endif // EPOS_H
