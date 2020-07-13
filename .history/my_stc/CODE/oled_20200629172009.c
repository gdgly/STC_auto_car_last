/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-29 16:52:25
 * @LastEditTime : 2020-06-29 17:20:09
 */ 
#include  "oled.h"

#define	Brightness	0xCF 

uint8  reverse=0;

const uint8 F6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 },   // horiz lines
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }    // cursor
};

void OLED_WrDat(uint8 dat, uint8 revs)
{
       uint8 i=8;	
       DC_HIGH;
        asm("nop"); 
        asm("nop"); 
       D0_LOW;
        asm("nop");
        asm("nop"); 
       if(revs) dat=~dat;
  while(i--)
  {
    if(dat&0x80)
    {
      D1_HIGH;
    }
    else
    {
      D1_LOW;
    }
      asm("nop");  
      asm("nop"); 
    D0_HIGH;
      asm("nop");  
      asm("nop"); 
    D0_LOW;  
    dat<<=1;    
  }
}
void OLED_WrCmd(uint8 cmd)
{
    uint8 i=8;
    DC_LOW;
    asm("nop"); 
    asm("nop"); 
    D0_LOW;   
    asm("nop"); 
    asm("nop"); 
  while(i--)
  {
    if(cmd&0x80)
    {
      D1_HIGH;
    }
    else
    {
     D1_LOW; 
    }
    D0_HIGH;
    asm("nop");   
    asm("nop");  
    D0_LOW;  
    cmd<<=1;   
  } 	
}
void OLED_Set_Pos(uint8 x, uint8 y)
{ 
  OLED_WrCmd(0xb0+y);
  OLED_WrCmd(((x&0xf0)>>4)|0x10);
  OLED_WrCmd(x&0x0f);//|0x01);
} 

void OLED_Fill(uint8 bmp_dat)
{
	uint8 y,x;
	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);   //0xb0+0~7表示页0~页7?
		OLED_WrCmd(0x00);       //0x00+0~16表示将128列分成16组其地址在某组中的第几列
		OLED_WrCmd(0x10);      //0x10+0~16表示将128列分成16组其地址所在第几组
		for(x=0;x<128;x++)
			OLED_WrDat(bmp_dat,0);
	}
}

void OLED_CLS(void)
{
	uint8 y,x;	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10); 
		for(x=0;x<128;x++)
	        OLED_WrDat(0,0);
	}
}
void OLED_DLY_ms(int ms)
{                         
  int a;
  while(ms)
  {
    a=13350;
    while(a--);
    ms--;
  }
  return;
}

void OLED_Init(void)        
{
  gpio_init (RST, GPO,0);//DC
  gpio_init (DC, GPO,0); //D1
  gpio_init (D0, GPO,0); //D0
  gpio_init (D1, GPO,0); //RST
  OLED_DLY_ms(50);
 
  D0_HIGH;
  RST_LOW;
  OLED_DLY_ms(60);
  RST_HIGH;


  OLED_WrCmd(0xae);//--turn off oled panel
  OLED_WrCmd(0x00);//---set low column address
  OLED_WrCmd(0x10);//---set high column address
  OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WrCmd(0x81);//--set contrast control register
  OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
  OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WrCmd(0xa6);//--set normal display
  OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  OLED_WrCmd(0x3f);//--1/64 duty
  OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WrCmd(0x00);//-not offset
  OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WrCmd(0xd9);//--set pre-charge period
  OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WrCmd(0xda);//--set com pins hardware configuration
  OLED_WrCmd(0x12);
  OLED_WrCmd(0xdb);//--set vcomh
  OLED_WrCmd(0x40);//Set VCOM Deselect Level
  OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WrCmd(0x02);//
  OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
  OLED_WrCmd(0x14);//--set(0x10) disable
  OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  OLED_WrCmd(0xaf);//--turn on oled panel
  OLED_Fill(0);  //初始清屏
  OLED_Set_Pos(0,0);  
	
} 
//==============================================================
//函数名：OLED_P6x8Str(uint8 x,uint8 y,uint8 *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void OLED_P6x8Str(uint8 x,uint8 y,char ch[])
{
  uint8 c=0,i=0,j=0,rs=0;      
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(reverse&&c) rs=1;  //是否反转显示
                else rs=0;      
    if(x>126){x=0;y++;}
    OLED_Set_Pos(x,y);    
  	for(i=0;i<6;i++)     
  	  OLED_WrDat(F6x8[c][i],rs);  
    x+=6;
    j++;
  }
}

//显示一个6x8标准ASCII字符
void OLED_P6x8Char(char ch)
{
	 unsigned char c=0,i=0,rs=0;       
	c =ch-32;
         if(reverse&&c) rs=1;
	for(i=0;i<6;i++)
	{     
              OLED_WrDat(F6x8[c][i],rs);  
	}
}

//将一个char型数转换成3位数进行显示
void OLED_PrintValueC(unsigned char x, unsigned char y, int dat)
{
        OLED_Set_Pos(x,y); 
	unsigned char i,j,k;
	if(dat<0)
	{
		OLED_P6x8Char('-');
		dat = - dat;	
	}
	else
	{
		OLED_P6x8Char('+');
	}
	i = dat/100;
	j = (dat%100)/10;
	k = dat%10;
	OLED_P6x8Char(i+48);
	OLED_P6x8Char(j+48);
	OLED_P6x8Char(k+48);		
}
//功能描述：将一个int型数转换成5位数进行显示
void OLED_PrintValueI(unsigned char x, unsigned char y, int dat)
{       
        OLED_Set_Pos(x,y); 
	unsigned char i,j,k,l,m,fn=0;  
        if(dat < 0)
	{
		OLED_P6x8Char('-');
		dat = - dat;	
	}
	l  = dat/10000;
	m= (dat%10000)/1000;
	i = (dat%1000)/100;
	j = (dat%100)/10;
	k = dat%10;
	
        if(l)
        {
          OLED_P6x8Char(l+48);
          fn=1;
        }
	if(m||fn)
        {
          OLED_P6x8Char(m+48);
          fn=1;
        }
	if(i||fn)
        {
          OLED_P6x8Char(i+48);
          fn=1;
        }
	if(j||fn)
          OLED_P6x8Char(j+48);
	OLED_P6x8Char(k+48);	
}
//显示unsigned int型
 void OLED_PrintValueFP(unsigned int dat, unsigned char num)
 {     unsigned char m,i,j,k;   	
 	OLED_P6x8Char('.');
        
	m= dat/1000;
	i = (dat%1000)/100;
	j = (dat%100)/10;
	k = dat%10;
	switch(num)
	{

		case 1:  	OLED_P6x8Char(k+48);
				break;
		case 2:  	OLED_P6x8Char(j+48);
				OLED_P6x8Char(k+48);
				break;
		case 3:	OLED_P6x8Char(i+48);
				OLED_P6x8Char(j+48);
				OLED_P6x8Char(k+48);
				break;
		case 4: 	OLED_P6x8Char(m+48);
				OLED_P6x8Char(i+48);
				OLED_P6x8Char(j+48);
				OLED_P6x8Char(k+48);
				break;	
	}
        OLED_P6x8Char(' ');     //清空后一位
 }

//功能描述：将一个float型数转换成2位整数部分带4位小数和符号的数据并进行显示
 void OLED_PrintValueF(unsigned char x, unsigned char y, float dat, unsigned char num)
 {
 	unsigned char l,m,i,j,k;  //wan
 	unsigned char Integer_Len = 6; //整数位数         
        
        if(dat>0)       
        dat=dat+0.00001;
  	int Integer_Part = (int)dat; //整数部分
 	long int Float_Part = (long int)((dat - (int)dat)*100000); //取小数位后5位
        OLED_Set_Pos(x,y); 
 	//整数部分显示
         
     
        
        if(dat<-0.00001)
        OLED_P6x8Char('-'); 

	if(Integer_Part < 0)Integer_Part = - Integer_Part;  //去掉整数部分负号
 	l  = Integer_Part/10000;
	m= (Integer_Part%10000)/1000;
	i = (Integer_Part%1000)/100;
	j = (Integer_Part%100)/10;
	k = Integer_Part%10;
        
        
        
        
 	if (l != 0)  //五位
 	{
                Integer_Len = 5;
 		OLED_P6x8Char(l+48);
 		OLED_P6x8Char(m+48);
		OLED_P6x8Char(i+48);
		OLED_P6x8Char(j+48);
		OLED_P6x8Char(k+48);
 	}
 	else if(m != 0) //四位
 	{
 		Integer_Len = 4;
 		OLED_P6x8Char(m+48);
 		OLED_P6x8Char(i+48);
		OLED_P6x8Char(j+48);
		OLED_P6x8Char(k+48);
 	}
  	else if(i != 0) //三位
  	{
  		Integer_Len = 3;
  	 	OLED_P6x8Char(i+48);
 		OLED_P6x8Char(j+48);
		OLED_P6x8Char(k+48);
  	}
  	else if(j != 0) //两位
  	{
    		Integer_Len = 2;	
  		OLED_P6x8Char(j+48);
 		OLED_P6x8Char(k+48);
  	}
	else 	
	{
		Integer_Len = 1;
		OLED_P6x8Char(k+48);
	}	
        if(Float_Part < 0)Float_Part = -Float_Part;
	switch(num-Integer_Len)
	{
		case 0:   OLED_P6x8Char(' '); break;
		case 1:  OLED_PrintValueFP((unsigned int)(Float_Part /10000),num-Integer_Len);break;
		case 2:  OLED_PrintValueFP((unsigned int)(Float_Part /1000),num-Integer_Len);break;
		case 3:  OLED_PrintValueFP((unsigned int)(Float_Part /100),num-Integer_Len);break;
		case 4:  OLED_PrintValueFP((unsigned int)(Float_Part/10),num-Integer_Len);break;					
	}
 }