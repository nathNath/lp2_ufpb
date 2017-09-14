################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Carro.cpp \
../Main.cpp \
../Parque.cpp \
../Passageiro.cpp \
../Thread.cpp 

OBJS += \
./Carro.o \
./Main.o \
./Parque.o \
./Passageiro.o \
./Thread.o 

CPP_DEPS += \
./Carro.d \
./Main.d \
./Parque.d \
./Passageiro.d \
./Thread.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Main.o: ../Main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"Main.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


