################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_arc.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_bg.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_composite.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_img.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_label.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_layer.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_line.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_mask.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_polygon.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_rect.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.c \
../HardWare/lvgl/src/draw/sdl/lv_draw_sdl_utils.c 

C_DEPS += \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_arc.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_bg.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_composite.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_img.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_label.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_layer.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_line.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_mask.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_polygon.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_rect.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.d \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_utils.d 

OBJS += \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_arc.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_bg.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_composite.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_img.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_label.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_layer.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_line.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_mask.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_polygon.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_rect.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.o \
./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_utils.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/draw/sdl/%.o HardWare/lvgl/src/draw/sdl/%.su HardWare/lvgl/src/draw/sdl/%.cyclo: ../HardWare/lvgl/src/draw/sdl/%.c HardWare/lvgl/src/draw/sdl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-sdl

clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-sdl:
	-$(RM) ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_arc.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_arc.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_arc.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_arc.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_bg.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_bg.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_bg.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_bg.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_composite.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_composite.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_composite.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_composite.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_img.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_img.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_img.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_img.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_label.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_label.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_label.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_label.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_layer.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_layer.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_layer.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_layer.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_line.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_line.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_line.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_line.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_mask.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_mask.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_mask.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_mask.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_polygon.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_polygon.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_polygon.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_polygon.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_rect.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_rect.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_rect.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_rect.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.su ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_utils.cyclo ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_utils.d ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_utils.o ./HardWare/lvgl/src/draw/sdl/lv_draw_sdl_utils.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-sdl

