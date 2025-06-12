################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/Bsp/S485_master.c \
../HardWare/Bsp/bsp_user_key.c \
../HardWare/Bsp/can_bsp.c \
../HardWare/Bsp/imu.c 

C_DEPS += \
./HardWare/Bsp/S485_master.d \
./HardWare/Bsp/bsp_user_key.d \
./HardWare/Bsp/can_bsp.d \
./HardWare/Bsp/imu.d 

OBJS += \
./HardWare/Bsp/S485_master.o \
./HardWare/Bsp/bsp_user_key.o \
./HardWare/Bsp/can_bsp.o \
./HardWare/Bsp/imu.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/Bsp/%.o HardWare/Bsp/%.su HardWare/Bsp/%.cyclo: ../HardWare/Bsp/%.c HardWare/Bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -I../HardWare/BLE/ -I../HardWare/BUZZ/ -I../HardWare/FAN/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-Bsp

clean-HardWare-2f-Bsp:
	-$(RM) ./HardWare/Bsp/S485_master.cyclo ./HardWare/Bsp/S485_master.d ./HardWare/Bsp/S485_master.o ./HardWare/Bsp/S485_master.su ./HardWare/Bsp/bsp_user_key.cyclo ./HardWare/Bsp/bsp_user_key.d ./HardWare/Bsp/bsp_user_key.o ./HardWare/Bsp/bsp_user_key.su ./HardWare/Bsp/can_bsp.cyclo ./HardWare/Bsp/can_bsp.d ./HardWare/Bsp/can_bsp.o ./HardWare/Bsp/can_bsp.su ./HardWare/Bsp/imu.cyclo ./HardWare/Bsp/imu.d ./HardWare/Bsp/imu.o ./HardWare/Bsp/imu.su

.PHONY: clean-HardWare-2f-Bsp

