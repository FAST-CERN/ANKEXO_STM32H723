################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/core/lv_disp.c \
../HardWare/lvgl/src/core/lv_event.c \
../HardWare/lvgl/src/core/lv_group.c \
../HardWare/lvgl/src/core/lv_indev.c \
../HardWare/lvgl/src/core/lv_indev_scroll.c \
../HardWare/lvgl/src/core/lv_obj.c \
../HardWare/lvgl/src/core/lv_obj_class.c \
../HardWare/lvgl/src/core/lv_obj_draw.c \
../HardWare/lvgl/src/core/lv_obj_pos.c \
../HardWare/lvgl/src/core/lv_obj_scroll.c \
../HardWare/lvgl/src/core/lv_obj_style.c \
../HardWare/lvgl/src/core/lv_obj_style_gen.c \
../HardWare/lvgl/src/core/lv_obj_tree.c \
../HardWare/lvgl/src/core/lv_refr.c \
../HardWare/lvgl/src/core/lv_theme.c 

C_DEPS += \
./HardWare/lvgl/src/core/lv_disp.d \
./HardWare/lvgl/src/core/lv_event.d \
./HardWare/lvgl/src/core/lv_group.d \
./HardWare/lvgl/src/core/lv_indev.d \
./HardWare/lvgl/src/core/lv_indev_scroll.d \
./HardWare/lvgl/src/core/lv_obj.d \
./HardWare/lvgl/src/core/lv_obj_class.d \
./HardWare/lvgl/src/core/lv_obj_draw.d \
./HardWare/lvgl/src/core/lv_obj_pos.d \
./HardWare/lvgl/src/core/lv_obj_scroll.d \
./HardWare/lvgl/src/core/lv_obj_style.d \
./HardWare/lvgl/src/core/lv_obj_style_gen.d \
./HardWare/lvgl/src/core/lv_obj_tree.d \
./HardWare/lvgl/src/core/lv_refr.d \
./HardWare/lvgl/src/core/lv_theme.d 

OBJS += \
./HardWare/lvgl/src/core/lv_disp.o \
./HardWare/lvgl/src/core/lv_event.o \
./HardWare/lvgl/src/core/lv_group.o \
./HardWare/lvgl/src/core/lv_indev.o \
./HardWare/lvgl/src/core/lv_indev_scroll.o \
./HardWare/lvgl/src/core/lv_obj.o \
./HardWare/lvgl/src/core/lv_obj_class.o \
./HardWare/lvgl/src/core/lv_obj_draw.o \
./HardWare/lvgl/src/core/lv_obj_pos.o \
./HardWare/lvgl/src/core/lv_obj_scroll.o \
./HardWare/lvgl/src/core/lv_obj_style.o \
./HardWare/lvgl/src/core/lv_obj_style_gen.o \
./HardWare/lvgl/src/core/lv_obj_tree.o \
./HardWare/lvgl/src/core/lv_refr.o \
./HardWare/lvgl/src/core/lv_theme.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/core/%.o HardWare/lvgl/src/core/%.su HardWare/lvgl/src/core/%.cyclo: ../HardWare/lvgl/src/core/%.c HardWare/lvgl/src/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-core

clean-HardWare-2f-lvgl-2f-src-2f-core:
	-$(RM) ./HardWare/lvgl/src/core/lv_disp.cyclo ./HardWare/lvgl/src/core/lv_disp.d ./HardWare/lvgl/src/core/lv_disp.o ./HardWare/lvgl/src/core/lv_disp.su ./HardWare/lvgl/src/core/lv_event.cyclo ./HardWare/lvgl/src/core/lv_event.d ./HardWare/lvgl/src/core/lv_event.o ./HardWare/lvgl/src/core/lv_event.su ./HardWare/lvgl/src/core/lv_group.cyclo ./HardWare/lvgl/src/core/lv_group.d ./HardWare/lvgl/src/core/lv_group.o ./HardWare/lvgl/src/core/lv_group.su ./HardWare/lvgl/src/core/lv_indev.cyclo ./HardWare/lvgl/src/core/lv_indev.d ./HardWare/lvgl/src/core/lv_indev.o ./HardWare/lvgl/src/core/lv_indev.su ./HardWare/lvgl/src/core/lv_indev_scroll.cyclo ./HardWare/lvgl/src/core/lv_indev_scroll.d ./HardWare/lvgl/src/core/lv_indev_scroll.o ./HardWare/lvgl/src/core/lv_indev_scroll.su ./HardWare/lvgl/src/core/lv_obj.cyclo ./HardWare/lvgl/src/core/lv_obj.d ./HardWare/lvgl/src/core/lv_obj.o ./HardWare/lvgl/src/core/lv_obj.su ./HardWare/lvgl/src/core/lv_obj_class.cyclo ./HardWare/lvgl/src/core/lv_obj_class.d ./HardWare/lvgl/src/core/lv_obj_class.o ./HardWare/lvgl/src/core/lv_obj_class.su ./HardWare/lvgl/src/core/lv_obj_draw.cyclo ./HardWare/lvgl/src/core/lv_obj_draw.d ./HardWare/lvgl/src/core/lv_obj_draw.o ./HardWare/lvgl/src/core/lv_obj_draw.su ./HardWare/lvgl/src/core/lv_obj_pos.cyclo ./HardWare/lvgl/src/core/lv_obj_pos.d ./HardWare/lvgl/src/core/lv_obj_pos.o ./HardWare/lvgl/src/core/lv_obj_pos.su ./HardWare/lvgl/src/core/lv_obj_scroll.cyclo ./HardWare/lvgl/src/core/lv_obj_scroll.d ./HardWare/lvgl/src/core/lv_obj_scroll.o ./HardWare/lvgl/src/core/lv_obj_scroll.su ./HardWare/lvgl/src/core/lv_obj_style.cyclo ./HardWare/lvgl/src/core/lv_obj_style.d ./HardWare/lvgl/src/core/lv_obj_style.o ./HardWare/lvgl/src/core/lv_obj_style.su ./HardWare/lvgl/src/core/lv_obj_style_gen.cyclo ./HardWare/lvgl/src/core/lv_obj_style_gen.d ./HardWare/lvgl/src/core/lv_obj_style_gen.o ./HardWare/lvgl/src/core/lv_obj_style_gen.su ./HardWare/lvgl/src/core/lv_obj_tree.cyclo ./HardWare/lvgl/src/core/lv_obj_tree.d ./HardWare/lvgl/src/core/lv_obj_tree.o ./HardWare/lvgl/src/core/lv_obj_tree.su ./HardWare/lvgl/src/core/lv_refr.cyclo ./HardWare/lvgl/src/core/lv_refr.d ./HardWare/lvgl/src/core/lv_refr.o ./HardWare/lvgl/src/core/lv_refr.su ./HardWare/lvgl/src/core/lv_theme.cyclo ./HardWare/lvgl/src/core/lv_theme.d ./HardWare/lvgl/src/core/lv_theme.o ./HardWare/lvgl/src/core/lv_theme.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-core

