/**    ¡¶Ò£¿ØÆ÷¶æÂÖ¿ØÖÆ¡·
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

extern fp32 Angle_Helm_Target[5];
extern fp32 Speed_Motor_Target[5];

extern fp32 M2006_Target[5];
extern fp32 M3508_Target[5];

extern Chassis_Speed_t absolute_chassis_speed;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		Remote_Control_Chassis_Set_Mode();
		Remote_Control_Chassis_Mode();
		helm_wheel_angle_calc(&absolute_chassis_speed,Angle_Helm_Target);
		helm_wheel_speed_calc(&absolute_chassis_speed,Speed_Motor_Target);
		Chassis_Loop_Out();
	}
}
