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

//board.h文件中FOSC的值设置为0,则程序自动识别系统频率

/*board.h文件中FOSC的值设置不为0，则系统频率为FOSC的值，
在使用stc-isp工具下载程序的时候需要将IRC频率设置为FOSC的值*/


//定义按键引脚
#define KEY1_PIN    P40
#define KEY2_PIN    P41
#define KEY3_PIN    P42
#define KEY4_PIN    P43
//定义拨码开关引脚
#define SW1_PIN     P32
#define SW2_PIN     P33


//拨码开关状态变量
uint8 sw1_status;
uint8 sw2_status;

//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

uint8 test1=0,test2=0,test3=0,test4=0;




void main()
{
	
	DisableGlobalIRQ();	//关闭总中断
	board_init();		//初始化
	

	lcd_init();
	

	gpio_mode(P43,GPO_PP);
	gpio_mode(P40,GPO_PP);
	gpio_pull_set(P43,1);
	gpio_pull_set(P40,1);
	
	EnableGlobalIRQ();	//开启总中断
    while(1)
	{
		//获取拨码开关状态
        sw1_status = SW1_PIN;
        sw2_status = SW2_PIN;

        //在TFT上显示拨码开关状态
        lcd_showstr(0,1,"SW1 STATUS:");     lcd_showint32(11*8,1,sw1_status,1);
        lcd_showstr(0,2,"SW2 STATUS:");     lcd_showint32(11*8,2,sw2_status,1);
        
        //使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
        //保存按键状态
        key1_last_status = key1_status;
        key2_last_status = key2_status;
        key3_last_status = key3_status;
        key4_last_status = key4_status;
        //读取当前按键状态
        key1_status = KEY1_PIN;
        key2_status = KEY2_PIN;
        key3_status = KEY3_PIN;
        key4_status = KEY4_PIN;
        
        //检测到按键按下之后  并放开置位标志位
        if(key1_status && !key1_last_status)    key1_flag = 1;
        if(key2_status && !key2_last_status)    key2_flag = 1;
        if(key3_status && !key3_last_status)    key3_flag = 1;
        if(key4_status && !key4_last_status)    key4_flag = 1;
        
        //标志位置位之后，可以使用标志位执行自己想要做的事件
        if(key1_flag)   
        {
            key1_flag = 0;//使用按键之后，应该清除标志位
            test1++;
        }
        
        if(key2_flag)   
        {
            key2_flag = 0;//使用按键之后，应该清除标志位
            test2++;
        }
        
        if(key3_flag)   
        {
            key3_flag = 0;//使用按键之后，应该清除标志位
            test3++;
        }
        
        if(key4_flag)   
        {
            key4_flag = 0;//使用按键之后，应该清除标志位
            test4++;
        }
        


        //在TFT上显示测试变量
        lcd_showstr(0,3,"KEY1 TEST:");     lcd_showint32(11*8,3,test1,3);
        lcd_showstr(0,4,"KEY2 TEST:");     lcd_showint32(11*8,4,test2,3);
        lcd_showstr(0,5,"KEY3 TEST:");     lcd_showint32(11*8,5,test3,3);
        lcd_showstr(0,6,"KEY4 TEST:");     lcd_showint32(11*8,6,test4,3);

    }
}
