/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-18 16:23:25
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-18 16:27:38
 */ 
#include "common.h"
#include "include.h"


#define Pi 3.1415926f

#define POINT_ANGLE 50.5
float Point_Angle = POINT_ANGLE;       //机械零位
#define Point_Speed 150         //目标速度


#define Angle_P 705//625     //直立角度环P
#define Angle_D 20//60.5    //直立角度环D   


#define Gyro_P 55//65       //直立角速度环P
#define Gyro_D 4        //直立角速度环D
 
//#define dt 0.001
//#define K1 0.03


#define Speed_P -0.00003  //速度控制比例参数
#define Speed_I -0.000001
#define Speed_D 0.0001  //速度控制积分参数
float Car_Speed_Set = 0;//2400  //速度设定值

   //方向环参数
#define Dir_Control_P  -2100//-4100//7270//3100//6570//6870//3570//10200//7950//6500//5200//7100//4200//3483//1980//440 //方向控制比例参数
#define Dir_Conrol_D -0.072//0.1340//0.048//0.1320//-0.0690//0.178//0.0760//0.0835//0.0700//0.0789//0.0899//0.0813//0.0713//0.0470//0.0050 //方向控制微分参数
#define Err_Conrol 29500//-3800//-41000//1800 //偏差控制

//圆环左右标志
float ring_L_R = 1;  //1为左环，2为右环
   
int filtering_L();
int filtering_R();
   
//方向环变量
double direction_control_out_new,direction_control_out_old,direction_control_out,DIR_CONTROL_D_VALUE;
int g_nDirectionControlPeriod,g_nDirectionControlCount;
double leftrightadd,leftrightsub,value;  //左右AD采集之和、之差、输出值
double fdvalue;  //陀螺仪输出值
int adc_get[5] = 0;  //5次采集
int adc_get_new[3] = 0;  //滤波
uint16 getleft,getright;
double error; //偏差值
double deviation; //差比和偏差值
double leftrignt_old ,leftrignt_new;
double err;
//float Dir_Control_P;
int acc_x;



//圆环检测变量
#define Dir_Control_Ring_P -60000  //圆环控制比例函数
#define Dir_Control_Ring_D -0.000
float  ring_add, ring_sub, deviation_ring, ring_value;
int16 ring_L, ring_R, ring_flag1 = 0, ring_flag2 = 0, ring_flag3 = 0, ring_flag4 = 0;

int angle_flag = 0, speed_flag = 0, direction_flag = 0, motor_flag = 0;
   
   //滤波变量
#define FILTER_N 12
int filter_buf_L[FILTER_N + 1];
int filter_buf_R[FILTER_N + 1];


//速度控制变量
int speed_slow = 0, speed_slow_count = 0;

//卡尔曼滤波变量
static float angle = POINT_ANGLE;
static float Q_angle=0.001, Q_gyro=0.0007, R_angle=5, dt=0.01;//Q增大，动态响应增大
static float Pk[2][2] = { {1, 0}, {0, 1 }};
static float Pdot[4] ={0,0,0,0};
static float q_bias=0, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;


   //示波器函数，变量
float OutData[4]={0};

S_FLOAT_XYZ 
	GYRO_Real,		// 陀螺仪转化后的数据
	ACC_Real,		// 加速度计转化后的数据
	Attitude_Angle,	// 当前角度
	Last_Angle,		// 上次角度
	Target_Angle;	// 目标角度
	

S_INT16_XYZ
	GYRO,			// 陀螺仪原始数据
	GYRO_Offset,	// 陀螺仪温飘
	GYRO_Last,		// 陀螺仪上次数据
	ACC, 			// 加速度计数据
	ACC_Offset,		// 加速度计温飘
	ACC_Last;		// 加速度计上次数据
S_INT32_XYZ
	Tar_Ang_Vel,	// 目标角速度
	Tar_Ang_Vel_Last;	// 上次目标角速度

float a,b,c,d,e;

float accel_x,accel_z, Ang_ACC, gyro_y, Angle, gyro_y_true, g_fAngleControLOut;
float Speed_out;

float speed_pwm_R;
float speed_pwm_L;

void AngleControl();
void Angle_Calculate();
float Angle_PID_Realize(float NowData, float Point);
void init_pit0();
void pit0_isr();
void ftm3_init();
float ACC_PID_Realize(float NowData, float Point);
void ftm_init();
float zero = 0;
void init_adc();
void direction_control();
int filtering_L_ring();
int filtering_R_ring();
void ring_judge();
void ring_get();
void ring_judge_out();
void obstacle_judge();
void circuit_send();
void init_uart();
int obstacle_get();
void uartn_handler();
void Kalman_Filter(float Accel,float Gyro);
float Angle_PID(float NowData);
float Gyro_PID(float NowData);
void speed_calculate();
float Speed_PID(float NowData);


//速度采集变量
uint8 g_nSpeedControlPeriod;
int left_counter = 0,right_counter,g_nSpeedControlCount;
float carspeed,fDelta,fP,fI;
float g_fSpeedControlIntegral = 0,g_fSpeedControlOutNew = 0,g_fSpeedControlOutOld = 0,g_fSpeedControlOut = 0;
int speed_Left,speed_Right;


uint16 i = 0, j = 0, k = 0, m, n;