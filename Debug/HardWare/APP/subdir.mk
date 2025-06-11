################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/APP/IMU_task.c \
../HardWare/APP/global_param.c \
../HardWare/APP/motor_task.c 

C_DEPS += \
./HardWare/APP/IMU_task.d \
./HardWare/APP/global_param.d \
./HardWare/APP/motor_task.d 

OBJS += \
./HardWare/APP/IMU_task.o \
./HardWare/APP/global_param.o \
./HardWare/APP/motor_task.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/APP/%.o HardWare/APP/%.su HardWare/APP/%.cyclo: ../HardWare/APP/%.c HardWare/APP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -I../HardWare/BLE/ -I../HardWare/BUZZ/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-APP

clean-HardWare-2f-APP:
	-$(RM) ./HardWare/APP/IMU_task.cyclo ./HardWare/APP/IMU_task.d ./HardWare/APP/IMU_task.o ./HardWare/APP/IMU_task.su ./HardWare/APP/global_param.cyclo ./HardWare/APP/global_param.d ./HardWare/APP/global_param.o ./HardWare/APP/global_param.su ./HardWare/APP/motor_task.cyclo ./HardWare/APP/motor_task.d ./HardWare/APP/motor_task.o ./HardWare/APP/motor_task.su

.PHONY: clean-HardWare-2f-APP

