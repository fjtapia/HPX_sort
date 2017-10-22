################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/francisco/Code/HPX_sort/benchmark/Linux/benchmark.cpp 

OBJS += \
./benchmark.o 

CPP_DEPS += \
./benchmark.d 


# Each subdirectory must supply rules for building sources it contributes
benchmark.o: /home/francisco/Code/HPX_sort/benchmark/Linux/benchmark.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I/home/francisco/Code/hpx -I/home/francisco/Code/HPX_sort/benchmark/include -I/home/francisco/Code/HPX_sort/include -O3 -Wall -c -fmessage-length=0 -march=native -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


