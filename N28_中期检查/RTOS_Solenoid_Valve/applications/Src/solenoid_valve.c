#include "solenoid_valve.h"

extern RC_ctrl_t rc_ctrl;

void solenoid_valve_task(void const * argument)
{
	while(1)
	{
		if(rc_ctrl.rc.ch[1] >= 580)
		{
			 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
		}
		else if(rc_ctrl.rc.ch[1] <= -580)
		{
			 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
		}
		else
		{
			
		}
		vTaskDelay(2);
	}
}
