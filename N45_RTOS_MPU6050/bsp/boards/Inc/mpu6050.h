#ifndef __MPU6050_I2C_H_
#define __MPU6050_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include "startup_main.h"

//#define MPU6050_ADDR I2C_Device_Addr
#define MPU6050_ADDR 0xD0  //�豸��ַͨ����ַ��ѯ�����ҵ���

//����ȫ��ͨ�������ֲ�鵽  ��ַ���׵�ַ��Ҳ���Ǹ�λ�ĵ�ַ��

//�üĴ������� MPU-60X0�������ǲ���Ƶ���������
#define SMPLRT_DIV_REG 0x19   //����Ƶ�ʷ�Ƶ���Ĵ�����ַ

//��Ĵ����������������ٶȼ��Լ�����ü��ٶȼƵ������̷�Χ������Ĵ���Ҳ���������������ָ�ͨ�˲�����DHPF����
#define ACCEL_CONFIG_REG 0x1c  //���ٶȼ����ü��Ĵ�����ַ
//����Ĵ����洢������ٶȼƵĲ���ֵ��
#define ACCEL_XOUT_H_REG 0x3B  //���ٶȼƲ���ֵ�Ĵ�����ַ

//��Ĵ��������������������Լ�����������ǵ������̷�Χ��
#define GYRO_CONFIG_REG 0x1B   //���������üĴ�����ַ
//����Ĵ����洢��������ǵĲ���ֵ��
#define GYRO_XOUT_H_REG 0x43	//�����ǲ���ֵ�Ĵ�����ַ

//��Щ�Ĵ����洢����¶ȴ������Ĳ���ֵ��
#define TEMP_OUT_H_REG 0x41   //�¶Ȳ���ֵ�Ĵ�����ַ


//����Ĵ��������û����õ�Դģʽ��ʱ��Դ�����ṩ�˸�λ�����豸�ͽ����¶ȴ�������λ
#define PWR_MGMT_1_REG 0x6B   //��Դ����1�Ĵ�����ַ

//����Ĵ������ڱ�ʶ�豸����ݡ�WHO_AM_I�������� MPU-60X0�� 7λ I2C��ַ��ͷ 6λ�����һλ��ַ�� AD0����ȷ����AD0���ŵ�ֵ���Ĵ����޹�
#define WHO_AM_I_REG 0x75     //����˭�Ĵ�����ַ

//�ȴ��豸��ʼ����ʱ��
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
	}Quat;//ŷ����
	
	__packed struct
	{
		fp32 yaw;    //ƫ����  ǰ����ƫ��
		fp32 pitch;  //������	 ǰ������°ڶ�
		fp32 roll;   //������  ���ҵ����°ڶ�
	}Euler;//ŷ����
	
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
