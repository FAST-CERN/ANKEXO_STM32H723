#include "HW_epos.h"
#include "sdo.h"
#include "epos.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "global_param.h"

/*
 * �����������մ���������
 * ����  ��
 * ����  ��
 */
Uint8 NODE_ID1 = 1;                          //EPOS�Ľڵ�ID
Uint8 NODE_ID2 = 2;
Epos Controller1,Controller2;        				 //����������


void Epos_INIT_Controller1()
{
    Epos_Init(&Controller1, NOT_USED, NODE_ID1);	//��ʼ�������ٶȣ��ٶȣ�������������1M/s��
    printf("\r\nInitial EPOS done Controller1!\r\n\r\n");
    osDelay(500);
    
    //******** ����ģʽ���� *******
    //Epos_setMode(&Controller1, Profile_Velocity_Mode);
    //Epos_setMode(&Controller1, Profile_Position_Mode);
    //osDelay(500);

    //printf("\r\n Mode set \r\n");
    
    //******** ʹ��EPOS *******
    Epos_OperEn(&Controller1);                                               //Switch On Disable to Operation Enable
    printf("\r\n Enable EPOS Controller1 \r\n\r\n");

    g_device_status.motor_devicestatus_1 = success;
    // �ͷ��ź���֪ͨLCD����״̬�и���
    osSemaphoreRelease(initSemaphore);
}




void Epos_INIT_Controller2()
{
    Epos_Init(&Controller2, NOT_USED, NODE_ID2);	//��ʼ�������ٶȣ��ٶȣ�������������1M/s��
    printf("\r\nInitial EPOS done Controller2!\r\n\r\n");
    osDelay(500);

    //******** ����ģʽ���� *******
    //Epos_setMode(&Controller1, Profile_Velocity_Mode);
    //Epos_setMode(&Controller1, Profile_Position_Mode);
    //osDelay(500);
    //printf("\r\n Mode set \r\n");

    //******** ʹ��EPOS *******
    Epos_OperEn(&Controller2);                                               //Switch On Disable to Operation Enable
    printf("\r\n Enable EPOS Controller2 \r\n\r\n");
    g_device_status.motor_devicestatus_2 = success;
    osSemaphoreRelease(initSemaphore);
}


/** �ٶ�ģʽ���ٶ����� */
void Epos_SpeedSet(Epos* epos, Uint32 speed){
	
	SDO_Write(epos,Target_Velocity,0x00,speed);
	SDO_Write(epos,OD_CTRL_WORD ,0x00,0x0F);
}


/** λ��ģʽ��λ������ */
/**************Position Mode*********************************/
void PM_SetAngle(Epos* epos, Uint32 angle){
    
    #if defined SDO
    SDO_Write(epos, Target_pos, 0x00, angle);
    #endif 
}

/** Position Set */
void Epos_PosSet(Epos* epos, Uint32 pos){
	
	SDO_Write(epos,OD_CTRL_WORD ,0x00,0x0F);
	PM_SetAngle(epos,pos);
	SDO_Write(epos,OD_CTRL_WORD ,0x00,0x7F);
	
}

/** ѭ������Ť��ģʽCST */
void Epos_CSTSet(Epos* epos, Uint32 torque, Uint32 offset){
	
	SDO_Write(epos,Torque_offset ,0x00,offset);
	SDO_Write(epos,Target_torque ,0x00,torque);
	SDO_Write(epos,OD_CTRL_WORD ,0x00,0x0F);
}


/** ������λ�� */
void Epos_HMMSetNowZero(Epos* epos, Uint32 pos, Uint32 offset)
{
		printf("����0λ��");
	
		Epos_setMode(epos, Homing_Mode);
	
		SDO_Write(epos,Home_SetNowPos ,0x00,pos);	
		SDO_Write(epos,Home_SetNowOffSet ,0x00,offset);	
	
		osDelay(1);
	
		SDO_Write(epos,Homing_Method ,0x00,37); //ACTUAL Position
	
		Epos_OperEn(&Controller1);

		osDelay(1);
	
		SDO_Write(epos,OD_CTRL_WORD ,0x00,0x001F); //ACTUAL Position
	
		osDelay(1);
	
		Epos_ClorEn(epos);

}

/**���������� */
void Epos_Start(void){
	
    //******** EPOS basic COMMANDING PARAMETERS *******
	SDO_Write(&Controller1, Profile_Acceleration,0x00,4000);
	SDO_Write(&Controller1, Profile_Deceleration,0x00,4000);
	SDO_Write(&Controller1, Motion_Profile_Type,0x00,0);
	
	SDO_Write(&Controller1, OD_P_VELOCITY ,0x00, 50);

	osDelay(2000);
	printf("\r\n EPOS control beginning!\r\n\r\n");

}


/*
 * ��������ʵʱ��������
 * ����  ��
 * ����  ��
 */
/**ʵ���ٶ�ҡ�ڿ��� */
void speed_Task(Epos control){
	
	Uint32 speed = 1000;
	
	Epos_SpeedSet(&control, speed);
	osDelay(500);
	
	Epos_SpeedSet(&control, -speed);
	osDelay(500);
	
	Epos_SpeedSet(&control, speed*5);
	osDelay(500);

	Epos_SpeedSet(&control, -speed*5);
	osDelay(500);
	
	
	Epos_SpeedSet(&control, speed*3);
	osDelay(500);
	
	Epos_SpeedSet(&control, -speed*3);
	osDelay(500);
	
	Epos_SpeedSet(&control, 0);
}


/** position task ҡ��*/
void pos_Task(Epos control){
	
	Uint32 pos = 200000,i;
	
	for(i=0;i<5;i++){
		Epos_PosSet(&control ,pos*i);
		osDelay(1000);
	}
	
	for(i=0;i<5;i++){
		Epos_PosSet(&control,-pos*i);
		osDelay(1000);
	}
}


void cst_Task(void)
{
	Epos_setMode(&Controller1, CST);
	
	Epos_OperEn(&Controller1);  
	
	Epos_CSTSet(&Controller1, 100, 0);
	
	osDelay(2000);
	
	Epos_CSTSet(&Controller1, -100, 0);
	
	osDelay(2000);
	
	Epos_ClorEn(&Controller1);
	
}

void Epos_ControlTask(void){
	
	//pos_Task();
	
		//******** ����ģʽ���� *******
//	printf("Epos_ControlTask Controller1");
//
//	Epos_setMode(&Controller1, Profile_Velocity_Mode);
//
//	speed_Task(Controller1);
//
//	Epos_setMode(&Controller1, Profile_Position_Mode);
//
//	pos_Task(Controller1);
//
//	Epos_ClorEn(&Controller1);


	printf("Epos_ControlTask Controller2");

	Epos_setMode(&Controller2, Profile_Velocity_Mode);

	speed_Task(Controller2);

	Epos_setMode(&Controller2, Profile_Position_Mode);

	pos_Task(Controller2);

	Epos_ClorEn(&Controller2);
}


/*
 * �����������մ���������
 * ����  ��
 * ����  ��
 */
Uint32 status;
Uint32 velocity, speed;
void Epos_ReceiveDate(){
	status = SDO_Read(&Controller1, 0x6041,0x00);
	osDelay(2);
	speed = SDO_Read(&Controller1, Velocity_Demand_Value,0x00);
	osDelay(2);
	
	printf("status:0x%X\t%d\r\n",status,speed);
	speed = SDO_Read(&Controller1, OP_MODE_Read,0x00);
	velocity = SDO_Read(&Controller1, Target_Velocity,0x00);
	osDelay(2);
	printf("mode:%d\t%d\r\n",speed, velocity);
	osDelay(10);
}



void Epos_ReadPosition(Epos* epos, int32_t* position){

	*position = SDO_Read(epos, Pos_Actual_Value,0x00);
}

void Epos_ReadVelocity(Epos* epos, int32_t* velocity){

	*velocity = SDO_Read(epos, Velocity_actual_value,0x00);
}

void Epos_ReadTorque(Epos* epos, int16_t* torque){

	*torque = SDO_Read(epos, Torque_actual_value,0x00);
}


// Apply motor parameters
//void Epos_SetMaxCurrent(uint8_t motor_index, int32_t max_current){
//    Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
//    SDO_Write(controller, OD_MAX_CURRENT, 0x00, max_current); // Assuming OD_MAX_CURRENT is the correct object dictionary key
//    printf("Set max current for Controller%d: %d\n", motor_index + 1, max_current);
//}

void Epos_SetAcceleration(uint8_t motor_index,int32_t acceleration){

    Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
    SDO_Write(controller, Profile_Acceleration, 0x00, acceleration);
    printf("Set acceleration for Controller%d: %d\n", motor_index + 1, acceleration);

}
void Epos_SetDeceleration(uint8_t motor_index, int32_t deceleration){

    Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
    SDO_Write(controller, Profile_Deceleration, 0x00, deceleration);
    printf("Set deceleration for Controller%d: %d\n", motor_index + 1, deceleration);
}
void Epos_StartSpeedMode(uint8_t motor_index, int32_t target_speed){

    Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
    Epos_setMode(controller, Profile_Velocity_Mode); // Set to speed mode
    Epos_OperEn(controller); // Ensure the controller is enabled
    Epos_SpeedSet(controller, target_speed); // Set the target speed
    printf("Started speed mode for Controller%d with target speed: %d\n", motor_index + 1, target_speed);

}
void Epos_StartPositionMode(uint8_t motor_index,int32_t target_position){

    Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
    Epos_setMode(controller, Profile_Position_Mode); // Set to position mode
    Epos_PosSet(controller, target_position); // Set the target position
    printf("Started position mode for Controller%d with target position: %d\n", motor_index + 1, target_position);

}
void Epos_StartTorqueMode(uint8_t motor_index, int16_t target_torque){
    Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
    Epos_setMode(controller, CST); // Set to torque mode (CST)
    Epos_OperEn(controller); // Ensure the controller is enabled
    Epos_CSTSet(controller, target_torque, 0); // Set the target torque with zero offset
    printf("Started torque mode for Controller%d with target torque: %d\n", motor_index + 1, target_torque);
}


void Epos_OnlyChangeTorque(uint8_t motor_index, int16_t target_torque){
    Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
    //Epos_setMode(controller, CST); // Set to torque mode (CST)
    //Epos_OperEn(controller); // Ensure the controller is enabled
    Epos_CSTSet(controller, target_torque, 0); // Set the target torque with zero offset
    printf("OnlyChangeTorque for Controller%d with target torque: %d\n", motor_index + 1, target_torque);
}


void Epos_Stop(uint8_t motor_index){
	Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
	Epos_ClorEn(controller);
}


void  Epos_HMMZero(uint8_t motor_index){
	Epos* controller = (motor_index == 0) ? &Controller1 : &Controller2;
	Epos_HMMSetNowZero(controller, 0, 0);
}
