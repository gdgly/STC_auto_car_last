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
void main()
{
	DisableGlobalIRQ();					//�ر����ж�
	board_init();						//��ʼ���Ĵ���
	System_Init();
	EnableGlobalIRQ();					//�������ж�
    while(1)    
    {
        Dif_Nor_coefficient=50;               //��Ⱥ�ϵ��
   /********************��Ⱥ�*****************************/
        AD_LVBO();
        Difference = (3800*(RAD-LAD)/(LAD+RAD))*Dif_Nor_coefficient;
        Difference = range_protect_main(Difference, -Dif_Nor_coefficient, Dif_Nor_coefficient);
   /**************�����ж�****(In_Roundabout_I>5500)&&********************** */
        if((LAD>I_LAD)&&(RAD>I_RAD)&&(LADC>I_LADC)&&(RADC>I_RADC)&&(!Out_Roundabout_Flag))//������
            {
                In_Roundabout_Flag=1;
                Roundabout_cnt++;
            }
         
        if(In_Roundabout_Flag&&Roundabout_cnt>=3)               
          	{
            
             
            if((In_Out_Roundabout_ture_I<30)&&(In_Out_Roundabout_ture_I>-30))
                { 
                  	Difference = ((3800*(RADC-LADC))/(LADC+MAD+RADC))*Dif_Nor_coefficient;  
                  	Fres = 4;
                }
            if((In_Out_Roundabout_ture_I<-360)||(In_Out_Roundabout_ture_I>360))
				{          
					Out_Roundabout_Flag=1;
                }    
            }
   
   /******************��������***************************/
         if(Out_Roundabout_Flag)
        {  
          	BUZZ_open;
           	Roundabout_cnt=0;
           	In_Roundabout_Flag=0;
          	In_Out_Roundabout_I=0;
           	In_Out_Roundabout_ture_I=0;
           	Difference = ((3800*((RAD-0.8*RADC)-(LAD-0.8*LADC))/(LAD+RAD))*Dif_Nor_coefficient;  
           	if(Out_Roundabout_I>O_I*100)
           	{
                BUZZ_shut;
             	Out_Roundabout_Flag=0;
               	Out_Roundabout_I=0;
           	}
        }
             
   /**********************ת�����ʼ���*************************/    
        Curvature = PlacePID_Control(&Turn_PID, Turn[Fres], Difference, 0);	// ��̬PID����ת��
   /************************�ƶ�ϵͳ***************************/    
        if((LAD<=10)&&(RAD<=10))
        {
         	Run_Flag=0;
        }	
        else
        {
           Run_Flag=1;
        }
   /************************ͣ�����***************************/    
    	if(Run_I>20000)
       		Stop_Flag = P44;
//    	led_turn (LED0); 
    	if(K2==0)   //���뿪��2����
		{
       		Send_Begin();//��λ��
        	Testdata_generate();
        	Send_Variable();
		}
    }  
}
