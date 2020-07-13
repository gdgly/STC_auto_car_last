/*
 * @Description  : 
 * @Version      : 
 * @Author       : yan_wen
 * @Date         : 2020-06-18 16:23:44
 * @LastEdiors   : yan_wen
 * @LastEditTime : 2020-06-21 19:38:25
 */ 
void MPU6050_Init() {
    I2C_Single_Write(PWR_MGMT_1, 0x00);	//解除休眠状态
    I2C_Single_Write(SMPLRT_DIV, 0x07);	//陀螺仪采样率	125hz
    I2C_Single_Write(CONFIG, 0x01);		//1为184hz  4为21HZ滤波 延时A8.5ms 此处取值应相当注意，延时与系统周期相近为宜
    I2C_Single_Write(GYRO_CONFIG, 0x08);	//角速度 陀螺仪500度/S 65.536LSB 即65.536为1度每秒
    I2C_Single_Write(ACCEL_CONFIG, 0x00);  //加速度 使用+-2g量程	 16384LSB
}