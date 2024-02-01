#include "startup_main.h"
#include "led_task.h"
#include "bsp_delay.h"


void startup_main(void)
{
	bsp_delay.f1.Init(72);

	
#if isRTOS==0    	//如果是裸机开发
	for(;;)  //等同于while(true)
	{
		bsp_led.LED_Toggle();  //实例化后调用翻转电平函数

//		BSP_LED::LED_Toggle();  //或者直接不实例化，直接把类当成命名空间使用。
	
	bsp_delay.f1.ms(500);
	}
#endif
}