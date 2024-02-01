/**    《遥控器舵轮控制》
 *           _____                    _____
 *          /\    \                  /\    \
 *         /::\    \                /::\    \
 *        /::::\    \              /::::\    \
 *       /::::::\    \            /::::::\    \
 *      /:::/\:::\    \          /:::/\:::\    \
 *     /:::/__\:::\    \        /:::/  \:::\    \
 *    /::::\   \:::\    \      /:::/    \:::\    \
 *   /::::::\   \:::\    \    /:::/    / \:::\    \
 *  /:::/\:::\   \:::\____\  /:::/    /   \:::\    \
 * /:::/  \:::\   \:::|    |/:::/____/     \:::\____\
 * \::/   |::::\  /:::|____|\:::\    \      \::/    /
 *  \/____|:::::\/:::/    /  \:::\    \      \/____/
 *        |:::::::::/    /    \:::\    \
 *        |::|\::::/    /      \:::\    \
 *        |::| \::/____/        \:::\    \
 *        |::|  ~|               \:::\    \
 *        |::|   |                \:::\    \
 *        \::|   |                 \:::\____\
 *         \:|   |                  \::/    /
 *          \|___|                   \/____/
 */
#include "timer_user.h"

extern RC_ctrl_t rc_ctrl;

Chassis_Speed_t absolute_chassis_speed;

fp32 Chassis_Speed_Target[4];
fp32 Shoot_Speed_Target[2];
fp32 Claw_Position_Target[1];

fp32 Chassis_Current_Target[4];
fp32 Shoot_Current_Target[2];
fp32 Claw_Current_Target[1];


//1-4底盘电机   5爪子电机    6-7摩擦轮

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		
		/***************************************底盘运动****************************************************************/
		 absolute_chassis_speed.vx =  (fp32)rc_ctrl.rc.ch[2]; 
     absolute_chassis_speed.vy =  (fp32)rc_ctrl.rc.ch[3]; 
     absolute_chassis_speed.vw = (fp32)(-rc_ctrl.rc.ch[4]); 

     absolute_chassis_speed.vx *= 6;
     absolute_chassis_speed.vy *= 6;
     absolute_chassis_speed.vw *= 5;
		
		Chassis_Speed_Target[0] =   absolute_chassis_speed.vy + absolute_chassis_speed.vx + absolute_chassis_speed.vw;
		Chassis_Speed_Target[1] = - absolute_chassis_speed.vy + absolute_chassis_speed.vx + absolute_chassis_speed.vw;
		Chassis_Speed_Target[2] = - absolute_chassis_speed.vy - absolute_chassis_speed.vx + absolute_chassis_speed.vw;
		Chassis_Speed_Target[3] =   absolute_chassis_speed.vy - absolute_chassis_speed.vx + absolute_chassis_speed.vw;
		
		
		
		
		Chassis_Current_Target[0] = PID_velocity_realize_1(Chassis_Speed_Target[0], 1);
    Chassis_Current_Target[1] = PID_velocity_realize_1(Chassis_Speed_Target[1], 2);
    Chassis_Current_Target[2] = PID_velocity_realize_1(Chassis_Speed_Target[2], 3);
    Chassis_Current_Target[3] = PID_velocity_realize_1(Chassis_Speed_Target[3], 4);

    CAN1_CMD_1(Chassis_Current_Target[0], Chassis_Current_Target[1], Chassis_Current_Target[2], Chassis_Current_Target[3]);
		
		
		
		if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 3) //爪子定位模式
		{
		/**********************************爪子****************************************************************/
//			if((rc_ctrl.rc.ch[1] >= 180) && (ABS(rc_ctrl.rc.ch[0]) <= 260))
//			{
//				Claw_Position_Target[0] = 8191.0f * 19.2f * 0.0f;
//				Claw_Current_Target[0] = pid_call_1(Claw_Position_Target[0],5);
//			}
//			else if(rc_ctrl.rc.ch[1] <= -180 && (ABS(rc_ctrl.rc.ch[0]) <= 260))
//			{
//				Claw_Position_Target[0] = 8191.0f * 19.2f * 0.40f;
//				Claw_Current_Target[0] = pid_call_1(Claw_Position_Target[0],5);
//			}
			
			if((ABS(rc_ctrl.rc.ch[1]) >= 180) && (ABS(rc_ctrl.rc.ch[0])) <= 260)
			{
				Claw_Position_Target[0] = (fp32)rc_ctrl.rc.ch[1];
			}
			else
			{
				Claw_Position_Target[0] = 0;
			}
				Claw_Current_Target[0] = PID_velocity_realize_1(Claw_Position_Target[0],5);
			
//			if(rc_ctrl.rc.ch[0] >= 580)
//			{
//				  HAL_GPIO_WritePin(Claw_Catch_GPIO_Port, Claw_Catch_Pin, GPIO_PIN_SET);
//			}			
//			else if(rc_ctrl.rc.ch[0] <= -580)
//			{
//				  HAL_GPIO_WritePin(Claw_Catch_GPIO_Port, Claw_Catch_Pin, GPIO_PIN_RESET);
//			}
		}
		
		if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 3) //底盘发射模式
		{
				/****************************************发射装置****************************************************************/
			if((ABS(rc_ctrl.rc.ch[0]) >= 180) && (ABS(rc_ctrl.rc.ch[1])) <= 260)
			{
				if(rc_ctrl.rc.ch[0] >= 580)
				{
					Shoot_Speed_Target[0] = -5000;
					Shoot_Speed_Target[1] = 5000;
				}
			}
			else
			{
				Shoot_Speed_Target[0] = 0;
				Shoot_Speed_Target[1] = 0;
			}
		
			for(int i = 0;i < 2;i++)
			{
				Shoot_Current_Target[i] = PID_velocity_realize_1(Shoot_Speed_Target[i],i+6);
			}
			CAN1_CMD_2(Claw_Current_Target[0], Shoot_Current_Target[0], Shoot_Current_Target[1],0);
		}
	}
}
