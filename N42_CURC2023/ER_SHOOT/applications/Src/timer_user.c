#include "timer_user.h"
#include <stdio.h>
uint8_t bluetooth_rx[1];
uint8_t bluetooth_rx_pre;
fp32 speed_delta[2] = {0,0};
fp32 shoot_speed_final[2];
fp32 speed_ratio = 1875.0f / 3075.0f;
extern RC_ctrl_t rc_ctrl;
fp32 Shoot_Speed_Preset[2];
fp32 Shoot_Speed_Target[2];
fp32 Shoot_Current_Target[2];
int8_t rc_s_pre = 0;
fp32 accel_time = 0;
int16_t correct_ch1_pre;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
{
	if(htim->Instance == TIM6)
	{
		//24℃场馆
		if(*bluetooth_rx == 0x01 && bluetooth_rx_pre != *bluetooth_rx) //左按钮 置最下面，发射电机速度为低速，射1型立柱
		{
			Shoot_Speed_Preset[0] =  3075;//2400
			Shoot_Speed_Preset[1] = -Shoot_Speed_Preset[0] * speed_ratio;
//			Shoot_Speed_Preset[0] =  3075;
//			Shoot_Speed_Preset[1] = -1875;
			accel_time = 0;
		}
		
		else if(*bluetooth_rx == 0x02 && bluetooth_rx_pre != *bluetooth_rx)
		{
			Shoot_Speed_Preset[0] =  2815;
			Shoot_Speed_Preset[1] = -Shoot_Speed_Preset[1];
			accel_time = 0;
		
		}
		
		else if(*bluetooth_rx == 0x03 && bluetooth_rx_pre != *bluetooth_rx) //左按钮 置最下面，发射电机速度为低速，射1型立柱
		{
			Shoot_Speed_Preset[0] =  3600;  //三星柱3600
			Shoot_Speed_Preset[1] = -Shoot_Speed_Preset[1];
//			Shoot_Speed_Preset[0] = 4680;
//			Shoot_Speed_Preset[1] = -Shoot_Speed_Preset[0] * speed_ratio;
			accel_time = 0;
		}
		else if (*bluetooth_rx == 0x04 && bluetooth_rx_pre != *bluetooth_rx)   //左按钮 置最上面，发射电机速度为高速，射2型立柱
		{
//			Shoot_Speed_Preset[0] =  3461.5;
//			Shoot_Speed_Preset[1] = -3461.5;
			Shoot_Speed_Preset[0] = 4000;
			Shoot_Speed_Preset[1] = -Shoot_Speed_Preset[0] * speed_ratio;
			accel_time = 0;
		}
		else if(*bluetooth_rx == 0x05 && bluetooth_rx_pre != *bluetooth_rx) //左按钮 置中间，发射电机速度为0，静止
		{
			Shoot_Speed_Preset[0] =  0;
			Shoot_Speed_Preset[1] =  -Shoot_Speed_Preset[1];
			accel_time = 0;
		}
		
		
		/****************************************correct_task纠正任务分支********************************************************************/		
//		if(*bluetooth_rx == 0x00 && bluetooth_rx_pre != 0x00)
//		{
//			speed_delta[0] = 0.0f;
//			speed_delta[1] = -speed_delta[0];
//		}
		
		
		if(*bluetooth_rx == 0x10 && bluetooth_rx_pre != *bluetooth_rx)
		{
			speed_delta[0] = 0.0f;
			speed_delta[1] = -speed_delta[0];
		}
		if(*bluetooth_rx == 0x11 && bluetooth_rx_pre != *bluetooth_rx)
		{
			speed_delta[0] += 2.5f;
			speed_delta[1] = -speed_delta[0];
		}
		if(*bluetooth_rx == 0x12 && bluetooth_rx_pre != *bluetooth_rx)
		{
			speed_delta[0] += 10.0f;
			speed_delta[1] = -speed_delta[0];
		}
		if(*bluetooth_rx == 0x13 && bluetooth_rx_pre != *bluetooth_rx)
		{
			speed_delta[0] += 50.0f;
			speed_delta[1] = -speed_delta[0];
		}
		
		if(*bluetooth_rx == 0x21 && bluetooth_rx_pre != *bluetooth_rx)
		{
			speed_delta[0] -= 2.5f;
			speed_delta[1] = -speed_delta[0];
		}
		if(*bluetooth_rx == 0x22 && bluetooth_rx_pre != *bluetooth_rx)
		{
			speed_delta[0] -= 10.0f;
			speed_delta[1] = -speed_delta[0];
		}
		
		
		shoot_speed_final[0] = Shoot_Speed_Preset[0] + speed_delta[0];
		shoot_speed_final[1] = -shoot_speed_final[0];
		
		bluetooth_rx_pre = *bluetooth_rx;

		
		for(int i = 0;i < 2;i++)
		{
			Shoot_Speed_Target[i] = accel_optimize(shoot_speed_final[i],1500);
			Shoot_Current_Target[i] = PID_velocity_realize_1(Shoot_Speed_Target[i],i+5);
		}
		CAN1_CMD_2(Shoot_Current_Target[0],Shoot_Current_Target[1],0,0);
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		
	}
}


//防止急停，线性加速度函数
fp32 accel_optimize(fp32 speed_target,fp32 interval_time)
{
	
	fp32 speed,shoot_accel;
	shoot_accel = speed_target / interval_time;
	speed = shoot_accel * accel_time;
	if(ABS(speed) <= ABS(speed_target))
	{
	 accel_time++;
	}
	return speed;
}






/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)                    /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");          /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");            /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);             /* 等待上一个字符发送完成 */

    USART1->DR = (uint8_t)ch;                     /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/***********************************************END*******************************************/



