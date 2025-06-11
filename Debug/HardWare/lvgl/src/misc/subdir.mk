################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/src/misc/lv_anim.c \
../HardWare/lvgl/src/misc/lv_anim_timeline.c \
../HardWare/lvgl/src/misc/lv_area.c \
../HardWare/lvgl/src/misc/lv_async.c \
../HardWare/lvgl/src/misc/lv_bidi.c \
../HardWare/lvgl/src/misc/lv_color.c \
../HardWare/lvgl/src/misc/lv_fs.c \
../HardWare/lvgl/src/misc/lv_gc.c \
../HardWare/lvgl/src/misc/lv_ll.c \
../HardWare/lvgl/src/misc/lv_log.c \
../HardWare/lvgl/src/misc/lv_lru.c \
../HardWare/lvgl/src/misc/lv_math.c \
../HardWare/lvgl/src/misc/lv_mem.c \
../HardWare/lvgl/src/misc/lv_printf.c \
../HardWare/lvgl/src/misc/lv_style.c \
../HardWare/lvgl/src/misc/lv_style_gen.c \
../HardWare/lvgl/src/misc/lv_templ.c \
../HardWare/lvgl/src/misc/lv_timer.c \
../HardWare/lvgl/src/misc/lv_tlsf.c \
../HardWare/lvgl/src/misc/lv_txt.c \
../HardWare/lvgl/src/misc/lv_txt_ap.c \
../HardWare/lvgl/src/misc/lv_utils.c 

C_DEPS += \
./HardWare/lvgl/src/misc/lv_anim.d \
./HardWare/lvgl/src/misc/lv_anim_timeline.d \
./HardWare/lvgl/src/misc/lv_area.d \
./HardWare/lvgl/src/misc/lv_async.d \
./HardWare/lvgl/src/misc/lv_bidi.d \
./HardWare/lvgl/src/misc/lv_color.d \
./HardWare/lvgl/src/misc/lv_fs.d \
./HardWare/lvgl/src/misc/lv_gc.d \
./HardWare/lvgl/src/misc/lv_ll.d \
./HardWare/lvgl/src/misc/lv_log.d \
./HardWare/lvgl/src/misc/lv_lru.d \
./HardWare/lvgl/src/misc/lv_math.d \
./HardWare/lvgl/src/misc/lv_mem.d \
./HardWare/lvgl/src/misc/lv_printf.d \
./HardWare/lvgl/src/misc/lv_style.d \
./HardWare/lvgl/src/misc/lv_style_gen.d \
./HardWare/lvgl/src/misc/lv_templ.d \
./HardWare/lvgl/src/misc/lv_timer.d \
./HardWare/lvgl/src/misc/lv_tlsf.d \
./HardWare/lvgl/src/misc/lv_txt.d \
./HardWare/lvgl/src/misc/lv_txt_ap.d \
./HardWare/lvgl/src/misc/lv_utils.d 

OBJS += \
./HardWare/lvgl/src/misc/lv_anim.o \
./HardWare/lvgl/src/misc/lv_anim_timeline.o \
./HardWare/lvgl/src/misc/lv_area.o \
./HardWare/lvgl/src/misc/lv_async.o \
./HardWare/lvgl/src/misc/lv_bidi.o \
./HardWare/lvgl/src/misc/lv_color.o \
./HardWare/lvgl/src/misc/lv_fs.o \
./HardWare/lvgl/src/misc/lv_gc.o \
./HardWare/lvgl/src/misc/lv_ll.o \
./HardWare/lvgl/src/misc/lv_log.o \
./HardWare/lvgl/src/misc/lv_lru.o \
./HardWare/lvgl/src/misc/lv_math.o \
./HardWare/lvgl/src/misc/lv_mem.o \
./HardWare/lvgl/src/misc/lv_printf.o \
./HardWare/lvgl/src/misc/lv_style.o \
./HardWare/lvgl/src/misc/lv_style_gen.o \
./HardWare/lvgl/src/misc/lv_templ.o \
./HardWare/lvgl/src/misc/lv_timer.o \
./HardWare/lvgl/src/misc/lv_tlsf.o \
./HardWare/lvgl/src/misc/lv_txt.o \
./HardWare/lvgl/src/misc/lv_txt_ap.o \
./HardWare/lvgl/src/misc/lv_utils.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/src/misc/%.o HardWare/lvgl/src/misc/%.su HardWare/lvgl/src/misc/%.cyclo: ../HardWare/lvgl/src/misc/%.c HardWare/lvgl/src/misc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-src-2f-misc

clean-HardWare-2f-lvgl-2f-src-2f-misc:
	-$(RM) ./HardWare/lvgl/src/misc/lv_anim.cyclo ./HardWare/lvgl/src/misc/lv_anim.d ./HardWare/lvgl/src/misc/lv_anim.o ./HardWare/lvgl/src/misc/lv_anim.su ./HardWare/lvgl/src/misc/lv_anim_timeline.cyclo ./HardWare/lvgl/src/misc/lv_anim_timeline.d ./HardWare/lvgl/src/misc/lv_anim_timeline.o ./HardWare/lvgl/src/misc/lv_anim_timeline.su ./HardWare/lvgl/src/misc/lv_area.cyclo ./HardWare/lvgl/src/misc/lv_area.d ./HardWare/lvgl/src/misc/lv_area.o ./HardWare/lvgl/src/misc/lv_area.su ./HardWare/lvgl/src/misc/lv_async.cyclo ./HardWare/lvgl/src/misc/lv_async.d ./HardWare/lvgl/src/misc/lv_async.o ./HardWare/lvgl/src/misc/lv_async.su ./HardWare/lvgl/src/misc/lv_bidi.cyclo ./HardWare/lvgl/src/misc/lv_bidi.d ./HardWare/lvgl/src/misc/lv_bidi.o ./HardWare/lvgl/src/misc/lv_bidi.su ./HardWare/lvgl/src/misc/lv_color.cyclo ./HardWare/lvgl/src/misc/lv_color.d ./HardWare/lvgl/src/misc/lv_color.o ./HardWare/lvgl/src/misc/lv_color.su ./HardWare/lvgl/src/misc/lv_fs.cyclo ./HardWare/lvgl/src/misc/lv_fs.d ./HardWare/lvgl/src/misc/lv_fs.o ./HardWare/lvgl/src/misc/lv_fs.su ./HardWare/lvgl/src/misc/lv_gc.cyclo ./HardWare/lvgl/src/misc/lv_gc.d ./HardWare/lvgl/src/misc/lv_gc.o ./HardWare/lvgl/src/misc/lv_gc.su ./HardWare/lvgl/src/misc/lv_ll.cyclo ./HardWare/lvgl/src/misc/lv_ll.d ./HardWare/lvgl/src/misc/lv_ll.o ./HardWare/lvgl/src/misc/lv_ll.su ./HardWare/lvgl/src/misc/lv_log.cyclo ./HardWare/lvgl/src/misc/lv_log.d ./HardWare/lvgl/src/misc/lv_log.o ./HardWare/lvgl/src/misc/lv_log.su ./HardWare/lvgl/src/misc/lv_lru.cyclo ./HardWare/lvgl/src/misc/lv_lru.d ./HardWare/lvgl/src/misc/lv_lru.o ./HardWare/lvgl/src/misc/lv_lru.su ./HardWare/lvgl/src/misc/lv_math.cyclo ./HardWare/lvgl/src/misc/lv_math.d ./HardWare/lvgl/src/misc/lv_math.o ./HardWare/lvgl/src/misc/lv_math.su ./HardWare/lvgl/src/misc/lv_mem.cyclo ./HardWare/lvgl/src/misc/lv_mem.d ./HardWare/lvgl/src/misc/lv_mem.o ./HardWare/lvgl/src/misc/lv_mem.su ./HardWare/lvgl/src/misc/lv_printf.cyclo ./HardWare/lvgl/src/misc/lv_printf.d ./HardWare/lvgl/src/misc/lv_printf.o ./HardWare/lvgl/src/misc/lv_printf.su ./HardWare/lvgl/src/misc/lv_style.cyclo ./HardWare/lvgl/src/misc/lv_style.d ./HardWare/lvgl/src/misc/lv_style.o ./HardWare/lvgl/src/misc/lv_style.su ./HardWare/lvgl/src/misc/lv_style_gen.cyclo ./HardWare/lvgl/src/misc/lv_style_gen.d ./HardWare/lvgl/src/misc/lv_style_gen.o ./HardWare/lvgl/src/misc/lv_style_gen.su ./HardWare/lvgl/src/misc/lv_templ.cyclo ./HardWare/lvgl/src/misc/lv_templ.d ./HardWare/lvgl/src/misc/lv_templ.o ./HardWare/lvgl/src/misc/lv_templ.su ./HardWare/lvgl/src/misc/lv_timer.cyclo ./HardWare/lvgl/src/misc/lv_timer.d ./HardWare/lvgl/src/misc/lv_timer.o ./HardWare/lvgl/src/misc/lv_timer.su ./HardWare/lvgl/src/misc/lv_tlsf.cyclo ./HardWare/lvgl/src/misc/lv_tlsf.d ./HardWare/lvgl/src/misc/lv_tlsf.o ./HardWare/lvgl/src/misc/lv_tlsf.su ./HardWare/lvgl/src/misc/lv_txt.cyclo ./HardWare/lvgl/src/misc/lv_txt.d ./HardWare/lvgl/src/misc/lv_txt.o ./HardWare/lvgl/src/misc/lv_txt.su ./HardWare/lvgl/src/misc/lv_txt_ap.cyclo ./HardWare/lvgl/src/misc/lv_txt_ap.d ./HardWare/lvgl/src/misc/lv_txt_ap.o ./HardWare/lvgl/src/misc/lv_txt_ap.su ./HardWare/lvgl/src/misc/lv_utils.cyclo ./HardWare/lvgl/src/misc/lv_utils.d ./HardWare/lvgl/src/misc/lv_utils.o ./HardWare/lvgl/src/misc/lv_utils.su

.PHONY: clean-HardWare-2f-lvgl-2f-src-2f-misc

