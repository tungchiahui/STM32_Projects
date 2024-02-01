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


//CAN1 1-4 M3508底盘电机       5 传环电机2       6摩擦轮1       7摩擦轮2  
//CAN2 1-4 M2006舵电机      		 5 爪子转盘电机    6爪子升降电机   7传环电机1



extern RC_ctrl_t rc_ctrl;

fp32 res = 0.00f, prev_res = 0.00f, res1 = 0.00f;
fp32 round_cnt = 0.00f;
fp32 round1 = 0.00f, round2 = 0.00f;
fp32 direction_coefficient = 1.00f;

fp32 Chassis_Helm_Angle_Target[4];
fp32 Chassis_Speed_Target[4];
fp32 Shoot_Speed_Target[2];
fp32 Claw_Speed_Target[2];
fp32 Conveyer_Speed_Target[2];


fp32 Chassis_Helm_Current_Target[4];
fp32 Chassis_Current_Target[4];
fp32 Shoot_Current_Target[2];
fp32 Claw_Current_Target[2];
fp32 Conveyer_Current_Target[2];


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		
		/***************************************底盘运动****************************************************************/
			res = calc_angle_helm_wheel(rc_ctrl.rc.ch[2], rc_ctrl.rc.ch[3]); //计算遥控器遥杆角度
			res1 = calc_min_angle(res, prev_res);							 //就近转圈的目标角度

			round_cnt = calc_motor_round_cnt((res + res1), prev_res); //计算圈数
			round1 = fabs(prev_res - res1);
			round2 = fabs(res - prev_res);
			if (round1 >= round2)
			{
				res1 = 0.00f;
				direction_coefficient = 1.00f;
			}
			if (round1 < round2)
			{
				res = 0.00f;
				direction_coefficient = -1.00f;
			}
			prev_res = res + res1;																				//记录上一次的角度数据
			Chassis_Helm_Angle_Target[0] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f)); //将数据转换成轮子转动的角度数据
			Chassis_Helm_Angle_Target[1] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));
			Chassis_Helm_Angle_Target[2] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));
			Chassis_Helm_Angle_Target[3] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));

			if (ABS(rc_ctrl.rc.ch[2]) > 250 || ABS(rc_ctrl.rc.ch[3]) > 250)
			{
				Chassis_Speed_Target[0] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]));
				Chassis_Speed_Target[1] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]) - rc_ctrl.rc.ch[3] / 98.636f);
				Chassis_Speed_Target[2] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]) - rc_ctrl.rc.ch[3] / 98.636f);
				Chassis_Speed_Target[3] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]));

				Chassis_Speed_Target[0] *= 7;
				Chassis_Speed_Target[1] *= 7;
				Chassis_Speed_Target[2] *= 7;
				Chassis_Speed_Target[3] *= 7;
			}
			if (ABS(rc_ctrl.rc.ch[2]) <= 250 && ABS(rc_ctrl.rc.ch[3]) <= 250)
			{
				Chassis_Speed_Target[0] = 0;
				Chassis_Speed_Target[1] = 0;
				Chassis_Speed_Target[2] = 0;
				Chassis_Speed_Target[3] = 0;
			}

			if (ABS(rc_ctrl.rc.ch[4]) >= 200)
			{
				Chassis_Helm_Angle_Target[0] = -123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
				Chassis_Helm_Angle_Target[1] = 123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
				Chassis_Helm_Angle_Target[2] = -123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
				Chassis_Helm_Angle_Target[3] = 123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
			}
			if (ABS(rc_ctrl.rc.ch[4]) >= 330)
			{
				Chassis_Speed_Target[0] = +0.707106f * rc_ctrl.rc.ch[4]; // 原地旋转
				Chassis_Speed_Target[1] = -0.707106f * rc_ctrl.rc.ch[4];
				Chassis_Speed_Target[2] = -0.707106f * rc_ctrl.rc.ch[4];
				Chassis_Speed_Target[3] = +0.707106f * rc_ctrl.rc.ch[4];

				Chassis_Speed_Target[0] *= 5.2f;
				Chassis_Speed_Target[1] *= 5.2f;
				Chassis_Speed_Target[2] *= 5.2f;
				Chassis_Speed_Target[3] *= 5.2f;
			}
			for(int i = 0;i < 4;i++)
			{
				Chassis_Current_Target[i] = PID_velocity_realize_1(Chassis_Speed_Target[i],i+1);
				Chassis_Helm_Current_Target[i] = pid_call_2(Chassis_Helm_Angle_Target[i],i+1);
			}
			CAN1_CMD_1(Chassis_Current_Target[0], Chassis_Current_Target[1], Chassis_Current_Target[2], Chassis_Current_Target[3]);
			CAN2_CMD_1(Chassis_Helm_Current_Target[0], Chassis_Helm_Current_Target[1], Chassis_Helm_Current_Target[2], Chassis_Helm_Current_Target[3]);
		
		

		if (rc_ctrl.rc.s[0] == 1 && rc_ctrl.rc.s[1] == 3) //底盘收环模式
		{
		/**********************************爪子****************************************************************/
			if(ABS(rc_ctrl.rc.ch[1]) <= 180)
			{
				Claw_Speed_Target[0] = -(fp32)rc_ctrl.rc.ch[0] * 1.0f;
				Claw_Current_Target[0] = PID_velocity_realize_2(Claw_Speed_Target[0],5);
			}
			if(ABS(rc_ctrl.rc.ch[0]) <= 180)
			{
				if(ABS(rc_ctrl.rc.ch[1]) >= 300)
				{
				Claw_Speed_Target[1] = (fp32)rc_ctrl.rc.ch[1] * 1.0f;
				}
				else if(ABS(rc_ctrl.rc.ch[1]) <= -300 )
				{
				Claw_Speed_Target[1] = (fp32)rc_ctrl.rc.ch[1] * 1.0f;
				}
				else if(ABS(rc_ctrl.rc.ch[1]) == 0 )
				{
					Claw_Speed_Target[1] = (fp32)rc_ctrl.rc.ch[1] * 0.0f;
				}
				Claw_Current_Target[1] = PID_velocity_realize_2(Claw_Speed_Target[1],6);
			}
			
//			if(rc_ctrl.rc.ch[1] >= 580)
//			{
//					HAL_GPIO_WritePin(Claw_Catch_GPIO_Port, Claw_Catch_Pin, GPIO_PIN_SET);
//			}			
//			else if(rc_ctrl.rc.ch[1] <= -580)
//			{
//				  HAL_GPIO_WritePin(Claw_Catch_GPIO_Port, Claw_Catch_Pin, GPIO_PIN_RESET);
//			}
		}
		
		if (rc_ctrl.rc.s[0] == 2 && rc_ctrl.rc.s[1] == 3) //底盘发射模式
		{
				/****************************************发射装置****************************************************************/
			if((ABS(rc_ctrl.rc.ch[0]) < 180) && (rc_ctrl.rc.ch[1] >= 580))
			{
			Shoot_Speed_Target[0] =  3000;
			Shoot_Speed_Target[1] = -3000;
			}
			else if((ABS(rc_ctrl.rc.ch[0]) < 180) && (rc_ctrl.rc.ch[1] <= -580 ))
			{
				Shoot_Speed_Target[0] = 0;
				Shoot_Speed_Target[1] = 0;
			}
			else
			{
				Shoot_Speed_Target[0] = Shoot_Speed_Target[0];
				Shoot_Speed_Target[1] = Shoot_Speed_Target[1];
				
			}
			if(rc_ctrl.rc.ch[0] >= 580)
			{
				for(int i = 0;i < 2;i++)
				{
					Conveyer_Speed_Target[i] = 5000;
				}
			}
			else if(rc_ctrl.rc.ch[0] <= -580)
			{
				for(int i = 0;i < 2;i++)
				{
					Conveyer_Speed_Target[i] = 0;
				}
			}

			Shoot_Current_Target[0] = PID_velocity_realize_1(Shoot_Speed_Target[0],6);
			Shoot_Current_Target[1] = PID_velocity_realize_1(Shoot_Speed_Target[1],7);
			
			Conveyer_Current_Target[0] = PID_velocity_realize_2(Conveyer_Speed_Target[0],7);
			Conveyer_Current_Target[1] = PID_velocity_realize_1(Conveyer_Speed_Target[1],5);
		}
			CAN1_CMD_2(Conveyer_Current_Target[1],Shoot_Current_Target[0],Shoot_Current_Target[1],0);
			CAN2_CMD_2(Claw_Current_Target[0],Claw_Current_Target[1],Conveyer_Current_Target[0],0);
	}
}

//CAN1 1-4 M3508底盘电机       5 传环电机2       6摩擦轮1       7摩擦轮2  
//CAN2 1-4 M2006舵电机      		 5 爪子转盘电机    6爪子升降电机   7传环电机1
