################################################################################
# MRS Version: {"version":"1.8.5","date":"2023/05/22"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/IIC.c \
../User/ch32v30x_it.c \
../User/es8388.c \
../User/lcd.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/IIC.o \
./User/ch32v30x_it.o \
./User/es8388.o \
./User/lcd.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/IIC.d \
./User/ch32v30x_it.d \
./User/es8388.d \
./User/lcd.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\opench-ch32v307\firmware\TcpClient\NetLib" -I"D:\opench-ch32v307\firmware\SRC\Core" -I"D:\opench-ch32v307\firmware\SRC\Debug" -I"D:\opench-ch32v307\firmware\SRC\Peripheral\inc" -I"D:\opench-ch32v307\firmware\TcpClient\User" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

