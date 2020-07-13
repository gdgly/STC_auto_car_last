/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		isr
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK5.27
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/
#include "headfile.h"





//UART1�ж�
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
		//�������ݼĴ���Ϊ��SBUF
        res = SBUF;
        //�����Զ�����
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

//UART2�ж�
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
		//�������ݼĴ���Ϊ��S2BUF

	}
}


//UART3�ж�
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
		//�������ݼĴ���Ϊ��S3BUF

	}
}


//UART4�ж�

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


//PCA���ڶ�ʱ���ж�
void PCA_Isr() interrupt 7
{
	if(PCA0_GET_FLAG)
	{
		//����жϱ�־λ
		PCA_CLEAR_FLAG(PCA_0);	

		//���ؼ�����
		pca_reload_counter(PCA_0);
		
		//����CCD�ɼ�����
		ccd_collect();
		//�ɼ���ɱ�־λ��1
		tsl1401_finish_flag = 1;
		
		
	}
	
	if(PCA1_GET_FLAG)
	{
		//����жϱ�־λ
		PCA_CLEAR_FLAG(PCA_1);	
		//���ؼ�����
		pca_reload_counter(PCA_1);

	}
	


	if(PCA2_GET_FLAG)
	{
		//����жϱ�־λ
		PCA_CLEAR_FLAG(PCA_2);	
		//���ؼ�����
		pca_reload_counter(PCA_2);

	}
	
//	PCA_3������ʱ
	if(PCA3_GET_FLAG)
	{
		//����жϱ�־λ
		PCA_CLEAR_FLAG(PCA_3);	
		
//		//���ؼ�����
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
  Ang_Velocity_Flag = 1;// ���ٶ�2ms��ȡһ��
	if (Time_10ms == 5)
	{
		Time_10ms = 0;
		Angle_Flag = 1;// ��̬�Ƕ�10ms����һ��	
	}
	if (Time_100ms == 50)
	{
		Time_100ms = 0;
		Speed_Flag = 1;// �ٶ�100ms����һ��
	}
  if (Time_200ms == 100)
	{
		Time_200ms = 0;
		Read_Switch();
	}
	LED1 = !LED1;		 // ��LED1ָʾ��
 	Balance_Control();// ����ƽ�����
	TIM3_CLEAR_FLAG; //����жϱ�־
}
//void TM4_Isr() interrupt 20
//void ADC_Isr() interrupt 5
//void LVD_Isr() interrupt 6
//void SPI_Isr() interrupt 9
//void PWM_Isr() interrupt 22
//void I2C_Isr() interrupt 24
//void PWMFD_Isr() interrupt 23	
