################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/francisco/Code/HPX_sort/test/detail/test_parallel_sort_original.cpp 

OBJS += \
./test_parallel_sort_original.o 

CPP_DEPS += \
./test_parallel_sort_original.d 


# Each subdirectory must supply rules for building sources it contributes
test_parallel_sort_original.o: /home/francisco/Code/HPX_sort/test/detail/test_parallel_sort_original.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I/home/francisco/Code/hpx/hpx -I/home/francisco/Code/HPX_sort/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


