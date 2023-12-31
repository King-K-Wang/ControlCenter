#include <IIC.h>
#include "debug.h"
#include "es8388.h"



/**
 * @brief	控制I2S的SD数据方向。
 *
 * @param   void
 *
 * @return  void
 */
void ES8388_Contrl_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/**
 * @brief	ES8388初始化
 *
 * @param   void
 *
 * @return  u8		0,初始化正常，其他,错误代码
 */
u8 ES8388_Init(void)
{
    ES8388_Contrl_Init();		//配置模拟开关控制SD数据方向
    IIC_Init(200000,0x01);                 //初始化IIC接口

    //软复位ES8388
    ES8388_Write_Reg(0, 0x80);
    ES8388_Write_Reg(0, 0x00);
    Delay_Ms(100);				//等待复位

    ES8388_Write_Reg(0x01, 0x58);
    ES8388_Write_Reg(0x01, 0x50);
    ES8388_Write_Reg(0x02, 0xF3);
    ES8388_Write_Reg(0x02, 0xF0);
	
	ES8388_Write_Reg(0x03, 0x09);	//麦克风偏置电源关闭
	ES8388_Write_Reg(0x00, 0x06);	//使能参考		500K驱动使能
	ES8388_Write_Reg(0x04, 0x00);	//DAC电源管理，不打开任何通道
	ES8388_Write_Reg(0x08, 0x00);	//MCLK不分频
    ES8388_Write_Reg(0x2B, 0x80);	//DAC控制	DACLRC与ADCLRC相同
   
    ES8388_Write_Reg(0x09, 0x88);	//ADC L/R PGA增益配置为+24dB
    ES8388_Write_Reg(0x0C, 0x4C);	//ADC	数据选择为left data = left ADC, right data = left ADC 	音频数据为16bit
    ES8388_Write_Reg(0x0D, 0x12);	//ADC配置 MCLK/采样率=250
    ES8388_Write_Reg(0x10, 0x00);	//ADC数字音量控制将信号衰减 L	设置为最小！！！
    ES8388_Write_Reg(0x11, 0x00);	//ADC数字音量控制将信号衰减 R	设置为最小！！！
	
    ES8388_Write_Reg(0x17, 0x18);	//DAC 音频数据为16bit
    ES8388_Write_Reg(0x18, 0x12);	//DAC	配置 MCLK/采样率=256
    ES8388_Write_Reg(0x1A, 0x00);	//DAC数字音量控制将信号衰减 L	设置为最小！！！
    ES8388_Write_Reg(0x1B, 0x00);	//DAC数字音量控制将信号衰减 R	设置为最小！！！
    ES8388_Write_Reg(0x27, 0xB8);	//L混频器
    ES8388_Write_Reg(0x2A, 0xB8);	//R混频器
    Delay_Ms(100);
    return 0;
}




/**
 * @brief	写数据到ES8388寄存器
 *
 * @param   reg		寄存器地址
 * @param   val		要写入寄存器的值
 *
 * @return  u8		0,成功，其他,错误代码
 */
u8 ES8388_Write_Reg(u8 reg, u8 val)
{
    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY ) != RESET );

    I2C_GenerateSTART( I2C2, ENABLE );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress(I2C2,((ES8388_ADDR << 1) | 0),I2C_Direction_Transmitter);
    //发送器件地址+写命令

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );  //等待应答
  
    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_TXE ) ==  RESET );
    I2C_SendData(I2C2,reg); //写寄存器地址

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ); //等待应答
  
	while( I2C_GetFlagStatus( I2C2, I2C_FLAG_TXE ) ==  RESET );
    I2C_SendData(I2C2,val & 0XFF); //发送数据

    I2C_GenerateSTOP( I2C2, ENABLE );
    return 0;
}

/**
 * @brief	从指定地址读出一个数据
 *
 * @param   reg		寄存器地址
 *
 * @return  u8		读到的数据
 */
u8 ES8388_Read_Reg(u8 reg)
{
    u8 temp = 0;

    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C2, ENABLE );

	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress(I2C2,(ES8388_ADDR << 1) | 0X00,I2C_Direction_Transmitter); 
    //发送器件地址+写命令

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );  //等待应答
    I2C_SendData(I2C2,reg);        //写寄存器地址

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );//等待应答

    I2C_GenerateSTART( I2C2, ENABLE );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );

    I2C_Send7bitAddress(I2C2,((ES8388_ADDR << 1) | 0x01),I2C_Direction_Receiver);//发送器件地址+读命令
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) ); //等待应答

    while(I2C_GetFlagStatus( I2C2, I2C_FLAG_RXNE ) ==  RESET);
    temp = I2C_ReceiveData( I2C2 );

    I2C_GenerateSTOP( I2C2, ENABLE );//产生一个停止条件

    return temp;
}

/**
 * @brief	设置I2S工作模式
 *
 * @param   fmt		0,飞利浦标准I2S;1,MSB(左对齐);2,LSB(右对齐);3,PCM/DSP
 * @param   len		0,24bit;1,20bit;2,18bit;3,16bit;4,32bit
 *
 * @return 	void
 */
void ES8388_I2S_Cfg(u8 fmt, u8 len)
{
    fmt &= 0X03;
    len &= 0X07; //限定范围
    ES8388_Write_Reg(23, (fmt << 1) | (len << 3));	//R23,ES8388工作模式设置
}

/**
 * @brief	设置音量大小，音量慢慢增加到最大
 *
 * @param   volume		音量大小(0-33)
 * 						0 – -30dB
 * 						1 – -29dB
 * 						2 – -28dB
 * 						…
 * 						30 – 0dB
 * 						31 – 1dB
 * 						…
 * 						33 – 3dB
 *
 * @return 	void
 */
void ES8388_Set_Volume(u8 volume)
{
    for(u8 i = 0; i < volume; i++)
    {
        ES8388_Write_Reg(0x2E, i);
        ES8388_Write_Reg(0x2F, i);
    }
}

/**
 * @brief	ES8388 DAC/ADC配置
 *
 * @param   dacen	dac使能(0)/关闭(1)
 * @param   adcen	adc使能(0)/关闭(1)
 *
 * @return 	void
 */
void ES8388_ADDA_Cfg(u8 dacen,u8 adcen)
{
	u8 res = 0;
	
	res |= (dacen<<0);
	res |= (adcen<<1);
	res |= (dacen<<2);
	res |= (adcen<<3);
	
	ES8388_Write_Reg(0x02, res);
}

/**
 * @brief	ES8388 DAC输出通道配置
 *
* @param   out		0:通道2输出，1:通道1输出
 *
 * @return 	void
 */
void ES8388_Output_Cfg(u8 out)
{
	ES8388_Write_Reg(0x04, 3<<(out*2+2));
}


/**
 * @brief	ES8388 ADC输出通道配置
 *
* @param   in		0:通道1输入，1:通道2输入
 *
 * @return 	void
 */
void ES8388_Input_Cfg(u8 in)
{
	ES8388_Write_Reg(0x0A,(5*in)<<4);	//ADC1 输入通道选择L/R	INPUT1
}
