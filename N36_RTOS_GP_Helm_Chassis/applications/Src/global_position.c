#include "global_position.h"

extern Chassis_measure_t absolute_chassis_measure;
extern HWT605_Modbus_t hwt605;

/**
 * @brief  获得底盘目标Yaw
 * @param  void
 * @retval void
 * @attention
 */
void Get_Chassis_Yaw_Target(void)
{
//	static fp32 RC_atan2_value = 0;
	static fp32 yaw_delta = 0;
	static fp32 control_ratio = 0.4f;   //速度系数
	
	//单圈控制角度(不启用)
//		RC_atan2_value = rad2deg(atan2(absolute_chassis_measure.Speed.vx,absolute_chassis_measure.Speed.vy));
	//多圈控制角度
		yaw_delta = absolute_chassis_measure.Speed.vw / 660.0f * control_ratio;
		absolute_chassis_measure.Euler.yaw_target += yaw_delta;
}


/**
 * @brief  获得底盘修正Yaw和修正后的相对Yaw
 * @param  void
 * @retval void
 * @attention
 */
void Get_Chassis_Yaw_Offset(void)
{
		static bool_t imu_offset_flags = 1;
			if(imu_offset_flags == 1)
		{
			absolute_chassis_measure.Euler.yaw_offset = hwt605.Euler.yaw;
			imu_offset_flags = 0;
		}
		absolute_chassis_measure.Euler.yaw_last_angle = absolute_chassis_measure.Euler.yaw_rel;
		absolute_chassis_measure.Euler.yaw_rel = absolute_chassis_measure.Euler.yaw - absolute_chassis_measure.Euler.yaw_offset;
}


/**
 * @brief  获得底盘总Yaw
 * @param  void
 * @retval void
 * @attention  可以实现多圈
 */
void Get_Chassis_Total_Yaw(void)
{
		if(absolute_chassis_measure.Euler.yaw_rel - absolute_chassis_measure.Euler.yaw_last_angle < -300.0f)
		{
			absolute_chassis_measure.Euler.yaw_round_cnt++;
		}
		else if(absolute_chassis_measure.Euler.yaw_rel - absolute_chassis_measure.Euler.yaw_last_angle > 300.0f)
		{
			absolute_chassis_measure.Euler.yaw_round_cnt--;
		}
		absolute_chassis_measure.Euler.yaw_total = 360.0f * absolute_chassis_measure.Euler.yaw_round_cnt + absolute_chassis_measure.Euler.yaw_rel;
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
    rad = deg * (PI / 180.0f);
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
    deg = rad * (180.0f / PI);
    return deg;
}

