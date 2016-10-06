################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BMA180Accelerometer.cpp \
../EasyDriver.cpp \
../SimpleGPIO.cpp \
../TestApplication.cpp 

OBJS += \
./BMA180Accelerometer.o \
./EasyDriver.o \
./SimpleGPIO.o \
./TestApplication.o 

CPP_DEPS += \
./BMA180Accelerometer.d \
./EasyDriver.d \
./SimpleGPIO.d \
./TestApplication.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


