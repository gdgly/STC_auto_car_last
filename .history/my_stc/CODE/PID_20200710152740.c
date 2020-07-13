/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-21 20:33:02
 * @LastEditTime : 2020-07-10 15:27:40
 */ 
#include "PID.h"

PID MOTOR_PID, Angle_PID, Ang_Vel_PID, Direct_PID, Turn_PID, Distance_PID;//定义舵机和电机的PID参数结构体
float MOTOR[4]   = {10, 0, 0, 1000};		// 速度环PID	最后一项为积分限幅
float Angle[4]   = {0.15, 0, 0.06, 500};		// 角度环PID
float Ang_Vel[4] = {0.20, 0.018, 0.001, 1000};		// 角速度环PID
float Direct[4]  = {0.017, 0.001, 0.023, 70};	// 转向环PID 位置	0.017	0.02
/********* 转向外环动态PID ********/
float Turn[5][4] = {{100, 2, 1, 100},   //起步转向参数PID
		    		{100, 3, 2, 100},   //低速转向参数PID	
		    		{100, 4, 3, 100},   //中速转向参数PID	
		    		{100, 5, 4, 100},   //高速转向参数PID	
		    		{200, 10, 8, 100}}; //环岛转向参数PID
float TURN_lim=165;//转向外环输出限制
// PID参数初始化
void PID_Parameter_Init(PID *sptr)
{
	sptr->SumError  = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastData  = 0;
}
// 位置式动态PID控制
int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
{
	//定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		  Actual;	//最后得出的实际输出值
	float Kp;		//动态P
	iError = SetPoint - NowPiont;	//更新当前误差
	/*
	sprt->SumError += iError*0.01;
	if (sprt->SumError >= PID[KT])    //误差和限幅  -PID[KT] 到  PID[KT]
	{
		sprt->SumError = PID[KT];
	}
	else if (sprt->SumError <=-PID[KT])
	{
		sprt->SumError = -PID[KT];
	}
    */
	Kp = 1.0 * (iError*iError) / PID[KP] + PID[KI];	//P值与差值成二次函数关系，此处P和I不是PID参数，而是动态PID参数，要注意！！！
	
	Actual = Kp * iError+ PID[KD] * (0.8(*iError - sprt->LastError));//只用PD   
	sprt->LastError = iError;		//更新上次误差

//	Actual += sprt->SumError*0.1;不用I所以不需要误差和
	Actual = range_protect_main(Actual, -TURN_lim, TURN_lim);

	return Actual;
}

//转向位置式PID控制
int32 PID_Realize_D(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	// 当前误差
		 Realize;	// 最后得出的实际增量
        float SetD;
	iError = Point - NowData;	// 计算当前误差
	sptr->SumError += PID[KI] * iError;	// 误差积分
	if (sptr->SumError >= PID[KT])
	{
		sptr->SumError = PID[KT];
	}
	else if (sptr->SumError <= -PID[KT])
	{
		sptr->SumError = -PID[KT];
	}
        SetD =PID[KD];
          
        if(Difference*GYRO_Real.Z>0)
         SetD =0;
//        Realize = PID[KP] * iError
//			+ sptr->SumError
//			+ SetD * GYRO_Real.Z;
	Realize = PID[KP] * iError
			+ sptr->SumError
			+ SetD * (iError - sptr->LastError);
	sptr->PrevError = sptr->LastError;	// 更新前次误差
	sptr->LastError = iError;		  	// 更新上次误差
	sptr->LastData  = NowData;			// 更新上次数据

	return Realize;	// 返回实际值
}
// 位置式PID控制
int32 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	// 当前误差
		 Realize;	// 最后得出的实际增量

	iError = Point - NowData;	// 计算当前误差
	sptr->SumError += PID[KI] * iError;	// 误差积分
	if (sptr->SumError >= PID[KT])
	{
		sptr->SumError = PID[KT];
	}
	else if (sptr->SumError <= -PID[KT])
	{
		sptr->SumError = -PID[KT];
	}

	Realize = PID[KP] * iError
			+ sptr->SumError
			+ PID[KD] * (iError - sptr->LastError);
	sptr->PrevError = sptr->LastError;	// 更新前次误差
	sptr->LastError = iError;		  	// 更新上次误差
	sptr->LastData  = NowData;			// 更新上次数据

	return Realize;	// 返回实际值
}
// 增量式PID电机控制
int32 PID_Increase(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		Increase;	//最后得出的实际增量

	iError = Point - NowData;	// 计算当前误差
	Increase =  PID[KP] * (iError - sptr->LastError)
			  + PID[KI] * iError
			  + PID[KD] * (iError - 2 * sptr->LastError + sptr->PrevError);
	
	sptr->PrevError = sptr->LastError;	// 更新前次误差
	sptr->LastError = iError;		  	// 更新上次误差
	sptr->LastData  = NowData;			// 更新上次数据
	
	return Increase;	// 返回增量
}//*/
