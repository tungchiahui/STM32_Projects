#include "startup_main.h"
#include "led_task.h"
#include "bsp_delay.h"


void startup_main(void)
{
	bsp_delay.f1.Init(72);

	
#if isRTOS==0    	//������������
	for(;;)  //��ͬ��while(true)
	{
		bsp_led.LED_Toggle();  //ʵ��������÷�ת��ƽ����

//		BSP_LED::LED_Toggle();  //����ֱ�Ӳ�ʵ������ֱ�Ӱ��൱�������ռ�ʹ�á�
	
	bsp_delay.f1.ms(500);
	}
#endif
}