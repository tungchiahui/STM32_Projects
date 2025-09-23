#include "claw_task.h"

extern RC_ctrl_t rc_ctrl;

extern OtherBehavior actOther;

extern Other_Devices_t absolute_robot_control;

extern osSemaphoreId ClawCatch_BinarySemHandle;

extern osSemaphoreId ClawPosition_BinarySemHandle;

extern osSemaphoreId ClawLinkage_BinarySemHandle;

bool_t claw_catch_flag;

fp32 Claw_Position;
fp32 Claw_Current;

void claw_catch_task(void const * argument)
{
    while(1)
    {
			xSemaphoreTake(ClawCatch_BinarySemHandle,portMAX_DELAY);
			
        if(rc_ctrl.rc.ch[1] >= 580 && abs(rc_ctrl.rc.ch[0]) <= 200)
        {
            absolute_robot_control.claw_catch_bool = 1;   
        }
        else if(rc_ctrl.rc.ch[1] <= -580 && abs(rc_ctrl.rc.ch[0]) <= 200)
        {
            absolute_robot_control.claw_catch_bool = 0;
        }

        claw_catch_flag = absolute_robot_control.claw_catch_bool;

        if(claw_catch_flag == 1)
        {
            HAL_GPIO_WritePin(Claw_Catch_GPIO_Port, Claw_Catch_Pin, GPIO_PIN_SET);
        }
        else if(claw_catch_flag == 0)
        {
            HAL_GPIO_WritePin(Claw_Catch_GPIO_Port, Claw_Catch_Pin, GPIO_PIN_RESET);
        }
				osDelay(1000);
			  xSemaphoreGive(ClawLinkage_BinarySemHandle);
    }
}

void claw_position_task(void const * argument)
{

    while(1)
    {
				xSemaphoreTake(ClawPosition_BinarySemHandle,portMAX_DELAY);
			
        if(rc_ctrl.rc.ch[1] >= 580 && abs(rc_ctrl.rc.ch[0]) <= 200)
        {
            absolute_robot_control.claw_position = 8191.0f * 19.2f * 0.0f;
        }
        else if(rc_ctrl.rc.ch[1] <= -580 && abs(rc_ctrl.rc.ch[0]) <= 200)
        {
            absolute_robot_control.claw_position = 8191.0f * 19.2f * 0.493f;
        }
				
				xSemaphoreTake(ClawLinkage_BinarySemHandle,portMAX_DELAY);
				
        Claw_Position = absolute_robot_control.claw_position;	
        Claw_Current = pid_call_1(Claw_Position,5);
        CAN1_CMD_2(Claw_Current,0,0,0);
        osDelay(2);
    }
}

