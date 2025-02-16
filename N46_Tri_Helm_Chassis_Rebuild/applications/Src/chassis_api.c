#include "chassis_api.h"
#include "remote_control.h"
#include "helm_wheel.h"

extern RC_ctrl_t rc_ctrl;   //遥控器控制变量

extern eChassisAction actChassis; //底盘默认遥控行走

extern Chassis_Orientation_t chassisOri;

/**
 * @brief  设定遥控器控制底盘模式
 * @param  void
 * @retval void
 * @attention
 */
void Remote_Control_Chassis_Set_Mode(eChassisAction *mode)
{
	if(rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 3) //底盘正常模式
	{
		*mode = CHASSIS_NORMAL;
	}
	else if(rc_ctrl.rc.s[0] == 1 && rc_ctrl.rc.s[1] == 3)
	{
		*mode = CHASSIS_NORMAL;
	}
	else if(rc_ctrl.rc.s[0] == 2 && rc_ctrl.rc.s[1] == 3)
	{
		*mode = CHASSIS_NORMAL;
	}
}



/**
 * @brief  遥控器控制方式
 * @param  chassis_speed 底盘速度
 * @retval void
 * @attention
 */
void Remote_Control_Chassis_Mode(eChassisAction *mode,Chassis_Orientation_t *ori)
{
	/***********************************确定底盘的目标速度*****************************************/
	switch (*mode)
	{
	case CHASSIS_NORMAL: //正常模式
		ori->chassis.vx = (fp32)rc_ctrl.rc.ch[2];
		ori->chassis.vy = (fp32)rc_ctrl.rc.ch[3];
		ori->chassis.vw = (fp32)rc_ctrl.rc.ch[0];

		ori->chassis.vx *= 5.2f;
		ori->chassis.vy *= 5.2f;
		ori->chassis.vw *= 3.2f;
		break;

	case CHASSIS_GYROSCOPE: //小陀螺模式
		ori->chassis.vx = (fp32)rc_ctrl.rc.ch[2];
		ori->chassis.vy = (fp32)rc_ctrl.rc.ch[3];
		ori->chassis.vw = 330.0f;

		ori->chassis.vx *= 3.0f;
		ori->chassis.vy *= 3.0f;
		ori->chassis.vw *= 3.0f;
		break;	
	default:
		break;
	}
}



/**
 * @brief  底盘姿态分析
 * @param  void
 * @retval void
 * @attention
 */
void Robot_Orientation_Analysis(void)
{
	
}


/**
 * @brief  底盘运动解析式计算
 * @param  speed 底盘速度
 * @retval void
 * @attention  此函数是舵轮底盘电机的速度解析式
 */
void Chassis_Sports_Calc(Chassis_Orientation_t *ori)
{
	Drive_Motor_Calc(ori);
	Helm_Motor_Calc(ori);
	Multi_Round_Support(ori);
	Min_Angle_Calc(ori);
	Non_Syn_Wheel_Support(ori);
}


/**
 * @brief  底盘电机输出
 * @param  void
 * @retval void
 * @attention
 */
void Chassis_Loop_Out(void)
{
	int16_t drive_motor_current[3] = {0};
	int16_t helm_motor_current[3] = {0};
	fp32 output_angle[3];

	for(int i = 0;i < 3;i++)
	{
		output_angle[i] = chassisOri.helm_motor.wheel_total_angle[i] / 360.0f * M2006_REDU_RATIO * 8192.0f * GEAR_REDU_RATIO;
		helm_motor_current[i] = PID_call_1(output_angle[i],i+1+3);
		drive_motor_current[i] = PID_velocity_realize_1(chassisOri.drive_motor.wheel_rpm[i],i+1);
	}
	
	CAN1_CMD_1(drive_motor_current[0],drive_motor_current[1],drive_motor_current[2],helm_motor_current[0]);
	CAN1_CMD_2(helm_motor_current[1],helm_motor_current[2],0,0);
}


