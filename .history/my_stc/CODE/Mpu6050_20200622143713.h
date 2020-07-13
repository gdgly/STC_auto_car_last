/*
 * @Description  : 逐飞做的
 * @Version      : 
 * @Date         : 2020-06-18 16:23:25
 * @LastEditTime : 2020-06-22 14:37:10
 */ 
#ifndef __MPU6050_H__
#define __MPU6050_H__
#include "common.h"
#include "headfile.h" 

#define MPUDELAY 6		    //MPU6050等待时间

//宏定义调用底层的I2C接口
#define MPU6050_OPEN()      	I2C_Init(IIC_1,IIC1_SCL_P15,IIC1_SDA_P14)
#define MPU6050_ADRESS          (0X68)      /*MPU6050_Device Address*/
#define MPU6050_WR(reg,value)  	I2C_Single_Write(MPU6050_ADRESS, reg)//mpu6050 写寄存器
#define MPU6050_RD(reg)         I2C_Single_Read(MPU6050_ADRESS)		//mpu6050 读寄存器

/* MPU6050 Register Address ------------------------------------------------------------*/
//#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs寄存器,可读取版本号,寄存器手册未提到
//#define MPU_PROD_ID_REG			0X0C	//prod id寄存器,在寄存器手册未提到
#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器A
#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				    0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG		  0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		  0X1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			  0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG			  0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		  0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			  0X38	//中断使能寄存器
#define MPU_INT_STA_REG		  	0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG	  	0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		  0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		  0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG	  	0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		  0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		  0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		  0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			  0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG	  	0X75	//器件ID寄存器
#define	SMPLRT_DIV		          0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	USER_CTRL			0x6A	//用户配置 寄存器允许用户使能或使能主机，传感器信号通道和传感器寄存器也可以使用这个寄存器复位。
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取
//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define MPU_ADDR				0X68

////因为模块AD0默认接GND,所以转为读写地址后,为0XD1和0XD0(如果接VCC,则为0XD3和0XD2)  
//#define MPU_READ    0XD1
//#define MPU_WRITE   0XD0

uchar My_MPU6050_Init(void);
void MPU6050_Offset(void);
void MPU6050_GetData(S_INT16_XYZ *GYRO, S_INT16_XYZ *ACC);
int16 GetData(uchar REG_Address);
void Data_Filter(void);
void KalmanFilter(float ACC_Angle);
void Get_Attitude(void);	// 姿态解算
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

#endif