/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
CANTxMsg_t TxMsg;  //定义发送邮件实体
CANRxMsg_t RxMsg;  //定义接收邮件实体
uint8_t rcvdFlag=0;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
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

//筛选器配置
void CAN1_Filter_Init(void)
{
		CAN_FilterTypeDef CAN1_FilerConf;
	/*筛选器具体的Id要求（配合掩码使用）
	比如：下面的掩码为 0x0101,即二进制0000|0001|0000|0001,表示我只关心从右数第一位，以及第9位。
	因此当具体Id要求为 0x0000,即二进制0000|0000|0000|0000。
	结合掩码实际就是筛选器最终允许通过的Id为
									  xxxx|xxx0|xxxx|xxx0  （x表示0或者1都可以）
	因此具体筛选器允许的Id范围是需要结合掩码来看的
	*/
		CAN1_FilerConf.FilterIdHigh=0X0000;					   //具体Id要求高16位
		CAN1_FilerConf.FilterIdLow=0X0000;					   //具体Id要求低16位
		CAN1_FilerConf.FilterMaskIdHigh=0X0000;                //掩码高16位全设置为0，表示对所有位报文Id高16位都不关心
		CAN1_FilerConf.FilterMaskIdLow=0X0000;                 //掩码低16位全设置为0，表示对所有位报文Id低16位都不关心
		CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;  //筛选器接收到的报文放入到FIFO0中，即为接收邮箱0
		CAN1_FilerConf.FilterActivation=ENABLE;                //筛选器使能（开启）
		CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;       //筛选器掩码模式
		CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;      //掩码用32位表示
		/*这里说明一下为什么填0
			首先一个筛选器有28个组。一块stm32板子的can外设共用一个can外设
			比如F4系列基本都有两个can模块，如果只用一个can的时候，我们FilterBank可选0-13，因此下面就是0，其实填0-13都可以。
			如果用两个can，则要分配两个筛选器组。一个是FilterBank
			则
			can1的筛选器组选择0-13。如CAN1_FilerConf.FilterBank=0;	
			can2的筛选器组选择14-27。如CAN1_FilerConf.SlaveStartFilterBank=14;	
		*/
		CAN1_FilerConf.FilterBank=0;							
		CAN1_FilerConf.SlaveStartFilterBank=14;
		
		/*	此处&hcan1指明是can1的筛选器配置，但实际上can1和can2的筛选器都配置好了。因为两个can是共用的。
			这是因为STM32的双路CAN共用过滤器组，
			而且过滤器组寄存器与CAN1配置寄存器在物理上是挨着的，HAL库将这些寄存器合并在一个结构里访问而已。
			下面通过调用 "HAL_CAN_ConfigFilter(&hcan1,&CAN1_FilerConf)" 配置can筛选器即可生效。
			无需再调用HAL_CAN_ConfigFilter(&hcan2,&CAN1_FilerConf)
		*/
		if(HAL_CAN_ConfigFilter(&hcan1,&CAN1_FilerConf)!=HAL_OK)
		{
				Error_Handler();
		}
}

uint8_t CAN1_Send_Msg(CANTxMsg_t *msg,uint16_t mailbox_id,uint8_t *sendbuff)
{
		uint8_t id;
	
		msg->TxMessage.StdId=mailbox_id;    		//邮箱id号
		msg->TxMessage.IDE=CAN_ID_STD; 				//邮件的id格式（标准为CAN_ID_STD	  |	  拓展为CAM_ID_EXT）
		msg->TxMessage.DLC=8;						//邮件数据长度 此处为8个字节
		msg->TxMessage.RTR=CAN_RTR_DATA;			//数据帧 一般都是数据帧
		msg->TxMessage.TransmitGlobalTime=DISABLE;  //默认DISABLE
		for(id=0;id<8;id++)
		{
				msg->payload[id]=sendbuff[id];  //装填数据
		}
		
		//发送邮件 注意邮件邮件信息（id号，邮件类型等等）和数据内容是分开发送的，具体看下面这句函数参数
		if(HAL_CAN_AddTxMessage(&hcan1,&msg->TxMessage,msg->payload,&msg->mailbox)!=HAL_OK)
				return 0;
		else
				return 1;
}
/*视频第4节有讲为什么要起这样的函数名字，这个叫回调函数
接收到邮件最终会来到这里。由于我们开了RX0中断，因此我们的回调函数名是
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
如果我们开的是RX1中断，那么回调函数名是void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	//这句话是判断对方发来的信息是发给本板子的can1还是can2，如果是can1，那通过接线就知道是哪个设备发来的数据。
		if(hcan->Instance==CAN1)
		{
			/*
				这个是获取邮件的函数，既然进了接收中断，那么就说明接收到了数据因此我们调用下方函数获取。
				我们提供一个RxMessage以及payload分别接收邮件邮件信息（id号，邮件类型等等）和数据内容。
			*/
				if(HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&(RxMsg.RxMessage),(RxMsg.payload))==HAL_OK)
						rcvdFlag=1;
				else
						Error_Handler();
		}
		//HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
