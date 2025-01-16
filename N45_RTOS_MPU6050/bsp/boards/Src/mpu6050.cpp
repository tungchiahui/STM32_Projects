#include "mpu6050.h"
#include <math.h>
#include "MahonyAHRS.h"
#include "i2c.h"

uint8_t I2C_Device_Addr;   //查找到后，写到MPU6050_ADDR宏里

//IMU_Kalman_t imu_kalman_data;  //卡尔曼滤波待写

MPU6050 mpu6050;



extern float q[4];

//extern uint32_t PreviousWakeTime;
void MPU6050::Delay(uint32_t ms)
{
//不建议在FreeRTOS中使用，建议直接主函数中进行初始化
//	osDelayUntil(&PreviousWakeTime,ms);
	HAL_Delay(ms);
}


/**
 * @brief  I2C设备地址查询函数
 * @param  void
 * @retval void
* @attention 找到一次设备地址就可以注释掉，然后在MPU6050_ADDR宏里写上
 */
 void MPU6050::I2C_Search_Addr(void)
{
  for(I2C_Device_Addr = 0;I2C_Device_Addr < 255;I2C_Device_Addr++)
  {
		//寻找设备地址，并打印地址，然后检查设备是否就绪
	  if(HAL_I2C_IsDeviceReady (&hi2c2 ,I2C_Device_Addr ,1 ,1000) == HAL_OK )
	  {
//			printf("HAL_OK\r\n");
//		  printf("%d\r\n",Addr_Inspect);
		  break;
	  }
  }
}


/**
 * @brief  MPU6050初始化函数  对各个寄存器进行配置
 * @param  void
 * @retval void
* @attention 
 */
void MPU6050::Init(void)
{
	uint8_t check,Data;
	
	// 检查设备ID WHO_AM_I
	//默认值是0x68(104)，只用到bit6-bit1 即x110 100x，也就是 0110 1000
	HAL_I2C_Mem_Read (&hi2c2 ,MPU6050_ADDR,WHO_AM_I_REG,1,&check ,1,1000);
	
	if(check == 0x68)  //如果设备存在
	{
		//把电源管理1寄存器清0，则不会失能对应传感器，时钟源选择内部8MHz振荡器
		Data = 0x00; //0000 0000
		HAL_I2C_Mem_Write (&hi2c2 ,MPU6050_ADDR ,PWR_MGMT_1_REG ,1,&Data ,1,1000);
		
		//SMPLRT_DIV寄存器,设置数据采样率为1KHz;
		// 采样频率=陀螺仪输出频率/（1+SMPLRT_DIV）
		Data = 0x07;   //0000 0111
		HAL_I2C_Mem_Write (&hi2c2 ,MPU6050_ADDR ,SMPLRT_DIV_REG ,1 ,&Data,1,1000);
		
		// 设置ACCEL_CONFIG寄存器
		// 不开启X,Y,Z,轴自检，加速计的满量程范围选择±2g
		Data = 0x00;   //0000 0000
		HAL_I2C_Mem_Write (&hi2c2 ,MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);
		
		// 设置GYRO_CONFIG寄存器
		//  不开启X,Y,Z,轴自检，陀螺仪的满量程范围选择± 250deg/s
		Data = 0x00;  //0000 0000
		HAL_I2C_Mem_Write (&hi2c2 ,MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);
	}
}




/**
 * @brief  MPU6050读取加速度函数
 * @param  void
 * @retval void
* @attention 
 */
void MPU6050::GET::Accel(void)
{
	uint8_t Rec_Data[6];
	
	int16_t Accel_X_Buffer = 0;
	int16_t Accel_Y_Buffer = 0;
	int16_t Accel_Z_Buffer = 0;
	//读取6字节的数据从ACCEL_XOUT_H寄存器中
	
	HAL_I2C_Mem_Read (&hi2c2 ,MPU6050_ADDR ,ACCEL_XOUT_H_REG ,1,Rec_Data ,6,1000);
	//把高8位和低8位合并为16位数据
	Accel_X_Buffer = (int16_t)(Rec_Data [0] <<8 | Rec_Data [1]);
	Accel_Y_Buffer = (int16_t)(Rec_Data [2] <<8 | Rec_Data [3]);
	Accel_Z_Buffer = (int16_t)(Rec_Data [4] <<8 | Rec_Data [5]);
	
	//因为加速计的满量程范围选择±2g,所以加速度计测量值的灵敏度最低分辨率要选16384LSB/g
	//此时单位是g(单位重力加速度)
	mpu6050.data.Accel.X = Accel_X_Buffer / 16384.0f;
	mpu6050.data.Accel.Y = Accel_Y_Buffer / 16384.0f;
	mpu6050.data.Accel.Z = Accel_Z_Buffer / 16384.0f;
}

/**
 * @brief  MPU6050读取角速度函数
 * @param  void
 * @retval void
* @attention 
 */
void MPU6050::GET::Gyro(void )
{
	uint8_t Rec_Data[6];
	
	int16_t Gyro_X_Buffer = 0;
	int16_t Gyro_Y_Buffer = 0;
	int16_t Gyro_Z_Buffer = 0;
	//读取6字节的数据从GYRO_XOUT_H寄存器中
	
	HAL_I2C_Mem_Read (&hi2c2, MPU6050_ADDR ,GYRO_XOUT_H_REG ,1,Rec_Data ,6 ,1000);
	//把高8位和低8位合并为16位数据
	Gyro_X_Buffer = (int16_t )(Rec_Data [0] << 8 | Rec_Data [1]);
	Gyro_Y_Buffer = (int16_t )(Rec_Data [2] << 8 | Rec_Data [3]);
	Gyro_Z_Buffer = (int16_t )(Rec_Data [4] << 8 | Rec_Data [5]);
	//因为陀螺仪的满量程范围选择± 250deg/s,所以陀螺仪测量值的灵敏度最低分辨率要选131LSB/°/s
	//此时单位是dec/s
	mpu6050.data.Gyro.X = Gyro_X_Buffer / 131.0f;
	mpu6050.data.Gyro.Y = Gyro_Y_Buffer / 131.0f;
	mpu6050.data.Gyro.Z = Gyro_Z_Buffer / 131.0f;
}

/**
 * @brief  MPU6050读取温度函数
 * @param  void
 * @retval void
* @attention 
 */
void MPU6050::GET::Temp(void)
{
	uint8_t Rec_Data[2];
	int16_t Temp_Buffer = 0;
	//读取2字节的数据从TEMP_OUT_H_REG寄存器中
	HAL_I2C_Mem_Read (&hi2c2 ,MPU6050_ADDR ,TEMP_OUT_H_REG ,1 ,Rec_Data  ,2 ,1000);
	
	//把高8位和低8位合并为16位数据
	Temp_Buffer = (int16_t )(Rec_Data [0]<<8 | Rec_Data [1]);
	
	//说明书里的公式
	//Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53
	//单位是摄氏度
	mpu6050.data.Temp = 36.53f + (Temp_Buffer ) / 340.0f;
}

void MPU6050::GET::Euler(void) 
{
		//转换单位
		mpu6050.Convert_Dimensions(q);
	
		//归一化四元数转欧拉角(rad)
		mpu6050.data.Euler.yaw = atan2f(2.0f*(q[0]*q[3]+q[1]*q[2]), 2.0f*(q[0]*q[0]+q[1]*q[1])-1.0f);
		mpu6050.data.Euler.pitch = asinf(-2.0f*(q[1]*q[3]-q[0]*q[2])); 
		mpu6050.data.Euler.roll = atan2f(2.0f*(q[0]*q[1]+q[2]*q[3]),2.0f*(q[0]*q[0]+q[3]*q[3])-1.0f); 
		//将欧拉角单位转化(dec)
		mpu6050.data.Euler.yaw = mpu6050.data.Euler.yaw * (180 / 3.14159265358979f);
		mpu6050.data.Euler.pitch = mpu6050.data.Euler.pitch * (180 / 3.14159265358979f);
		mpu6050.data.Euler.roll = mpu6050.data.Euler.roll * (180 / 3.14159265358979f);
}

void MPU6050::Convert_Dimensions(fp32 *q)
{
	//把单位从g转化为m/s
	mpu6050.data.Accel.X = mpu6050.data.Accel.X  * 9.8f;
	mpu6050.data.Accel.Y = mpu6050.data.Accel.Y * 9.8f;
	mpu6050.data.Accel.Z = mpu6050.data.Accel.Z * 9.8f;
	
	//把单位从dec/s转化为rad/s
	mpu6050.data.Gyro.X = mpu6050.data.Gyro.X  * (3.14159265358979f / 180.0f);
	mpu6050.data.Gyro.Y = mpu6050.data.Gyro.Y * (3.14159265358979f / 180.0f);
	mpu6050.data.Gyro.Z = mpu6050.data.Gyro.Z * (3.14159265358979f / 180.0f);
	
//	MadgwickAHRSupdate(mpu6050.data.Gyro.X,mpu6050.data.Gyro.Y,mpu6050.data.Gyro.Z,
//										 mpu6050.data.Accel.X,mpu6050.data.Accel.Y,mpu6050.data.Accel.Z,
//										 0,0,0);
	
	MahonyAHRSupdate(mpu6050.data.Gyro.X,mpu6050.data.Gyro.Y,mpu6050.data.Gyro.Z,
										 mpu6050.data.Accel.X,mpu6050.data.Accel.Y,mpu6050.data.Accel.Z,
										 0,0,0);
	
		for(int16_t i = 0;i < 4;i++)
		{
			mpu6050.data.Quat.q[i] = q[i];
		}
}


//读取IMU6轴和温度数据
void MPU6050::GET::All(void)
{
	this->Accel();
	this->Gyro();
	
	//处理零飘
	mpu6050.cail.ProcessGyro();
	
	this->Euler();
	this->Temp();
	
}


#define CALIBRATION_SAMPLES 500

float gyro_bias_x = 0.0f;
float gyro_bias_y = 0.0f;
float gyro_bias_z = 0.0f;

void MPU6050::CAIL::CalibrateGyro()
{
	  float sum_gx = 0.0f, sum_gy = 0.0f, sum_gz = 0.0f;

    for (int i = 0; i < CALIBRATION_SAMPLES; i++) 
		{
      // 从陀螺仪读取数据
      float gx, gy, gz;
			mpu6050.Get.Gyro();
			gx = mpu6050.data.Gyro.X;
			gy = mpu6050.data.Gyro.Y;
			gz = mpu6050.data.Gyro.Z;
			
      sum_gx += gx;
      sum_gy += gy;
      sum_gz += gz;
		
		//矫正时间
		mpu6050.Delay(5);
    }

    // 计算平均值
    gyro_bias_x = sum_gx / CALIBRATION_SAMPLES;
    gyro_bias_y = sum_gy / CALIBRATION_SAMPLES;
    gyro_bias_z = sum_gz / CALIBRATION_SAMPLES;
}

void MPU6050::CAIL::ProcessGyro(void)
{
	//校准
	mpu6050.data.Gyro.X -= gyro_bias_x;
	mpu6050.data.Gyro.Y -= gyro_bias_y;
	mpu6050.data.Gyro.Z -= gyro_bias_z;
}


