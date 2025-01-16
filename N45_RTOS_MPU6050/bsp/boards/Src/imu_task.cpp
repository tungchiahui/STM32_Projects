#include "imu_task.h"
#include "mpu6050.h"
#include "tim.h"

uint32_t PreviousWakeTime;

// __weakÈõº¯Êý
extern "C"
void imu_task(void const * argument)
{
	
	PreviousWakeTime = osKernelSysTick();

	
	for(;;)
	{
		
		osDelayUntil(&PreviousWakeTime,200);
	}
}


extern "C"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM7) 
	{
    HAL_IncTick();
  }
	else if (htim->Instance == TIM6) 
	{
    mpu6050.Get.All();
  }
}



