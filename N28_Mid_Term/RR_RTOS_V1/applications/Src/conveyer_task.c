#include "conveyer_task.h"

fp32 conveyer_speed;
fp32 conveyer_current;
extern Other_Devices_t absolute_robot_control;

extern osSemaphoreId Conveyer_BinarySemHandle;

void conveyer_task(void const * argument)
{

    while(1)
    {
		xSemaphoreTake(Conveyer_BinarySemHandle,portMAX_DELAY);
			
        conveyer_speed = absolute_robot_control.conveyer_speed;
        conveyer_current = PID_velocity_realize_2(conveyer_speed,1);
        CAN2_CMD_1(conveyer_current,0,0,0);
        osDelay(2);
    }
}
