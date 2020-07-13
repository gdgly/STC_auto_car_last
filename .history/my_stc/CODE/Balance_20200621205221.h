/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-21 20:30:40
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-21 20:52:21
 */ 
#ifndef __BALANCE_H__
#define __BALANCE_H__
#include "common.h"
#include "headfile.h"
extern S_INT16_XYZ gyro,acc,acc_offset,gyro_offset,ACC_Real,GYRO_Real,Attitude_Angle;
extern int32 
        In_Out_Roundabout_I,
        In_Out_Roundabout_ture_I,
        Out_Roundabout_I,        //出环岛距离
        Run_I,
	Speed_Now,		// 当前实际速度
    Speed_Now_Last,       // 上次实际速度
	Speed_Min,		// 左右最小速度
    Speed_Set, 		// 目标设定速度
	    Vel_Set,		// 目标转向角速度
	    Direct_Parameter,
        Direct_Last,
        Theory_Duty,
        Curvature;  
extern float Target_Angle_min,       //跑动前倾最大角度
             Target_Angle_max;       //跑动后仰最大角度
extern char Speed_Flag, Angle_Flag, Ang_Velocity_Flag,Roundabout_cnt,In_Roundabout_Flag,Out_Roundabout_Flag, DMP_Flag;
int32 range_protect(int32 duty, int32 min, int32 max); //限幅保护
void Balance_Control(void);
void Balance_Init(void);

#endif