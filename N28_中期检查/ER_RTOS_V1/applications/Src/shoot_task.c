#include "shoot_task.h"

//CAN1 1-4 M3508底盘电机        5 爪子转盘电机  6爪子升降电机  
//CAN2 1-4 M2006舵电机      		 5 摩擦轮A  6 摩擦轮B  7传环电机


fp32 shoot_current[2];

extern Other_Devices_t absolute_robot_control;

extern osSemaphoreId Shoot_BinarySemHandle;
extern osSemaphoreId ConveyerLink_BinarySemHandle;

extern RC_ctrl_t rc_ctrl;

void shoot_task(void const * argument)
{
    while(1)
    {
        xSemaphoreTake(Shoot_BinarySemHandle,portMAX_DELAY);
        
			 absolute_robot_control.shoot_speed[0] =  rc_ctrl.rc.ch[1] * 13.67;
			 absolute_robot_control.shoot_speed[1] = -rc_ctrl.rc.ch[1] * 13.67;
			
        for(int i = 0;i < 2;i++)
        {
            shoot_current[i] = PID_velocity_realize_1(absolute_robot_control.shoot_speed[i],i + 5);
        }
				xSemaphoreGive(ConveyerLink_BinarySemHandle);
//        CAN2_CMD_2(shoot_current[0],shoot_current[1],0,0);
        osDelay(1);
    }
}
