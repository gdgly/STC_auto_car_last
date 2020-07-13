/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-21 20:30:40
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-21 21:01:14
 */ 
#ifndef __BALANCE_H__
#define __BALANCE_H__
#include "common.h"
#include "headfile.h"
extern S_FLOAT_XYZ 
	GYRO_Real,		// 陀螺仪转化后的数据
	ACC_Real,		// 加速度计转化后的数据
	Attitude_Angle,	// 当前角度
	Last_Angle,		// 上次角度
	Target_Angle	// 目标角度
	

extern S_INT16_XYZ
	GYRO,			// 陀螺仪原始数据
	GYRO_Offset,	// 陀螺仪温飘
	GYRO_Last,		// 陀螺仪上次数据
	ACC, 			// 加速度计数据
	ACC_Offset,		// 加速度计温飘
	ACC_Last;		// 加速度计上次数据
extern S_INT32_XYZ
	Tar_Ang_Vel,	// 目标角速度
	Tar_Ang_Vel_Last	// 上次目标角速度
    Target_Angle_Grow     // 目标角度增长
    Tar_Ang_Vel_Grow;       // 目标角速度增长
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