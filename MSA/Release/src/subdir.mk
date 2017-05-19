################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Dispatcher.cpp \
../src/LoginAndSignUp.cpp \
../src/MThread.cpp \
../src/MultipleTCPSocketsListener.cpp \
../src/Server.cpp \
../src/TCPSocket.cpp \
../src/UDPSocket.cpp \
../src/User.cpp 

OBJS += \
./src/Dispatcher.o \
./src/LoginAndSignUp.o \
./src/MThread.o \
./src/MultipleTCPSocketsListener.o \
./src/Server.o \
./src/TCPSocket.o \
./src/UDPSocket.o \
./src/User.o 

CPP_DEPS += \
./src/Dispatcher.d \
./src/LoginAndSignUp.d \
./src/MThread.d \
./src/MultipleTCPSocketsListener.d \
./src/Server.d \
./src/TCPSocket.d \
./src/UDPSocket.d \
./src/User.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


