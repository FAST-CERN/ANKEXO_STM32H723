################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite.c \
../HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.c \
../HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.c \
../HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.c \
../HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.c \
../HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_buf.c \
../HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_utils.c 

C_DEPS += \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite.d \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.d \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.d \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.d \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.d \
./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_buf.d \
./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_utils.d 

OBJS += \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite.o \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.o \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.o \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.o \
./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.o \
./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_buf.o \
./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_utils.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/draw/nxp/vglite/%.o HardWare/lvgl/src/draw/nxp/vglite/%.su HardWare/lvgl/src/draw/nxp/vglite/%.cyclo: ../HardWare/lvgl/src/draw/nxp/vglite/%.c HardWare/lvgl/src/draw/nxp/vglite/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-vglite

clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-vglite:
	-$(RM) ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite.cyclo ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite.d ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite.o ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite.su ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.cyclo ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.d ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.o ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.su ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.cyclo ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.d ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.o ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.su ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.cyclo ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.d ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.o ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.su ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.cyclo ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.d ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.o ./HardWare/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.su ./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_buf.cyclo ./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_buf.d ./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_buf.o ./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_buf.su ./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_utils.cyclo ./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_utils.d ./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_utils.o ./HardWare/lvgl/src/draw/nxp/vglite/lv_vglite_utils.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-vglite

