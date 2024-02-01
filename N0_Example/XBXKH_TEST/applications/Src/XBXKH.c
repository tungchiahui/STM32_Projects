#include "XBXKH.h"
extern uint8_t RX[1];

 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
	 if(htim->Instance == TIM1)
	 {
		  HAL_GPIO_TogglePin(GPIOH,GPIO_PIN_11);
	 }
 }
 
 
 
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {
	 	if(huart->Instance == USART1)
		{
			switch(RX[0])
			{
				case '0' :__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,500);break;
				case '1' :__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,1500);break;
				case '2' :__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,2500);break;
				default:;
			}
		}
}
 
