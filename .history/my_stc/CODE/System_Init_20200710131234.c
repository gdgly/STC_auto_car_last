/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-18 16:26:13
 * @LastEditTime : 2020-07-10 13:12:32
 */ 
#include "System_Init.h"
uchar System_OK = 0;
int8 Voltage_add;
int8 Voltage_check_time = 0;

float Zero_Angle;
float  Difference = 0,Differencelast=0;
int8 Dif_Nor_coefficient;
int8 K1,K2,K3,K4;
float LAD,RAD,LADC,RADC; 
int   I_LAD,I_LADC,I_RADC,I_RAD,
      O_I;
float Voltage[2];

void System_Init()
{
  uint32 i;
/************************ LED 初始化  *******************/
  gpio_mode(P52,GPO_PP);                
  LED1 = !LED1;                          // 打开LED指示灯	全部初始化成功
/************************ 参数初始化 ********************/
  PID_Parameter_Init(&MOTOR_PID);	// 速度环PID参数初始化
  PID_Parameter_Init(&Angle_PID);	// 角度环PID参数初始化
  PID_Parameter_Init(&Ang_Vel_PID);	// 角速度环PID参数初始化
  PID_Parameter_Init(&Direct_PID);	// 转向环PID参数初始化
  PID_Parameter_Init(&Distance_PID);	// 位置环PID参数初始化
  Curvature = 0;			// 初始化目标转向曲率为0
  Speed_Set = 280;			// 初始化目标速度为0
  Zero_Angle = 3;
  Target_Angle.Y = Zero_Angle*100;
/************************ 电池电压 ************************/   
  adc_init(ADC_P10, ADC_SYSclk_DIV_2);           // 
  Voltage[1] = adc_once(ADC_P10, ADC_12BIT)*Voltage_co;
/************************ 电机 初始化 ********************/
  pwm_init(MOTOR1_PWM,MOTOR_HZ,0);// 初始化 电机 PWM右前
  pwm_init(MOTOR2_PWM,MOTOR_HZ,0);// 初始化 电机 PWM右后 
  pwm_init(MOTOR3_PWM,MOTOR_HZ,0);// 初始化 电机 PWM左后 
  pwm_init(MOTOR4_PWM,MOTOR_HZ,0);// 初始化 电机 PWM左前
/************************ 无线串口 初始化 ********************/
  uart_init(UART4,UART4_RX_P02,UART4_TX_P03,115200,TIM_4);
/************************ 定时器 初始化  *****************/ 
  pit_timer_ms(TIM_3, 2);
/************************ 蜂鸣器初始化 ***********************/  
  gpio_mode(P4_4,GPO_PP);                //蜂鸣器初始化 
/************************ 拨码开关 ***************************/  
  gpio_mode(P3_2,GPIO);
  gpio_pull_set(P3_2,PULLUP);          //使能上拉
  gpio_mode(P3_3,GPIO);
  gpio_pull_set(P3_3,PULLUP);          //使能上拉
//  gpio_mode(P3_4,GPIO);
//  gpio_pull_set(P3_4,PULLUP);          //使能上拉
/************************ 停车管脚 ***************************/ 
  gpio_mode(P7_5,GPIO);
  gpio_pull_set(P7_5,PULLUP);          //使能上拉
/************************ 按键屏幕初始化 **********************/ 
  gpio_mode(P4_0,GPIO);          //上
  gpio_pull_set(P4_0,PULLUP);          //使能上拉
  gpio_mode(P4_1,GPIO);          //下
  gpio_pull_set(P4_1,PULLUP);          //使能上拉
  gpio_mode(P4_2,GPIO);          //左
  gpio_pull_set(P4_2,PULLUP);          //使能上拉
  gpio_mode(P4_3,GPIO);          //右
  gpio_pull_set(P4_3,PULLUP);          //使能上拉
  gpio_mode(P4_4,GPIO);          //取消
  gpio_pull_set(P4_4,PULLUP);          //使能上拉
  gpio_mode(P4_5,GPIO);          //确定
  gpio_pull_set(P4_5,PULLUP);          //使能上拉
//  OLED_Init();
/************************ 编码器初始化 ************************/   
  gpio_mode(P3_5,GPIO);
  gpio_pull_set(P3_5,PULLUP);
  gpio_mode(P5_3,GPIO);
  gpio_pull_set(P5_3,PULLUP);   //以上两个端口是判断方向的
  ctimer_count_init(CTIM0_P34); 
  ctimer_count_init(CTIM3_P04); //以上两个端口是采集速度
/*********************** 电感初始化 ***********************/
  adc_init(ADC_P00, ADC_SYSclk_DIV_2);         //
  adc_init(ADC_P01, ADC_SYSclk_DIV_2);         //
  adc_init(ADC_P02, ADC_SYSclk_DIV_2);        // 
  adc_init(ADC_P03, ADC_SYSclk_DIV_2);        // 
  //adc_init(ADC_P04, ADC_SYSclk_DIV_2);        // 
  //adc_init(ADC_P05, ADC_SYSclk_DIV_2);        // 
  LAD  = adc_once(ADC_P00, ADC_12BIT);
  LADC = adc_once(ADC_P01, ADC_12BIT);
  RAD  = adc_once(ADC_P02, ADC_12BIT);
  RADC = adc_once(ADC_P03, ADC_12BIT);
  Dif_Nor_coefficient=50;               //差比和系数
/*********************** 环岛标志位电感赋值 ***************/
  /***新车***/
  I_LAD =1900;                          
  I_LADC=900;                                 
  I_RADC=900;                                   
  I_RAD =1900; 
  O_I   =60;
/************************ 陀螺仪 初始化 ******************/
  while (My_MPU6050_Init());
  for (i = 0; i < 100; i++)
  {
    Get_Attitude();	// 姿态解算
    Tar_Ang_Vel.Y = PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y*100), Zero_Angle*100);
  }
  Run_Flag = 1;
/*********************** 正确充电发车 ****************************/
//结果是通过
//1.进入充电角度
//2.检测到电压升高    表明正在充电---灯闪烁
//充电完成   熄灯
//充电故障   持续亮灯
  while (!System_OK)
  {
    Voltage[0] = adc_once(ADC_P10, ADC_12BIT)*Voltage_co;
    if (Voltage_check_time > 10)   // 1s检测一次 
    {
      Voltage_check_time = 0;
      if(Voltage[0] <= Voltage[1])  //新值比旧值小或相同则 充电故障
        Voltage_add = 0;
      Voltage[1] = Voltage[0];     //更新 旧电压值
    }
    if
    LED1 = !LED1;
    delay_ms(100);
    Voltage_check_time++;
    if (Voltage_add == 0)  //正常充电Voltage_add 为  1
    {
      LED1 = 0;            //充电故障则持续亮灯
    }
    if (Voltage >= 7.5)    // 充电完成后熄灯  退出初始化
    {
      System_OK = 1;
      LED1 = 1;     
    }
  }
}

/**** 读拨码开关的值 ****/
void Read_Switch() 
{
  K1 = P3_2;
  K2 = P3_3;
  //K3 = P3_4;
  //K4 = P3_5;
  if(K1==0)  //拨码开关1功能
  {
    //OLED_Draw_UI();//屏
    //Check_BottonPress();//按键
  }
  if(K2==0)   //拨码开关2功能
  {
    //Send_Variable();
  }
  if(K3==0)  //拨码开关4功能
  {
     
  }
  if(K4==0)  //拨码开关4功能
  {
     
  }
} 