#include "claw_catch_task.h"

extern osSemaphoreId Claw_Catch_BinarySemHandle;
extern uint8_t bluetooth_rx_buffer[1];

void claw_catch_task(void const * argument)
{
    while(1)
    {
      xSemaphoreTake(Claw_Catch_BinarySemHandle,portMAX_DELAY);
			
			if(*bluetooth_rx_buffer == '0')
			{
				HAL_GPIO_WritePin(Claw_Catch_GPIO_Port,Claw_Catch_Pin,GPIO_PIN_RESET);
			}
			else if(*bluetooth_rx_buffer == '1')
			{
				HAL_GPIO_WritePin(Claw_Catch_GPIO_Port,Claw_Catch_Pin,GPIO_PIN_SET);
			}
			osDelay(1);
    }
}

