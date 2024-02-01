#ifndef __TIMER_USER_H_
#define __TIMER_USER_H_
#include "include.h"


#define ABS(x)	((x>0) ? (x) : (-x))  //通用型绝对值函数(x是什么类型就是什么类型)


//底盘速度
typedef struct
{
    fp32 vx;
    fp32 vy;
    fp32 vw;
} Chassis_Speed_t;


#endif



