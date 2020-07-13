/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-18 16:26:13
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-21 21:27:11
 */ 
#include "System.h"
uchar System_OK = 0;
float Zero_Angle;
float  Difference = 0,Differencelast=0;
int8 Dif_Nor_coefficient;
int8 K1,K2,K3,K4;
float LAD,RAD,MAD,LADC,RADC; 
int   I_LAD,I_LADC,I_MAD,I_RADC,I_RAD,
      O_I;
float Voltage;

void System_Init()
{
  uint32 i, j;
/************************ LED 初始化  *******************/
  gpio_mode(P52,GPO_PP);                
  P52 = 0;                          // 打开LED指示灯	全部初始化成功
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
/************************ 电机 初始化 ********************/
  pwm_init(MOTOR1_PWM,MOTOR_HZ,0);// 初始化 电机 PWM右前
  pwm_init(MOTOR2_PWM,MOTOR_HZ,0);// 初始化 电机 PWM右后 
  pwm_init(MOTOR3_PWM,MOTOR_HZ,0);// 初始化 电机 PWM左后 
  pwm_init(MOTOR4_PWM,MOTOR_HZ,0);// 初始化 电机 PWM左前
/************************ 串口 初始化 ********************/
  uart_init(UART_2,UART2_RX_P10,UART2_TX_P11,115200,TIM_0);
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
  gpio_mode(P3_5,GPIO);
  gpio_pull_set(P3_5,PULLUP);          //使能上拉
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
/************************ 电池电压 ************************/   
 //adc_init(ADC_P10, ADC_SYSclk_DIV_2);           // 
 //Voltage=adc_once(ADC_P10, ADC_12BIT)*Voltage_co;
 // TURN_lim=TURN_lim;//*(Voltage_st/Voltage);
/*********************** 电感初始化 ***********************/
  adc_init(ADC_P00, ADC_SYSclk_DIV_2);         //
  adc_init(ADC_P01, ADC_SYSclk_DIV_2);         //
  adc_init(ADC_P02, ADC_SYSclk_DIV_2);        // 
  //adc_init(ADC_P03, ADC_SYSclk_DIV_2);        // 
  //adc_init(ADC_P04, ADC_SYSclk_DIV_2);        // 
  //adc_init(ADC_P05, ADC_SYSclk_DIV_2);        // 
 
  LAD  = adc_once(ADC_P00, ADC_12BIT);
  MAD  = adc_once(ADC_P01, ADC_12BIT);
  RAD  = adc_once(ADC_P02, ADC_12BIT);
/*********************** 环岛标志位电感赋值 ***************/
  /***新车***/
  I_LAD =1900;                          
  I_LADC=900;                                 
  I_MAD =2700;                                   
  I_RADC=900;                                   
  I_RAD =1900; 
  O_I   =60;
/************************ 陀螺仪 初始化 ******************/
  while (MPU6050_Init());
  for (i = 0; i < 200; i++)
  {
    Get_Attitude();	// 姿态解算
    Tar_Ang_Vel.Y = PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y*100), Zero_Angle*100);
    
  }
  Run_Flag = 1;
/*********************** 正确角度发车****************************/
  while (!System_OK)
  {
    if (Attitude_Angle.Y < 20 && Attitude_Angle.Y > -30)	// 姿态进入正常范围
    {
      System_OK = 1;
    }
  }

//  led(LED_MAX,LED_OFF);// 关闭LED指示灯
}
/****读拨码开关的值****/
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
    //Send_Begin();//上位机
    //Testdata_generate();
    //Send_Variable();
  }
  if(K3==0)  //拨码开关4功能
  {
     
  }
  if(K4==0)  //拨码开关4功能
  {
     
  }
} 