#include "startup_main.h"
#include "mpu6050.h"
#include "tim.h"


void startup_main(void)
{
	//��ʼ��MPU6050
	mpu6050.I2C_Search_Addr();
  mpu6050.Init();
	//�ȴ��豸��ʼ���ɹ�
	HAL_Delay(1000);
	
	//������Ʈ
	mpu6050.cail.CalibrateGyro();
	
	//MPU6050���ն�ʱ���ж�
	HAL_TIM_Base_Start_IT(&htim6);
	
#if isRTOS==0    	//������������
	for(;;)  //��ͬ��while(true)
	{

	}
#endif
}