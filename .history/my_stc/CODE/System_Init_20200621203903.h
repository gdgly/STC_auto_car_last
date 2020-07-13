/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-18 16:25:59
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-21 20:39:03
 */ 
#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "common.h"
#include "headfile.h"

extern uchar System_OK;

extern float Zero_Angle;
extern float Difference,Differencelast;
extern int8 Dif_Nor_coefficient;
extern int8 K1,K2,K3;
extern float LAD,RAD,MAD,LADC,RADC;
extern int   I_LAD,I_LADC,I_MAD,I_RADC,I_RAD,
             O_I;
extern float Voltage;
void System_Init();		 //所有模块初始化
void Read_Switch();

#endif