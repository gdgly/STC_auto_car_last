/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK5.27
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-5-13
 ********************************************************************************************************************/

#include "headfile.h"

//board.h�ļ���FOSC��ֵ����Ϊ0,������Զ�ʶ��ϵͳƵ��

/*board.h�ļ���FOSC��ֵ���ò�Ϊ0����ϵͳƵ��ΪFOSC��ֵ��
��ʹ��stc-isp�������س����ʱ����Ҫ��IRCƵ������ΪFOSC��ֵ*/


//���尴������
#define KEY1_PIN    P40
#define KEY2_PIN    P41
#define KEY3_PIN    P42
#define KEY4_PIN    P43
//���岦�뿪������
#define SW1_PIN     P32
#define SW2_PIN     P33


//���뿪��״̬����
uint8 sw1_status;
uint8 sw2_status;

//����״̬����
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

//��һ�ο���״̬����
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

//���ر�־λ
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

uint8 test1=0,test2=0,test3=0,test4=0;




void main()
{
	
	DisableGlobalIRQ();	//�ر����ж�
	board_init();		//��ʼ��
	

	lcd_init();
	

	gpio_mode(P43,GPO_PP);
	gpio_mode(P40,GPO_PP);
	gpio_pull_set(P43,1);
	gpio_pull_set(P40,1);
	
	EnableGlobalIRQ();	//�������ж�
    while(1)
	{
		//��ȡ���뿪��״̬
        sw1_status = SW1_PIN;
        sw2_status = SW2_PIN;

        //��TFT����ʾ���뿪��״̬
        lcd_showstr(0,1,"SW1 STATUS:");     lcd_showint32(11*8,1,sw1_status,1);
        lcd_showstr(0,2,"SW2 STATUS:");     lcd_showint32(11*8,2,sw2_status,1);
        
        //ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
        //���水��״̬
        key1_last_status = key1_status;
        key2_last_status = key2_status;
        key3_last_status = key3_status;
        key4_last_status = key4_status;
        //��ȡ��ǰ����״̬
        key1_status = KEY1_PIN;
        key2_status = KEY2_PIN;
        key3_status = KEY3_PIN;
        key4_status = KEY4_PIN;
        
        //��⵽��������֮��  ���ſ���λ��־λ
        if(key1_status && !key1_last_status)    key1_flag = 1;
        if(key2_status && !key2_last_status)    key2_flag = 1;
        if(key3_status && !key3_last_status)    key3_flag = 1;
        if(key4_status && !key4_last_status)    key4_flag = 1;
        
        //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
        if(key1_flag)   
        {
            key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            test1++;
        }
        
        if(key2_flag)   
        {
            key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            test2++;
        }
        
        if(key3_flag)   
        {
            key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            test3++;
        }
        
        if(key4_flag)   
        {
            key4_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            test4++;
        }
        


        //��TFT����ʾ���Ա���
        lcd_showstr(0,3,"KEY1 TEST:");     lcd_showint32(11*8,3,test1,3);
        lcd_showstr(0,4,"KEY2 TEST:");     lcd_showint32(11*8,4,test2,3);
        lcd_showstr(0,5,"KEY3 TEST:");     lcd_showint32(11*8,5,test3,3);
        lcd_showstr(0,6,"KEY4 TEST:");     lcd_showint32(11*8,6,test4,3);

    }
}
