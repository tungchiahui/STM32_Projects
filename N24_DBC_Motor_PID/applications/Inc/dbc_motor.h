#ifndef __DBC_MOTOR_H_
#define __DBC_MOTOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "struct_typedef.h"
#include "pid_user.h"
#include "tim.h"

typedef enum
{
	Parking = 0,
	Foreward,
	Reversal
}Motor_DirState;

void Motor_CMD_PID(PID_ModeState ModeState,fp32 value,int16_t i);

void Motor_Set_Pulse(uint32_t pulse,int16_t i);
void Motor_Set_Dir(Motor_DirState DirState,int16_t i);

void Motor_Judge_Dir(PID_ModeState ModeState,fp32 value,int16_t i);

#ifdef __cplusplus
}
#endif

#endif
