#include "robot_behaviour_task.h"

extern osSemaphoreId Chassis_BinarySemHandle;
extern osSemaphoreId ClawCatch_BinarySemHandle;
extern osSemaphoreId ClawPosition_BinarySemHandle;
extern osSemaphoreId Shoot_BinarySemHandle;
extern osSemaphoreId Conveyer_BinarySemHandle;



void robot_behaviour_task(void const * argument)
{
	// wait a time
	//空闲一段时间
	vTaskDelay(CHASSIS_TASK_INIT_TIME);
	
    while(1)
    {
    //set chassis motion mode
		//设定机器人行为模式
		Remote_Control_Robot_Behavior_Set_Mode();

		//robot control data update
		//机器人控制数据更新
		Remote_Control_Robot_Behavior_Mode();
			
		xSemaphoreGive(Chassis_BinarySemHandle);
		xSemaphoreGive(ClawCatch_BinarySemHandle);
		xSemaphoreGive(ClawPosition_BinarySemHandle);
		xSemaphoreGive(Shoot_BinarySemHandle);
		xSemaphoreGive(Conveyer_BinarySemHandle);
			
    osDelay(1);
    }
}

