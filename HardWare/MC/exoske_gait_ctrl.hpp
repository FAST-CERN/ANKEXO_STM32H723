/*
 * control_state.hpp
 *
 *  Created on: Nov 27, 2025
 *      Author: ruich
 */
#ifndef MC_EXOSKE_GAIT_CTRL_HPP_
#define MC_EXOSKE_GAIT_CTRL_HPP_

#include <cmath>
#include <cstdint>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// using damiao motor
//#include <dm_motor_ctrl.h>

namespace exoske_ctrl{

// 步态
typedef enum {
	GAIT_LOCK = 0,	// 锁定状态
	GAIT_PRE_SWING,	// 准备跟踪状态
	GAIT_SWING,		// 跟踪状态
	GAIT_RELAX,		// 跟踪-锁定过渡状态
	GAIT_SAFE,		// 安全放松状态
} gait_phase;

// 传感器状态
typedef enum {
	SENSOR_RUN = 0,
	SENSOR_RESET,
	SENSOR_ERROR,
	SENSOR_STOP,
} sensor_state;

// 执行器（电机）状态
typedef enum {
	ACT_RUN = 0,
	ACT_RESET,
	ACT_ERROR,
	ACT_STOP,
} actuator_state;

typedef enum {
	MIT_M = 0,
	POS_M,
}ctrl_mode;

typedef struct {
    float kp = 0.0;
    float kd = 0.0;

    float pos_set;
    float vel_set;
    float tor_set;

    ctrl_mode ctrl_mode_ = MIT_M;
} ctrl_param_struct;

// 执行器数据结构体
typedef struct {
    uint8_t actuator_id = 0x00;
    actuator_state act_state = ACT_STOP;	// actuator state

    float pos_set;	// set value
    float vel_set;
    float tor_set;

    float pos_fdb;	// feedback value
	float vel_fdb;
	float tor_fdb;
} exoske_actuator;

// IMU数据结构体
typedef struct{
	uint8_t sensor_id = 0x00;
	sensor_state sensor_state_ = SENSOR_STOP;

	// 直接用计算出来的倾角
	float inc_angle;
} exoske_imu;


typedef struct {
	// 下列数组均按照 0:左腿 1:右腿 索引
	// 执行器和传感器
    exoske_actuator motor[2];
    exoske_imu imu[2];

    // 控制输入
    uint8_t intention;	 		// 脑电意图指令(0:休息, 1:左腿, 2:右腿)
    float step_height_set;		// 期望步高
    float knee_angle_set[2];	// 期望膝关节角
    float hip_angle_set[2];		// 期望髋关节角

    // 关节角位置
	float knee_angle[2];	// 实际膝关节角
	float hip_angle[2];		// 实际髋关节角

	// 关节角速度(暂时不用)
	float knee_omega[2];
	float hip_omega[2];

	// 步态
	gait_phase prev_phase[2];
	gait_phase cur_phase[2];

    // 外骨骼状态
	uint32_t time_ms;
	uint32_t error_state;
} exoske_handle;


class exoske{
public:
	exoske();
	~exoske(){}

	void exoske_timer_cbk(void);

private:
	exoske_handle exo_handle_;

	// 阈值设定
	float PRSW2SW_STEP_TH = 0.3;	// (0~1)
	float SW2RLX_STEP_TH  = 0.1;	// (0~1)
	float SW2RLX_HIP_TH   = 30;		// deg
	float RLX2LK_KNEE_TH  = 5;		// deg

	// 控制周期设定
	float CTRL_LOOP_PERIOD = 0.01;	// 控制周期(s)
	float FSM_LOOP_PERIOD  = 0.05;	// 状态机周期(s)

	// 电机控制参数
	float Kp_LOCK = 15.0;
	float Kd_LOCK = 0.5;
	float Kp_SAFE = 15.0;
	float Kd_SAFE = 0.5;
	float Kp_SWING = 10.0;
	float Kd_SWING = 0.3;
	float Kp_RELAX = 5.0;
	float Kd_RELAX = 0.3;

	float MOTOR_POS_MIN = 0.0; 		// deg
	float MOTOR_POS_MAX = 120.0; 	// deg
	float MOTOR_VEL_MAX = 100.0;	// deg/s
	float MOTOR_VEL_MIN = -100.0;	// deg/s
	float MOTOR_TOR_MAX = 50.0;		// Nm
    float MOTOR_TOR_MIN = -50.0;	// Nm

    float MOTOR_GEAR_RATIO = 0.1; 	// 1:10

    float EXO_GRAV_COMP 	= 10.0;	// 重力补偿系数: m * g * l
    float VEL_DEADZONE 		= 10.0f;// 10 deg/s
    float EXO_FRIC_COMP_V 	= 5.0;	// 黏性摩擦补偿系数
    float EXO_FRIC_COMP_C 	= 2.0;	// 库伦摩擦补偿系数

	// FSM update and state handlers
	void exoske_fsm_update(void);
	void exoske_fsm_reset(void);

	void exoske_fsm_lock(uint8_t leg_id);
	void exoske_fsm_safe(uint8_t leg_id);
	void exoske_fsm_preswing(uint8_t leg_id);
	void exoske_fsm_swing(uint8_t leg_id);
	void exoske_fsm_relax(uint8_t leg_id);

	// Exoskeleton Control
	void exoske_ctrl_cbk(void);

	void exoske_ctrl_lock(uint8_t leg_id);
	void exoske_ctrl_safe(uint8_t leg_id);
	void exoske_ctrl_preswing(uint8_t leg_id);
	void exoske_ctrl_swing(uint8_t leg_id);
	void exoske_ctrl_relax(uint8_t leg_id);

	float exoske_tor_comp(uint8_t leg_id);

	// Motor Control Interface
	void exoske_motor_control(uint8_t motor_id, ctrl_param_struct* ctrl_param){
		// todo ...
	}
};
}



#endif /* MC_EXOSKE_GAIT_CTRL_HPP_ */
