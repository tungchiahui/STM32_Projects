#include "emit_elevation_task.h"

extern osSemaphoreId Emit_Elevation_BinarySemHandle;
extern uint8_t bluetooth_rx_buffer[1];

void emit_elevation_task(void const * argument)
{
	while(1)
	{
			xSemaphoreTake(Emit_Elevation_BinarySemHandle,portMAX_DELAY);
		
			if(*bluetooth_rx_buffer == '2')   //…Ï≥§
			{
				HAL_GPIO_WritePin(Emit_Elevation_1_GPIO_Port,Emit_Elevation_1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(Emit_Elevation_2_GPIO_Port,Emit_Elevation_2_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Emit_Elevation_3_GPIO_Port,Emit_Elevation_3_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(Emit_Elevation_4_GPIO_Port,Emit_Elevation_4_Pin,GPIO_PIN_RESET);
			}
			else if(*bluetooth_rx_buffer == '3')  // ’Àı
			{
				HAL_GPIO_WritePin(Emit_Elevation_1_GPIO_Port,Emit_Elevation_1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Emit_Elevation_2_GPIO_Port,Emit_Elevation_2_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(Emit_Elevation_3_GPIO_Port,Emit_Elevation_3_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Emit_Elevation_4_GPIO_Port,Emit_Elevation_4_Pin,GPIO_PIN_SET);
			}
			else if(*bluetooth_rx_buffer == '4')  //Õ£÷π
			{
				HAL_GPIO_WritePin(Emit_Elevation_1_GPIO_Port,Emit_Elevation_1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Emit_Elevation_2_GPIO_Port,Emit_Elevation_2_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Emit_Elevation_3_GPIO_Port,Emit_Elevation_3_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Emit_Elevation_4_GPIO_Port,Emit_Elevation_4_Pin,GPIO_PIN_RESET);
			}

			osDelay(1);
	}
}

