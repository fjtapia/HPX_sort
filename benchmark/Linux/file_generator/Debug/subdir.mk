################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/francisco/Code/HPX_sort/benchmark/Linux/file_generator.cpp 

OBJS += \
./file_generator.o 

CPP_DEPS += \
./file_generator.d 


# Each subdirectory must supply rules for building sources it contributes
file_generator.o: /home/francisco/Code/HPX_sort/benchmark/Linux/file_generator.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I/home/francisco/Code/HPX_sort/benchmark/include -I/home/francisco/Code/HPX_sort/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


