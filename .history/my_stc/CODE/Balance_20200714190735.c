/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-21 20:30:51
 * @LastEditTime : 2020-07-14 19:07:28
 */
#include "Balance.h"
S_FLOAT_XYZ
    GYRO_Real,             // 陀螺仪转化后的数据
    ACC_Real,          // 加速度计转化后的数据
    Attitude_Angle,    // 当前角度
    Last_Angle,        // 上次角度
    Target_Angle,      // 目标角度
    Target_Angle_Grow; // 目标角度增长
S_INT16_XYZ
    GYRO,        // 陀螺仪原始数据
    GYRO_Offset, // 陀螺仪温飘
    GYRO_Last,   // 陀螺仪上次数据
    ACC,         // 加速度计数据
    ACC_Offset,  // 加速度计温飘
    ACC_Last;    // 加速度计上次数据
S_INT32_XYZ
    Tar_Ang_Vel,      // 目标角速度
    Tar_Ang_Vel_Last, // 上次目标角速度
    Tar_Ang_Vel_Grow; // 目标角速度增长
int32
    In_Out_Roundabout_I = 0,
    In_Out_Roundabout_ture_I = 0,
    Out_Roundabout_I = 0,
    Run_I = 0,
    Speed_Now = 0,        // 当前实际速度
    Speed_Now_Last = 0,   // 上次实际速度
    Speed_Min = 0,        // 左右最小速度
    Speed_Set = 0,        // 目标设定速度
    Theory_Duty = 0,      // 理论直立占空比
    Vel_Set = 0,          // 目标转向角速度
    Direct_Parameter = 0, // 转向系数
    Direct_Last = 0,
    Curvature = 0;            // 目标转向半径倒数
float 
    Target_Angle_min = 200,   //跑动前倾最大角度
    Target_Angle_max = 800;   //跑动后仰最大角度
/**** 各种标志位，放定时器中进行时序控制 ****/
char Speed_Flag, Angle_Flag, Ang_Velocity_Flag, Roundabout_cnt = 0, In_Roundabout_Flag = 0, Out_Roundabout_Flag = 0;
/********************* 串级平衡控制 *********************/
// 频率控制在定时器中设置
void Balance_Control(void)
{
  if (Ang_Velocity_Flag) // 直立角速度环	2ms
  {
    Ang_Velocity_Flag = 0;

    MPU6050_GetData(&GYRO, &ACC); // 读取陀螺仪数据
    Data_Filter();
    if (In_Roundabout_Flag)       // 圆环控制
    {
      In_Out_Roundabout_I += GYRO_Real.Z;
      In_Out_Roundabout_ture_I = In_Out_Roundabout_I * Angle_coe;
    }
    /* 角速度环作为最内环控制直立 */  //增量式PID 所以 用  +=
    Theory_Duty += PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y * 10), (int32)(Tar_Ang_Vel.Y)); // 计算直立PWM
    Theory_Duty = range_protect(Theory_Duty, -950, 950);

    if (System_OK)
    {
      // 获取当前速度  角度环作为最内环控制转向   Speed_Min
      Direct_Parameter = PID_Realize_D(&Direct_PID, Direct, (int32)(GYRO_Real.Z * 100), Curvature * Speed_Min); 
      Direct_Parameter = range_protect(Direct_Parameter, -1200, 1200);
    }

    Direct_Last = Direct_Last * 0.2 + Direct_Parameter * 0.8; // 更新上次角速度环结果
    MOTOR_Duty_Left = Theory_Duty - Direct_Last;              // 左右电机根据转向系数调整差速
    MOTOR_Duty_Right = Theory_Duty + Direct_Last;             

    if (Run_Flag)   // Run_Flag=1 开跑
    {
      if (Stop_Flag)   // Stop_Flag=1 正常电机控制
        MOTOR_Control(MOTOR_Duty_Left, MOTOR_Duty_Right); // 控制左右电机
      else
      {
        MOTOR_Control(-300, -300); // 停车电机控制
        delay_ms(500);
        MOTOR_Control(0, 0); // 停车电机控制
        delay_ms(7000);
        Run_I = 0;
        Stop_Flag = 1;
      }
    }
    else           // Run_Flag=0  停止
    {
      MOTOR_Control(0, 0); // 关电机控制
    }
    Another_Get_Attitude();
  }
  if (Angle_Flag) // 直立角度环 10ms
  {
    Speed_Measure();
    Angle_Flag = 0;
    /* 角度环加到角速度环上串级控制 */ // 结果为放大10倍的目标角速度
    Tar_Ang_Vel.Y = PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y * 100), (int32)Target_Angle.Y);
    Tar_Ang_Vel.Y = range_protect(Tar_Ang_Vel.Y, -1500, 1500); // 注意正负号
  }
  if (Speed_Flag) // 速度环     100ms
  {
    Speed_Flag = 0;
    /* 速度环加到角度环上串级控制 */  // 结果为目标角度   角度环为内环
    if (Run_I < 10000)
    {
      Target_Angle.Y = Target_Angle_min - 1000;
    }  //以下PID负号 原因是速度离期望偏差大的过程中角度（前进方向）变小，假设速度为40，现在变为50，
    else//                                                           说明角度变小，假设本目标角度100，
    {  // 结果为放大100倍的目标角度                                    那么角度误差变大而输出目标角度要为负号 
      Target_Angle.Y = -PID_Realize(&MOTOR_PID, MOTOR, Speed_Now, Speed_Set);                
      Target_Angle.Y += Zero_Angle * 100;    //结果叠加在零点上                                                 
      Target_Angle.Y = range_protect((int32)Target_Angle.Y, Target_Angle_min, Target_Angle_max); 
    }
  }
}
/* 初始化用到的一些变量 */
void Balance_Init(void)
{
  Attitude_Angle.Y = 0;
  Target_Angle.Y = 0;
  Tar_Ang_Vel.Y = 0;
  Tar_Ang_Vel.Z = 0;
}
/******** 限幅保护 *********/
int32 range_protect(int32 duty, int32 min, int32 max) //限幅保护
{
  if (duty >= max)
  {
    return max;
  }
  if (duty <= min)
  {
    return min;
  }
  else
  {
    return duty;
  }
}
/******** 限幅保护2 *********/
int32 range_protect_main(int32 duty, int32 min, int32 max) //限幅保护
{
  if (duty >= max)
  {
    return max;
  }
  if (duty <= min)
  {
    return min;
  }
  else
  {
    return duty;
  }
}