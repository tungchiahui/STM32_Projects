#include "catapult_task.h"

extern osSemaphoreId Catapult_BinarySemHandle;
extern uint8_t bluetooth_rx_buffer[1];

void catapult_task(void const * argument)
{
	while(1)
	{
		  xSemaphoreTake(Catapult_BinarySemHandle,portMAX_DELAY);
		
			if(*bluetooth_rx_buffer == '5')
			{
				HAL_GPIO_WritePin(Catapult_GPIO_Port,Catapult_Pin,GPIO_PIN_RESET);
			}
			else if(*bluetooth_rx_buffer == '6')
			{
				HAL_GPIO_WritePin(Catapult_GPIO_Port,Catapult_Pin,GPIO_PIN_SET);
			}
			osDelay(1);
	}
}
