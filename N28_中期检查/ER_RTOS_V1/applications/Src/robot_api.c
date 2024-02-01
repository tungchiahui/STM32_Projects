#include "robot_api.h"

extern RC_ctrl_t rc_ctrl;

RobotBehavior actRobot = ROBOT_NORMAL; //机器人默认全机构就绪状态

ChassisBehavior actChassis = CHASSIS_NORMAL; //底盘默认遥控行走

OtherBehavior actOther = Other_ALL_ENABLE; //其他机构默认开启

Chassis_Speed_t absolute_chassis_speed;

Other_Devices_t absolute_robot_control;

extern osThreadId CHASSIS_TASKHandle;
extern osThreadId CLAW_CATCHHandle;
extern osThreadId CLAW_POSITIONHandle;
extern osThreadId SHOOT_TASKHandle;
extern osThreadId CONVEYER_TASKHandle;

/*
 * @brief  设定遥控器控制机器人模式
 * @param  void
 * @retval void
 * @attention
 */
void Remote_Control_Robot_Behavior_Set_Mode(void)
{
    if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 3) //正常模式
    {
        actRobot = ROBOT_NORMAL;
    }
    else if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 1) //仅移动
    {
        actRobot = ROBOT_POSITION;
    }
    else if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 2) //不可以移动，固定射击和抓环
    {
        actRobot = ROBOT_POSITION_DISABLE;
    }
}


/**
 * @brief  设定遥控器控制机器人模式
 * @param  void
 * @retval void
 * @attention
 */
void Remote_Control_Robot_Behavior_Mode(void)
{
    switch (actRobot)
    {
    case ROBOT_NORMAL:
        actChassis = CHASSIS_NORMAL;
        actOther = Other_ALL_ENABLE;
        break;

    case ROBOT_POSITION:
        actChassis = CHASSIS_NORMAL;
        actOther = Other_ALL_DISABLE;
        break;

    case ROBOT_POSITION_DISABLE:
        actChassis = CHASSIS_STOP;
        actOther = Other_ALL_ENABLE;
        break;

    case ROBOT_ONLY_SHOOT:
        actChassis = CHASSIS_STOP;
        actOther = Other_ONLY_SHOOT;
        break;

    default:
        break;
    }
    Remote_Control_Chassis_Mode(&absolute_chassis_speed);
    Remote_Control_Other_Mode(&actOther);
}

/**
 * @brief  底盘遥控器控制方式
 * @param  void
 * @retval void
 * @attention
 */
void Remote_Control_Chassis_Mode(Chassis_Speed_t *chassis_speed)
{
    /***********************************确定底盘的目标速度*****************************************/
    switch (actChassis)
    {
    case CHASSIS_NORMAL: //正常模式
        chassis_speed->vx = (fp32)rc_ctrl.rc.ch[2]; 
        chassis_speed->vy = (fp32)rc_ctrl.rc.ch[3]; 
        chassis_speed->vw = -(fp32)rc_ctrl.rc.ch[4]; 

        chassis_speed->vx *= 1;
        chassis_speed->vy *= 1;
        chassis_speed->vw *= 3;
        vTaskResume(CHASSIS_TASKHandle);
        break;

    case CHASSIS_STOP:    //闭环急停，定点
        chassis_speed->vx = (fp32)rc_ctrl.rc.ch[2]; 
        chassis_speed->vy = (fp32)rc_ctrl.rc.ch[3]; 
        chassis_speed->vw = -(fp32)rc_ctrl.rc.ch[4]; 

        chassis_speed->vx *= 0;
        chassis_speed->vy *= 0;
        chassis_speed->vw *= 0;
        vTaskResume(CHASSIS_TASKHandle);
        break;

    case CHASSIS_GYROSCOPE: //小陀螺模式
        chassis_speed->vx = (fp32)rc_ctrl.rc.ch[2];
        chassis_speed->vy = (fp32)rc_ctrl.rc.ch[3];
        chassis_speed->vw = -330.0f;

        chassis_speed->vx *= 6;
        chassis_speed->vy *= 6;
        chassis_speed->vw *= 5;
        vTaskResume(CHASSIS_TASKHandle);
        break;

    case CHASSIS_OFFLINE:   //掉线模式
        chassis_speed->vx = (fp32)rc_ctrl.rc.ch[2];
        chassis_speed->vy = (fp32)rc_ctrl.rc.ch[3];
        chassis_speed->vw = -(fp32)rc_ctrl.rc.ch[4]; 

        chassis_speed->vx *= 0;
        chassis_speed->vy *= 0;
        chassis_speed->vw *= 0;
        vTaskSuspend(CHASSIS_TASKHandle);
        break;
    default:
        break;
    }
}



/**
 * @brief  其他机构遥控器控制方式
 * @param  void
 * @retval void
 * @attention
 */
void Remote_Control_Other_Mode(OtherBehavior *actOther)
{
    switch (*actOther)
    {
    case Other_ALL_ENABLE:
//        //射击
//        if(rc_ctrl.rc.ch[1] >= 0 && ABS(rc_ctrl.rc.ch[0]) <=260)
//        {
//            robot_control_message->shoot_speed[0] = rc_ctrl.rc.ch[0] * 13.67f;
//            robot_control_message->shoot_speed[1] = -rc_ctrl.rc.ch[0] * 13.67f;
//        }
//        //抓环
//        if (CLAW_DEFAULT_STATUS)
//        {
//            robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f; //默认原位置
//            robot_control_message->claw_catch_bool = 0;                    //默认张开
//        }
//        //传送
//        if(rc_ctrl.rc.ch[0] >= 580)
//        {
//            robot_control_message->conveyer_speed = 3000.0f; //默认3000速度
//        }
//        else if(rc_ctrl.rc.ch[0] <= -580)
//        {
//            robot_control_message->conveyer_speed = 0.0f;
//        }
        vTaskResume(CLAW_CATCHHandle);
        vTaskResume(CLAW_POSITIONHandle);
        vTaskResume(SHOOT_TASKHandle);
        vTaskResume(CONVEYER_TASKHandle);
        break;

    case Other_ALL_DISABLE:
//        //射击
//        if(rc_ctrl.rc.ch[1] >= 0 && ABS(rc_ctrl.rc.ch[0]) <=260)
//        {
//            robot_control_message->shoot_speed[0] = 0.0f;
//            robot_control_message->shoot_speed[1] = 0.0f;
//        }
//        //抓环
//        if (CLAW_DEFAULT_STATUS)
//        {
//            robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f; //默认原位置
//            robot_control_message->claw_catch_bool = 0;                    //默认张开
//        }
//        //传送
//        robot_control_message->conveyer_speed = 0.0f;
        vTaskSuspend(CLAW_CATCHHandle);
        vTaskSuspend(CLAW_POSITIONHandle);
        vTaskSuspend(SHOOT_TASKHandle);
        vTaskSuspend(CONVEYER_TASKHandle);
        break;

    case Other_ONLY_SHOOT:
//        //射击
//        if(rc_ctrl.rc.ch[1] >= 0 && ABS(rc_ctrl.rc.ch[0]) <=260)
//        {
//        robot_control_message->shoot_speed[0] = rc_ctrl.rc.ch[0] * 13.67f;
//        robot_control_message->shoot_speed[1] = -rc_ctrl.rc.ch[0] * 13.67f;
//        }
//        //抓环
//        if (CLAW_DEFAULT_STATUS)
//        {
//            robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f; //默认原位置
//            robot_control_message->claw_catch_bool = 0;                    //默认张开
//        }
//        //传送
//        robot_control_message->conveyer_speed = 0.0f;
        vTaskSuspend(CLAW_CATCHHandle);
        vTaskSuspend(CLAW_POSITIONHandle);
        vTaskResume(SHOOT_TASKHandle);
        vTaskSuspend(CONVEYER_TASKHandle);
        break;

    case Other_ONLY_CLAW:
//        //射击
//        if(rc_ctrl.rc.ch[1] >= 0 && ABS(rc_ctrl.rc.ch[0]) <=260)
//        {
//        robot_control_message->shoot_speed[0] = 0.0f;
//        robot_control_message->shoot_speed[1] = 0.0f;
//        }
//        //抓环
//        if (CLAW_DEFAULT_STATUS)
//        {
//            robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f; //默认原位置
//            robot_control_message->claw_catch_bool = 0;                    //默认张开
//        }
//        //传送
//        robot_control_message->conveyer_speed = 0.0f;
        vTaskResume(CLAW_CATCHHandle);
        vTaskResume(CLAW_POSITIONHandle);
        vTaskSuspend(SHOOT_TASKHandle);
        vTaskSuspend(CONVEYER_TASKHandle);
        break;

    case Other_ONLY_CONVEYER:
//        //射击
//        if(rc_ctrl.rc.ch[1] >= 0 && ABS(rc_ctrl.rc.ch[0]) <=260)
//        {
//        robot_control_message->shoot_speed[0] = 0.0f;
//        robot_control_message->shoot_speed[1] = 0.0f;
//        }
//        //抓环
//        if (CLAW_DEFAULT_STATUS)
//        {
//            robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f; //默认原位置
//            robot_control_message->claw_catch_bool = 0;                    //默认张开
//        }
//        //传送
//        if(rc_ctrl.rc.ch[0] >= 580)
//        {
//            robot_control_message->conveyer_speed = 3000.0f; //默认3000速度
//        }
//        else if(rc_ctrl.rc.ch[0] <= -580)
//        {
//            robot_control_message->conveyer_speed = 0.0f;
//        }

        vTaskSuspend(CLAW_CATCHHandle);
        vTaskSuspend(CLAW_POSITIONHandle);
        vTaskSuspend(SHOOT_TASKHandle);
        vTaskResume(CONVEYER_TASKHandle);
        break;
    }
}


