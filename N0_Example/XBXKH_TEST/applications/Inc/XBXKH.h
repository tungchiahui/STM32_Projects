#ifndef __XBXKH_H_
#define __XBXKH_H_
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif 
