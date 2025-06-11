################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/LED/WS2812B.c \
../HardWare/LED/WS2812B_HAL.c 

C_DEPS += \
./HardWare/LED/WS2812B.d \
./HardWare/LED/WS2812B_HAL.d 

OBJS += \
./HardWare/LED/WS2812B.o \
./HardWare/LED/WS2812B_HAL.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/LED/%.o HardWare/LED/%.su HardWare/LED/%.cyclo: ../HardWare/LED/%.c HardWare/LED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -I../HardWare/BLE/ -I../HardWare/BUZZ/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-LED

clean-HardWare-2f-LED:
	-$(RM) ./HardWare/LED/WS2812B.cyclo ./HardWare/LED/WS2812B.d ./HardWare/LED/WS2812B.o ./HardWare/LED/WS2812B.su ./HardWare/LED/WS2812B_HAL.cyclo ./HardWare/LED/WS2812B_HAL.d ./HardWare/LED/WS2812B_HAL.o ./HardWare/LED/WS2812B_HAL.su

.PHONY: clean-HardWare-2f-LED

