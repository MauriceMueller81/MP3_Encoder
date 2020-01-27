################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CApplinput.cpp \
../CEncoderLogger.cpp \
../CInputWaveFile.cpp \
../CLameEncoder.cpp \
../COutputEncodedFile.cpp \
../MP3Encoder.cpp 

OBJS += \
./CApplinput.o \
./CEncoderLogger.o \
./CInputWaveFile.o \
./CLameEncoder.o \
./COutputEncodedFile.o \
./MP3Encoder.o 

CPP_DEPS += \
./CApplinput.d \
./CEncoderLogger.d \
./CInputWaveFile.d \
./CLameEncoder.d \
./COutputEncodedFile.d \
./MP3Encoder.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/lame -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


