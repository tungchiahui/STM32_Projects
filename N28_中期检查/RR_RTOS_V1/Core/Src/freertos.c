/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId CHASSIS_TASKHandle;
osThreadId CLAW_CATCHHandle;
osThreadId CLAW_POSITIONHandle;
osThreadId SHOOT_TASKHandle;
osThreadId CONVEYER_TASKHandle;
osThreadId ROBOT_BEHAVIOURHandle;
osSemaphoreId Chassis_BinarySemHandle;
osSemaphoreId ClawCatch_BinarySemHandle;
osSemaphoreId ClawPosition_BinarySemHandle;
osSemaphoreId Shoot_BinarySemHandle;
osSemaphoreId Conveyer_BinarySemHandle;
osSemaphoreId ClawLinkage_BinarySemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void chassis_task(void const * argument);
void claw_catch_task(void const * argument);
void claw_position_task(void const * argument);
void shoot_task(void const * argument);
void conveyer_task(void const * argument);
void robot_behaviour_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of Chassis_BinarySem */
  osSemaphoreDef(Chassis_BinarySem);
  Chassis_BinarySemHandle = osSemaphoreCreate(osSemaphore(Chassis_BinarySem), 1);

  /* definition and creation of ClawCatch_BinarySem */
  osSemaphoreDef(ClawCatch_BinarySem);
  ClawCatch_BinarySemHandle = osSemaphoreCreate(osSemaphore(ClawCatch_BinarySem), 1);

  /* definition and creation of ClawPosition_BinarySem */
  osSemaphoreDef(ClawPosition_BinarySem);
  ClawPosition_BinarySemHandle = osSemaphoreCreate(osSemaphore(ClawPosition_BinarySem), 1);

  /* definition and creation of Shoot_BinarySem */
  osSemaphoreDef(Shoot_BinarySem);
  Shoot_BinarySemHandle = osSemaphoreCreate(osSemaphore(Shoot_BinarySem), 1);

  /* definition and creation of Conveyer_BinarySem */
  osSemaphoreDef(Conveyer_BinarySem);
  Conveyer_BinarySemHandle = osSemaphoreCreate(osSemaphore(Conveyer_BinarySem), 1);

  /* definition and creation of ClawLinkage_BinarySem */
  osSemaphoreDef(ClawLinkage_BinarySem);
  ClawLinkage_BinarySemHandle = osSemaphoreCreate(osSemaphore(ClawLinkage_BinarySem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of CHASSIS_TASK */
  osThreadDef(CHASSIS_TASK, chassis_task, osPriorityNormal, 0, 128);
  CHASSIS_TASKHandle = osThreadCreate(osThread(CHASSIS_TASK), NULL);

  /* definition and creation of CLAW_CATCH */
  osThreadDef(CLAW_CATCH, claw_catch_task, osPriorityNormal, 0, 128);
  CLAW_CATCHHandle = osThreadCreate(osThread(CLAW_CATCH), NULL);

  /* definition and creation of CLAW_POSITION */
  osThreadDef(CLAW_POSITION, claw_position_task, osPriorityNormal, 0, 128);
  CLAW_POSITIONHandle = osThreadCreate(osThread(CLAW_POSITION), NULL);

  /* definition and creation of SHOOT_TASK */
  osThreadDef(SHOOT_TASK, shoot_task, osPriorityNormal, 0, 128);
  SHOOT_TASKHandle = osThreadCreate(osThread(SHOOT_TASK), NULL);

  /* definition and creation of CONVEYER_TASK */
  osThreadDef(CONVEYER_TASK, conveyer_task, osPriorityNormal, 0, 128);
  CONVEYER_TASKHandle = osThreadCreate(osThread(CONVEYER_TASK), NULL);

  /* definition and creation of ROBOT_BEHAVIOUR */
  osThreadDef(ROBOT_BEHAVIOUR, robot_behaviour_task, osPriorityNormal, 0, 128);
  ROBOT_BEHAVIOURHandle = osThreadCreate(osThread(ROBOT_BEHAVIOUR), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_chassis_task */
/**
* @brief Function implementing the CHASSIS_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_chassis_task */
__weak void chassis_task(void const * argument)
{
  /* USER CODE BEGIN chassis_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END chassis_task */
}

/* USER CODE BEGIN Header_claw_catch_task */
/**
* @brief Function implementing the CLAW_CATCH thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_claw_catch_task */
__weak void claw_catch_task(void const * argument)
{
  /* USER CODE BEGIN claw_catch_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END claw_catch_task */
}

/* USER CODE BEGIN Header_claw_position_task */
/**
* @brief Function implementing the CLAW_POSITION thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_claw_position_task */
__weak void claw_position_task(void const * argument)
{
  /* USER CODE BEGIN claw_position_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END claw_position_task */
}

/* USER CODE BEGIN Header_shoot_task */
/**
* @brief Function implementing the SHOOT_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_shoot_task */
__weak void shoot_task(void const * argument)
{
  /* USER CODE BEGIN shoot_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END shoot_task */
}

/* USER CODE BEGIN Header_conveyer_task */
/**
* @brief Function implementing the CONVEYER_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_conveyer_task */
__weak void conveyer_task(void const * argument)
{
  /* USER CODE BEGIN conveyer_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END conveyer_task */
}

/* USER CODE BEGIN Header_robot_behaviour_task */
/**
* @brief Function implementing the ROBOT_BEHAVIOUR thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_robot_behaviour_task */
__weak void robot_behaviour_task(void const * argument)
{
  /* USER CODE BEGIN robot_behaviour_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END robot_behaviour_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
