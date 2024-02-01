#ifndef __PID_USER_H
#define __PID_USER_H
#include "pid.h"


typedef enum
{
	velocity = 0,
	position = 1,
	call = 2,
	current = 3
}PID_ModeState;


void PID_devices_Init(void);

fp32 PID_velocity_realize(fp32 set_speed,int16_t i);
fp32 PID_position_realize(fp32 set_pos,int16_t i);
fp32 PID_call_realize(fp32 set_pos,int16_t i);



#endif























