/**
  ******************************************************************************
  * File Name          : CAN.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __can_H
#define __can_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal_can.h"

typedef struct
{
		uint32_t mailbox;
		CAN_TxHeaderTypeDef TxMessage;
		uint8_t payload[8];
}CANTxMsg_t;

typedef struct
{
		CAN_RxHeaderTypeDef RxMessage;
		uint8_t payload[8];
}CANRxMsg_t;
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */
extern uint8_t rcvdFlag;
extern CANTxMsg_t TxMsg;
extern CANRxMsg_t RxMsg;

/* USER CODE END Private defines */

void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */
void CAN1_Filter_Init(void);
uint8_t CAN1_Send_Msg(CANTxMsg_t *msg,uint16_t mailbox_id,uint8_t *sendbuff);
void CAN1_Receive_Msg(CANRxMsg_t *msg);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ can_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
