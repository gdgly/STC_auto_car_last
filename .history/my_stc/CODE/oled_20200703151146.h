/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-29 16:53:09
 * @LastEditTime : 2020-07-03 15:11:40
 */ 
#ifnde_OLED12864_H_
#define _OLED12864_H_
#include "common.h"
#include "headfile.h"
      //屏幕
#define D0     P25//初始化记得要改
#define D1     P23
#define RST    P20
#define DC     P21
#define CS     P22

#define DC_HIGH DC = 1
#define DC_LOW  DC = 0

#define D0_HIGH D0 = 1
#define D0_LOW  D0 = 0

#define D1_HIGH D1 = 1
#define D1_LOW  D1 = 0

#define RST_HIGH RST = 1
#define RST_LOW  RST = 0
#define CS_HIGH  CS = 1
#define CS_LOW   CS = 0

 extern uint8  reverse; //反转显示
 
 void OLED_Set_Pos(uint8 x, uint8 y);
 void OLED_Init(void);
 void OLED_CLS(void);
 //重载OLED_WrDat
 void OLED_WrDat(uint8 dat, uint8 revs);
 //显示一个6x8标准ASCII字符
void OLED_P6x8Char(char ch);
 void OLED_P6x8Str(uint8 x,uint8 y,char ch[]);
//将一个char型数转换成3位数进行显示 
void OLED_PrintValueC(unsigned char x, unsigned char y, int dat);
//功能描述：将一个int型数转换成5位数进行显示
void OLED_PrintValueI(unsigned char x, unsigned char y, int dat);
//显示unsigned int型
void OLED_PrintValueFP(unsigned int dat, unsigned char num);
//功能描述：将一个float型数转换成整数部分5位带小数和符号的数据并进行显示
void OLED_PrintValueF(unsigned char x, unsigned char y, float dat, unsigned char num);
void OLED_Fill(uint8 dat);

#endif