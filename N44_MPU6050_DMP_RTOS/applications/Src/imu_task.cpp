#include "imu_task.h"
#include "MPU6050.h"


extern "C"
void StartDefaultTask(void const * argument)
{
	
	int ret = 0;
  do {
	ret = MPU6050_DMP_init();
  } while (ret);
	
	for(;;)
	{
		MPU6050_DMP_Get_Data();
	}
}