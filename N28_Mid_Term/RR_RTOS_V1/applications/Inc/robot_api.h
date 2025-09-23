#ifndef __ROBOT_API_H_
#define __ROBOT_API_H_
#include "include.h"

#define CLAW_DEFAULT_STATUS 0

//机器人模式
typedef enum
{
    ROBOT_NORMAL     = 0,    //机器人全机构就绪模式
    ROBOT_POSITION   = 1,	 //定位模式，禁用除底盘以外的所有机构
    ROBOT_POSITION_DISABLE = 2,  //禁用移动模式，其他机构正常
    ROBOT_ONLY_SHOOT = 3,        //定点射击模式
} RobotBehavior;


//其他模式
typedef enum
{
    Other_ALL_ENABLE    = 0,     //机器人全机构就绪模式
    Other_ALL_DISABLE   = 1,	 //禁用除底盘以外的所有机构
    Other_ONLY_SHOOT    = 2,     //仅射击模式
    Other_ONLY_CLAW     = 3,     //仅抓取模式
    Other_ONLY_CONVEYER = 4,     //仅传送模式
} OtherBehavior;





//其他电机信息
typedef struct
{
    fp32 shoot_speed[2];
    fp32 claw_position;
    bool_t claw_catch_bool;
    fp32 conveyer_speed;
} Other_Devices_t;



void Remote_Control_Robot_Behavior_Set_Mode(void);

void Remote_Control_Robot_Behavior_Mode(void);

void Remote_Control_Other_Mode(Other_Devices_t *robot_control_message);




#endif
