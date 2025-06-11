################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/EPOS/HW_epos.c \
../HardWare/EPOS/epos.c \
../HardWare/EPOS/sdo.c 

C_DEPS += \
./HardWare/EPOS/HW_epos.d \
./HardWare/EPOS/epos.d \
./HardWare/EPOS/sdo.d 

OBJS += \
./HardWare/EPOS/HW_epos.o \
./HardWare/EPOS/epos.o \
./HardWare/EPOS/sdo.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/EPOS/%.o HardWare/EPOS/%.su HardWare/EPOS/%.cyclo: ../HardWare/EPOS/%.c HardWare/EPOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -I../HardWare/BLE/ -I../HardWare/BUZZ/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-EPOS

clean-HardWare-2f-EPOS:
	-$(RM) ./HardWare/EPOS/HW_epos.cyclo ./HardWare/EPOS/HW_epos.d ./HardWare/EPOS/HW_epos.o ./HardWare/EPOS/HW_epos.su ./HardWare/EPOS/epos.cyclo ./HardWare/EPOS/epos.d ./HardWare/EPOS/epos.o ./HardWare/EPOS/epos.su ./HardWare/EPOS/sdo.cyclo ./HardWare/EPOS/sdo.d ./HardWare/EPOS/sdo.o ./HardWare/EPOS/sdo.su

.PHONY: clean-HardWare-2f-EPOS

