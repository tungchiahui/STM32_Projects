#include "pid_user.h"
#include "dbc_motor.h"


extern Motor_TypeDef motor_data;

pid_type_def pid_v,pid_pos;

fp32 dbc_motor_speed_pid[3] = {10,0.1,0};			//DBC_Motor_PID参数
fp32 dbc_motor_position_pid[3] = {0.2,0,1};


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
    PID_init(&pid_v, PID_POSITION, dbc_motor_speed_pid, 5000, 1000);
		PID_init(&pid_pos, PID_POSITION, dbc_motor_position_pid, 800, 300);
}



fp32 PID_velocity_realize(fp32 set_speed,int16_t i)
{
		PID_calc(&pid_v,motor_data.rotor_speed,set_speed);
		return pid_v.out;
}

fp32 PID_position_realize(fp32 set_pos,int16_t i)
{

		PID_calc(&pid_pos,motor_data.rotor_position , set_pos);
		return pid_pos.out;

}

fp32 PID_call_realize(fp32 set_pos,int16_t i)
{
		return PID_velocity_realize(PID_position_realize(set_pos,i),i);
}
