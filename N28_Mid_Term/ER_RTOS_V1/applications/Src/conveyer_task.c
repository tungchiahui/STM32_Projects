#include "conveyer_task.h"

fp32 conveyer_current;

extern RC_ctrl_t rc_ctrl;

extern Other_Devices_t absolute_robot_control;

extern osSemaphoreId Conveyer_BinarySemHandle;
extern osSemaphoreId ShootLink_BinarySemHandle;

void conveyer_task(void const * argument)
{

    while(1)
    {
        xSemaphoreTake(Conveyer_BinarySemHandle,portMAX_DELAY);
       if(rc_ctrl.rc.ch[0] >= 580)
			{
					absolute_robot_control.conveyer_speed = 5000;
			}
			else if(rc_ctrl.rc.ch[0] <= -580)
			{
					absolute_robot_control.conveyer_speed = 0;
			}

      conveyer_current = PID_velocity_realize_2(absolute_robot_control.conveyer_speed,7);
			xSemaphoreGive(ShootLink_BinarySemHandle);
			//CAN2_CMD_2(0,0,conveyer_current,0);
      osDelay(1);
    }
}

