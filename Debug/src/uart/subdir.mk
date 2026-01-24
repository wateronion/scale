################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/uart/uart.c 

C_DEPS += \
./src/uart/uart.d 

OBJS += \
./src/uart/uart.o 

SREC += \
scale.srec 

MAP += \
scale.map 


# Each subdirectory must supply rules for building sources it contributes
src/uart/%.o: ../src/uart/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"D:/Renesas/workplace/scale/ra_gen" -I"." -I"D:/Renesas/workplace/scale/ra_cfg/fsp_cfg/bsp" -I"D:/Renesas/workplace/scale/ra_cfg/fsp_cfg" -I"D:/Renesas/workplace/scale/src" -I"D:/Renesas/workplace/scale/ra/fsp/inc" -I"D:/Renesas/workplace/scale/ra/fsp/inc/api" -I"D:/Renesas/workplace/scale/ra/fsp/inc/instances" -I"D:/Renesas/workplace/scale/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

