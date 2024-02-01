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

#include "chassis_task.h"

extern Chassis_Speed_t absolute_chassis_speed;

extern RC_ctrl_t rc_ctrl;

extern osSemaphoreId Chassis_BinarySemHandle;

fp32 res = 0.00f, prev_res = 0.00f, res1 = 0.00f;
fp32 round_cnt = 0.00f;
fp32 round1 = 0.00f, round2 = 0.00f;
fp32 direction_coefficient = 1.00f;

fp32 Angle_Helm_Target[4];
fp32 Speed_Motor_Target[4];

fp32 M2006_Target[4];
fp32 M3508_Target[4];

void chassis_task(void const * argument)
{
	// wait a time
	//空闲一段时间
	vTaskDelay(CHASSIS_TASK_INIT_TIME);

	//PID parameter initialization
	//PID参数初始化
	PID_devices_Init();
	
	while(1)
	{
		xSemaphoreTake(Chassis_BinarySemHandle,portMAX_DELAY);
		
		//calculate the speed of the chassis
		//底盘速度解算
		Chassis_Sports_Calc();

		//chassis control pid calculate
		//底盘控制PID计算与数据发送
		Chassis_Loop_Out();

		//chassis task control time
		//底盘任务控制间隔
		osDelay(CHASSIS_CONTROL_TIME_MS);
	}
		

}

/**
 * @brief  底盘运动解析式计算
 * @param  void
 * @retval void
 * @attention  此函数是全向轮底盘解析式
 */
void Chassis_Sports_Calc(void)
{
	if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 3) //底盘正常模式
	{
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
		prev_res = res + res1;																				 //记录上一次的角度数据
		Angle_Helm_Target[0] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f)); //将数据转换成轮子转动的角度数据
		Angle_Helm_Target[1] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));
		Angle_Helm_Target[2] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));
		Angle_Helm_Target[3] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));

		if (ABS(rc_ctrl.rc.ch[2]) > 250 || ABS(rc_ctrl.rc.ch[3]) > 250)
		{
			Speed_Motor_Target[0] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]));
			Speed_Motor_Target[1] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]) - rc_ctrl.rc.ch[3] / 98.636f);
			Speed_Motor_Target[2] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]) - rc_ctrl.rc.ch[3] / 98.636f);
			Speed_Motor_Target[3] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]));

			Speed_Motor_Target[0] *= 7;
			Speed_Motor_Target[1] *= 7;
			Speed_Motor_Target[2] *= 7;
			Speed_Motor_Target[3] *= 7;
		}
		if (ABS(rc_ctrl.rc.ch[2]) <= 250 && ABS(rc_ctrl.rc.ch[3]) <= 250)
		{
			Speed_Motor_Target[0] = 0;
			Speed_Motor_Target[1] = 0;
			Speed_Motor_Target[2] = 0;
			Speed_Motor_Target[3] = 0;
		}

		if (ABS(rc_ctrl.rc.ch[4]) >= 200)
		{
			Angle_Helm_Target[0] = -123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
			Angle_Helm_Target[1] = 123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
			Angle_Helm_Target[2] = -123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
			Angle_Helm_Target[3] = 123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
		}
		if (ABS(rc_ctrl.rc.ch[4]) >= 330)
		{
			Speed_Motor_Target[0] = +0.707106f * rc_ctrl.rc.ch[4]; // 原地旋转
			Speed_Motor_Target[1] = -0.707106f * rc_ctrl.rc.ch[4];
			Speed_Motor_Target[2] = -0.707106f * rc_ctrl.rc.ch[4];
			Speed_Motor_Target[3] = +0.707106f * rc_ctrl.rc.ch[4];

			Speed_Motor_Target[0] *= 5.2f;
			Speed_Motor_Target[1] *= 5.2f;
			Speed_Motor_Target[2] *= 5.2f;
			Speed_Motor_Target[3] *= 5.2f;
		}
	}
}
/**
 * @brief  底盘电机输出
 * @param  void
 * @retval void
 * @attention
 */
void Chassis_Loop_Out(void)
{
	M3508_Target[0] = PID_velocity_realize_1(Speed_Motor_Target[0], 1);
	M3508_Target[1] = PID_velocity_realize_1(Speed_Motor_Target[1], 2);
	M3508_Target[2] = PID_velocity_realize_1(Speed_Motor_Target[2], 3);
	M3508_Target[3] = PID_velocity_realize_1(Speed_Motor_Target[3], 4);

	M2006_Target[0] = pid_call_2(Angle_Helm_Target[0], 1);
	M2006_Target[1] = pid_call_2(Angle_Helm_Target[1], 2);
	M2006_Target[2] = pid_call_2(Angle_Helm_Target[2], 3);
	M2006_Target[3] = pid_call_2(Angle_Helm_Target[3], 4);

	CAN1_CMD_1(M3508_Target[0], M3508_Target[1], M3508_Target[2], M3508_Target[3]);
	CAN2_CMD_1(M2006_Target[0], M2006_Target[1], M2006_Target[2], M2006_Target[3]);
}
