#ifndef __MPU6050_I2C_H_
#define __MPU6050_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include "startup_main.h"

//#define MPU6050_ADDR I2C_Device_Addr
#define MPU6050_ADDR 0xD0  //设备地址通过地址查询函数找到的

//以下全是通过数据手册查到  地址是首地址，也就是高位的地址。

//该寄存器用于 MPU-60X0的陀螺仪采样频率输出设置
#define SMPLRT_DIV_REG 0x19   //采样频率分频器寄存器地址

//这寄存器是用来触发加速度计自检和配置加速度计的满量程范围。这个寄存器也可以用于配置数字高通滤波器（DHPF）。
#define ACCEL_CONFIG_REG 0x1c  //加速度计配置级寄存器地址
//这个寄存器存储最近加速度计的测量值。
#define ACCEL_XOUT_H_REG 0x3B  //加速度计测量值寄存器地址

//这寄存器是用来触发陀螺仪自检和配置陀螺仪的满量程范围。
#define GYRO_CONFIG_REG 0x1B   //陀螺仪配置寄存器地址
//这个寄存器存储最近陀螺仪的测量值。
#define GYRO_XOUT_H_REG 0x43	//陀螺仪测量值寄存器地址

//这些寄存器存储最近温度传感器的测量值。
#define TEMP_OUT_H_REG 0x41   //温度测量值寄存器地址


//这个寄存器允许用户配置电源模式和时钟源。还提供了复位整个设备和禁用温度传感器的位
#define PWR_MGMT_1_REG 0x6B   //电源管理1寄存器地址

//这个寄存器用于标识设备的身份。WHO_AM_I的内容是 MPU-60X0的 7位 I2C地址的头 6位。最后一位地址由 AD0引脚确定。AD0引脚的值跟寄存器无关
#define WHO_AM_I_REG 0x75     //我是谁寄存器地址

//等待设备初始化的时间
#define IMU_INIT_WAITING_TIME 500

typedef struct
{
	__packed struct
	{
		fp32 X;
		fp32 Y;
		fp32 Z;
	}Accel;

	__packed struct
	{
		fp32 X;
		fp32 Y;
		fp32 Z;
	}Gyro;
	
	__packed struct
	{
		fp32 X;
		fp32 Y;
		fp32 Z;
	}Magnet;
	
		__packed struct
	{
		fp32 q[4];
	}Quat;//欧拉角
	
	__packed struct
	{
		fp32 yaw;    //偏航角  前进的偏移
		fp32 pitch;  //俯仰角	 前后的上下摆动
		fp32 roll;   //翻滚角  左右的上下摆动
	}Euler;//欧拉角
	
	fp32 Temp;
	
}MPU6050_t;

class MPU6050
{
	public:
		void I2C_Search_Addr(void);
		void Init(void );
		void Convert_Dimensions(fp32 *q);
		void Delay(uint32_t ms);
	
		MPU6050_t data;
		
		class GET
		{
			public:
			void Accel(void);
			void Gyro(void);
			void Euler(void);
			void Temp(void);
			void All(void);
		}Get;
		
		class CAIL
		{
			public:
				void CalibrateGyro(void);
				void ProcessGyro(void);
		}cail;

};




//void MPU6050_Calc_Euler(IMU_t * data);

extern MPU6050 mpu6050;


#ifdef __cplusplus
}
#endif

#endif
