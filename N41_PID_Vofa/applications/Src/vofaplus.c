#include "vofaplus.h"
extern motor_measure_t motor_can1[8];
extern motor_measure_t motor_can2[8];

extern fp32 Position_Motor_Target[3];
extern fp32 Speed_Motor_Target[3];
extern fp32 Current_Motor_Target[3];

extern RC_ctrl_t rc_ctrl;
fp32 push_deg_angle;
fp32 pre,last;

fp32 push_cnt;
fp32 push_cnt_pre;

fp32 push_speed = 4000;
fp32 catch_speed = 800;
fp32 accel_time = 0;
fp32 push_speed_cmd;
int16_t flags;
int16_t origin_flags;
int16_t go_flags;


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
void test1(void)
{
//	if(htim->Instance == TIM6)
//	{

		if( go_flags == 2)
		{
		if(PUSH_GO_IO == 0)
		{
			HAL_Delay(10);
			if(PUSH_GO_IO == 0)
			{
				Speed_Motor_Target[2] = push_speed;
				accel_time = 0;
				go_flags = 1;
				origin_flags = 2;
			}
		}
	}
			if(origin_flags == 2)
			{
			if(PUSH_ORIGIN_IO == 0)
			{
				HAL_Delay(10);
				if(PUSH_ORIGIN_IO == 0)
				{
					Speed_Motor_Target[2] = 0;
					 accel_time = 0;
					origin_flags = 1;
				}
			}
		}
		
		if(rc_ctrl.rc.ch[3] < 500 && pre > 500)
		{
			Speed_Motor_Target[2] = -push_speed;
			 accel_time = 0;
			go_flags = 2;
		}
		
		if(rc_ctrl.rc.ch[3] > -500 && pre < -500)
		{
			Speed_Motor_Target[2] = push_speed;
			 accel_time = 0;
		}
		pre = rc_ctrl.rc.ch[3];
		push_speed_cmd = accel_optimize(Speed_Motor_Target[2],500);
		
		
		
		
		
		if(rc_ctrl.rc.ch[1] < 500 && last > 500)
		{
			push_cnt++;
		}
		
		if(rc_ctrl.rc.ch[1] > -500 && last < -500)
		{
			push_cnt = 0;
		}
		if(push_cnt_pre < push_cnt && push_cnt <= 10)
		{
			Speed_Motor_Target[0] = -catch_speed;
			Speed_Motor_Target[1] =  catch_speed;
		}
		if(CATCH_MOTOR5_IO == 0)
		{
			HAL_Delay(10);
			if(CATCH_MOTOR5_IO == 0)
			{
				Speed_Motor_Target[0] = 0;
			}
		}
		if(CATCH_MOTOR6_IO == 0)
		{
			HAL_Delay(10);
			if(CATCH_MOTOR6_IO == 0)
			{
				Speed_Motor_Target[1] = 0;
			}
		}

		if(push_cnt == 0 && push_cnt_pre != 0)
		{
			Speed_Motor_Target[0] =  catch_speed;
			Speed_Motor_Target[1] = -catch_speed;
			flags = 1;
		}
		if(flags == 1)
		{
			if(motor_can2[4].total_angle == 0)
			{
				Speed_Motor_Target[0] = 0;
			}
			if(motor_can2[5].total_angle == 0)
			{
				Speed_Motor_Target[1] = 0;
			}
			if(Speed_Motor_Target[0] == 0 && Speed_Motor_Target[1] == 0)
			{
				flags = 0;
			}
		}
		push_cnt_pre = push_cnt;
		last = rc_ctrl.rc.ch[1];
		for(int i = 0;i < 2;i++)
		{
			Current_Motor_Target[i] = PID_velocity_realize_2(Speed_Motor_Target[i],i+5);
		}
		Current_Motor_Target[2] = PID_velocity_realize_2(push_speed_cmd,7);
		CAN2_CMD_2(*Current_Motor_Target,*(Current_Motor_Target+1),*(Current_Motor_Target+2),0);
//	}
}



//防止急停，线性加速度函数
fp32 accel_optimize(fp32 speed_target,fp32 interval_time)
{
	fp32 speed,shoot_accel;
	shoot_accel = speed_target / interval_time;
	speed = shoot_accel * accel_time;
	if(speed != speed_target)
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
    while ((UART5->SR & 0X40) == 0);             /* 等待上一个字符发送完成 */

    UART5->DR = (uint8_t)ch;                     /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/***********************************************END*******************************************/
    

