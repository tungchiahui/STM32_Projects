#ifndef __TIM_USER_H_
#define __TIM_USER_H_
#include "main.h"
#include "tim.h"
#include "pstwo.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Forward(int speed);
void Reverse(int speed);
void Stop(void);
void Emergency_Stop(void);
void TurnLeft(int fast_speed);
void TurnRight(int fast_speed);

#endif
