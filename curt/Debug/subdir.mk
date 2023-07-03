################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DC_motor.c \
../GPIO.c \
../LCD.c \
../Timer0_pwm.c \
../adc.c \
../lm35.c \
../main.c 

OBJS += \
./DC_motor.o \
./GPIO.o \
./LCD.o \
./Timer0_pwm.o \
./adc.o \
./lm35.o \
./main.o 

C_DEPS += \
./DC_motor.d \
./GPIO.d \
./LCD.d \
./Timer0_pwm.d \
./adc.d \
./lm35.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


