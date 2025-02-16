#ifndef __HELM_WHEEL_H_
#define __HELM_WHEEL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "chassis_api.h"
#include <math.h>
#include "remote_control.h"

#define theta45 (atan(1.0/1.0))
#define RADIUS (5)
#define PI (3.141592)
#define M3508_REDU_RATIO (3591.0f / 187.0f)
#define M2006_REDU_RATIO (36.0f / 1.0f)
#define GEAR_REDU_RATIO (67.0f / 20.0f)


void Helm_Chassis_Reset(motor_measure_t *motor);
void Drive_Motor_Calc(Chassis_Orientation_t *ori);
void Helm_Motor_Calc(Chassis_Orientation_t *ori);
void Multi_Round_Support(Chassis_Orientation_t *ori);
void Min_Angle_Calc(Chassis_Orientation_t *ori);
void Non_Syn_Wheel_Support(Chassis_Orientation_t *ori);

fp32 deg2rad(fp32 deg);
fp32 rad2deg(fp32 rad);

#ifdef __cplusplus
}
#endif

#endif
