################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/extra/others/ime/lv_ime_pinyin.c 

C_DEPS += \
./HardWare/lvgl/src/extra/others/ime/lv_ime_pinyin.d 

OBJS += \
./HardWare/lvgl/src/extra/others/ime/lv_ime_pinyin.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/extra/others/ime/%.o HardWare/lvgl/src/extra/others/ime/%.su HardWare/lvgl/src/extra/others/ime/%.cyclo: ../HardWare/lvgl/src/extra/others/ime/%.c HardWare/lvgl/src/extra/others/ime/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-extra-2f-others-2f-ime

clean-HardWare-2f-lvgl-2f-src-2f-extra-2f-others-2f-ime:
	-$(RM) ./HardWare/lvgl/src/extra/others/ime/lv_ime_pinyin.cyclo ./HardWare/lvgl/src/extra/others/ime/lv_ime_pinyin.d ./HardWare/lvgl/src/extra/others/ime/lv_ime_pinyin.o ./HardWare/lvgl/src/extra/others/ime/lv_ime_pinyin.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-extra-2f-others-2f-ime

