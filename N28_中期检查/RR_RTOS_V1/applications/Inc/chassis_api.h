#ifndef __CHASSIS_API_H_
#define __CHASSIS_API_H_
#include "include.h"

//底盘模式
typedef enum
{
    CHASSIS_NORMAL    = 0,      //底盘遥控行走
    CHASSIS_STOP   = 1,       //底盘定点不动
    CHASSIS_GYROSCOPE = 2,	   //小陀螺模式
    CHASSIS_OFFLINE = 3,       //掉线状态，各电机开环，电流为0
} ChassisBehavior;


//底盘速度
typedef struct
{
    fp32 vx;
    fp32 vy;
    fp32 vw;
} Chassis_Speed_t;

void Remote_Control_Chassis_Set_Mode(void);

void Remote_Control_Chassis_Mode(Chassis_Speed_t *chassis_speed);

void Chassis_Sports_Calc(Chassis_Speed_t speed);

void Chassis_Loop_Out(void);

#endif
