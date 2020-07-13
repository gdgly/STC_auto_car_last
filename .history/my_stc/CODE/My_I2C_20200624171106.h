/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-18 16:22:51
 * @LastEditTime : 2020-06-24 17:11:06
 */ 
#ifndef __My_I2C_H__
#define __My_I2C_H__
#include "common.h"
#include "headfile.h"

typedef enum
{
    IIC_1 = 0,
	
    IIC_2 = 1,
	
    IIC_3 = 2,
	
    IIC_4 = 3,
	
} IICN_enum;


typedef enum
{
    IIC1_SCL_P15 = 0x00,IIC1_SDA_P14,

    IIC2_SCL_P25 = 0x10,IIC2_SDA_P24,

    IIC3_SCL_P77 = 0x20,IIC3_SDA_P76,

    IIC4_SCL_P32 = 0x30,IIC4_SDA_P33,
	
} IIC_PIN_enum;

sbit    SCL = P1^3;
sbit    SDA = P1^5;
extern  bit ack;
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uchar dat);
uchar I2C_RecvByte(bit _ack);
uchar I2C_Single_Write(uchar REG_Address,uchar REG_data);
uchar I2C_Single_Read(uchar REG_Address);
uchar I2C_reads(uchar addr,uchar length,uchar *Data);
uchar I2C_read(uchar addr,uchar *Data);
uchar I2C_WriteBits(uchar addr,uchar bitStart,uchar length,uchar Data);
uchar I2C_WriteBit(uchar addr,uchar bitNum,uchar Data);
uchar I2C_readBit(uchar addr,uchar bitNum,uchar *Data);
uchar I2C_readBits(uchar addr,uchar bitStart,uchar length,uchar *Data);
void I2C_Init(Pin_enum );
#endif