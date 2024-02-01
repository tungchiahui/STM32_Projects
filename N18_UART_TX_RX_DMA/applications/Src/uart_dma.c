#include "uart_dma.h"

uint8_t rx_buffer[3];

uint8_t tx_buffer[3] = {0x01,0x02,0x03};


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		HAL_UART_Transmit_DMA(&huart1,rx_buffer,3);
	}
}

