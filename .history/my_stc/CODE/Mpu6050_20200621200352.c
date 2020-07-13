/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-18 16:23:44
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-21 20:03:52
 */ 
#include "Mpu6050.h"

uchar MPU6050_Init(void)
{
    
	uchar res;
	
//	MPU6050_OPEN();           				// 初始化 mpu6050 接口
//	DELAY_MS(100);
	MPU6050_WR(MPU_PWR_MGMT1_REG,0X80);		// 复位MPU6050
  	DELAY_MS(10);
	MPU6050_WR(MPU_PWR_MGMT1_REG,0X00);		// 唤醒MPU6050 
	DELAY_MS(10);
	MPU6050_WR(MPU_PWR_MGMT1_REG,0X01);		// 设置CLKSEL,PLL X轴为参考
	DELAY_MS(10);
	MPU6050_WR(MPU_GYRO_CFG_REG,3<<3);		// 陀螺仪传感器,±2000dps	// 0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
	DELAY_MS(10);
	MPU6050_WR(MPU_ACCEL_CFG_REG,0<<3);		// 加速度传感器,±2g		// 0,±2g;1,±4g;2,±8g;3,±16g
	DELAY_MS(10);
	MPU6050_WR(MPU_SAMPLE_RATE_REG,0X00);	// 设置MPU6050的采样率,1KHz
	DELAY_MS(10);
	MPU6050_WR(MPU_CFG_REG,0X01);			// 设置数字低通滤波器,188Hz
	DELAY_MS(100);
	
	res=MPU6050_RD(MPU_DEVICE_ID_REG);
	if(res==MPU6050_ADRESS)// 器件ID正确
	{
//		MPU6050_Offset();
		Balance_Init();
		
		return 0;
 	}
	else 
	{
		return 1;
	}
}