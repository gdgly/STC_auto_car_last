#include "outputdata.h"

float OutData[4] = {0};

unsigned short My_CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

void OutPut_Data(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]&);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = My_CRC_CHECK(databuf,8);
  databuf[8] = CRC16&0xff;
  databuf[9] = CRC16>>8;
  
  for(i=0;i<10;i++)
    uart_putchar(UART_4,databuf[i]);//此函数可能需要根据不同的库更改参数
}
/*主函数中这样调用
OutData[0]=angle_m;  // angle_m是采集的数据
OutData[1]=funsion;
OutPut_Data();  
*/