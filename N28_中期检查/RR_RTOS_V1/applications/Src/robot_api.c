#include "robot_api.h"

extern RC_ctrl_t rc_ctrl;

RobotBehavior actRobot = ROBOT_NORMAL;  //机器人默认全机构就绪状态

extern ChassisBehavior actChassis;

OtherBehavior actOther = Other_ALL_ENABLE; //其他机构默认开启

extern Chassis_Speed_t absolute_chassis_speed;

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
    switch(actRobot)
    {
        case ROBOT_NORMAL:
        actChassis = CHASSIS_NORMAL;
        actOther   = Other_ALL_ENABLE;
        break;

        case ROBOT_POSITION:
        actChassis = CHASSIS_NORMAL;
        actOther   = Other_ALL_DISABLE;
        break;

        case ROBOT_POSITION_DISABLE:
        actChassis = CHASSIS_STOP;
        actOther   = Other_ALL_ENABLE;
        break;

        case ROBOT_ONLY_SHOOT:
        actChassis = CHASSIS_STOP;
        actOther = Other_ONLY_SHOOT;
        break;

        default:
        break;
    }
    Remote_Control_Chassis_Mode(&absolute_chassis_speed);
    Remote_Control_Other_Mode(&absolute_robot_control);
}




/**
 * @brief  遥控器控制方式
 * @param  void
 * @retval void
 * @attention
 */
void Remote_Control_Other_Mode(Other_Devices_t *robot_control_message)
{
    switch(actOther)
    {
        case Other_ALL_ENABLE:
        //射击
        robot_control_message->shoot_speed[0] = rc_ctrl.rc.ch[0] * 13.67f;
        robot_control_message->shoot_speed[1] = -rc_ctrl.rc.ch[0] * 13.67f;
				//抓环
				if(CLAW_DEFAULT_STATUS)
				{
				  robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f;  //默认原位置
					robot_control_message->claw_catch_bool = 0; //默认张开
				}
        //传送
        robot_control_message->conveyer_speed = 3000.0f; //给3000rpm传送速度
        vTaskResume(CLAW_CATCHHandle);
        vTaskResume(CLAW_POSITIONHandle);
        vTaskResume(SHOOT_TASKHandle);
        vTaskResume(CONVEYER_TASKHandle);
        break;

        case Other_ALL_DISABLE:
        //射击
        robot_control_message->shoot_speed[0] = 0.0f;
        robot_control_message->shoot_speed[1] = 0.0f;
       //抓环
				if(CLAW_DEFAULT_STATUS)
				{
				  robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f;  //默认原位置
					robot_control_message->claw_catch_bool = 0; //默认张开
				}
        //传送
        robot_control_message->conveyer_speed = 0.0f;
        vTaskSuspend(CLAW_CATCHHandle);
        vTaskSuspend(CLAW_POSITIONHandle);
        vTaskSuspend(SHOOT_TASKHandle);
        vTaskSuspend(CONVEYER_TASKHandle);
        break;

        case Other_ONLY_SHOOT:
        //射击
        robot_control_message->shoot_speed[0] = rc_ctrl.rc.ch[0] * 13.67f;
        robot_control_message->shoot_speed[1] = -rc_ctrl.rc.ch[0] * 13.67f;
        //抓环
				if(CLAW_DEFAULT_STATUS)
				{
				  robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f;  //默认原位置
					robot_control_message->claw_catch_bool = 0; //默认张开
				}
        //传送
        robot_control_message->conveyer_speed = 0.0f;
        vTaskSuspend(CLAW_CATCHHandle);
        vTaskSuspend(CLAW_POSITIONHandle);
        vTaskResume(SHOOT_TASKHandle);
        vTaskSuspend(CONVEYER_TASKHandle);
        break;

        case Other_ONLY_CLAW:
        //射击
        robot_control_message->shoot_speed[0] = 0.0f;
        robot_control_message->shoot_speed[1] = 0.0f;
        //抓环
				if(CLAW_DEFAULT_STATUS)
				{
				  robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f;  //默认原位置
					robot_control_message->claw_catch_bool = 0; //默认张开
				}
        //传送
        robot_control_message->conveyer_speed = 0.0f;
        vTaskResume(CLAW_CATCHHandle);
        vTaskResume(CLAW_POSITIONHandle);
        vTaskSuspend(SHOOT_TASKHandle);
        vTaskSuspend(CONVEYER_TASKHandle);
        break;

        case Other_ONLY_CONVEYER:
        //射击
        robot_control_message->shoot_speed[0] = 0.0f;
        robot_control_message->shoot_speed[1] = 0.0f;
        //抓环
				if(CLAW_DEFAULT_STATUS)
				{
				  robot_control_message->claw_position = 8191.0f * 19.2f * 0.0f;  //默认原位置
					robot_control_message->claw_catch_bool = 0; //默认张开
				}
        //传送
        robot_control_message->conveyer_speed = 3000.0f; //默认3000速度
        vTaskSuspend(CLAW_CATCHHandle);
        vTaskSuspend(CLAW_POSITIONHandle);
        vTaskSuspend(SHOOT_TASKHandle);
        vTaskResume(CONVEYER_TASKHandle);
        break;
    }
}

