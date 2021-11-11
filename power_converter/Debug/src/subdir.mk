################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/buttons.c \
../src/power_converter.c \
../src/settings_ui.c \
../src/tasks.c 

OBJS += \
./src/buttons.o \
./src/power_converter.o \
./src/settings_ui.o \
./src/tasks.o 

C_DEPS += \
./src/buttons.d \
./src/power_converter.d \
./src/settings_ui.d \
./src/tasks.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../power_converter_bsp/ps7_cortexa9_0/include -I"/home/xd/workspace_assignment/zybo_platform" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


