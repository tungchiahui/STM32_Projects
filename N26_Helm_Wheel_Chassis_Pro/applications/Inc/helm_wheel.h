#ifndef __HELM_WHEEL_H_
#define __HELM_WHEEL_H_
#include "include.h"

#define ABS(x)	((x>0) ? (x) : (-x))  //通用型绝对值函数(x是什么类型就是什么类型)

#define PI (3.141592f)

//底盘模式
typedef enum
{
    CHASSIS_GYROSCOPE = 0,	   //小陀螺模式
    CHASSIS_NORMAL   = 1,      //底盘遥控行走
} eChassisAction;

//底盘速度
typedef struct
{
    fp32 vx;
    fp32 vy;
    fp32 vw;
} Chassis_Speed_t;

void Remote_Control_Chassis_Set_Mode(void);

void Remote_Control_Chassis_Mode(void);

void helm_wheel_angle_calc(Chassis_Speed_t *speed, fp32 *out_angle);

void helm_wheel_speed_calc(Chassis_Speed_t *speed, fp32 *out_speed);

void Chassis_Loop_Out(void);

//void calc_min_angle(fp32* set_angle,fp32* last_angle,fp32* target_angle);

fp32 atan2_angle_calc(fp32 x,fp32 y);

//fp32 calc_motor_round_cnt(fp32 angle, fp32 last_angle);

void calc_min_angle_round(fp32 *set_angle,fp32 *previous_angle,fp32 *angle_temp,fp32 *round_cnt,int8_t *direction_coefficient);

fp32 deg2rad(fp32 deg);

fp32 rad2deg(fp32 rad);

//float calc_angle_helm_wheel(float set_ch2,float set_ch3);

//float calc_motor_round_cnt(float angle,float last_angle);

//float calc_min_angle(float set_angle,float last_set_angle);

#endif




