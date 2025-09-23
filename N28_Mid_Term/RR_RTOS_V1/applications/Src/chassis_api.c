#include "chassis_api.h"

extern RC_ctrl_t rc_ctrl;

ChassisBehavior actChassis = CHASSIS_NORMAL; //底盘默认遥控行走

Chassis_Speed_t absolute_chassis_speed;

//fp32 Angle_Helm_Target[4];
fp32 Speed_Motor_Target[4];

fp32 M3508_Target[4];

extern osThreadId CHASSIS_TASKHandle;
extern osThreadId CLAW_CATCHHandle;
extern osThreadId CLAW_POSITIONHandle;
extern osThreadId SHOOT_TASKHandle;
extern osThreadId CONVEYER_TASKHandle;


/**
 * @brief  遥控器控制方式
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
 * @brief  底盘运动解析式计算
 * @param  void
 * @retval void
 * @attention  此函数是全向轮底盘解析式
 */
void Chassis_Sports_Calc(Chassis_Speed_t speed)
{
    Speed_Motor_Target[0] =   speed.vy + speed.vx + speed.vw;
	Speed_Motor_Target[1] = - speed.vy + speed.vx + speed.vw;
	Speed_Motor_Target[2] = - speed.vy - speed.vx + speed.vw;
	Speed_Motor_Target[3] =   speed.vy - speed.vx + speed.vw;
}





/**
 * @brief  底盘电机输出
 * @param  void
 * @retval void
 * @attention
 */
void Chassis_Loop_Out(void)
{
    M3508_Target[0] = PID_velocity_realize_1(Speed_Motor_Target[0], 1);
    M3508_Target[1] = PID_velocity_realize_1(Speed_Motor_Target[1], 2);
    M3508_Target[2] = PID_velocity_realize_1(Speed_Motor_Target[2], 3);
    M3508_Target[3] = PID_velocity_realize_1(Speed_Motor_Target[3], 4);

    CAN1_CMD_1(M3508_Target[0], M3508_Target[1], M3508_Target[2], M3508_Target[3]);
}


