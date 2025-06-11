#include "IMU_task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "../BSP/imu.h"

#include "global_param.h"




void app_imu_task_init(){
	IMU_INIT();
}

void app_imu_task_loop(){
	for(;;)
	{
	 // Check if monitoring should be started
		bool should_monitor = true;
//
//		lock_system_state();
////		if (g_system_state.system_mode == MODE_FACTORY_TEST &&
////			(g_system_state.factory_test_submode == SUBMODE_IMU_MONITOR) &&
////			g_system_state.monitoring_active) {
//		// ����
//		if (g_system_state.imu_monitoring_active) {
//			should_monitor = true;
//		}
//		unlock_system_state();

		// If monitoring is active, collect IMU data
		//if (should_monitor) {

		  IMU_LOOP();

			// Transmit data if USB transmission is enabled
//			bool should_transmit = false;
//
//			lock_system_state();
//			should_transmit = g_system_state.usb_settings.imu_data_enabled;
//			unlock_system_state();
//
//			if (should_transmit) {
//				// Send IMU data via USB
//				// This would be implemented in the USB communication module
//			}
		//}
		osDelay(6);
	}
}


