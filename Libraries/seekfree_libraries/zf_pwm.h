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

#ifndef __ZF_PWM_H
#define __ZF_PWM_H
#include "common.h"



#define PWM_DUTY_MAX 10000


typedef enum
{
	PWM0_P20 = 0x00,PWM0_P10,PWM0_P60,
	                                     
	PWM1_P21 = 0x10,PWM1_P11,PWM1_P61,
	                                  
	PWM2_P22 = 0x20,PWM2_P12,PWM2_P62,
	                                  
	PWM3_P23 = 0x30,PWM3_P13,PWM3_P63,
	                                  
	PWM4_P24 = 0x40,PWM4_P14,PWM4_P64,
	                                  
	PWM5_P25 = 0x50,PWM5_P15,PWM5_P65,

	PWM6_P26 = 0x60,PWM6_P16,PWM6_P66,
	
	PWM7_P27 = 0x70,PWM7_P17,PWM7_P67,
}PWMCH_enum;


void pwm_init(PWMCH_enum pwmch,uint32 freq, uint16 duty);
void pwm_duty(PWMCH_enum pwmch, uint16 duty);
void pwm_freq(uint32 freq);


#endif
