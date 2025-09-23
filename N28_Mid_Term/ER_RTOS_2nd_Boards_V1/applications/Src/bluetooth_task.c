#include "bluetooth_task.h"

uint8_t bluetooth_rx_buffer[1];

extern osSemaphoreId Claw_Catch_BinarySemHandle;


void bluetooth_task(void const * argument)
{
	while(1)
	{
		HAL_UART_Receive(&huart1,bluetooth_rx_buffer,1,portMAX_DELAY);
		xSemaphoreGive(Claw_Catch_BinarySemHandle);
		osDelay(10);
	}
}




