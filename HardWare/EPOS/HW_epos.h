#ifndef __HW_EPOS_H
#define __HW_EPOS_H

#include "main.h"
#include "epos.h"

extern Epos Controller1,Controller2;        				 //控制器对象

//void Epos_INIT(void);
void Epos_INIT_Controller1();
void Epos_INIT_Controller2();

void Epos_ReceiveDate(void);
void Epos_Conroller_TIMBack(void);
void Epos_Start(void);


void Epos_ControlTask(void);
void cst_Task(void);


void Epos_PosSet(Epos* epos, Uint32 pos);
void Epos_SpeedSet(Epos* epos, Uint32 speed);
void Epos_HMMSetNowZero(Epos* epos, Uint32 pos, Uint32 offset); //还存在问题
void Epos_CSTSet(Epos* epos, Uint32 torque, Uint32 offset);

void speed_Task(Epos control);

void pos_Task(Epos control);


void Epos_ReadPosition(Epos* epos, int32_t* position);
void Epos_ReadVelocity(Epos* epos, int32_t* velocity);
void Epos_ReadTorque(Epos* epos, int16_t* torque);


//void Epos_SetMaxCurrent(uint8_t motor_index, int32_t max_current);
void Epos_SetAcceleration(uint8_t motor_index,int32_t acceleration);
void Epos_SetDeceleration(uint8_t motor_index, int32_t deceleration);
void Epos_StartSpeedMode(uint8_t motor_index, int32_t target_speed);
void Epos_StartPositionMode(uint8_t motor_index,int32_t target_position);
void Epos_StartTorqueMode(uint8_t motor_index, int16_t target_torque);
void Epos_OnlyChangeTorque(uint8_t motor_index, int16_t target_torque);
void Epos_Stop(uint8_t motor_index);
void  Epos_HMMZero(uint8_t motor_index);
#endif

