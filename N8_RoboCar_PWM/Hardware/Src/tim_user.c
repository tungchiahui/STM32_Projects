#include "tim_user.h"
#include "math.h"
#include "stdlib.h"

extern PS2_UserDataTypedef PStwo;
int Speed_Motor_Target_1,Speed_Motor_Target_2,Speed_Motor_Target_3,Speed_Motor_Target_4;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{		
		if(htim->Instance == htim1.Instance)               //htim==(&htim1)
		{
			if((PStwo.PSS_LY > 0) && (abs(PStwo.PSS_LY) > abs(PStwo.PSS_LX)))
			{
				Forward(PStwo.PSS_LY);
			}
			
			else if((PStwo.PSS_LY < 0) && (abs(PStwo.PSS_LY) > abs(PStwo.PSS_LX)))
			{
				Reverse(PStwo.PSS_LY);
			}
			
			else if((PStwo.PSS_LX > 0) && (abs(PStwo.PSS_LY) < abs(PStwo.PSS_LX)))
			{
				TurnRight(PStwo.PSS_LX);
			}
			
			else if((PStwo.PSS_LY < 0) && (abs(PStwo.PSS_LY) < abs(PStwo.PSS_LX)))
			{
				TurnLeft(PStwo.PSS_LY);
			}
			
			else 
			{
				Stop();
			}
			
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Speed_Motor_Target_1);   //ARR = 20000-1
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,Speed_Motor_Target_2);			
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,Speed_Motor_Target_3);			
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,Speed_Motor_Target_4);			
		}
}






void Stop(void)
{
	Speed_Motor_Target_1 = 	0;
	Speed_Motor_Target_2 = 	0;
	Speed_Motor_Target_3 = 	0;
	Speed_Motor_Target_4 = 	0;
}


void Emergency_Stop(void)
{
	HAL_GPIO_WritePin(MOTOR1_IN1_GPIO_Port, MOTOR1_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR1_IN2_GPIO_Port, MOTOR1_IN2_Pin, GPIO_PIN_RESET);	
				
	HAL_GPIO_WritePin(MOTOR2_IN1_GPIO_Port, MOTOR2_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR2_IN2_GPIO_Port, MOTOR2_IN2_Pin, GPIO_PIN_RESET);	
				
	HAL_GPIO_WritePin(MOTOR3_IN1_GPIO_Port, MOTOR3_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR3_IN2_GPIO_Port, MOTOR3_IN2_Pin, GPIO_PIN_RESET);	
				
	HAL_GPIO_WritePin(MOTOR4_IN1_GPIO_Port, MOTOR4_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR4_IN2_GPIO_Port, MOTOR4_IN2_Pin, GPIO_PIN_RESET);	
}




void Forward(int speed)
{
	HAL_GPIO_WritePin(MOTOR1_IN1_GPIO_Port, MOTOR1_IN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR1_IN2_GPIO_Port, MOTOR1_IN2_Pin, GPIO_PIN_RESET);	
				
	HAL_GPIO_WritePin(MOTOR2_IN1_GPIO_Port, MOTOR2_IN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR2_IN2_GPIO_Port, MOTOR2_IN2_Pin, GPIO_PIN_RESET);	
				
	HAL_GPIO_WritePin(MOTOR3_IN1_GPIO_Port, MOTOR3_IN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR3_IN2_GPIO_Port, MOTOR3_IN2_Pin, GPIO_PIN_RESET);	
				
	HAL_GPIO_WritePin(MOTOR4_IN1_GPIO_Port, MOTOR4_IN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR4_IN2_GPIO_Port, MOTOR4_IN2_Pin, GPIO_PIN_RESET);	
				
	Speed_Motor_Target_1 = abs(speed);
	Speed_Motor_Target_2 = 	abs(speed);
	Speed_Motor_Target_3 = 	abs(speed);
	Speed_Motor_Target_4 = 	abs(speed);
}


void Reverse(int speed)
{
	HAL_GPIO_WritePin(MOTOR1_IN1_GPIO_Port, MOTOR1_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR1_IN2_GPIO_Port, MOTOR1_IN2_Pin, GPIO_PIN_SET);	
				
	HAL_GPIO_WritePin(MOTOR2_IN1_GPIO_Port, MOTOR2_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR2_IN2_GPIO_Port, MOTOR2_IN2_Pin, GPIO_PIN_SET);	
				
	HAL_GPIO_WritePin(MOTOR3_IN1_GPIO_Port, MOTOR3_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR3_IN2_GPIO_Port, MOTOR3_IN2_Pin, GPIO_PIN_SET);	
				
	HAL_GPIO_WritePin(MOTOR4_IN1_GPIO_Port, MOTOR4_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR4_IN2_GPIO_Port, MOTOR4_IN2_Pin, GPIO_PIN_SET);	
	
	Speed_Motor_Target_1 =  abs(speed);
	Speed_Motor_Target_2 = 	abs(speed);
	Speed_Motor_Target_3 = 	abs(speed);
	Speed_Motor_Target_4 = 	abs(speed);
}



void TurnLeft(int fast_speed)
{
	int slow_speed;
	slow_speed = abs(fast_speed) - 0.1 * abs(fast_speed);
	
	Speed_Motor_Target_1 =  abs(slow_speed);
	Speed_Motor_Target_2 = 	abs(fast_speed);
	Speed_Motor_Target_3 = 	abs(slow_speed);
	Speed_Motor_Target_4 = 	abs(fast_speed);
}

void TurnRight(int fast_speed)
{
	int slow_speed;
	slow_speed = abs(fast_speed) - 0.1 * abs(fast_speed);
	
	Speed_Motor_Target_1 =  abs(fast_speed);
	Speed_Motor_Target_2 = 	abs(slow_speed);
	Speed_Motor_Target_3 = 	abs(fast_speed);
	Speed_Motor_Target_4 = 	abs(slow_speed);
}

