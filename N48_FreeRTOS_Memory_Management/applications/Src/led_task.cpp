#include "led_task.h"


int pre_heapsize,heapsize,after_heapsize;

extern "C"
void led_task(void const * argument)
{
	
	pre_heapsize = xPortGetFreeHeapSize();
	
	int * pa;
	pa = (int *)pvPortMalloc(30);
	*pa = 50;
	
	heapsize = xPortGetFreeHeapSize();
	
	vPortFree(pa);
	
	after_heapsize = xPortGetFreeHeapSize();
	
	for(;;)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		vTaskDelay(500);
	}
}