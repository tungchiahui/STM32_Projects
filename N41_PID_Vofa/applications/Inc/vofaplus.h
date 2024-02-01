#ifndef __VOFAPLUS_H_
#define __VOFAPLUS_H_
#include "include.h"
#include <stdio.h>

#define PUSH_ORIGIN_IO 	 HAL_GPIO_ReadPin(Push_Origin_GPIO_Port,Push_Origin_Pin)
#define PUSH_GO_IO    	 HAL_GPIO_ReadPin(Push_Go_GPIO_Port,Push_Go_Pin)
#define CATCH_MOTOR5_IO  HAL_GPIO_ReadPin(Catch_Motor5_GPIO_Port,Catch_Motor5_Pin)
#define CATCH_MOTOR6_IO  HAL_GPIO_ReadPin(Catch_Motor6_GPIO_Port,Catch_Motor6_Pin)
void test1(void);
fp32 accel_optimize(fp32 speed_target,fp32 interval_time);

#endif
