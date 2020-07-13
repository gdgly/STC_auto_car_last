/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-22 16:49:30
 * @LastEditTime : 2020-07-08 16:35:49
 */ 

#ifndef __DEBUG_H__
#define __DEBUG_H__
#include "common.h"
#include "headfile.h"
#define true  1
#define false 0
//将变量名转化为字符串
#define BYTE0(Temp)       (*(char *)(&Temp))     
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))

extern uint8 KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEY_CEL,KEY_SURE;

extern float Variable[16];

void Testdata_generate();
void OLED_Draw_UI();  //画出界面
void Check_BottonPress();
#endif 