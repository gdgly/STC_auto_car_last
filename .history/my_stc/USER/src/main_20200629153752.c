/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK5.27
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-5-13
 ********************************************************************************************************************/

#include "headfile.h"
void main()
{
	DisableGlobalIRQ();					//关闭总中断
	board_init();						//初始化寄存器
	System_Init();
	EnableGlobalIRQ();					//开启总中断
    while(1)    
    {
        Dif_Nor_coefficient=50;               //差比和系数
   /********************差比和*****************************/
        AD_LVBO();
        Difference = (3800*(RAD-LAD)/(LAD+RAD))*Dif_Nor_coefficient;
        Difference = range_protect_main(Difference, -Dif_Nor_coefficient, Dif_Nor_coefficient);
   /**************环岛判断****(In_Roundabout_I>5500)&&********************** */
        if((LAD>I_LAD)&&(RAD>I_RAD)&&(LADC>I_LADC)&&(RADC>I_RADC)&&(!Out_Roundabout_Flag))//进环岛
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
   
   /******************环岛后处理***************************/
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
             
   /**********************转向曲率计算*************************/    
        Curvature = PlacePID_Control(&Turn_PID, Turn[Fres], Difference, 0);	// 动态PID控制转向
   /************************制动系统***************************/    
        if((LAD<=10)&&(RAD<=10))
        {
         	Run_Flag=0;
        }	
        else
        {
           Run_Flag=1;
        }
   /************************停车检测***************************/    
    	if(Run_I>20000)
       		Stop_Flag = P44;
//    	led_turn (LED0); 
    	if(K2==0)   //拨码开关2功能
		{
       		Send_Begin();//上位机
        	Testdata_generate();
        	Send_Variable();
		}
    }  
}
