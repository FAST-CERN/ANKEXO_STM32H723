################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HardWare/lvgl/gui/factory_test.c \
../HardWare/lvgl/gui/factory_test_typeface.c \
../HardWare/lvgl/gui/homepage.c \
../HardWare/lvgl/gui/homepage_typeface.c \
../HardWare/lvgl/gui/interface_typeface.c \
../HardWare/lvgl/gui/key.c \
../HardWare/lvgl/gui/sys_set.c 

C_DEPS += \
./HardWare/lvgl/gui/factory_test.d \
./HardWare/lvgl/gui/factory_test_typeface.d \
./HardWare/lvgl/gui/homepage.d \
./HardWare/lvgl/gui/homepage_typeface.d \
./HardWare/lvgl/gui/interface_typeface.d \
./HardWare/lvgl/gui/key.d \
./HardWare/lvgl/gui/sys_set.d 

OBJS += \
./HardWare/lvgl/gui/factory_test.o \
./HardWare/lvgl/gui/factory_test_typeface.o \
./HardWare/lvgl/gui/homepage.o \
./HardWare/lvgl/gui/homepage_typeface.o \
./HardWare/lvgl/gui/interface_typeface.o \
./HardWare/lvgl/gui/key.o \
./HardWare/lvgl/gui/sys_set.o 


# Each subdirectory must supply rules for building sources it contributes
HardWare/lvgl/gui/%.o HardWare/lvgl/gui/%.su HardWare/lvgl/gui/%.cyclo: ../HardWare/lvgl/gui/%.c HardWare/lvgl/gui/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../HardWare/ -I../HardWare/BSP/ -I../HardWare/APP/ -I../HardWare/EPOS/ -I../HardWare/lvgl/ -I../HardWare/lvgl/src/ -I../HardWare/lvgl/porting/ -I../HardWare/lvgl/gui/ -I../HardWare/LED/ -I../HardWare/BSP/BMI088/ -I../HardWare/BSP/BMI088/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HardWare-2f-lvgl-2f-gui

clean-HardWare-2f-lvgl-2f-gui:
	-$(RM) ./HardWare/lvgl/gui/factory_test.cyclo ./HardWare/lvgl/gui/factory_test.d ./HardWare/lvgl/gui/factory_test.o ./HardWare/lvgl/gui/factory_test.su ./HardWare/lvgl/gui/factory_test_typeface.cyclo ./HardWare/lvgl/gui/factory_test_typeface.d ./HardWare/lvgl/gui/factory_test_typeface.o ./HardWare/lvgl/gui/factory_test_typeface.su ./HardWare/lvgl/gui/homepage.cyclo ./HardWare/lvgl/gui/homepage.d ./HardWare/lvgl/gui/homepage.o ./HardWare/lvgl/gui/homepage.su ./HardWare/lvgl/gui/homepage_typeface.cyclo ./HardWare/lvgl/gui/homepage_typeface.d ./HardWare/lvgl/gui/homepage_typeface.o ./HardWare/lvgl/gui/homepage_typeface.su ./HardWare/lvgl/gui/interface_typeface.cyclo ./HardWare/lvgl/gui/interface_typeface.d ./HardWare/lvgl/gui/interface_typeface.o ./HardWare/lvgl/gui/interface_typeface.su ./HardWare/lvgl/gui/key.cyclo ./HardWare/lvgl/gui/key.d ./HardWare/lvgl/gui/key.o ./HardWare/lvgl/gui/key.su ./HardWare/lvgl/gui/sys_set.cyclo ./HardWare/lvgl/gui/sys_set.d ./HardWare/lvgl/gui/sys_set.o ./HardWare/lvgl/gui/sys_set.su

.PHONY: clean-HardWare-2f-lvgl-2f-gui

