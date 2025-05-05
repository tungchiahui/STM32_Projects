#ifndef __CHASSIS_API_H_
#define __CHASSIS_API_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "pid_user.h"
#include "can_receive.h"

//底盘模式
typedef enum
{
    CHASSIS_GYROSCOPE = 0,	   //小陀螺模式
    CHASSIS_NORMAL   = 1,      //底盘遥控行走
		CHASSIS_GLOBAL_POSITION   = 2,
} eChassisAction;

//底盘速度
typedef struct
{
	__packed struct
	{
		fp32 vx;
    fp32 vy;
    fp32 vw;
	}chassis;
	
	__packed struct
	{
		fp32 dire;
		fp32 wheel_rpm[4];
	}drive_motor;
	
	__packed struct
	{
		fp32 wheel_angle[4];
		fp32 wheel_last_angle[4];
		fp32 wheel_delta_angle[4];
		fp32 wheel_total_angle[4];
		fp32 wheel_last_total_angle[4];
		fp32 wheel_delta_total_angle[4];
	}swerve_motor;
	

}Chassis_Orientation_t;



void Remote_Control_Chassis_Set_Mode(eChassisAction *mode);

void Remote_Control_Chassis_Mode(eChassisAction *mode,Chassis_Orientation_t *ori);

void Robot_Orientation_Analysis(void);

void Chassis_Sports_Calc(Chassis_Orientation_t *ori);

void Chassis_Loop_Out(void);



#ifdef __cplusplus
}
#endif

#endif

