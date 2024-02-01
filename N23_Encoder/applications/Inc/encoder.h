#ifndef __ENCODER_H_
#define __ENCODER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "include.h"

#define ABS(x)	( (x>0) ? (x) : (-x) ) 

#define PULSE_PER_REVOLUTION  13.0f	/* 线数 */
#define FRE_DOU_RATIO         4.0f  	/* 倍频系数，根据编码器模式选 */
#define REDUCTION_RATIO       30.0f  /* 减速比30:1 */



typedef struct
{
	int32_t encoder_now;
	int32_t encoder_pre;
	int32_t encoder_delta;
	int32_t encoder_delta_sum;
}Encoder_TypeDef;


/* 电机参数结构体 */
typedef struct 
{
  bool_t state;          /*电机状态*/
  fp32 current;          /*电机电流*/
  fp32 volatage;         /*电机电压*/
  fp32 power;            /*电机功率*/
  fp32 rotor_speed;      /*电机[转子]实际速度*/
	fp32 motor_speed;		   /*电机[转轴]实际速度*/
	fp32 rotor_position;	 /*电机[转子]实际转动距离*/
	fp32 motor_position;	 /*电机[转轴]实际转动距离*/
	fp32 rotor_round_cnt;  /*电机[转子]实际转动圈数*/
	fp32 motor_round_cnt;  /*电机[转轴]实际转动圈数*/
  int32_t motor_pwm;     /*设置比较值大小 */
}Motor_TypeDef;




int32_t get_encoder_value(void);
void motor_message_filtering(Encoder_TypeDef *encoder,Motor_TypeDef *motor,uint8_t ms);

#ifdef __cplusplus
}
#endif

#endif
