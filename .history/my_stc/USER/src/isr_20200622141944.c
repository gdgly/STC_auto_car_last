/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK5.27
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/
#include "headfile.h"





//UART1中断
void UartIsr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
		//接收数据寄存器为：SBUF
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
		
    }
}

//UART2中断
void Uart2Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF

	}
}


//UART3中断
void Uart3Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF

	}
}


//UART4中断

void Uart4Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;
		//S4BUF;

	}
}


//PCA周期定时器中断
void PCA_Isr() interrupt 7
{
	if(PCA0_GET_FLAG)
	{
		//清除中断标志位
		PCA_CLEAR_FLAG(PCA_0);	

		//重载计数器
		pca_reload_counter(PCA_0);
		
		//线性CCD采集数据
		ccd_collect();
		//采集完成标志位置1
		tsl1401_finish_flag = 1;
		
		
	}
	
	if(PCA1_GET_FLAG)
	{
		//清除中断标志位
		PCA_CLEAR_FLAG(PCA_1);	
		//重载计数器
		pca_reload_counter(PCA_1);

	}
	


	if(PCA2_GET_FLAG)
	{
		//清除中断标志位
		PCA_CLEAR_FLAG(PCA_2);	
		//重载计数器
		pca_reload_counter(PCA_2);

	}
	
//	PCA_3已做延时
	if(PCA3_GET_FLAG)
	{
		//清除中断标志位
		PCA_CLEAR_FLAG(PCA_3);	
		
//		//重载计数器
//		pca_reload_counter(PCA_3);
		 
	}
	
}



void INT0_Isr() interrupt 0
{

}
void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{

}
void INT3_Isr() interrupt 11
{

}

void INT4_Isr() interrupt 16
{

}


//void TM0_Isr() interrupt 1
//void TM1_Isr() interrupt 3
//void TM2_Isr() interrupt 12
void TM3_Isr() interrupt 19
{
	static uchar Time_10ms = 0;
  static uchar Time_100ms = 0;
  static uchar Time_200ms = 0;
	Time_10ms++;
	Time_100ms++;
	Time_200ms++;
  Ang_Velocity_Flag = 1;// 角速度2ms获取一次
	if (Time_10ms == 5)
	{
		Time_10ms = 0;
		Angle_Flag = 1;// 姿态角度10ms控制一次	
	}
	if (Time_100ms == 50)
	{
		Time_100ms = 0;
		Speed_Flag = 1;// 速度100ms控制一次
	}
  if (Time_200ms == 100)
	{
		Time_200ms = 0;
		Read_Switch();
	}
	LED1 = !LED1;		 // 打开LED1指示灯
 	Balance_Control();// 车身平衡控制
	TIM3_CLEAR_FLAG; //清除中断标志
}
//void TM4_Isr() interrupt 20
//void ADC_Isr() interrupt 5
//void LVD_Isr() interrupt 6
//void SPI_Isr() interrupt 9
//void PWM_Isr() interrupt 22
//void I2C_Isr() interrupt 24
//void PWMFD_Isr() interrupt 23	
