################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/BLE/ble_server.c 

C_DEPS += \
./HardWare/BLE/ble_server.d 

OBJS += \
./HardWare/BLE/ble_server.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/BLE/%.o HardWare/BLE/%.su HardWare/BLE/%.cyclo: ../HardWare/BLE/%.c HardWare/BLE/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM7 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -c -I../Core/Inc -I"D:/WORK/2025-07-REW/Code/temp/KEXO_STM32H723/Drivers/CMSIS/DSP/Include" -I"D:/WORK/2025-07-REW/Code/temp/KEXO_STM32H723/Drivers/CMSIS/DSP/Include/dsp" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -I../HardWare/BLE/ -I../HardWare/BUZZ/ -I../HardWare/FAN/ -I"D:/WORK/2025-07-REW/Code/temp/KEXO_STM32H723/HardWare/APP/IMU" -I"D:/WORK/2025-07-REW/Code/temp/KEXO_STM32H723/HardWare/APP/PID" -I"D:/WORK/2025-07-REW/Code/temp/KEXO_STM32H723/HardWare/Bsp/ICM20948/include" -I"D:/WORK/2025-07-REW/Code/temp/KEXO_STM32H723/HardWare/Bsp/ICM20948" -I"D:/WORK/2025-07-REW/Code/temp/KEXO_STM32H723/HardWare/Bsp/BMI088/inc" -I"D:/WORK/2025-07-REW/Code/temp/KEXO_STM32H723/HardWare/MC" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-BLE

clean-HardWare-2f-BLE:
	-$(RM) ./HardWare/BLE/ble_server.cyclo ./HardWare/BLE/ble_server.d ./HardWare/BLE/ble_server.o ./HardWare/BLE/ble_server.su

.PHONY: clean-HardWare-2f-BLE

