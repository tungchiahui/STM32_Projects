/*
 * MPU6050.h
 *
 *  Created on: Aug 25, 2024
 *      Author: 王滋行
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "struct_typedef.h"
	
#define ERROR_MPU_INIT      -1
#define ERROR_SET_SENSOR    -2
#define ERROR_CONFIG_FIFO   -3
#define ERROR_SET_RATE      -4
#define ERROR_LOAD_MOTION_DRIVER    -5
#define ERROR_SET_ORIENTATION       -6
#define ERROR_ENABLE_FEATURE        -7
#define ERROR_SET_FIFO_RATE         -8
#define ERROR_SELF_TEST             -9
#define ERROR_DMP_STATE             -10

#define DEFAULT_MPU_HZ  100
#define Q30  1073741824.0f
	
typedef struct
{
	__packed struct
	{
		fp32 X;
		fp32 Y;
		fp32 Z;
	}Accel;

	__packed struct
	{
		fp32 X;
		fp32 Y;
		fp32 Z;
	}Gyro;
	
	__packed struct
	{
		fp32 X;
		fp32 Y;
		fp32 Z;
	}Magnet;
	
		__packed struct
	{
		fp32 q[4];
	}Quat;//欧拉角
	
	__packed struct
	{
		fp32 yaw;    //偏航角  前进的偏移
		fp32 pitch;  //俯仰角	 前后的上下摆动
		fp32 roll;   //翻滚角  左右的上下摆动
	}Euler;//欧拉角
	
	fp32 Temp;
	
}MPU6050_DMP_t;
	
	
int MPU6050_DMP_init(void);
int MPU6050_DMP_Get_Data(void);

#ifdef __cplusplus
}
#endif
	
#endif /* INC_MPU6050_H_ */
