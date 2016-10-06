################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SimpleGPIO/SimpleGPIO.cpp 

OBJS += \
./SimpleGPIO/SimpleGPIO.o 

CPP_DEPS += \
./SimpleGPIO/SimpleGPIO.d 


# Each subdirectory must supply rules for building sources it contributes
SimpleGPIO/%.o: ../SimpleGPIO/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


