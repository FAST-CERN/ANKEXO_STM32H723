################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/porting/lv_port_disp.c \
../HardWare/lvgl/porting/lv_port_fs.c \
../HardWare/lvgl/porting/lv_port_indev.c 

C_DEPS += \
./HardWare/lvgl/porting/lv_port_disp.d \
./HardWare/lvgl/porting/lv_port_fs.d \
./HardWare/lvgl/porting/lv_port_indev.d 

OBJS += \
./HardWare/lvgl/porting/lv_port_disp.o \
./HardWare/lvgl/porting/lv_port_fs.o \
./HardWare/lvgl/porting/lv_port_indev.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/porting/%.o HardWare/lvgl/porting/%.su HardWare/lvgl/porting/%.cyclo: ../HardWare/lvgl/porting/%.c HardWare/lvgl/porting/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-porting

clean-HardWare-2f-lvgl-2f-porting:
	-$(RM) ./HardWare/lvgl/porting/lv_port_disp.cyclo ./HardWare/lvgl/porting/lv_port_disp.d ./HardWare/lvgl/porting/lv_port_disp.o ./HardWare/lvgl/porting/lv_port_disp.su ./HardWare/lvgl/porting/lv_port_fs.cyclo ./HardWare/lvgl/porting/lv_port_fs.d ./HardWare/lvgl/porting/lv_port_fs.o ./HardWare/lvgl/porting/lv_port_fs.su ./HardWare/lvgl/porting/lv_port_indev.cyclo ./HardWare/lvgl/porting/lv_port_indev.d ./HardWare/lvgl/porting/lv_port_indev.o ./HardWare/lvgl/porting/lv_port_indev.su

.PHONY: clean-HardWare-2f-lvgl-2f-porting

