#ifndef __BSP_CAN_H
#define __BSP_CAN_H
#include "main.h"


void CAN_Start(CAN_HandleTypeDef *hcan);

void CAN1_Filter_Init(void);
void CAN2_Filter_Init(void);


#endif
