#include "shoot_task.h"

fp32 Shoot_Speed_Target[2];

fp32 Shoot_Current_Target[2];

extern RC_ctrl_t rc_ctrl;

void shoot_task(void const * argument)
{
	
	while(1)
	{
		if (rc_ctrl.rc.s[1] == 1)   //左按钮 置最上面，发射电机速度为高速，射2型立柱
		{
			Shoot_Speed_Target[0] =  -4000;
			Shoot_Speed_Target[1] =   4000;
		}
		else if(rc_ctrl.rc.s[1] == 2) //左按钮 置最下面，发射电机速度为低速，射1型立柱
		{
			Shoot_Speed_Target[0] =  -2500;
			Shoot_Speed_Target[1] =   2500;
		}
		else if(rc_ctrl.rc.s[1] == 3) //左按钮 置中间，发射电机速度为0，静止
		{
			Shoot_Speed_Target[0] =  0;
			Shoot_Speed_Target[1] =  0;
		}
		
		Shoot_Current_Target[0] = PID_velocity_realize_1(Shoot_Speed_Target[0],5);
		Shoot_Current_Target[1] = PID_velocity_realize_1(Shoot_Speed_Target[1],6);
		
		CAN1_CMD_2(Shoot_Current_Target[0],Shoot_Current_Target[1],0,0);
		osDelay(2);
	}
}


