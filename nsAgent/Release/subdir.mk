################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../novusAgent.c \
../nsRadioSer.c \
../nsStartup.c \
../nsStr.c \
../nsStrParser.c \
../nsTrap.c 

OBJS += \
./novusAgent.o \
./nsRadioSer.o \
./nsStartup.o \
./nsStr.o \
./nsStrParser.o \
./nsTrap.o 

C_DEPS += \
./novusAgent.d \
./nsRadioSer.d \
./nsStartup.d \
./nsStr.d \
./nsStrParser.d \
./nsTrap.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
#	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	gcc -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


