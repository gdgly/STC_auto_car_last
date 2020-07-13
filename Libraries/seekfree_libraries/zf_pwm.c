/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		pwm
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#include "zf_pwm.h"
#include "zf_gpio.h"
#include "board.h"

#define PWMx_CR_BASE_ADDR 0xff04
#define PWMx_T1_BASE_ADDR 0xff00
#define PWMx_T2_BASE_ADDR 0xff02


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM_gpio��ʼ�����ڲ�ʹ���û�������ģ�
//  @param      pwmch       PWMͨ���ż�����
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_gpio(PWMCH_enum pwmch)
{
	uint8 gpio_num;

	if((pwmch & 0x03) == 0) 	 //P2
	{
		gpio_num = 0x20;
	}
	else if((pwmch & 0x03) == 1) //P1
	{
		gpio_num = 0x10;
	}
	else if((pwmch & 0x03) == 2) //P6
	{
		gpio_num = 0x60;
	}
	gpio_num |= (pwmch >> 4);
	
	gpio_mode(gpio_num,GPO_PP);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM��ʼ��
//  @param      pwmch       PWMͨ���ż�����
//  @param      freq        PWMƵ��(64Hz-3MHz)
//  @param      duty        PWMռ�ձ�
//  @return     void
//  Sample usage:           pwm_init(PWM0_P00, 100, 5000);     //��ʼ��PWM0  ʹ������P0.0  ���PWMƵ��100HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//							PWM_DUTY_MAX��fsl_pwm.h�ļ��� Ĭ��Ϊ10000
//	@note					���е�PWMͨ��ֻ������Ϊͬһ��Ƶ��
//							PWMx ͬһʱ�䣬ֻ����һ���������PWM
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(PWMCH_enum pwmch,uint32 freq, uint16 duty)
{
	
	uint16 match_temp;
	uint16 period_temp; 
	uint8 freq_div = 0;
	
	pwm_set_gpio(pwmch);					//����Ϊ�������
	P_SW2 |= 0x80;
	
	//���PWM����
	(*(unsigned char volatile xdata *)(PWMx_CR_BASE_ADDR + (pwmch & 0xF0))) &= ~(0x03 << 3);
	//�л�PWM����
	(*(unsigned char volatile xdata *)(PWMx_CR_BASE_ADDR + (pwmch & 0xF0))) |= (pwmch & 0x03) << 3;
	
	freq_div = (sys_clk / freq) >> 15;
	period_temp = sys_clk / freq / (freq_div + 1);					//����
	match_temp = period_temp * ((float) duty / PWM_DUTY_MAX);	//ռ�ձ�

	//����pwmnƵ��Ϊ ϵͳʱ��/freq_div
	PWMCKS = freq_div;
	//����PWMn����Ϊperiod_temp�� PWM ʱ��
	PWMC = (uint16)period_temp; 	

	// �ڼ���ֵΪ match_temp �ط� pwmch ͨ������͵�ƽ
	(*(unsigned int  volatile xdata *)(PWMx_T1_BASE_ADDR + (pwmch & 0xF0))) = (uint16)match_temp;
//	// �ڼ���ֵΪ match_temp �ط� pwmch ͨ������ߵ�ƽ
//	(*(unsigned int  volatile xdata *)(PWMx_T2_BASE_ADDR + (pwmch & 0xF0))) = (uint16)0;


	//ʹ�� PWMx ���
	(*(unsigned char volatile xdata *)(PWMx_CR_BASE_ADDR + (pwmch & 0xF0))) |= 1 << 7;

	P_SW2 &= ~0x80;

	PWMCR = 0x80; // ���� PWM ģ��
	

}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMռ�ձ�����
//  @param      pwmch       PWMͨ���ż�����
//  @param      duty        PWMռ�ձ�
//  @return     void
//  Sample usage:           pwm_duty(PWM0_P00, 5000);     //��ʼ��PWM0  ʹ������P0.0  ���PWMƵ��50HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//							PWM_DUTY_MAX��fsl_pwm.h�ļ��� Ĭ��Ϊ50000
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty(PWMCH_enum pwmch, uint16 duty)
{
	uint16 match_temp;
	P_SW2 |= 0x80;
	
	match_temp = (uint16)(PWMC) * ((float)duty/PWM_DUTY_MAX);	//ռ�ձ�
	// �ڼ���ֵΪ match_temp �ط� pwmch ͨ������͵�ƽ
	(*(unsigned int  volatile xdata *)(PWMx_T1_BASE_ADDR + (pwmch & 0xF0))) = (uint16)match_temp;
//	// �ڼ���ֵΪ match_temp �ط� pwmch ͨ������ߵ�ƽ
//	(*(unsigned int  volatile xdata *)(PWMx_T2_BASE_ADDR + (pwmch & 0xF0))) = (uint16)0;
	
	
	P_SW2 &= ~0x80;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMƵ�����ã����е�ͨ��ֻ��ʹ��һ��Ƶ�ʣ�
//  @param      freq        PWMƵ��
//  @param      duty        PWMռ�ձ�
//  @return     void
//  Sample usage:           pwm_freq(PWM0_P00, 50, 5000);     //�޸Ļ�PWM0  ʹ������P0.0  ���PWMƵ��50HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//	@note					���е�PWMͨ��ֻ������Ϊһ��Ƶ��
//-------------------------------------------------------------------------------------------------------------------
void pwm_freq(uint32 freq)
{
	uint16 period_temp; 
	uint8 freq_div = 0;
	P_SW2 |= 0x80;
	freq_div = (sys_clk / freq) >> 15;
	period_temp = sys_clk / freq / (freq_div + 1);					//����

	//����pwmnƵ��Ϊ ϵͳʱ��/freq_div
	PWMCKS = freq_div;
	//����PWMn����Ϊperiod_temp�� PWM ʱ��
	PWMC = (uint16)period_temp; 
	
	P_SW2 &= ~0x80;
}


