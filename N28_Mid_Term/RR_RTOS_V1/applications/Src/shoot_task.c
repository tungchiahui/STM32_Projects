#include "shoot_task.h"

fp32 shoot_current[2];
fp32 shoot_speed[2];

extern Other_Devices_t absolute_robot_control;

extern osSemaphoreId Shoot_BinarySemHandle;

void shoot_task(void const * argument)
{
    while(1)
    {
		xSemaphoreTake(Shoot_BinarySemHandle,portMAX_DELAY);
			
        shoot_speed[0] = absolute_robot_control.shoot_speed[0];
        shoot_speed[1] = absolute_robot_control.shoot_speed[0];
        shoot_current[0] = PID_position_realize_2(shoot_speed[0],5);
        shoot_current[1] = PID_position_realize_2(shoot_speed[1],6);
        CAN2_CMD_2(shoot_current[0],shoot_current[1],0,0);
        osDelay(2);
    }
}

