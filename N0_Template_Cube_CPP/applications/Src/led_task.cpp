#include "led_task.h"

BSP_LED bsp_led;

extern "C"
void led_task(void const * argument)
{
	
	
	for(;;)
	{
		bsp_led.LED_Toggle();  //ʵ��������÷�ת��ƽ����

//		BSP_LED::LED_Toggle();  //����ֱ�Ӳ�ʵ������ֱ�Ӱ��൱�������ռ�ʹ��
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