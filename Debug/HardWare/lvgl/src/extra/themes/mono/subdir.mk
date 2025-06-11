################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/extra/themes/mono/lv_theme_mono.c 

C_DEPS += \
./HardWare/lvgl/src/extra/themes/mono/lv_theme_mono.d 

OBJS += \
./HardWare/lvgl/src/extra/themes/mono/lv_theme_mono.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/extra/themes/mono/%.o HardWare/lvgl/src/extra/themes/mono/%.su HardWare/lvgl/src/extra/themes/mono/%.cyclo: ../HardWare/lvgl/src/extra/themes/mono/%.c HardWare/lvgl/src/extra/themes/mono/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-extra-2f-themes-2f-mono

clean-HardWare-2f-lvgl-2f-src-2f-extra-2f-themes-2f-mono:
	-$(RM) ./HardWare/lvgl/src/extra/themes/mono/lv_theme_mono.cyclo ./HardWare/lvgl/src/extra/themes/mono/lv_theme_mono.d ./HardWare/lvgl/src/extra/themes/mono/lv_theme_mono.o ./HardWare/lvgl/src/extra/themes/mono/lv_theme_mono.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-extra-2f-themes-2f-mono

