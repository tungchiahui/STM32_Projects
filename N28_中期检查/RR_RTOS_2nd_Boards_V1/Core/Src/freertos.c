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
osThreadId BLUETOOTHHandle;
osThreadId CLAW_CATCHHandle;
osThreadId CATAPULTHandle;
osThreadId EMIT_ELEVATIONHandle;
osSemaphoreId Claw_Catch_BinarySemHandle;
osSemaphoreId Catapult_BinarySemHandle;
osSemaphoreId Emit_Elevation_BinarySemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void bluetooth_task(void const * argument);
void claw_catch_task(void const * argument);
void catapult_task(void const * argument);
void emit_elevation_task(void const * argument);

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
  /* definition and creation of Claw_Catch_BinarySem */
  osSemaphoreDef(Claw_Catch_BinarySem);
  Claw_Catch_BinarySemHandle = osSemaphoreCreate(osSemaphore(Claw_Catch_BinarySem), 1);

  /* definition and creation of Catapult_BinarySem */
  osSemaphoreDef(Catapult_BinarySem);
  Catapult_BinarySemHandle = osSemaphoreCreate(osSemaphore(Catapult_BinarySem), 1);

  /* definition and creation of Emit_Elevation_BinarySem */
  osSemaphoreDef(Emit_Elevation_BinarySem);
  Emit_Elevation_BinarySemHandle = osSemaphoreCreate(osSemaphore(Emit_Elevation_BinarySem), 1);

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
  /* definition and creation of BLUETOOTH */
  osThreadDef(BLUETOOTH, bluetooth_task, osPriorityNormal, 0, 128);
  BLUETOOTHHandle = osThreadCreate(osThread(BLUETOOTH), NULL);

  /* definition and creation of CLAW_CATCH */
  osThreadDef(CLAW_CATCH, claw_catch_task, osPriorityNormal, 0, 128);
  CLAW_CATCHHandle = osThreadCreate(osThread(CLAW_CATCH), NULL);

  /* definition and creation of CATAPULT */
  osThreadDef(CATAPULT, catapult_task, osPriorityNormal, 0, 128);
  CATAPULTHandle = osThreadCreate(osThread(CATAPULT), NULL);

  /* definition and creation of EMIT_ELEVATION */
  osThreadDef(EMIT_ELEVATION, emit_elevation_task, osPriorityNormal, 0, 128);
  EMIT_ELEVATIONHandle = osThreadCreate(osThread(EMIT_ELEVATION), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_bluetooth_task */
/**
  * @brief  Function implementing the BLUETOOTH thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_bluetooth_task */
__weak void bluetooth_task(void const * argument)
{
  /* USER CODE BEGIN bluetooth_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END bluetooth_task */
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

/* USER CODE BEGIN Header_catapult_task */
/**
* @brief Function implementing the CATAPULT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_catapult_task */
__weak void catapult_task(void const * argument)
{
  /* USER CODE BEGIN catapult_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END catapult_task */
}

/* USER CODE BEGIN Header_emit_elevation_task */
/**
* @brief Function implementing the EMIT_ELEVATION thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_emit_elevation_task */
__weak void emit_elevation_task(void const * argument)
{
  /* USER CODE BEGIN emit_elevation_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END emit_elevation_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

