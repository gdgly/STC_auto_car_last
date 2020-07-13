/*
 * @Description  : 
 * @Version      : 
 * @Date         : 2020-06-18 16:22:29
 * @LastEditTime : 2020-06-24 17:48:56
 */ 
#include "My_I2C.h"

sbit SCL = P1^3;
sbit SDA = P1^5;
bit ack;	//应答标志位 0:ack 1：nak
//I^C时序中延时设置，具体参见各芯片的数据手册
void Delay_us(uchar cnt) {
    while(--cnt) {
        _nop_();;
    }
}
//**************************************
//I2C起始信号
//**************************************
void I2C_Start() {
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    _nop_();;         					//延时
    SDA = 0;                    //产生下降沿
    Delay_us(MPUDELAY);         				//延时
    SCL = 0;                    //拉低时钟线
}
//**************************************
//I2C停止信号
//**************************************
void I2C_Stop() {
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay_us(MPUDELAY);         //延时
    SDA = 1;                    //产生上升沿
    _nop_();
    _nop_();         	//延时
}

//**************************************
//向I2C总线发送一个字节数据
//**************************************
void I2C_SendByte(uchar dat) {
    uchar i;
    for (i=0; i<8; i++) {        //8位计数器
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay_us(MPUDELAY);     //延时
        SCL = 0;                //拉低时钟线
    }
    //下面是接收从设备发回的应答信号
    SDA = 1;
    SCL = 1;
    Delay_us(MPUDELAY);         //延时
    ack = SDA;	//接收设备的ack	_n
    SCL = 0;
    SDA = 1;
}
//**************************************
//从I2C总线接收一个字节数据
//**************************************
uchar I2C_RecvByte(bit _ack) {
    uchar i;
    uchar dat = 0;
    for (i=0; i<8; i++) {        //8位计数器
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay_us(MPUDELAY);     //延时
        dat |= SDA;             //读数据
        SCL = 0;                //拉低时钟线
        _nop_();
        _nop_();     //延时
    }
    //下面时主控器发送ACK或则NAK
    SDA = _ack;	//ack或者nak
    SCL = 1;	//拉高时钟产生第9个时钟
    Delay_us(MPUDELAY);     //延时
    SCL = 0;	//拉底时钟
    SDA = 1;	//释放总线
    _nop_();
    _nop_();
    return dat;
}
//**************************************
//向I2C设备写入一个字节数据
//**************************************
uchar I2C_Single_Write(uchar REG_Address,uchar REG_data) {
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    if(ack) return 0;	//判断是否是ACK如果不是返回0
    I2C_SendByte(REG_Address);    //内部寄存器地址
    if(ack) return 0;	//判断是否是ACK如果不是返回0
    I2C_SendByte(REG_data);       //内部寄存器数据，
    if(ack) return 0;	//判断是否是ACK如果不是返回0
    I2C_Stop();                   //发送停止信号
    return 1;
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
uchar I2C_Single_Read(uchar REG_Address) {
    uchar REG_data;
    I2C_Start();                   //起始信号
    I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
    I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始
    I2C_Start();                   //起始信号
    I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
    REG_data=I2C_RecvByte(1);       //读出寄存器数据

    I2C_Stop();                    //停止信号
    return REG_data;
}
/*读取8位寄存器多个字节
 * @参数 addr    I2C从器件内部地址
 * @参数 length  写入数据长度
 * @参数 Data    保存数据的地址
 * @返回值 返回状态 (1=成功)
 */
uchar I2C_reads(uchar addr,uchar length,uchar *Data) {
    uchar i;
    length --;
    I2C_Start();	//起始信号
    I2C_SendByte(SlaveAddress);	//设备地址+写信号
    if(ack) return 0;
    I2C_SendByte(addr);	//设备内部地址
    if(ack) return 0;
    I2C_Start();	//起始信号
    I2C_SendByte(SlaveAddress+1);	//设备地址+读信号
    if(ack) return 0;
    for(i=0; i<length; i++) {
        Data[i]= I2C_RecvByte(0);	//读取数据,发送ACK
    }
    Data[length] = I2C_RecvByte(1);//读取数据,发送NAK
    I2C_Stop();	//停止信号
    return 1;
}
/*读取8位寄存器一个字节
  参数 addr 寄存器地址
  参数 *Data 数据存储地址
  返回值 (1=成功,0=失败)
*/
uchar I2C_read(uchar addr,uchar *Data) {
    return I2C_reads(addr,1,Data);
}

/**写入8位寄存器的一个位。
 * @参数 addr    I2C从器件内部地址
 * @参数 bitNum  写入的比特位(0-7)
 * @参数 data    写入数据
 * @返回值 返回状态 (1=成功)
 */
uchar I2C_WriteBit(uchar addr,uchar bitNum,uchar Data) {
    uchar b;
    if (I2C_read(addr,&b)) {
        b = (Data != 0) ? (b | (1 << bitNum)):(b & ~(1 << bitNum));
        return I2C_Single_Write(addr,b);	//写入数据
    }
    else
        return 0;
}

/**写入8位寄存器的多个位。
 * @参数 addr     I2C从器件内部地址
 * @参数 bitStart 第一位的写入位置（0-7）
 * @参数 length   写的比特数(不超过8)
 * @参数 Data     写入数据
 * @返回值 返回状态 (1=成功)
 */
uchar I2C_WriteBits(uchar addr,uchar bitStart,uchar length,uchar Data) {
    //      010 要写入的值
    // 76543210 比特位
    //    xxx   args: bitStart=4, length=3
    // 00011100 掩码字节
    // 10101111 原始值（样本）
    // 10100011 原始值 & ~掩码
    // 10101011 掩码 | 原始值
    uchar b,mask=0;
    if (I2C_read(addr,&b))    {
        mask = (((1<<length) - 1) << (bitStart-length + 1));	//掩码
        Data <<=(bitStart - length + 1);	//把写入的数据移动到位
        Data &= mask;
        b &= ~(mask);
        b |= Data;
        I2C_Single_Write(addr,b);	//写入数据
        return 1;
    }
    else
        return 0;
}

/**读取一个位从8位器件的寄存器。
* @参数 addr    I2C从器件内部地址
* @参数 bitNum  位的位置来读取（0-7）
* @参数 *data   数据存储地址
* @返回值（1=成功）
*/
uchar I2C_readBit(uchar addr,uchar bitNum,uchar *Data) {
    uchar b;
    if(I2C_read(addr,&b)) {
        *Data = b & (1 << bitNum);
        return 1;
    }
    else  {
        return 0;
    }
}
/**读取8位寄存器的多个位。
* @参数 addr    I2C从器件内部地址
* @参数 bitStart第一位的位置读取（0-7）
* @参数 length  位读取@参数长度数（不超过8）
* @参数 *data   数据存储地址（即'101'任何bitStart位置读取将等于0X05）
* @返回值（1=成功）
*/
uchar I2C_readBits(uchar addr,uchar bitStart,uchar length,uchar *Data) {
    // 01101001 读取字节
    // 76543210 比特位
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uchar b,mask=0;
    if(I2C_read(addr,&b)) {
        mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *Data = b;
        return 1;
    }
    else
        return 0;
}

void  I2C_Init()
{
    P1M1 &= ~(1 << 3);
    P1M0
    SDA = 1;
    SCL = 1;
}
