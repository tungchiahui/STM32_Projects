#ifndef __TIM_USER_H_
#define __TIM_USER_H_
#include "tim.h"
#include "can_receive.h"
#include "remote_control.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void RC_VC_DATA(void);

void Mecanum_Wheel_Speed_Calculation(void);
	
void Transmit_Speed(void);

#endif
