################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/draw/lv_draw.c \
../HardWare/lvgl/src/draw/lv_draw_arc.c \
../HardWare/lvgl/src/draw/lv_draw_img.c \
../HardWare/lvgl/src/draw/lv_draw_label.c \
../HardWare/lvgl/src/draw/lv_draw_layer.c \
../HardWare/lvgl/src/draw/lv_draw_line.c \
../HardWare/lvgl/src/draw/lv_draw_mask.c \
../HardWare/lvgl/src/draw/lv_draw_rect.c \
../HardWare/lvgl/src/draw/lv_draw_transform.c \
../HardWare/lvgl/src/draw/lv_draw_triangle.c \
../HardWare/lvgl/src/draw/lv_img_buf.c \
../HardWare/lvgl/src/draw/lv_img_cache.c \
../HardWare/lvgl/src/draw/lv_img_decoder.c 

C_DEPS += \
./HardWare/lvgl/src/draw/lv_draw.d \
./HardWare/lvgl/src/draw/lv_draw_arc.d \
./HardWare/lvgl/src/draw/lv_draw_img.d \
./HardWare/lvgl/src/draw/lv_draw_label.d \
./HardWare/lvgl/src/draw/lv_draw_layer.d \
./HardWare/lvgl/src/draw/lv_draw_line.d \
./HardWare/lvgl/src/draw/lv_draw_mask.d \
./HardWare/lvgl/src/draw/lv_draw_rect.d \
./HardWare/lvgl/src/draw/lv_draw_transform.d \
./HardWare/lvgl/src/draw/lv_draw_triangle.d \
./HardWare/lvgl/src/draw/lv_img_buf.d \
./HardWare/lvgl/src/draw/lv_img_cache.d \
./HardWare/lvgl/src/draw/lv_img_decoder.d 

OBJS += \
./HardWare/lvgl/src/draw/lv_draw.o \
./HardWare/lvgl/src/draw/lv_draw_arc.o \
./HardWare/lvgl/src/draw/lv_draw_img.o \
./HardWare/lvgl/src/draw/lv_draw_label.o \
./HardWare/lvgl/src/draw/lv_draw_layer.o \
./HardWare/lvgl/src/draw/lv_draw_line.o \
./HardWare/lvgl/src/draw/lv_draw_mask.o \
./HardWare/lvgl/src/draw/lv_draw_rect.o \
./HardWare/lvgl/src/draw/lv_draw_transform.o \
./HardWare/lvgl/src/draw/lv_draw_triangle.o \
./HardWare/lvgl/src/draw/lv_img_buf.o \
./HardWare/lvgl/src/draw/lv_img_cache.o \
./HardWare/lvgl/src/draw/lv_img_decoder.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/draw/%.o HardWare/lvgl/src/draw/%.su HardWare/lvgl/src/draw/%.cyclo: ../HardWare/lvgl/src/draw/%.c HardWare/lvgl/src/draw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-draw

clean-HardWare-2f-lvgl-2f-src-2f-draw:
	-$(RM) ./HardWare/lvgl/src/draw/lv_draw.cyclo ./HardWare/lvgl/src/draw/lv_draw.d ./HardWare/lvgl/src/draw/lv_draw.o ./HardWare/lvgl/src/draw/lv_draw.su ./HardWare/lvgl/src/draw/lv_draw_arc.cyclo ./HardWare/lvgl/src/draw/lv_draw_arc.d ./HardWare/lvgl/src/draw/lv_draw_arc.o ./HardWare/lvgl/src/draw/lv_draw_arc.su ./HardWare/lvgl/src/draw/lv_draw_img.cyclo ./HardWare/lvgl/src/draw/lv_draw_img.d ./HardWare/lvgl/src/draw/lv_draw_img.o ./HardWare/lvgl/src/draw/lv_draw_img.su ./HardWare/lvgl/src/draw/lv_draw_label.cyclo ./HardWare/lvgl/src/draw/lv_draw_label.d ./HardWare/lvgl/src/draw/lv_draw_label.o ./HardWare/lvgl/src/draw/lv_draw_label.su ./HardWare/lvgl/src/draw/lv_draw_layer.cyclo ./HardWare/lvgl/src/draw/lv_draw_layer.d ./HardWare/lvgl/src/draw/lv_draw_layer.o ./HardWare/lvgl/src/draw/lv_draw_layer.su ./HardWare/lvgl/src/draw/lv_draw_line.cyclo ./HardWare/lvgl/src/draw/lv_draw_line.d ./HardWare/lvgl/src/draw/lv_draw_line.o ./HardWare/lvgl/src/draw/lv_draw_line.su ./HardWare/lvgl/src/draw/lv_draw_mask.cyclo ./HardWare/lvgl/src/draw/lv_draw_mask.d ./HardWare/lvgl/src/draw/lv_draw_mask.o ./HardWare/lvgl/src/draw/lv_draw_mask.su ./HardWare/lvgl/src/draw/lv_draw_rect.cyclo ./HardWare/lvgl/src/draw/lv_draw_rect.d ./HardWare/lvgl/src/draw/lv_draw_rect.o ./HardWare/lvgl/src/draw/lv_draw_rect.su ./HardWare/lvgl/src/draw/lv_draw_transform.cyclo ./HardWare/lvgl/src/draw/lv_draw_transform.d ./HardWare/lvgl/src/draw/lv_draw_transform.o ./HardWare/lvgl/src/draw/lv_draw_transform.su ./HardWare/lvgl/src/draw/lv_draw_triangle.cyclo ./HardWare/lvgl/src/draw/lv_draw_triangle.d ./HardWare/lvgl/src/draw/lv_draw_triangle.o ./HardWare/lvgl/src/draw/lv_draw_triangle.su ./HardWare/lvgl/src/draw/lv_img_buf.cyclo ./HardWare/lvgl/src/draw/lv_img_buf.d ./HardWare/lvgl/src/draw/lv_img_buf.o ./HardWare/lvgl/src/draw/lv_img_buf.su ./HardWare/lvgl/src/draw/lv_img_cache.cyclo ./HardWare/lvgl/src/draw/lv_img_cache.d ./HardWare/lvgl/src/draw/lv_img_cache.o ./HardWare/lvgl/src/draw/lv_img_cache.su ./HardWare/lvgl/src/draw/lv_img_decoder.cyclo ./HardWare/lvgl/src/draw/lv_img_decoder.d ./HardWare/lvgl/src/draw/lv_img_decoder.o ./HardWare/lvgl/src/draw/lv_img_decoder.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-draw

