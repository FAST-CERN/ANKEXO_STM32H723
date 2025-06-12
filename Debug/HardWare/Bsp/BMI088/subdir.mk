################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/Bsp/BMI088/BMI088Middleware.c \
../HardWare/Bsp/BMI088/BMI088driver.c 

C_DEPS += \
./HardWare/Bsp/BMI088/BMI088Middleware.d \
./HardWare/Bsp/BMI088/BMI088driver.d 

OBJS += \
./HardWare/Bsp/BMI088/BMI088Middleware.o \
./HardWare/Bsp/BMI088/BMI088driver.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/Bsp/BMI088/%.o HardWare/Bsp/BMI088/%.su HardWare/Bsp/BMI088/%.cyclo: ../HardWare/Bsp/BMI088/%.c HardWare/Bsp/BMI088/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -I../HardWare/BLE/ -I../HardWare/BUZZ/ -I../HardWare/FAN/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-Bsp-2f-BMI088

clean-HardWare-2f-Bsp-2f-BMI088:
	-$(RM) ./HardWare/Bsp/BMI088/BMI088Middleware.cyclo ./HardWare/Bsp/BMI088/BMI088Middleware.d ./HardWare/Bsp/BMI088/BMI088Middleware.o ./HardWare/Bsp/BMI088/BMI088Middleware.su ./HardWare/Bsp/BMI088/BMI088driver.cyclo ./HardWare/Bsp/BMI088/BMI088driver.d ./HardWare/Bsp/BMI088/BMI088driver.o ./HardWare/Bsp/BMI088/BMI088driver.su

.PHONY: clean-HardWare-2f-Bsp-2f-BMI088

