#include "mpu6050.h"
#include <math.h>
#include "MahonyAHRS.h"
#include "i2c.h"

uint8_t I2C_Device_Addr;   //���ҵ���д��MPU6050_ADDR����

//IMU_Kalman_t imu_kalman_data;  //�������˲���д

MPU6050 mpu6050;



extern float q[4];

//extern uint32_t PreviousWakeTime;
void MPU6050::Delay(uint32_t ms)
{
//��������FreeRTOS��ʹ�ã�����ֱ���������н��г�ʼ��
//	osDelayUntil(&PreviousWakeTime,ms);
	HAL_Delay(ms);
}


/**
 * @brief  I2C�豸��ַ��ѯ����
 * @param  void
 * @retval void
* @attention �ҵ�һ���豸��ַ�Ϳ���ע�͵���Ȼ����MPU6050_ADDR����д��
 */
 void MPU6050::I2C_Search_Addr(void)
{
  for(I2C_Device_Addr = 0;I2C_Device_Addr < 255;I2C_Device_Addr++)
  {
		//Ѱ���豸��ַ������ӡ��ַ��Ȼ�����豸�Ƿ����
	  if(HAL_I2C_IsDeviceReady (&hi2c2 ,I2C_Device_Addr ,1 ,1000) == HAL_OK )
	  {
//			printf("HAL_OK\r\n");
//		  printf("%d\r\n",Addr_Inspect);
		  break;
	  }
  }
}


/**
 * @brief  MPU6050��ʼ������  �Ը����Ĵ�����������
 * @param  void
 * @retval void
* @attention 
 */
void MPU6050::Init(void)
{
	uint8_t check,Data;
	
	// ����豸ID WHO_AM_I
	//Ĭ��ֵ��0x68(104)��ֻ�õ�bit6-bit1 ��x110 100x��Ҳ���� 0110 1000
	HAL_I2C_Mem_Read (&hi2c2 ,MPU6050_ADDR,WHO_AM_I_REG,1,&check ,1,1000);
	
	if(check == 0x68)  //����豸����
	{
		//�ѵ�Դ����1�Ĵ�����0���򲻻�ʧ�ܶ�Ӧ��������ʱ��Դѡ���ڲ�8MHz����
		Data = 0x00; //0000 0000
		HAL_I2C_Mem_Write (&hi2c2 ,MPU6050_ADDR ,PWR_MGMT_1_REG ,1,&Data ,1,1000);
		
		//SMPLRT_DIV�Ĵ���,�������ݲ�����Ϊ1KHz;
		// ����Ƶ��=���������Ƶ��/��1+SMPLRT_DIV��
		Data = 0x07;   //0000 0111
		HAL_I2C_Mem_Write (&hi2c2 ,MPU6050_ADDR ,SMPLRT_DIV_REG ,1 ,&Data,1,1000);
		
		// ����ACCEL_CONFIG�Ĵ���
		// ������X,Y,Z,���Լ죬���ټƵ������̷�Χѡ���2g
		Data = 0x00;   //0000 0000
		HAL_I2C_Mem_Write (&hi2c2 ,MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);
		
		// ����GYRO_CONFIG�Ĵ���
		//  ������X,Y,Z,���Լ죬�����ǵ������̷�Χѡ��� 250deg/s
		Data = 0x00;  //0000 0000
		HAL_I2C_Mem_Write (&hi2c2 ,MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);
	}
}




/**
 * @brief  MPU6050��ȡ���ٶȺ���
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
	//��ȡ6�ֽڵ����ݴ�ACCEL_XOUT_H�Ĵ�����
	
	HAL_I2C_Mem_Read (&hi2c2 ,MPU6050_ADDR ,ACCEL_XOUT_H_REG ,1,Rec_Data ,6,1000);
	//�Ѹ�8λ�͵�8λ�ϲ�Ϊ16λ����
	Accel_X_Buffer = (int16_t)(Rec_Data [0] <<8 | Rec_Data [1]);
	Accel_Y_Buffer = (int16_t)(Rec_Data [2] <<8 | Rec_Data [3]);
	Accel_Z_Buffer = (int16_t)(Rec_Data [4] <<8 | Rec_Data [5]);
	
	//��Ϊ���ټƵ������̷�Χѡ���2g,���Լ��ٶȼƲ���ֵ����������ͷֱ���Ҫѡ16384LSB/g
	//��ʱ��λ��g(��λ�������ٶ�)
	mpu6050.data.Accel.X = Accel_X_Buffer / 16384.0f;
	mpu6050.data.Accel.Y = Accel_Y_Buffer / 16384.0f;
	mpu6050.data.Accel.Z = Accel_Z_Buffer / 16384.0f;
}

/**
 * @brief  MPU6050��ȡ���ٶȺ���
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
	//��ȡ6�ֽڵ����ݴ�GYRO_XOUT_H�Ĵ�����
	
	HAL_I2C_Mem_Read (&hi2c2, MPU6050_ADDR ,GYRO_XOUT_H_REG ,1,Rec_Data ,6 ,1000);
	//�Ѹ�8λ�͵�8λ�ϲ�Ϊ16λ����
	Gyro_X_Buffer = (int16_t )(Rec_Data [0] << 8 | Rec_Data [1]);
	Gyro_Y_Buffer = (int16_t )(Rec_Data [2] << 8 | Rec_Data [3]);
	Gyro_Z_Buffer = (int16_t )(Rec_Data [4] << 8 | Rec_Data [5]);
	//��Ϊ�����ǵ������̷�Χѡ��� 250deg/s,���������ǲ���ֵ����������ͷֱ���Ҫѡ131LSB/��/s
	//��ʱ��λ��dec/s
	mpu6050.data.Gyro.X = Gyro_X_Buffer / 131.0f;
	mpu6050.data.Gyro.Y = Gyro_Y_Buffer / 131.0f;
	mpu6050.data.Gyro.Z = Gyro_Z_Buffer / 131.0f;
}

/**
 * @brief  MPU6050��ȡ�¶Ⱥ���
 * @param  void
 * @retval void
* @attention 
 */
void MPU6050::GET::Temp(void)
{
	uint8_t Rec_Data[2];
	int16_t Temp_Buffer = 0;
	//��ȡ2�ֽڵ����ݴ�TEMP_OUT_H_REG�Ĵ�����
	HAL_I2C_Mem_Read (&hi2c2 ,MPU6050_ADDR ,TEMP_OUT_H_REG ,1 ,Rec_Data  ,2 ,1000);
	
	//�Ѹ�8λ�͵�8λ�ϲ�Ϊ16λ����
	Temp_Buffer = (int16_t )(Rec_Data [0]<<8 | Rec_Data [1]);
	
	//˵������Ĺ�ʽ
	//Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53
	//��λ�����϶�
	mpu6050.data.Temp = 36.53f + (Temp_Buffer ) / 340.0f;
}

void MPU6050::GET::Euler(void) 
{
		//ת����λ
		mpu6050.Convert_Dimensions(q);
	
		//��һ����Ԫ��תŷ����(rad)
		mpu6050.data.Euler.yaw = atan2f(2.0f*(q[0]*q[3]+q[1]*q[2]), 2.0f*(q[0]*q[0]+q[1]*q[1])-1.0f);
		mpu6050.data.Euler.pitch = asinf(-2.0f*(q[1]*q[3]-q[0]*q[2])); 
		mpu6050.data.Euler.roll = atan2f(2.0f*(q[0]*q[1]+q[2]*q[3]),2.0f*(q[0]*q[0]+q[3]*q[3])-1.0f); 
		//��ŷ���ǵ�λת��(dec)
		mpu6050.data.Euler.yaw = mpu6050.data.Euler.yaw * (180 / 3.14159265358979f);
		mpu6050.data.Euler.pitch = mpu6050.data.Euler.pitch * (180 / 3.14159265358979f);
		mpu6050.data.Euler.roll = mpu6050.data.Euler.roll * (180 / 3.14159265358979f);
}

void MPU6050::Convert_Dimensions(fp32 *q)
{
	//�ѵ�λ��gת��Ϊm/s
	mpu6050.data.Accel.X = mpu6050.data.Accel.X  * 9.8f;
	mpu6050.data.Accel.Y = mpu6050.data.Accel.Y * 9.8f;
	mpu6050.data.Accel.Z = mpu6050.data.Accel.Z * 9.8f;
	
	//�ѵ�λ��dec/sת��Ϊrad/s
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


//��ȡIMU6����¶�����
void MPU6050::GET::All(void)
{
	this->Accel();
	this->Gyro();
	
	//������Ʈ
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
      // �������Ƕ�ȡ����
      float gx, gy, gz;
			mpu6050.Get.Gyro();
			gx = mpu6050.data.Gyro.X;
			gy = mpu6050.data.Gyro.Y;
			gz = mpu6050.data.Gyro.Z;
			
      sum_gx += gx;
      sum_gy += gy;
      sum_gz += gz;
		
		//����ʱ��
		mpu6050.Delay(5);
    }

    // ����ƽ��ֵ
    gyro_bias_x = sum_gx / CALIBRATION_SAMPLES;
    gyro_bias_y = sum_gy / CALIBRATION_SAMPLES;
    gyro_bias_z = sum_gz / CALIBRATION_SAMPLES;
}

void MPU6050::CAIL::ProcessGyro(void)
{
	//У׼
	mpu6050.data.Gyro.X -= gyro_bias_x;
	mpu6050.data.Gyro.Y -= gyro_bias_y;
	mpu6050.data.Gyro.Z -= gyro_bias_z;
}


