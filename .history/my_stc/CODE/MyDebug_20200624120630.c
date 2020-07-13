/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-22 16:48:49
 * @LastEditTime : 2020-06-24 12:06:30
 */ 
#include "MyDebug.h"
//上位机相关定义
float Variable[16];

//按键屏幕相关定义
uint8 KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEY_CEL,KEY_SURE;
uint8 Page_Index=3,Para_Index=1,Para_Checked=0,Para_Choice=0;
uint8 Para_Index_Limit=7;   //一页最多有7个变量序号
uint8 Display_Edge=1;
//对应不同的页面
char Para_Name0[7][12]={"angle\0","Eangle\0","T_anglemin\0","T_anglemax\0","Speed_P\0","TURN_lim\0"};
float *Control_Para0[6]={&Attitude_Angle.Y,&Zero_Angle,&Target_Angle_min,&Target_Angle_max,&MOTOR[0],&TURN_lim};
char Para_Name1[6][12]={"I_LAD\0","I_LADC\0","I_MAD\0","I_RADC\0","I_RAD\0","O_I\0"};
int *Control_Para1[6]={&I_LAD,&I_LADC,&I_MAD,&I_RADC,&I_RAD,&O_I};
///////////////////////////////////////////////////////////////////////
char Car_Set_Name0[6][12]={"Speed_Set\0","Speed_L\0","Speed_R\0","System_OK\0","Run_Flag\0","Stop_Flag\0"};
char Car_Set_Name1[6][12]={"LAD\0","LADC\0","MAD\0","RADC\0","RAD\0"};
float Step[4]={1.000,10.00,100.0,1000};   //默认调节步长为0.01
unsigned char Step_Index=2;

/**************上位机函数************/
void my_putchar(char temp)
{
  uart_putchar(UART_2,temp); //根据实际的串口号来修改
}
/*用来通知上位机新的一组数据开始，要保存数据必须发送它*/
void Send_Begin()
{
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0x11);
}
void Testdata_generate()
{
  Variable[0]=LAD;
  Variable[1]=LADC;
  Variable[2]=MAD;
  Variable[3]=RADC;
  Variable[4]=RAD;
  Variable[5]=In_Roundabout_Flag;
  Variable[6]=Out_Roundabout_Flag;
  Variable[7]=In_Out_Roundabout_ture_I;
  Variable[8]=Attitude_Angle.Z;
}
void Send_Variable()
{
  uint8 i=0,ch=0;
  float temp=0;
  uint8 Variable_num=9;
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xff);
  my_putchar(0x01);
  my_putchar(Variable_num);
 for(i=0;i<Variable_num;i++)
  {
    temp=Variable[i];
    ch=BYTE0(temp);
      my_putchar(ch);
    ch=BYTE1(temp);
      my_putchar(ch);
    ch=BYTE2(temp);
      my_putchar(ch);
    ch=BYTE3(temp);
      my_putchar(ch);
  }
     my_putchar(0x01);
}

/**************屏幕显示*************/

void OLED_Draw_UI()  //画出界面
{ 
  uint8 i;         //此程序待完成

     oled_p6x8str(0,0,"Voltage=");                          //显示电池电压
     oled_printf_float(48, 0,Voltage,2,3);                     
     oled_printf_float(72, 0,Step[Step_Index],2,3);            //显示调节步进值
     if((Page_Index!=3)&&Para_Index==7)
     {
        oled_putpixel(,0x00); 
         oled_p6x8str(110,0,"BYS");
         oled_p6x8str(110,1,"NO1");
         oled_p6x8str(110,2,"GHC");
         oled_p6x8str(110,3,"TYB");
         oled_p6x8str(110,4,"Y S");
        oled_putpixel(,0xff);
     }
     else  
     {
         oled_p6x8str(110,0,"BYS");
         oled_p6x8str(110,1,"NO1");
         oled_p6x8str(110,2,"GHC");
         oled_p6x8str(110,3,"TYB");
         oled_p6x8str(110,4,"Y S");
     }
     oled_set_pos(122,7);
     oled_p6x8str(123,5,Page_Index+48);                         //写出页面序号

  /////////////////////////////////////////////////////////第0页  发车调试
  if(Page_Index==0)                
  {
    for(i=0;i<6;i++)
    {
      if(i==Para_Index&&Para_Choice==true&&Para_Checked==false)
      {
       oled_putpixel(0,i,0x00);
       oled_p6x8str(0,i+1,Para_Name0[i]);   //将参量名反转显示
       oled_putpixel(0,i,0xff);
      }
      else oled_p6x8str(0,i+1,Para_Name0[i]);

      if(i==(Para_Index)&&Para_Checked)
      {
        oled_putpixel(72,,0x00);
        oled_printf_float(72, i+1,*Control_Para0[i],4);
        oled_putpixel(,0xff);
      }
      else  oled_printf_float(72, i+1,*Control_Para0[i],4);
      
      oled_set_pos(116,i+1);

    }
  }
    /////////////////////////////////////////////////////////第1页  发车调试
  if(Page_Index==1)                
  {
    for(i=0;i<5;i++)
    {
      if(i==Para_Index&&Para_Choice==true&&Para_Checked==false)
      {
       oled_putpixel(,0x00);
       oled_p6x8str(0,i+1,Para_Name1[i]);   //将参量名反转显示
       oled_putpixel(,0xff);
      }
      else oled_p6x8str(0,i+1,Para_Name1[i]);

      if(i==(Para_Index)&&Para_Checked)
      {
        oled_putpixel(,0x00);
        oled_printf_float(72, i+1,*Control_Para1[i],4);
        oled_putpixel(,0xff);
      }
      else  oled_printf_float(72, i+1,*Control_Para1[i],4);
      
      oled_set_pos(116,i+1);

    }
  }

  /////////////////////////////////////////////////////////第3页   查看车身信息
  else if(Page_Index==2)
  {   
    for(i=0;i<6;i++)
    {
      if(i==Para_Index&&Para_Choice==true&&Para_Checked==false)
      {
       oled_putpixel(,0x00);
       oled_p6x8str(0,i+1,Car_Set_Name0[i]);   //将查看量名反转显示
       oled_putpixel(,0xff);
      }
      else oled_p6x8str(0,i+1,Car_Set_Name0[i]);

      oled_set_pos(116,i+1);
  
    }  
     //参数的反转显示
      if((0==Para_Index)&&Para_Checked)    //查看0    设定速度
      {
        oled_putpixel(,0x00);
        oled_printf_float(72, 1,Speed_Set,4);
        oled_putpixel(,0xff);
      }
      else    oled_printf_float(72,1,Speed_Set,4);
    
      if((1==Para_Index)&&Para_Checked)    //查看1    左编码器
      {
        oled_putpixel(,0x00);
        oled_printf_float(72, 2,MOTOR_Speed_Left,4);
        oled_putpixel(,0xff);
      }
      else    oled_printf_float(72,2,MOTOR_Speed_Left,4);
      
      if((2==Para_Index)&&Para_Checked)    //查看2    右编码器
      {
        oled_putpixel(,0x00);
        oled_printf_float(72, 3,MOTOR_Speed_Right,4);
        oled_putpixel(,0xff);
      }
      else    oled_printf_float(72,3,MOTOR_Speed_Right,4);
      if((3==Para_Index)&&Para_Checked)    //查看3    系统初始化
      {
        oled_putpixel(,0x00);
        oled_printf_float(72, 4,System_OK,4);
        oled_putpixel(,0xff);
      }
      else    oled_printf_float(72,4,System_OK,4);
      
      if((4==Para_Index)&&Para_Checked)    //查看4    开跑标志
      {
        oled_putpixel(,0x00);
        oled_printf_float(72, 5,Run_Flag,4);
        oled_putpixel(,0xff);
      }
      else    oled_printf_float(72,5,Run_Flag,4);
      
      if((5==Para_Index)&&Para_Checked)    //查看5    停车标志
      {
        oled_putpixel(,0x00);
        oled_printf_float(72, 6,Stop_Flag,4);
        oled_putpixel(,0xff);
      }
      else    oled_printf_float(72,6,Stop_Flag,4);
      
  }
    /////////////////////////////////////////////////////////第4页 测场地电感值
  else if(Page_Index==3)
  {
    oled_p6x8str(0,1,"LAD");
    oled_printf_float(72,1,LAD,3);
    oled_p6x8str(0,2,"LADC");
    oled_printf_float(72, 2,LADC,3);
    oled_p6x8str(0,3,"MAD");
    oled_printf_float(72, 3,MAD,3);
    oled_p6x8str(0,4,"RADC");
    oled_printf_float(72, 4,RADC,4);
    oled_p6x8str(0,5,"RAD");
    oled_printf_float(72, 5,RAD,4);
  } 
}

/**************按键选择函数************/
/
void Check_BottonPress()
{   //按键读值
  KEY_UP    = P40;
  KEY_DOWN  = P41;    
  KEY_LEFT  = P42;
  KEY_RIGHT = P43;
  KEY_CEL   = P44;
  KEY_SURE  = P45;
   //确定键
   if(KEY_SURE==0) 
   {
     //去抖
      delay_ms(10);
      if(KEY_SURE==0)
     {    
       if(Para_Index==7) 
       { 
         Para_Index=0; 
       }
       else
       {
        if(Para_Choice==false)  Para_Choice=true;  //  选择参量   
        else
       {
         if(Para_Checked==false) Para_Checked=true;
         else Para_Checked=false;       
        }
       }
      }
      while(KEY_SURE==1); //直到按键松开再运行
   }
   //取消键
     if(KEY_CEL==0)
   {
      //去抖
      delay_ms(10);
      if(KEY_CEL==0)
      {    
       if(Para_Choice==true) Para_Choice=false; //不选择参量
       if(Para_Checked==true)Para_Checked=false;
       if(Page_Index==6)
       {
         if(Display_Edge) Display_Edge=0;
         else Display_Edge=1;
       }
      }
      while(KEY_CEL==1);  //直到按键松开再运行
   } 
   //向左键
     if(KEY_LEFT==0)
   {
      //去抖
      delay_ms(10);
      if(KEY_LEFT==0)
      {
        if(Para_Checked) 
        {
          if(Step_Index==4) 
          Step_Index=4;   //最大的步长为1000
          else Step_Index++;
        }
        else 
        { 
          Para_Index=0;
          if(Page_Index==0) Page_Index=3; //当参数没被选中的时候，按左右键翻页
          else Page_Index--;
          oled_fill(0);//清屏 
        }
      }
      while(KEY_LEFT==1);//直到按键松开再运行
   } 
   //向右键
     if(KEY_RIGHT==0)
   {
      //去抖
      delay_ms(10);
      if(KEY_RIGHT==0)
      {
        if(Para_Checked) 
        {
          if(Step_Index==0) 
           Step_Index=0;//最小的步长为0.01
          else
          {
            Step_Index--;
          }
        }
        else 
        { 
          Para_Index=0;
          if(Page_Index==3) Page_Index=0;
          else Page_Index++;
         oled_fill(0);//清屏 
        }
      }
      while(KEY_RIGHT==1);      //直到按键松开再运行
   }
   //向上键
     if(KEY_UP==0)
   {
      delay_ms(10);
      if(KEY_UP==0)
      {
        if(Para_Choice==true)
        {
          if(Para_Checked==false)
          {
           if(Para_Index==0) Para_Index=Para_Index_Limit;
           else Para_Index-=1;
          }
          else
          {
              if(Page_Index==0&&Para_Index<=6)                    //修改第0页的参数
            {
              *Control_Para0[Para_Index]+=Step[Step_Index];
            }
            
              if(Page_Index==1&&Para_Index<=6)                    //修改第1页的参数
            {
              *Control_Para1[Para_Index]+=Step[Step_Index];
            }
     
              if(Page_Index==2&&Para_Index<=6)                    //修改第3页的参数
            {
              if(Para_Index==0) Speed_Set+=(int)Step[Step_Index];  //设定速度 
            }
              
          }
        }
      }  
      while(KEY_UP==1);//直到按键松开再运行  
   }
   //向下键
     if(KEY_DOWN==0)
   {
      delay_ms(10);
      if(KEY_DOWN==0)
      {
       if(Para_Choice==true)
        {
          if(Para_Checked==false)
          {             
            if(Para_Index==Para_Index_Limit)Para_Index=0;   //防止序号超出范围
            else  Para_Index+=1; 
          }
          else
          {
              if(Page_Index==0&&Para_Index<=6)                    //修改第0页的参数
            {
              *Control_Para0[Para_Index]-=Step[Step_Index];
            }
            
              if(Page_Index==1&&Para_Index<=6)                    //修改第1页的参数
            {
              *Control_Para1[Para_Index]-=Step[Step_Index];
            }
            
              if(Page_Index==2&&Para_Index<=6)                    //修改第3页的参数
            {
              if(Para_Index==0) Speed_Set-=(int)Step[Step_Index];  //设定速度 
            }
                  
          }
        }
      }

      while(KEY_DOWN==1);  //直到按键松开再运行
   }
}
