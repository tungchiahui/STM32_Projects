#include "claw_task.h"

//CAN1 1-4 M3508底盘电机        5 爪子转盘电机  6爪子升降电机  
//CAN2 1-4 M2006舵电机      		 5 摩擦轮A  6 摩擦轮B  7传环电机

extern RC_ctrl_t rc_ctrl;

fp32 claw_position_current[2];

extern Other_Devices_t absolute_robot_control;

extern osSemaphoreId ClawCatch_BinarySemHandle;
extern osSemaphoreId ClawPosition_BinarySemHandle;

void claw_catch_task(void const * argument)
{

    while(1)
    {
			xSemaphoreTake(ClawCatch_BinarySemHandle,portMAX_DELAY);
        //双板，此板不需要抓
    }
}

void claw_position_task(void const * argument)
{

    while(1)
    {
			xSemaphoreTake(ClawPosition_BinarySemHandle,portMAX_DELAY);
/*********************************************转盘电机*****************************************************/
			if(ABS(rc_ctrl.rc.ch[1]) <= 180)
			{
				absolute_robot_control.claw_position_speed[0] = -(fp32)rc_ctrl.rc.ch[0] * 1.0f;
				claw_position_current[0] = PID_velocity_realize_1(absolute_robot_control.claw_position_speed[0],5);
			}
/*********************************************升降电机*****************************************************/			
			if(ABS(rc_ctrl.rc.ch[0]) <= 180)
			{
				if(ABS(rc_ctrl.rc.ch[1]) >= 300 )
				{
				absolute_robot_control.claw_position_speed[1] = (fp32)rc_ctrl.rc.ch[1] * 1.0f;
				}
				else if(ABS(rc_ctrl.rc.ch[1]) <= -300 )
				{
				absolute_robot_control.claw_position_speed[1] = (fp32)rc_ctrl.rc.ch[1] * 1.0f;
				}
				else if(ABS(rc_ctrl.rc.ch[1]) == 0 )
				{
					absolute_robot_control.claw_position_speed[1] = (fp32)rc_ctrl.rc.ch[1] * 0.0f;
				}
				claw_position_current[1] = PID_velocity_realize_1(absolute_robot_control.claw_position_speed[1],6);
			}
			CAN1_CMD_2(claw_position_current[0],claw_position_current[1],0,0);   //5,6号电机
			osDelay(2);
    }
}

