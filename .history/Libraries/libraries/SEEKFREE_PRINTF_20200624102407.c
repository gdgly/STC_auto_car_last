/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897 �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		printf
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/


#include <stdarg.h>
#include <string.h>

#include "SEEKFREE_PRINTF.h"


uint8 number_conversion_ascii(uint32 dat, int8 *p, uint8 neg_type, uint8 radix)
{
    int32   neg_dat;
    uint32  pos_dat;
    uint8   temp_data = 0;
    uint8   valid_num = 0;
    
    if(neg_type)
    {
        neg_dat = (int32)dat;
        if(neg_dat<0)   neg_dat = -neg_dat;
        while(1)
        {
            *p = neg_dat%radix + '0';
            neg_dat = neg_dat/radix;
            valid_num++;
            
            if(!neg_dat) break;
            p++;
        }
    }
    else
    {
        pos_dat = dat;
        while(1)
        {
            temp_data = pos_dat%radix;
            if(10 <= temp_data) temp_data += 'A'-10;
            else                temp_data += '0';
            
            *p = temp_data;
            
            pos_dat = pos_dat/radix;
            valid_num++;
            
            if(!pos_dat) break;
            p++;
        }
    }
    return valid_num;
}

void printf_reverse_order(int8 *d_buff, uint32 len)
{
    uint32 i;
    int8  temp_data;
    for(i=0;i<len/2;i++)
    {
        temp_data = d_buff[len-1-i];
        d_buff[len-1-i] = d_buff[i];
        d_buff[i] = temp_data; 
    }
}


void printf(const int8 *format, ...)
{
#if(PRINTF)     //����궨��PRINTFΪ0 �򲻱���printf�������ݣ�����printfû���κβ���
    va_list arg;
	va_start(arg, format);
    
	while (*format)
	{
		int8 ret = *format;
		if (ret == '%')
		{
			switch (*++format)
			{
                case 'a':// ʮ������p��������������� ��δʵ��
                {
                    
                    
                }break;
                
                
                case 'c':// һ���ַ�
                {
                    int8 ch = (int8)va_arg(arg, uint32);
                    uart_putchar(DEBUG_UART, (int8)ch);
                            
                }break;
                
                
                case 'd':
                case 'i':// �з���ʮ��������
                {
                    int8 vstr[33];
                    int32 ival = (int32)va_arg(arg, int32);
                    uint8 vlen = number_conversion_ascii((uint32)ival, vstr, 1, 10);
                    if(ival<0)  uart_putchar(DEBUG_UART, '-');
                    printf_reverse_order(vstr,vlen);
                    uart_putbuff(DEBUG_UART, (uint8 *)vstr,vlen);
                }break;
                
                case 'f':// �����������С�������λ  ����ָ���������
                case 'F':// �����������С�������λ  ����ָ���������
                {
                    int8 vstr[33];
                    double ival = (double)va_arg(arg, double);
                    uint8 vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
                    if(ival<0)  uart_putchar(DEBUG_UART, '-');
                    printf_reverse_order(vstr,vlen);
                    uart_putbuff(DEBUG_UART, (uint8 *)vstr,vlen);
                    uart_putchar(DEBUG_UART, '.');

                    ival = ((double)ival - (int32)ival)*1000000;
                    vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
				      while(6>vlen)
                    {
                        vstr[vlen] = '0';
                        vlen++;
                    }
                    printf_reverse_order(vstr,vlen);
                    uart_putbuff(DEBUG_UART, (uint8 *)vstr,vlen);
                    break;
                }
                
                case 'u':// �޷���ʮ��������
                {
                    int8 vstr[33];
                    uint32 ival = (uint32)va_arg(arg, uint32);
                    uint8 vlen = number_conversion_ascii(ival, vstr, 0, 10);
                    printf_reverse_order(vstr,vlen);
                    uart_putbuff(DEBUG_UART, (uint8 *)vstr,vlen);
                }break;
                
                case 'o':// �޷��Ű˽������� 
                {
                    int8 vstr[33];
                    uint32 ival = (uint32)va_arg(arg, uint32);
                    uint8 vlen = number_conversion_ascii(ival, vstr, 0, 8);
                    printf_reverse_order(vstr,vlen);
                    uart_putbuff(DEBUG_UART, (uint8 *)vstr,vlen);
                    
                }break;
                
                case 'x':// �޷���ʮ����������
                case 'X':// �޷���ʮ����������
                {
                    int8 vstr[33];
                    uint32 ival = (uint32)va_arg(arg, uint32);
                    uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);
                    printf_reverse_order(vstr,vlen);
                    uart_putbuff(DEBUG_UART, (uint8 *)vstr,vlen);
                }break;
                
                
                case 's':// �ַ���
                {
                    int8 *pc = va_arg(arg, int8 *);
                    while (*pc)
                    {
                        uart_putchar(DEBUG_UART, (int8)(*pc));
                        pc++;
                    }
                }break;
                
                case 'p':// ��16������ʽ���ָ��
                {
                    int8 vstr[33];
                    uint32 ival = (uint32)va_arg(arg, uint32);
                    uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);
                    printf_reverse_order(vstr,8);
                    uart_putbuff(DEBUG_UART, (uint8 *)vstr,8);
                            
                }break;
                
                
                case '%':// ����ַ�% 
                {
                    uart_putchar(DEBUG_UART, '%');
                }break;

                default:break;
			}
		}
		else
		{
			uart_putchar(DEBUG_UART, (int8)(*format));
		}
		format++;
	}
	va_end(arg);
#endif
}


nt32 zf_sprintf(int8 *buff, const int8 *format, ...)

    nt32 buff_len=0;
    _list arg;
	_start(arg, format);

	ile (*format)

		t8 ret = *format;
		 (ret == '%')
	
			itch (*++format)
		
                se 'a':// ʮ������p��������������� ��δʵ��
               
                  
                  
                reak;
              
              
                se 'c':// һ���ַ�
               
                    t8 ch = (int8)va_arg(arg, uint32);
                    uff = ch;
                    ff++;
                    ff_len++;
                          
                reak;
              
              
                se 'd':
                se 'i':// �з���ʮ��������
               
                    t8 vstr[33];
                    t32 ival = (int32)va_arg(arg, int32);
                    nt8 vlen = number_conversion_ascii((uint32)ival, vstr, 1, 10);
                  
                    (ival<0)  
                   
                        tr[vlen] = '-';
                        en++;
                   
                    intf_reverse_order(vstr,vlen);
                    mcpy(buff,vstr,vlen);
                    ff += vlen;
                    ff_len += vlen;
                  
                  
                reak;
              
                se 'f':// �����������С�������λ  ����ָ���������
                se 'F':// �����������С�������λ  ����ָ���������
               
                    t8 vstr[33];
                    uble ival = (double)va_arg(arg, double);
                    nt8 vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
                  
                    (ival<0)  
                   
                        tr[vlen] = '-';
                        en++;
                   
                    intf_reverse_order(vstr,vlen);
                    mcpy(buff,vstr,vlen);
                    ff += vlen;
                    ff_len += vlen;

                    al = ((double)ival - (int32)ival)*1000000;
                    (ival)
                   
                        en = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
                   
                    se
                   
                        tr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
                        en = 6;
                   
			
                    ile(6>vlen)
                   
                        tr[vlen] = '0';
                        en++;
                   
			
                    tr[vlen] = '.';
                    en++;
                  
                    intf_reverse_order(vstr,vlen);
                    mcpy(buff,vstr,vlen);
                    ff_len += vlen;

                    eak;
               
              
                se 'u':// �޷���ʮ��������
               
                    t8 vstr[33];
                    nt32 ival = (uint32)va_arg(arg, uint32);
                    nt8 vlen = number_conversion_ascii(ival, vstr, 0, 10);
                  
                    intf_reverse_order(vstr,vlen);
                    mcpy(buff,vstr,vlen);
                    ff += vlen;
                    ff_len += vlen;
                reak;
              
                se 'o':// �޷��Ű˽������� 
               
                    t8 vstr[33];
                    nt32 ival = (uint32)va_arg(arg, uint32);
                    nt8 vlen = number_conversion_ascii(ival, vstr, 0, 8);
                  
                    intf_reverse_order(vstr,vlen);
                    mcpy(buff,vstr,vlen);
                    ff += vlen;
                    ff_len += vlen;
                  
                reak;
              
                se 'x':// �޷���ʮ����������
                se 'X':// �޷���ʮ����������
               
                    t8 vstr[33];
                    nt32 ival = (uint32)va_arg(arg, uint32);
                    nt8 vlen = number_conversion_ascii(ival, vstr, 0, 16);
                  
                    intf_reverse_order(vstr,vlen);
                    mcpy(buff,vstr,vlen);
                    ff += vlen;
                    ff_len += vlen;
                reak;
              
              
                se 's':// �ַ���
               
                    t8 *pc = va_arg(arg, int8 *);
                    ile (*pc)
                   
                        uff = *pc;
                        ff++;
                        ff_len++;
                        ++;
                   
                reak;
              
                se 'p':// ��16������ʽ���ָ��
               
                    t8 vstr[33];
                    nt32 ival = (uint32)va_arg(arg, uint32);
                    nt8 vlen = number_conversion_ascii(ival, vstr, 0, 16);
                  
                    intf_reverse_order(vstr,8);
                    mcpy(buff,vstr,8);
                    ff += 8;
                    ff_len += 8;
                          
                reak;
              
              
                se '%':// ����ַ�% 
               
                    uff = '%';
                    ff++;
                    ff_len++;
                reak;

                fault:break;
		
	
		se
	
            uff = (int8)(*format);
            ff++;
            ff_len++;
	
		rmat++;

	_end(arg);

    turn buff_len;
