#include "led_task.h"

BSP_LED bsp_led;

extern "C"
void led_task(void const * argument)
{
	
	
	for(;;)
	{
		bsp_led.LED_Toggle();  //实例化后调用翻转电平函数

//		BSP_LED::LED_Toggle();  //或者直接不实例化，直接把类当成命名空间使用
	}
}



BSP_LED::BSP_LED()
{
	
}

BSP_LED::~BSP_LED()
{
	
}

void BSP_LED::LED_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}