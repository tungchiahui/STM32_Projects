/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
CANTxMsg_t TxMsg;
CANRxMsg_t RxMsg;
uint8_t rcvdFlag=0;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_LOOPBACK;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration    
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN1 GPIO Configuration    
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void CAN1_Filter_Init(void)
{
		CAN_FilterTypeDef CAN1_FilerConf;
		CAN1_FilerConf.FilterIdHigh=0X0000;
		CAN1_FilerConf.FilterIdLow=0X0000;
		CAN1_FilerConf.FilterMaskIdHigh=0X0000;
		CAN1_FilerConf.FilterMaskIdLow=0X0000;
		CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;
		CAN1_FilerConf.FilterActivation=ENABLE;
		CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
		CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
		CAN1_FilerConf.FilterBank=0;
		//CAN1_FilerConf.SlaveStartFilterBank=14;
		
		if(HAL_CAN_ConfigFilter(&hcan1,&CAN1_FilerConf)!=HAL_OK)
		{
				Error_Handler();
		}
}

uint8_t CAN1_Send_Msg(CANTxMsg_t *msg,uint16_t mailbox_id,uint8_t *sendbuff)
{
		uint8_t id;
	
		msg->TxMessage.StdId=mailbox_id;
		msg->TxMessage.IDE=CAN_ID_STD;
		msg->TxMessage.DLC=8;
		msg->TxMessage.RTR=CAN_RTR_DATA;
		msg->TxMessage.TransmitGlobalTime=DISABLE;
		for(id=0;id<8;id++)
		{
				msg->payload[id]=sendbuff[id];
		}
		if(HAL_CAN_AddTxMessage(&hcan1,&msg->TxMessage,msg->payload,&msg->mailbox)!=HAL_OK)
				return 0;
		else
				return 1;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
		if(hcan->Instance==CAN1)
		{
				if(HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&(RxMsg.RxMessage),(RxMsg.payload))==HAL_OK)
				rcvdFlag=1;
				else
				Error_Handler();
		}
		//HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
