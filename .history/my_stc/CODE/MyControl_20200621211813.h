/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-18 16:25:13
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-21 21:18:13
 */ 

#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "common.h"
#include "headfile.h"
#define PAI             3.14159265358979f            //圆周率
#define Pulse_CM_coe    0.01986491855383f            //厘米脉冲转化系数(大编码器齿)
#define Angle_coe       0.002154581f                        //角度转化系数
#define BodyR           10.0f                        //轴向半径
#define Voltage_co      0.0398f                   //实际电压转化系数
#define Voltage_st      7.5f                         //标准电压
//
#define BUZZ_open   P44 = 0;
#define BUZZ_shut   P44 = 1;

#define MOTOR1_PWM  PWM1P_P60    // 左 轮 正 转
#define MOTOR2_PWM  PWM2P_P62    // 左 轮 反 转
#define MOTOR3_PWM  PWM3P_P64    // 右 轮 正 转
#define MOTOR4_PWM  PWM4P_P66    // 右 轮 反 转
#define MOTOR_HZ    (18000)	//滑行模式下，频率应该是 30~100。
								//常规模式下，频率应该是 20k 左右
#define MOTOR_MAX   1000

extern int32 MOTOR_Duty_Left, MOTOR_Duty_Right;
extern int32 MOTOR_Speed_Left, MOTOR_Speed_Right;
extern int32 MOTOR_Left_Acc, MOTOR_Right_Acc;

extern uchar Run_Flag;
extern uchar Stop_Flag;

extern float Speed_dev,Speed_sum,Steer_Radius;
extern int Fres;
/*********** 函数声明 ************/
void Speed_Measure(void);	//电机速度测量
void AD_LVBO();
void MOTOR_Control(int32 LDuty, int32 RDuty);	// 电机控制
#endif