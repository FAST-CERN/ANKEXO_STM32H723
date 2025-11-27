/*
 * control_state.cpp
 *
 *  Created on: Nov 27, 2025
 *      Author: ruich
 */
#include <exoske_gait_ctrl.hpp>

namespace exoske_ctrl{

exoske::exoske(){
	exo_handle_.error_state = 0;
	exo_handle_.time_ms = 0;
}

void exoske::exoske_timer_cbk(void){
	exoske_fsm_update();
	exoske_ctrl_cbk();
}

void exoske::exoske_fsm_update(void){
	// todo: get sensor data and input command

	// safe check
	if(exo_handle_.error_state != 0){
		exo_handle_.cur_phase[0] = GAIT_SAFE;
		exo_handle_.cur_phase[1] = GAIT_SAFE;
	}
	else{
		// handle gait state(left)
		switch(exo_handle_.prev_phase[0]){
			case GAIT_LOCK:
				exoske_fsm_lock(0);
				break;
			case GAIT_SAFE:
				exoske_fsm_safe(0);
				break;
			case GAIT_PRE_SWING:
				exoske_fsm_preswing(0);
				break;
			case GAIT_SWING:
				exoske_fsm_swing(0);
				break;
			case GAIT_RELAX:
				exoske_fsm_relax(0);
				break;
			default:
				break;
		}

		// handle gait state(right)
		switch(exo_handle_.prev_phase[1]){
			case GAIT_LOCK:
				exoske_fsm_lock(1);
				break;
			case GAIT_SAFE:
				exoske_fsm_safe(1);
				break;
			case GAIT_PRE_SWING:
				exoske_fsm_preswing(1);
				break;
			case GAIT_SWING:
				exoske_fsm_swing(1);
				break;
			case GAIT_RELAX:
				exoske_fsm_relax(1);
				break;
			default:
				break;
		}
	}

	// set previous state
	exo_handle_.prev_phase[0] = exo_handle_.cur_phase[0];
	exo_handle_.prev_phase[1] = exo_handle_.cur_phase[1];
}

void exoske::exoske_fsm_lock(uint8_t leg_id){
	if(exo_handle_.intention == (leg_id + 1) &&
	   exo_handle_.prev_phase[abs(1-leg_id)] == GAIT_LOCK){	 	// lock -> pre-swing
		exo_handle_.cur_phase[leg_id] = GAIT_PRE_SWING;
	}
}

void exoske::exoske_fsm_preswing(uint8_t leg_id){
	if(exo_handle_.intention != (leg_id + 1)){					// pre-swing -> lock
		exo_handle_.cur_phase[leg_id] = GAIT_LOCK;
	}
	else if(exo_handle_.step_height_set >= PRSW2SW_STEP_TH){	// pre-swing -> swing
		exo_handle_.cur_phase[leg_id] = GAIT_SWING;
	}
}

void exoske::exoske_fsm_safe(uint8_t leg_id){
	if(exo_handle_.error_state == 0){			// safe -> lock
		exo_handle_.cur_phase[leg_id] = GAIT_LOCK;
	}
}

void exoske::exoske_fsm_swing(uint8_t leg_id){
	// swing -> relax
	if( exo_handle_.intention == 0 ||
	   (exo_handle_.step_height_set < SW2RLX_STEP_TH && exo_handle_.hip_angle[leg_id]<SW2RLX_HIP_TH)){
		exo_handle_.cur_phase[leg_id] = GAIT_RELAX;
	}
}

void exoske::exoske_fsm_relax(uint8_t leg_id){
	// relax -> lock
	if(exo_handle_.knee_angle[leg_id] < SW2RLX_HIP_TH){
		exo_handle_.cur_phase[leg_id] = GAIT_LOCK;
	}

	// todo if relax failed?
}

// exoske control
void exoske::exoske_ctrl_cbk(void){
	// left leg
	switch(exo_handle_.cur_phase[0]){
		case GAIT_LOCK:
			exoske_ctrl_lock(0);
			break;
		case GAIT_SAFE:
			exoske_ctrl_safe(0);
			break;
		case GAIT_PRE_SWING:
			exoske_ctrl_preswing(0);
			break;
		case GAIT_SWING:
			exoske_ctrl_swing(0);
			break;
		case GAIT_RELAX:
			exoske_ctrl_relax(0);
			break;
		default:
			break;
	}
	// right leg
	switch(exo_handle_.cur_phase[1]){
		case GAIT_LOCK:
			exoske_ctrl_lock(1);
			break;
		case GAIT_SAFE:
			exoske_ctrl_safe(1);
			break;
		case GAIT_PRE_SWING:
			exoske_ctrl_preswing(1);
			break;
		case GAIT_SWING:
			exoske_ctrl_swing(1);
			break;
		case GAIT_RELAX:
			exoske_ctrl_relax(1);
			break;
		default:
			break;
	}
}

void exoske::exoske_ctrl_lock(uint8_t leg_id){
	ctrl_param_struct ctrl_param;

	ctrl_param.ctrl_mode_ = MIT_M;
	ctrl_param.kp = Kp_LOCK;
	ctrl_param.kd = Kd_LOCK;

	ctrl_param.pos_set = MOTOR_POS_MIN;
	ctrl_param.vel_set = 0.0;
	ctrl_param.tor_set = 0.0;

	exoske_motor_control(leg_id,&ctrl_param);
}

void exoske::exoske_ctrl_safe(uint8_t leg_id){
	ctrl_param_struct ctrl_param;

	ctrl_param.ctrl_mode_ = MIT_M;
	ctrl_param.kp = Kp_SAFE;
	ctrl_param.kd = Kd_SAFE;

	ctrl_param.pos_set = MOTOR_POS_MIN;
	ctrl_param.vel_set = 0.0;
	ctrl_param.tor_set = 0.0;

	exoske_motor_control(leg_id,&ctrl_param);
}

void exoske::exoske_ctrl_preswing(uint8_t leg_id){
	ctrl_param_struct ctrl_param;

	ctrl_param.ctrl_mode_ = MIT_M;
	ctrl_param.kp = Kp_LOCK;
	ctrl_param.kd = Kd_LOCK;

	ctrl_param.pos_set = MOTOR_POS_MIN;
	ctrl_param.vel_set = 0.0;
	ctrl_param.tor_set = 0.0;

	exoske_motor_control(leg_id,&ctrl_param);
}

void exoske::exoske_ctrl_swing(uint8_t leg_id){
	ctrl_param_struct ctrl_param;

	ctrl_param.ctrl_mode_ = MIT_M;
	ctrl_param.kp = Kp_SWING;
	ctrl_param.kd = Kd_SWING;

	ctrl_param.pos_set = exo_handle_.knee_angle_set[leg_id];
	ctrl_param.vel_set = 0.0;
	ctrl_param.tor_set = exoske_tor_comp(leg_id);

	exoske_motor_control(leg_id,&ctrl_param);
}

void exoske::exoske_ctrl_relax(uint8_t leg_id){
	ctrl_param_struct ctrl_param;

	ctrl_param.ctrl_mode_ = MIT_M;
	ctrl_param.kp = Kp_RELAX;
	ctrl_param.kd = Kd_RELAX;

	ctrl_param.pos_set = MOTOR_POS_MIN;
	ctrl_param.vel_set = 0.0;
	ctrl_param.tor_set = exoske_tor_comp(leg_id);

	exoske_motor_control(leg_id,&ctrl_param);
}

float exoske::exoske_tor_comp(uint8_t leg_id){
	float inc_angle = exo_handle_.imu[leg_id].inc_angle;
	float vel = exo_handle_.motor[leg_id].vel_fdb;

	// gravity compensation
	// todo sign maybe opposite
	float tor_grav = EXO_GRAV_COMP * sinf(inc_angle * M_PI / 180.0);

	// friction compensation
	float tor_fric_v = EXO_FRIC_COMP_V * vel;
	float tor_fric_c = 0.0f;

	if (vel > VEL_DEADZONE) {
		tor_fric_c = EXO_FRIC_COMP_C;
	} else if (vel < -VEL_DEADZONE) {
		tor_fric_c = -EXO_FRIC_COMP_C;
	} else {
		tor_fric_c = (vel / VEL_DEADZONE) * EXO_FRIC_COMP_C;	// 在死区内线性插值，平滑过渡
	}

	return (tor_grav + tor_fric_v + tor_fric_c);
}

}
