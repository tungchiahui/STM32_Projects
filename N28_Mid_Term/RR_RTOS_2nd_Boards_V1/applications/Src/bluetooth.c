#include "bluetooth.h"

uint8_t bluetooth_rx_buffer[1];

extern osSemaphoreId Claw_Catch_BinarySemHandle;
extern osSemaphoreId Catapult_BinarySemHandle;
extern osSemaphoreId Emit_Elevation_BinarySemHandle;


void bluetooth_task(void const * argument)
{
	while(1)
	{
		HAL_UART_Receive(&huart1,bluetooth_rx_buffer,1,portMAX_DELAY);
		xSemaphoreGive(Claw_Catch_BinarySemHandle);
		xSemaphoreGive(Catapult_BinarySemHandle);
		xSemaphoreGive(Emit_Elevation_BinarySemHandle);
		osDelay(10);
	}
}




