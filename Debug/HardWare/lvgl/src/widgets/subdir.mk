################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/widgets/lv_arc.c \
../HardWare/lvgl/src/widgets/lv_bar.c \
../HardWare/lvgl/src/widgets/lv_btn.c \
../HardWare/lvgl/src/widgets/lv_btnmatrix.c \
../HardWare/lvgl/src/widgets/lv_canvas.c \
../HardWare/lvgl/src/widgets/lv_checkbox.c \
../HardWare/lvgl/src/widgets/lv_dropdown.c \
../HardWare/lvgl/src/widgets/lv_img.c \
../HardWare/lvgl/src/widgets/lv_label.c \
../HardWare/lvgl/src/widgets/lv_line.c \
../HardWare/lvgl/src/widgets/lv_objx_templ.c \
../HardWare/lvgl/src/widgets/lv_roller.c \
../HardWare/lvgl/src/widgets/lv_slider.c \
../HardWare/lvgl/src/widgets/lv_switch.c \
../HardWare/lvgl/src/widgets/lv_table.c \
../HardWare/lvgl/src/widgets/lv_textarea.c 

C_DEPS += \
./HardWare/lvgl/src/widgets/lv_arc.d \
./HardWare/lvgl/src/widgets/lv_bar.d \
./HardWare/lvgl/src/widgets/lv_btn.d \
./HardWare/lvgl/src/widgets/lv_btnmatrix.d \
./HardWare/lvgl/src/widgets/lv_canvas.d \
./HardWare/lvgl/src/widgets/lv_checkbox.d \
./HardWare/lvgl/src/widgets/lv_dropdown.d \
./HardWare/lvgl/src/widgets/lv_img.d \
./HardWare/lvgl/src/widgets/lv_label.d \
./HardWare/lvgl/src/widgets/lv_line.d \
./HardWare/lvgl/src/widgets/lv_objx_templ.d \
./HardWare/lvgl/src/widgets/lv_roller.d \
./HardWare/lvgl/src/widgets/lv_slider.d \
./HardWare/lvgl/src/widgets/lv_switch.d \
./HardWare/lvgl/src/widgets/lv_table.d \
./HardWare/lvgl/src/widgets/lv_textarea.d 

OBJS += \
./HardWare/lvgl/src/widgets/lv_arc.o \
./HardWare/lvgl/src/widgets/lv_bar.o \
./HardWare/lvgl/src/widgets/lv_btn.o \
./HardWare/lvgl/src/widgets/lv_btnmatrix.o \
./HardWare/lvgl/src/widgets/lv_canvas.o \
./HardWare/lvgl/src/widgets/lv_checkbox.o \
./HardWare/lvgl/src/widgets/lv_dropdown.o \
./HardWare/lvgl/src/widgets/lv_img.o \
./HardWare/lvgl/src/widgets/lv_label.o \
./HardWare/lvgl/src/widgets/lv_line.o \
./HardWare/lvgl/src/widgets/lv_objx_templ.o \
./HardWare/lvgl/src/widgets/lv_roller.o \
./HardWare/lvgl/src/widgets/lv_slider.o \
./HardWare/lvgl/src/widgets/lv_switch.o \
./HardWare/lvgl/src/widgets/lv_table.o \
./HardWare/lvgl/src/widgets/lv_textarea.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/widgets/%.o HardWare/lvgl/src/widgets/%.su HardWare/lvgl/src/widgets/%.cyclo: ../HardWare/lvgl/src/widgets/%.c HardWare/lvgl/src/widgets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-widgets

clean-HardWare-2f-lvgl-2f-src-2f-widgets:
	-$(RM) ./HardWare/lvgl/src/widgets/lv_arc.cyclo ./HardWare/lvgl/src/widgets/lv_arc.d ./HardWare/lvgl/src/widgets/lv_arc.o ./HardWare/lvgl/src/widgets/lv_arc.su ./HardWare/lvgl/src/widgets/lv_bar.cyclo ./HardWare/lvgl/src/widgets/lv_bar.d ./HardWare/lvgl/src/widgets/lv_bar.o ./HardWare/lvgl/src/widgets/lv_bar.su ./HardWare/lvgl/src/widgets/lv_btn.cyclo ./HardWare/lvgl/src/widgets/lv_btn.d ./HardWare/lvgl/src/widgets/lv_btn.o ./HardWare/lvgl/src/widgets/lv_btn.su ./HardWare/lvgl/src/widgets/lv_btnmatrix.cyclo ./HardWare/lvgl/src/widgets/lv_btnmatrix.d ./HardWare/lvgl/src/widgets/lv_btnmatrix.o ./HardWare/lvgl/src/widgets/lv_btnmatrix.su ./HardWare/lvgl/src/widgets/lv_canvas.cyclo ./HardWare/lvgl/src/widgets/lv_canvas.d ./HardWare/lvgl/src/widgets/lv_canvas.o ./HardWare/lvgl/src/widgets/lv_canvas.su ./HardWare/lvgl/src/widgets/lv_checkbox.cyclo ./HardWare/lvgl/src/widgets/lv_checkbox.d ./HardWare/lvgl/src/widgets/lv_checkbox.o ./HardWare/lvgl/src/widgets/lv_checkbox.su ./HardWare/lvgl/src/widgets/lv_dropdown.cyclo ./HardWare/lvgl/src/widgets/lv_dropdown.d ./HardWare/lvgl/src/widgets/lv_dropdown.o ./HardWare/lvgl/src/widgets/lv_dropdown.su ./HardWare/lvgl/src/widgets/lv_img.cyclo ./HardWare/lvgl/src/widgets/lv_img.d ./HardWare/lvgl/src/widgets/lv_img.o ./HardWare/lvgl/src/widgets/lv_img.su ./HardWare/lvgl/src/widgets/lv_label.cyclo ./HardWare/lvgl/src/widgets/lv_label.d ./HardWare/lvgl/src/widgets/lv_label.o ./HardWare/lvgl/src/widgets/lv_label.su ./HardWare/lvgl/src/widgets/lv_line.cyclo ./HardWare/lvgl/src/widgets/lv_line.d ./HardWare/lvgl/src/widgets/lv_line.o ./HardWare/lvgl/src/widgets/lv_line.su ./HardWare/lvgl/src/widgets/lv_objx_templ.cyclo ./HardWare/lvgl/src/widgets/lv_objx_templ.d ./HardWare/lvgl/src/widgets/lv_objx_templ.o ./HardWare/lvgl/src/widgets/lv_objx_templ.su ./HardWare/lvgl/src/widgets/lv_roller.cyclo ./HardWare/lvgl/src/widgets/lv_roller.d ./HardWare/lvgl/src/widgets/lv_roller.o ./HardWare/lvgl/src/widgets/lv_roller.su ./HardWare/lvgl/src/widgets/lv_slider.cyclo ./HardWare/lvgl/src/widgets/lv_slider.d ./HardWare/lvgl/src/widgets/lv_slider.o ./HardWare/lvgl/src/widgets/lv_slider.su ./HardWare/lvgl/src/widgets/lv_switch.cyclo ./HardWare/lvgl/src/widgets/lv_switch.d ./HardWare/lvgl/src/widgets/lv_switch.o ./HardWare/lvgl/src/widgets/lv_switch.su ./HardWare/lvgl/src/widgets/lv_table.cyclo ./HardWare/lvgl/src/widgets/lv_table.d ./HardWare/lvgl/src/widgets/lv_table.o ./HardWare/lvgl/src/widgets/lv_table.su ./HardWare/lvgl/src/widgets/lv_textarea.cyclo ./HardWare/lvgl/src/widgets/lv_textarea.d ./HardWare/lvgl/src/widgets/lv_textarea.o ./HardWare/lvgl/src/widgets/lv_textarea.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-widgets

