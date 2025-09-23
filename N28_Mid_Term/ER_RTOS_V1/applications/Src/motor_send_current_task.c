#include "motor_send_current_task.h"

extern osSemaphoreId ShootLink_BinarySemHandle;
extern osSemaphoreId ConveyerLink_BinarySemHandle;

extern fp32 conveyer_current;
extern fp32 shoot_current[2];

void can_send_task(void const * argument)
{
	while(1)
	{
		xSemaphoreTake(ShootLink_BinarySemHandle,portMAX_DELAY);
		xSemaphoreTake(ConveyerLink_BinarySemHandle,portMAX_DELAY);
		CAN2_CMD_2(shoot_current[0],shoot_current[1],conveyer_current,0);
	}

}
