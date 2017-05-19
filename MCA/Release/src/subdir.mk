################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ClientMessenger.cpp \
../src/MThread.cpp \
../src/MultipleTCPSocketsListener.cpp \
../src/Session.cpp \
../src/TCPSocket.cpp \
../src/UDPSocket.cpp 

OBJS += \
./src/ClientMessenger.o \
./src/MThread.o \
./src/MultipleTCPSocketsListener.o \
./src/Session.o \
./src/TCPSocket.o \
./src/UDPSocket.o 

CPP_DEPS += \
./src/ClientMessenger.d \
./src/MThread.d \
./src/MultipleTCPSocketsListener.d \
./src/Session.d \
./src/TCPSocket.d \
./src/UDPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


