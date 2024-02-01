#include "dbc_motor.h"

extern Motor_TypeDef motor_data;

void Motor_CMD_PID(PID_ModeState ModeState,fp32 value,int16_t i)
{
	Motor_Judge_Dir(ModeState,value,i);
	uint32_t Pulse; 
	if(ModeState == velocity)
	{
		Pulse = PID_velocity_realize(value,i);
	}
	else if(ModeState == position)
	{
		Pulse = PID_position_realize(value,i);
	}
	else if(ModeState == call)
	{
		Pulse = PID_call_realize(value,i);
	}
	Motor_Set_Pulse(Pulse,i);
}




void Motor_Set_Pulse(uint32_t Pulse,int16_t i)
{
	__HAL_TIM_SetCompare(&htim10,TIM_CHANNEL_1,Pulse);
}

void Motor_Judge_Dir(PID_ModeState ModeState,fp32 value,int16_t i)
{
	if(ModeState == velocity)
	{
		if(value >= 0)
		{
			Motor_Set_Dir(Foreward,i);
		}
		else
		{
			Motor_Set_Dir(Reversal,i);
		}
	}
	
	else if(ModeState == position)
	{
		fp32 delta_position;
		delta_position = value - motor_data.rotor_position;
		if(delta_position >= 0)
		{
			Motor_Set_Dir(Foreward,i);
		}
		else
		{
			Motor_Set_Dir(Reversal,i);
		}
	}
	
	else if(ModeState == call)
	{
		Motor_Judge_Dir(position,value,i);
	}
	
}
	


void Motor_Set_Dir(Motor_DirState DirState,int16_t i)
{
	if(DirState == Parking)
	{
		if(i == 0)
		{
			HAL_GPIO_WritePin(Motor1_IN1_GPIO_Port,Motor1_IN1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Motor1_IN2_GPIO_Port,Motor1_IN2_Pin,GPIO_PIN_RESET);
		}
	}
	else if(DirState == Foreward)
	{
		if(i == 0)
		{
			HAL_GPIO_WritePin(Motor1_IN1_GPIO_Port,Motor1_IN1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(Motor1_IN2_GPIO_Port,Motor1_IN2_Pin,GPIO_PIN_RESET);
		}
	}
	else if(DirState == Reversal)
	{
		if(i == 0)
		{
			HAL_GPIO_WritePin(Motor1_IN1_GPIO_Port,Motor1_IN1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Motor1_IN2_GPIO_Port,Motor1_IN2_Pin,GPIO_PIN_SET);
		}
	}
}
