################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/draw/sw/lv_draw_sw.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_arc.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_blend.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_dither.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_gradient.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_img.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_layer.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_letter.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_line.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_polygon.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_rect.c \
../HardWare/lvgl/src/draw/sw/lv_draw_sw_transform.c 

C_DEPS += \
./HardWare/lvgl/src/draw/sw/lv_draw_sw.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_arc.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_blend.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_dither.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_gradient.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_img.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_layer.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_letter.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_line.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_polygon.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_rect.d \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_transform.d 

OBJS += \
./HardWare/lvgl/src/draw/sw/lv_draw_sw.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_arc.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_blend.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_dither.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_gradient.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_img.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_layer.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_letter.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_line.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_polygon.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_rect.o \
./HardWare/lvgl/src/draw/sw/lv_draw_sw_transform.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/draw/sw/%.o HardWare/lvgl/src/draw/sw/%.su HardWare/lvgl/src/draw/sw/%.cyclo: ../HardWare/lvgl/src/draw/sw/%.c HardWare/lvgl/src/draw/sw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-sw

clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-sw:
	-$(RM) ./HardWare/lvgl/src/draw/sw/lv_draw_sw.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_arc.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_arc.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_arc.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_arc.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_blend.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_blend.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_blend.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_blend.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_dither.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_dither.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_dither.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_dither.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_gradient.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_gradient.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_gradient.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_gradient.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_img.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_img.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_img.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_img.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_layer.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_layer.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_layer.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_layer.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_letter.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_letter.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_letter.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_letter.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_line.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_line.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_line.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_line.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_polygon.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_polygon.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_polygon.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_polygon.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_rect.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_rect.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_rect.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_rect.su ./HardWare/lvgl/src/draw/sw/lv_draw_sw_transform.cyclo ./HardWare/lvgl/src/draw/sw/lv_draw_sw_transform.d ./HardWare/lvgl/src/draw/sw/lv_draw_sw_transform.o ./HardWare/lvgl/src/draw/sw/lv_draw_sw_transform.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-sw

