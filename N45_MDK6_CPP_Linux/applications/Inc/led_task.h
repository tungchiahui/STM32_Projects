#ifndef __LED_TASK_H_
#define __LED_TASK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "startup_main.h"
	
class BSP_LED
{
	public:
		BSP_LED(void);
		~BSP_LED(void);
		void LED_Toggle(void);
};

extern BSP_LED bsp_led;

#ifdef __cplusplus
}
#endif
	
#endif
