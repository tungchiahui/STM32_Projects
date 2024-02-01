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



//��ֹ��ͣ�����Լ��ٶȺ���
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
/* �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB */
#if 1
#if (__ARMCC_VERSION >= 6010050)                    /* ʹ��AC6������ʱ */
__asm(".global __use_no_semihosting\n\t");          /* ������ʹ�ð�����ģʽ */
__asm(".global __ARM_use_no_argv \n\t");            /* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */

#else
/* ʹ��AC5������ʱ, Ҫ�����ﶨ��__FILE �� ��ʹ�ð�����ģʽ */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE �� stdio.h���涨��. */
FILE __stdout;

/* �ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
int fputc(int ch, FILE *f)
{
    while ((UART5->SR & 0X40) == 0);             /* �ȴ���һ���ַ�������� */

    UART5->DR = (uint8_t)ch;                     /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
    return ch;
}
#endif
/***********************************************END*******************************************/
    

