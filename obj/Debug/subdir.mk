################################################################################
# MRS Version: {"version":"1.8.5","date":"2023/05/22"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/opench-ch32v307/firmware/SRC/Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/debug.o: D:/opench-ch32v307/firmware/SRC/Debug/debug.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\opench-ch32v307\firmware\TcpClient\NetLib" -I"D:\opench-ch32v307\firmware\SRC\Core" -I"D:\opench-ch32v307\firmware\SRC\Debug" -I"D:\opench-ch32v307\firmware\SRC\Peripheral\inc" -I"D:\opench-ch32v307\firmware\TcpClient\User" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

