################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../StepperDriver/EasyDriver.cpp 

OBJS += \
./StepperDriver/EasyDriver.o 

CPP_DEPS += \
./StepperDriver/EasyDriver.d 


# Each subdirectory must supply rules for building sources it contributes
StepperDriver/%.o: ../StepperDriver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=gnu++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


