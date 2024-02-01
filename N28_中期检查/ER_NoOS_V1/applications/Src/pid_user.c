#include "pid_user.h"

extern motor_measure_t motor_can1[8];
extern motor_measure_t motor_can2[8];

pid_type_def pid_v_1[8],pid_pos_1[8];
pid_type_def pid_v_2[8],pid_pos_2[8];

fp32 chassis_speed_3508_pid[3] = {10, 0.1, 0};//底盘3508参数
fp32 chassis_position_3508_pid[3] = {0.2, 0, 1};

fp32 chassis_speed_2006_pid[3] = {8.3,0.1,1};//底盘2006参数
fp32 chassis_position_2006_pid[3] = {0.27,0.022,0.3};


fp32 claw_speed_3508_pid[3] = {10, 0.1, 0};//爪子3508参数
fp32 claw_position_3508_pid[3] = {0.2, 0, 1};

fp32 shoot_speed_3508_pid[3] = {10, 0.1, 0};//射击3508参数
fp32 shoot_position_3508_pid[3] = {0.2, 0, 1};

fp32 conveyer_speed_3508_pid[3] = {10, 0.1, 0};//传环3508参数
fp32 conveyer_position_3508_pid[3] = {0.2, 0, 1};


#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }


//底盘电机PID初始化
void PID_devices_Init(void)
{
	//默认的PID参数
	for(int i=0;i<4;i++)
	{
    PID_init(&pid_v_1[i], PID_POSITION, chassis_speed_3508_pid, 10000, 6000);
		PID_init(&pid_pos_1[i], PID_POSITION, chassis_position_3508_pid, 400, 300);
		
		PID_init(&pid_v_2[i], PID_POSITION, chassis_speed_2006_pid, 9000, 6000);
		PID_init(&pid_pos_2[i], PID_POSITION, chassis_position_2006_pid, 10000, 2000);
	}
	
//默认的PID参数
	for(int i=4;i<8;i++)
	{		
    PID_init(&pid_v_1[i], PID_POSITION, chassis_speed_3508_pid, 10000, 6000);
		PID_init(&pid_pos_1[i], PID_POSITION, chassis_position_3508_pid, 400, 300);
		
		PID_init(&pid_v_2[i], PID_POSITION, chassis_speed_3508_pid, 10000, 6000);
		PID_init(&pid_pos_2[i], PID_POSITION, chassis_position_3508_pid, 400, 300);
	}
	
//CAN1 1-4 M3508底盘电机       5 传环电机2       6摩擦轮1       7摩擦轮2  
//CAN2 1-4 M2006舵电机      	 5 爪子转盘电机    6爪子升降电机   7传环电机1
	
	//修改某些电机的PID参数
//    PID_init(&pid_v_1[4], PID_POSITION, conveyer_speed_3508_pid, 10000, 6000);
//		PID_init(&pid_pos_1[4], PID_POSITION, conveyer_position_3508_pid, 400, 300);
//	
//	  PID_init(&pid_v_1[5], PID_POSITION, shoot_speed_3508_pid, 12000, 6000);
//		PID_init(&pid_pos_1[5], PID_POSITION, shoot_position_3508_pid, 400, 300);
//	
//		PID_init(&pid_v_1[6], PID_POSITION, shoot_speed_3508_pid, 12000, 6000);
//		PID_init(&pid_pos_1[6], PID_POSITION, shoot_position_3508_pid, 400, 300);
//		
//		PID_init(&pid_v_2[4], PID_POSITION, claw_speed_3508_pid, 10000, 6000);
//		PID_init(&pid_pos_2[4], PID_POSITION, claw_position_3508_pid, 400, 300);
//	
//	  PID_init(&pid_v_2[5], PID_POSITION, claw_speed_3508_pid, 12000, 6000);
//		PID_init(&pid_pos_2[5], PID_POSITION, claw_position_3508_pid, 400, 300);
//	
//		PID_init(&pid_v_2[6], PID_POSITION, conveyer_speed_3508_pid, 12000, 6000);
//		PID_init(&pid_pos_2[6], PID_POSITION, conveyer_speed_3508_pid, 400, 300);

}



float PID_velocity_realize_1(float set_speed,int i)
{
		PID_calc(&pid_v_1[i-1],motor_can1[i-1].speed_rpm , set_speed);
		return pid_v_1[i-1].out;
}

float PID_position_realize_1(float set_pos,int i)
{

		PID_calc(&pid_pos_1[i-1],motor_can1[i-1].total_angle , set_pos);
		return pid_pos_1[i-1].out;

}

float pid_call_1(float position,int i)
{
		return PID_velocity_realize_1(PID_position_realize_1(position,i),i);
}






float PID_velocity_realize_2(float set_speed,int i)
{
		PID_calc(&pid_v_2[i-1],motor_can2[i-1].speed_rpm , set_speed);
		return pid_v_2[i-1].out;
}

float PID_position_realize_2(float set_pos,int i)
{

		PID_calc(&pid_pos_2[i-1],motor_can2[i-1].total_angle , set_pos);
		return pid_pos_2[i-1].out;

}

float pid_call_2(float position,int i)
{
		return PID_velocity_realize_2(PID_position_realize_2(position,i),i);
}


