################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/board.c \
../Core/Src/challenge_1.c \
../Core/Src/challenge_2.c \
../Core/Src/challenge_3.c \
../Core/Src/challenge_4.c \
../Core/Src/data_handler.c \
../Core/Src/led.c \
../Core/Src/login.c \
../Core/Src/main.c \
../Core/Src/ptu_i2c.c \
../Core/Src/serial.c \
../Core/Src/serialise.c \
../Core/Src/servo.c \
../Core/Src/stm32f3xx_hal_msp.c \
../Core/Src/stm32f3xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f3xx.c \
../Core/Src/waiting.c 

OBJS += \
./Core/Src/board.o \
./Core/Src/challenge_1.o \
./Core/Src/challenge_2.o \
./Core/Src/challenge_3.o \
./Core/Src/challenge_4.o \
./Core/Src/data_handler.o \
./Core/Src/led.o \
./Core/Src/login.o \
./Core/Src/main.o \
./Core/Src/ptu_i2c.o \
./Core/Src/serial.o \
./Core/Src/serialise.o \
./Core/Src/servo.o \
./Core/Src/stm32f3xx_hal_msp.o \
./Core/Src/stm32f3xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f3xx.o \
./Core/Src/waiting.o 

C_DEPS += \
./Core/Src/board.d \
./Core/Src/challenge_1.d \
./Core/Src/challenge_2.d \
./Core/Src/challenge_3.d \
./Core/Src/challenge_4.d \
./Core/Src/data_handler.d \
./Core/Src/led.d \
./Core/Src/login.d \
./Core/Src/main.d \
./Core/Src/ptu_i2c.d \
./Core/Src/serial.d \
./Core/Src/serialise.d \
./Core/Src/servo.d \
./Core/Src/stm32f3xx_hal_msp.d \
./Core/Src/stm32f3xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f3xx.d \
./Core/Src/waiting.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/board.cyclo ./Core/Src/board.d ./Core/Src/board.o ./Core/Src/board.su ./Core/Src/challenge_1.cyclo ./Core/Src/challenge_1.d ./Core/Src/challenge_1.o ./Core/Src/challenge_1.su ./Core/Src/challenge_2.cyclo ./Core/Src/challenge_2.d ./Core/Src/challenge_2.o ./Core/Src/challenge_2.su ./Core/Src/challenge_3.cyclo ./Core/Src/challenge_3.d ./Core/Src/challenge_3.o ./Core/Src/challenge_3.su ./Core/Src/challenge_4.cyclo ./Core/Src/challenge_4.d ./Core/Src/challenge_4.o ./Core/Src/challenge_4.su ./Core/Src/data_handler.cyclo ./Core/Src/data_handler.d ./Core/Src/data_handler.o ./Core/Src/data_handler.su ./Core/Src/led.cyclo ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/login.cyclo ./Core/Src/login.d ./Core/Src/login.o ./Core/Src/login.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/ptu_i2c.cyclo ./Core/Src/ptu_i2c.d ./Core/Src/ptu_i2c.o ./Core/Src/ptu_i2c.su ./Core/Src/serial.cyclo ./Core/Src/serial.d ./Core/Src/serial.o ./Core/Src/serial.su ./Core/Src/serialise.cyclo ./Core/Src/serialise.d ./Core/Src/serialise.o ./Core/Src/serialise.su ./Core/Src/servo.cyclo ./Core/Src/servo.d ./Core/Src/servo.o ./Core/Src/servo.su ./Core/Src/stm32f3xx_hal_msp.cyclo ./Core/Src/stm32f3xx_hal_msp.d ./Core/Src/stm32f3xx_hal_msp.o ./Core/Src/stm32f3xx_hal_msp.su ./Core/Src/stm32f3xx_it.cyclo ./Core/Src/stm32f3xx_it.d ./Core/Src/stm32f3xx_it.o ./Core/Src/stm32f3xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f3xx.cyclo ./Core/Src/system_stm32f3xx.d ./Core/Src/system_stm32f3xx.o ./Core/Src/system_stm32f3xx.su ./Core/Src/waiting.cyclo ./Core/Src/waiting.d ./Core/Src/waiting.o ./Core/Src/waiting.su

.PHONY: clean-Core-2f-Src

