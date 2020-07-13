/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-18 16:24:56
 * @LastEditTime : 2020-06-27 22:52:33
 */

#include "MyControl.h"

uchar Run_Flag = 0;
uchar Stop_Flag = 1;

int32 MOTOR_Duty_Left = 0;
int32 MOTOR_Duty_Right = 0;
int32 MOTOR_Speed_Left = 0;
int32 MOTOR_Speed_Right = 0;
int32 MOTOR_Speed_Left_Last = 0;
int32 MOTOR_Speed_Right_Last = 0;
int32 MOTOR_Left_Acc = 0;
int32 MOTOR_Right_Acc = 0;

float SpeedRw, SpeedLw;
float Speed_dev, Speed_sum, Steer_Radius;
int Fres = 0; // 动态参数变量
/******* 电机速度测量 ********/
void Speed_Measure(void)
{
  static int32 left_data, right_data;
  /******* 右电机速度相关控制 ********/
  MOTOR_Speed_Right = ctimer_count_read(CTIM0_P34) * 100 * Pulse_CM_coe; //右轮
  ctimer_count_clean(CTIM0_P34);
  right_data = P35;
  if (right_data == 1)
    MOTOR_Speed_Right = -MOTOR_Speed_Right;
  if (right_data == 0)
    MOTOR_Speed_Right = MOTOR_Speed_Right;
  MOTOR_Right_Acc = MOTOR_Speed_Right - MOTOR_Speed_Right_Last; // 计算加速度
  if (MOTOR_Right_Acc > 100)
    MOTOR_Speed_Right += 100;
  else if (MOTOR_Right_Acc < -100)
    MOTOR_Speed_Right -= 100;
  else
    MOTOR_Speed_Right = MOTOR_Speed_Right;
  MOTOR_Speed_Right = range_protect(MOTOR_Speed_Right, 0, 500); //限幅保护
  MOTOR_Speed_Right_Last = MOTOR_Speed_Right;
  /******* 右电机速度相关控制结束 ********/
  /******* 左电机速度相关控制 ********/
  MOTOR_Speed_Left = ctimer_count_read(CTIM3_P04) * 100 * Pulse_CM_coe; //左轮
  ctimer_count_clean(CTIM3_P04);
  left_data = P53;
  if (left_data == 1)
    MOTOR_Speed_Left = MOTOR_Speed_Left;
  if (left_data == 0)
    MOTOR_Speed_Left = -MOTOR_Speed_Left;
  MOTOR_Left_Acc = MOTOR_Speed_Left - MOTOR_Speed_Left_Last; // 计算加速度
  if (MOTOR_Left_Acc > 100)
    MOTOR_Speed_Left += 100;
  else if (MOTOR_Left_Acc < -100)
    MOTOR_Speed_Left -= 100;
  else
    MOTOR_Speed_Left = MOTOR_Speed_Left;
  MOTOR_Speed_Left = range_protect(MOTOR_Speed_Left, 0, 500); //限幅保护
  MOTOR_Speed_Left_Last = MOTOR_Speed_Left;
  /*****************转向和直道真实速度计算*****************/
  SpeedLw = MOTOR_Speed_Left + BodyR * GYRO_Real.Z * PAI / 180;
  SpeedRw = MOTOR_Speed_Right - BodyR * GYRO_Real.Z * PAI / 180;
  if (GYRO_Real.Z > 50)
  {
    Speed_Now = SpeedRw; // 左转，用右边
  }
  else if (GYRO_Real.Z < -50)
  {
    Speed_Now = SpeedLw; // 右转，用左边
  }
  else
  {
    Speed_Now = (MOTOR_Speed_Right + MOTOR_Speed_Left) / 2; // 不怎么转，用两边
  }
  if ((Speed_Now - Speed_Now_Last) > 100)
    Speed_Now += 100;
  else if ((Speed_Now - Speed_Now_Last) < -100)
    Speed_Now -= 100;
  else
    Speed_Now = Speed_Now;
  Speed_Now = range_protect(Speed_Now, 0, 500); //限幅保护
  Speed_Now_Last = Speed_Now;

  Run_I += Speed_Now; //起跑距离
  /**********出环岛距离积分**********/
  if (Out_Roundabout_Flag)
  {
    Out_Roundabout_I += Speed_Now;
  }
  /**********转向最小速度计算**********/
  Speed_Min = Speed_Min * 0.1 + Speed_Now * 0.9;
  Speed_Min = range_protect(Speed_Min, 10, 290); //限幅保护
  /**********转向参数选择**********/
  if (Speed_Min <= Speed_Set * 0.3)
    Fres = 0;
  else if (Speed_Min <= Speed_Set * 0.7)
    Fres = 1;
  else if (Speed_Min <= Speed_Set)
    Fres = 2;
  else
    Fres = 3;
}
void AD_LVBO()
{
  uint8 i, j;
#define FILTER_N 5
  float filter_temp, filter_sum_L = 0, filter_sum_LC = 0, filter_sum_M = 0, filter_sum_R = 0, filter_sum_RC = 0;
  float filter_buf_L[FILTER_N];
  float filter_buf_LC[FILTER_N];
  float filter_buf_M[FILTER_N];
  float filter_buf_R[FILTER_N];
  float filter_buf_RC[FILTER_N];
  for (i = 0; i < FILTER_N; i++)
  {
    filter_buf_L[i] = adc_once(ADC_P00, ADC_12BIT);
    filter_buf_LC[i] = adc_once(ADC_P01, ADC_12BIT);
    filter_buf_M[i] = adc_once(ADC_P02, ADC_12BIT);
    filter_buf_R[i] = adc_once(ADC_P03, ADC_12BIT);
    filter_buf_RC[i] = adc_once(ADC_P04, ADC_12BIT);
  }
  // 采样值从小到大排列（冒泡法）
  for (j = 0; j < FILTER_N - 1; j++)
  {
    for (i = 0; i < FILTER_N - 1 - j; i++)
    {
      if (filter_buf_L[i] > filter_buf_L[i + 1])
      {
        filter_temp = filter_buf_L[i];
        filter_buf_L[i] = filter_buf_L[i + 1];
        filter_buf_L[i + 1] = filter_temp;
      }
    }
  }
  for (j = 0; j < FILTER_N - 1; j++)
  {
    for (i = 0; i < FILTER_N - 1 - j; i++)
    {
      if (filter_buf_LC[i] > filter_buf_LC[i + 1])
      {
        filter_temp = filter_buf_LC[i];
        filter_buf_LC[i] = filter_buf_LC[i + 1];
        filter_buf_LC[i + 1] = filter_temp;
      }
    }
  }
  for (j = 0; j < FILTER_N - 1; j++)
  {
    for (i = 0; i < FILTER_N - 1 - j; i++)
    {
      if (filter_buf_M[i] > filter_buf_M[i + 1])
      {
        filter_temp = filter_buf_M[i];
        filter_buf_M[i] = filter_buf_M[i + 1];
        filter_buf_M[i + 1] = filter_temp;
      }
    }
  }
  for (j = 0; j < FILTER_N - 1; j++)
  {
    for (i = 0; i < FILTER_N - 1 - j; i++)
    {
      if (filter_buf_R[i] > filter_buf_R[i + 1])
      {
        filter_temp = filter_buf_R[i];
        filter_buf_R[i] = filter_buf_R[i + 1];
        filter_buf_R[i + 1] = filter_temp;
      }
    }
  }
  for (j = 0; j < FILTER_N - 1; j++)
  {
    for (i = 0; i < FILTER_N - 1 - j; i++)
    {
      if (filter_buf_RC[i] > filter_buf_RC[i + 1])
      {
        filter_temp = filter_buf_RC[i];
        filter_buf_RC[i] = filter_buf_RC[i + 1];
        filter_buf_RC[i + 1] = filter_temp;
      }
    }
  }
  // 去除最大最小极值后求平均
  for (i = 1; i < FILTER_N - 1; i++)
  {
    filter_sum_L += filter_buf_L[i];
    filter_sum_LC += filter_buf_LC[i];
    filter_sum_M += filter_buf_M[i];
    filter_sum_R += filter_buf_R[i];
    filter_sum_RC += filter_buf_RC[i];
  }
  LAD = filter_sum_L / (FILTER_N - 2);
  LADC = filter_sum_LC / (FILTER_N - 2);
  MAD = filter_sum_M / (FILTER_N - 2);
  RAD = filter_sum_R / (FILTER_N - 2);
  RADC = filter_sum_RC / (FILTER_N - 2);
}

void MOTOR_Control(int32 LDuty, int32 RDuty)
{

  if (LDuty >= 0)
  {
    LDuty = range_protect(LDuty, 0, MOTOR_MAX); // 限幅保护
    pwm_duty(MOTOR3_PWM, 0);                    // 占空比最大790！！！
    pwm_duty(MOTOR4_PWM, LDuty);                // 占空比最大990！！！
  }
  else
  {
    LDuty = range_protect(-LDuty, 0, MOTOR_MAX); // 限幅保护
    pwm_duty(MOTOR4_PWM, 0);                     // 占空比最大990！！！
    pwm_duty(MOTOR3_PWM, LDuty);                 // 占空比最大990！！！
  }
  if (RDuty >= 0)
  {
    RDuty = range_protect(RDuty, 0, MOTOR_MAX); // 限幅保护
    pwm_duty(MOTOR2_PWM, 0);                    // 占空比最大990！！！
    pwm_duty(MOTOR1_PWM, RDuty);                // 占空比最大990！！！
  }
  else
  {
    RDuty = range_protect(-RDuty, 0, MOTOR_MAX); // 限幅保护
    pwm_duty(MOTOR1_PWM, 0);                     // 占空比最大990！！！
    pwm_duty(MOTOR2_PWM, RDuty);                 // 占空比最大990！！！
  }
}