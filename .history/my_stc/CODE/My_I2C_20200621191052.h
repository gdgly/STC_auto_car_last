/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-18 16:22:51
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-21 19:10:47
 */ 
#include "common.h"
#include "headfile.h"

void Delay_us(uchar cnt);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uchar dat);
uchar I2C_RecvByte(bit _ack);
uchar 