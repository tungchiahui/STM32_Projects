#include "swerve.h"

/*
wheel0（左前）        wheel1（右前）
wheel3（左后）        wheel2（右后）
*/

extern RC_ctrl_t rc_ctrl; // 遥控器控制变量

/**
 * @brief  遥控器控制方式
 * @param  chassis_speed 底盘速度
 * @retval void
 * @attention
 */
void Swerve_Chassis_Reset(motor_measure_t *motor)
{
	motor[0].offset_angle = 0;   //再写值
	motor[1].offset_angle = 0;   //再写值
	motor[2].offset_angle = 0;   //再写值
	motor[3].offset_angle = 0;   //再写值
}


fp32 L = 5.0f;  // 前后轮之间距离
fp32 W = 5.0f;   // 左右轮之间距离
fp32 R = sqrt(L * L + W * W) / 2;  // 每个轮子离中心的距离


/**
 * @brief  驱动电机运动解析
 * @param  ori 机器人姿态地址
 * @retval void
 * @attention
 */
void Drive_Motor_Calc(Chassis_Orientation_t *ori)
{
	// 电机转速计算（单位：RPM）
	ori->drive_motor.wheel_rpm[0] = sqrt(pow(ori->chassis.vx - ori->chassis.vw * W / 2, 2) +
										pow(ori->chassis.vy - ori->chassis.vw * L / 2, 2)) * ori->drive_motor.dire;

	ori->drive_motor.wheel_rpm[1] = sqrt(pow(ori->chassis.vx + ori->chassis.vw * W / 2, 2) +
										pow(ori->chassis.vy - ori->chassis.vw * L / 2, 2)) * ori->drive_motor.dire;

	ori->drive_motor.wheel_rpm[2] = sqrt(pow(ori->chassis.vx + ori->chassis.vw * W / 2, 2) +
										pow(ori->chassis.vy + ori->chassis.vw * L / 2, 2)) * ori->drive_motor.dire;

	ori->drive_motor.wheel_rpm[3] = sqrt(pow(ori->chassis.vx - ori->chassis.vw * W / 2, 2) +
										pow(ori->chassis.vy + ori->chassis.vw * L / 2, 2)) * ori->drive_motor.dire; 
}

/**
 * @brief  舵电机运动解析
 * @param  ori 机器人姿态地址
 * @retval void
 * @attention
 */
void Swerve_Motor_Calc(Chassis_Orientation_t *ori)
{
	// 舵角计算（单位：弧度）
	ori->swerve_motor.wheel_angle[0] = atan2(ori->chassis.vx - ori->chassis.vw * W / 2,
		ori->chassis.vy - ori->chassis.vw * L / 2);

	ori->swerve_motor.wheel_angle[1] = atan2(ori->chassis.vx + ori->chassis.vw * W / 2,
		ori->chassis.vy - ori->chassis.vw * L / 2);

	ori->swerve_motor.wheel_angle[2] = atan2(ori->chassis.vx + ori->chassis.vw * W / 2,
		ori->chassis.vy + ori->chassis.vw * L / 2);

	ori->swerve_motor.wheel_angle[3] = atan2(ori->chassis.vx - ori->chassis.vw * W / 2,
		ori->chassis.vy + ori->chassis.vw * L / 2);

	for (int i = 0; i < 4; i++)
	{
		ori->swerve_motor.wheel_angle[i] = rad2deg(ori->swerve_motor.wheel_angle[i]);
		ori->swerve_motor.wheel_angle[i] = - ori->swerve_motor.wheel_angle[i];   //双齿轮啮合，角速度需要取反
	}
}


/**
 * @brief  支持多圈
 * @param  ori 机器人姿态地址
 * @retval void
 * @attention
 */
void Multi_Round_Support(Chassis_Orientation_t *ori)
{
	static fp32 round_cnt[4] = {0};
	for (int i = 0; i < 4; i++)
	{	
		ori->swerve_motor.wheel_delta_angle[i] = ori->swerve_motor.wheel_angle[i] - ori->swerve_motor.wheel_last_angle[i];
		if ((ori->swerve_motor.wheel_delta_angle[i]) <= -180.0f)
		{
			round_cnt[i]++;
		}
		else if ((ori->swerve_motor.wheel_delta_angle[i]) >= 180.0f)
		{
			round_cnt[i]--;
		}
		ori->swerve_motor.wheel_total_angle[i] = ori->swerve_motor.wheel_angle[i] + 360.0f * round_cnt[i];
		ori->swerve_motor.wheel_last_angle[i] = ori->swerve_motor.wheel_angle[i];
	}
}

/**
 * @brief  就近原则
 * @param  ori 机器人姿态地址
 * @retval void
 * @attention
 */
void Min_Angle_Calc(Chassis_Orientation_t *ori)
{
	for (int i = 0;i < 4;i++)
	{
		ori->swerve_motor.wheel_delta_total_angle[i] = ori->swerve_motor.wheel_total_angle[i] - ori->swerve_motor.wheel_last_total_angle[i];
		if(fabs(ori->swerve_motor.wheel_delta_total_angle[i]) >= 90.0f)
		{
			ori->swerve_motor.wheel_total_angle[i] = ori->swerve_motor.wheel_total_angle[i] + 180.0f;
			ori->drive_motor.dire = -1;
		}
		else
		{
			ori->swerve_motor.wheel_total_angle[i] = ori->swerve_motor.wheel_total_angle[i];
		}
		ori->swerve_motor.wheel_last_total_angle[i] = ori->swerve_motor.wheel_total_angle[i];
		ori->drive_motor.dire = 1;
	}
}


/**
 * @brief  防止舵电机转动偏移
 * @param  ori 机器人姿态地址
 * @retval void
 * @attention
 */
void Non_Syn_Wheel_Support(Chassis_Orientation_t *ori)
{
	for (int i = 0;i < 4;i++)
	{
		ori->drive_motor.wheel_rpm[i] = ori->drive_motor.wheel_rpm[i] * pow(cos(ori->swerve_motor.wheel_delta_total_angle[i]),3);;
	}
}


/**
 * @brief  角度转换为弧度
 * @param  deg 角度值
 * @retval rad 弧度值
 * @attention
 */
fp32 deg2rad(fp32 deg)
{
	fp32 rad;
	rad = (fp32)(deg * (PI / 180.0f));
	return rad;
}

/**
 * @brief  弧度转换为角度
 * @param  rad 弧度值
 * @retval deg 角度值
 * @attention
 */
fp32 rad2deg(fp32 rad)
{
	fp32 deg;
	deg = (fp32)(rad * (180.0f / PI));
	return deg;
}
