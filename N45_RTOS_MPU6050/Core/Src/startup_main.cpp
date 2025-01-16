#include "startup_main.h"
#include "mpu6050.h"
#include "tim.h"


void startup_main(void)
{
	//初始化MPU6050
	mpu6050.I2C_Search_Addr();
  mpu6050.Init();
	//等待设备初始化成功
	HAL_Delay(1000);
	
	//矫正零飘
	mpu6050.cail.CalibrateGyro();
	
	//MPU6050接收定时器中断
	HAL_TIM_Base_Start_IT(&htim6);
	
#if isRTOS==0    	//如果是裸机开发
	for(;;)  //等同于while(true)
	{

	}
#endif
}