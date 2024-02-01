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

extern RC_ctrl_t rc_ctrl;

fp32 res = 0.00f, prev_res = 0.00f, res1 = 0.00f;
fp32 round_cnt = 0.00f;
fp32 round1 = 0.00f, round2 = 0.00f;
fp32 direction_coefficient = 1.00f;

fp32 Angle_Helm_Target[5];
fp32 Speed_Motor_Target[5];

fp32 M2006_Target[5];
fp32 M3508_Target[5];

void chassis_task(void const *argument)
{
	// wait a time
	//空闲一段时间
	vTaskDelay(CHASSIS_TASK_INIT_TIME);

	//底盘初始化
	PID_devices_Init();

	while (1)
	{
		// chassis data update
		//底盘数据更新
		chassis_feedback_update();
		// set chassis control set-point
		//底盘控制量设置
		chassis_set_contorl();
		// chassis control pid calculate
		// Speed_Motor_Target_1 = 1000;    //测试电机闭环是否可用的代码，正式使用时请注释该行代码
		//底盘控制PID计算
		chassis_control_loop();

		// os delay
		//系统延时
		vTaskDelay(CHASSIS_CONTROL_TIME_MS);
		// osDelay(2);
	}
}

/**
 * @brief          chassis some measure data updata, such as motor speed, euler angle， robot speed
 * @param[out]     chassis_move_update: "chassis_move" valiable point
 * @retval         none
 */
/**
 * @brief          底盘测量数据更新，包括电机速度，欧拉角度，机器人速度
 * @param[out]     chassis_move_update:"chassis_move"变量指针.
 * @retval         none
 */
static void chassis_feedback_update(void)
{
}

/**
 * @brief          set chassis control set-point, three movement control value is set by "chassis_behaviour_control_set".
 * @param[out]     chassis_move_update: "chassis_move" valiable point
 * @retval         none
 */
/**
 * @brief          设置底盘控制设置值, 三运动控制值是通过chassis_behaviour_control_set函数设置的
 * @param[out]     chassis_move_update:"chassis_move"变量指针.
 * @retval         none
 */
static void chassis_set_contorl(void)
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
		Angle_Helm_Target[1] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f)); //将数据转换成轮子转动的角度数据
		Angle_Helm_Target[2] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));
		Angle_Helm_Target[3] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));
		Angle_Helm_Target[4] = 67.00f / 20.00f * (36.00f * 8192.00f * (round_cnt + (res + res1) / 360.00f));

		if (ABS(rc_ctrl.rc.ch[2]) > 250 || ABS(rc_ctrl.rc.ch[3]) > 250)
		{
			Speed_Motor_Target[1] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]));
			Speed_Motor_Target[2] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]) - rc_ctrl.rc.ch[3] / 98.636f);
			Speed_Motor_Target[3] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]) - rc_ctrl.rc.ch[3] / 98.636f);
			Speed_Motor_Target[4] = direction_coefficient * (-0.849106f * sqrt(rc_ctrl.rc.ch[2] * rc_ctrl.rc.ch[2] + rc_ctrl.rc.ch[3] * rc_ctrl.rc.ch[3]));

			Speed_Motor_Target[1] *= 7;
			Speed_Motor_Target[2] *= 7;
			Speed_Motor_Target[3] *= 7;
			Speed_Motor_Target[4] *= 7;
		}
		if (ABS(rc_ctrl.rc.ch[2]) <= 250 && ABS(rc_ctrl.rc.ch[3]) <= 250)
		{
			Speed_Motor_Target[1] = 0;
			Speed_Motor_Target[2] = 0;
			Speed_Motor_Target[3] = 0;
			Speed_Motor_Target[4] = 0;
		}

		if (ABS(rc_ctrl.rc.ch[4]) >= 200)
		{
			Angle_Helm_Target[1] = -123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
			Angle_Helm_Target[2] = 123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
			Angle_Helm_Target[3] = -123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
			Angle_Helm_Target[4] = 123494.400f + 67.00f / 20.00f * round_cnt * 8192.00f * 36.00f;
		}
		if (ABS(rc_ctrl.rc.ch[4]) >= 330)
		{
			Speed_Motor_Target[1] = +0.707106f * rc_ctrl.rc.ch[4]; // 原地旋转
			Speed_Motor_Target[2] = -0.707106f * rc_ctrl.rc.ch[4];
			Speed_Motor_Target[3] = -0.707106f * rc_ctrl.rc.ch[4];
			Speed_Motor_Target[4] = +0.707106f * rc_ctrl.rc.ch[4];

			Speed_Motor_Target[1] *= 5.2f;
			Speed_Motor_Target[2] *= 5.2f;
			Speed_Motor_Target[3] *= 5.2f;
			Speed_Motor_Target[4] *= 5.2f;
		}
	}
}
/**
 * @brief          control loop, according to control set-point, calculate motor current,
 *                 motor current will be sentto motor
 * @param[out]     chassis_move_control_loop: "chassis_move" valiable point
 * @retval         none
 */
/**
 * @brief          控制循环，根据控制设定值，计算电机电流值，进行控制
 * @param[out]     chassis_move_control_loop:"chassis_move"变量指针.
 * @retval         none
 */
static void chassis_control_loop(void)
{
	M3508_Target[1] = PID_velocity_realize_1(Speed_Motor_Target[1], 1);
	M3508_Target[2] = PID_velocity_realize_1(Speed_Motor_Target[2], 2);
	M3508_Target[3] = PID_velocity_realize_1(Speed_Motor_Target[3], 3);
	M3508_Target[4] = PID_velocity_realize_1(Speed_Motor_Target[4], 4);

	M2006_Target[1] = pid_call_2(Angle_Helm_Target[1], 1);
	M2006_Target[2] = pid_call_2(Angle_Helm_Target[2], 2);
	M2006_Target[3] = pid_call_2(Angle_Helm_Target[3], 3);
	M2006_Target[4] = pid_call_2(Angle_Helm_Target[4], 4);

	CAN1_CMD_1(M3508_Target[1], M3508_Target[2], M3508_Target[3], M3508_Target[4]);
	CAN2_CMD_1(M2006_Target[1], M2006_Target[2], M2006_Target[3], M2006_Target[4]);
}
