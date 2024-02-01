/***      《 舵轮角度计算 》

 *      ┌─┐       ┌─┐ + +
 *   ┌──┘ ┴───────┘ ┴──┐++
 *   │                 │
 *   │       ───       │++ + + +
 *   ███████───███████│+
 *   │                 │+
 *   │      ─┴─        │
 *   │                 │
 *   └───┐         ┌───┘
 *       │         │
 *       │         │   + +
 *       │         │
 *       │         └──────────────┐
 *       │                        │
 *       │                        ├─┐
 *       │                        ┌─┘
 *       │                        │
 *       └─┐  ┐  ┌───────┬──┐  ┌──┘  + + + +
 *         │ ─┤ ─┤       │ ─┤ ─┤
 *         └──┴──┘       └──┴──┘  + + + +
 *
 *
 */

#include "helm_wheel.h"

eChassisAction actChassis = CHASSIS_NORMAL; //底盘默认遥控行走

extern RC_ctrl_t rc_ctrl;

Chassis_Speed_t absolute_chassis_speed;

extern motor_measure_t motor_can1[8];
extern motor_measure_t motor_can2[8];

fp32 Angle_Helm_Target[4];
fp32 Speed_Motor_Target[4];

fp32 M2006_Target[4];
fp32 M3508_Target[4];

/**
 * @brief  设定遥控器控制底盘模式
 * @param  void
 * @retval void
 * @attention
 */
void Remote_Control_Chassis_Set_Mode(void)
{
    if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 3) //底盘正常模式
    {
        actChassis = CHASSIS_NORMAL;
    }
    else if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 1) //底盘大陀螺模式   无云台无法实现
    {
        actChassis = CHASSIS_GYROSCOPE;
    }
}

/**
 * @brief  遥控器控制方式
 * @param  void
 * @retval void
 * @attention
 */
void Remote_Control_Chassis_Mode(void)
{
    /***********************************确定底盘四个电机的目标速度*****************************************/
    switch (actChassis)
    {
    case CHASSIS_NORMAL: //正常模式
        absolute_chassis_speed.vx = (fp32)rc_ctrl.rc.ch[2];
        absolute_chassis_speed.vy = (fp32)rc_ctrl.rc.ch[3];
        absolute_chassis_speed.vw = (fp32)rc_ctrl.rc.ch[4];
        break;
    case CHASSIS_GYROSCOPE: //小陀螺模式
        absolute_chassis_speed.vx = (fp32)rc_ctrl.rc.ch[2];
        absolute_chassis_speed.vy = (fp32)rc_ctrl.rc.ch[3];
        absolute_chassis_speed.vw = 330.0f;
        break;
    default:
        break;
    }
}

/**
 * @brief  计算底盘驱动电机的目标速度
 * @param  speed 底盘坐标的速度
 * @param  out_speed 3508目标速度
 * @retval void
 * @attention
 */
int8_t dir = -1; //决定驱动电机正反转
// fp32 WHEEL_PERIMETER = 1;      //车轮周长
// fp32 CHASSIS_DECELE_RATIO = 1; //底盘减速器比例
fp32 Radius = 1; //半径
void helm_wheel_speed_calc(Chassis_Speed_t *speed, fp32 *out_speed)
{
    // 3508目标速度计算
    fp32 helm_wheel_rpm[4];
    fp32 wheel_rpm_ratio; //轮子速度比率

    //    wheel_rpm_ratio = 60.0f / (WHEEL_PERIMETER * 3.14159f) * CHASSIS_DECELE_RATIO * 1000;
    wheel_rpm_ratio = 1;

    helm_wheel_rpm[0] = sqrt(pow(speed->vy - speed->vw * Radius * 0.707107f, 2) + pow(speed->vx + speed->vw * Radius * 0.707107f, 2)) * wheel_rpm_ratio; //求次方后开平方根
    helm_wheel_rpm[1] = sqrt(pow(speed->vy - speed->vw * Radius * 0.707107f, 2) + pow(speed->vx - speed->vw * Radius * 0.707107f, 2)) * wheel_rpm_ratio;
    helm_wheel_rpm[2] = sqrt(pow(speed->vy - speed->vw * Radius * 0.707107f, 2) + pow(speed->vx + speed->vw * Radius * 0.707107f, 2)) * wheel_rpm_ratio;
    helm_wheel_rpm[3] = sqrt(pow(speed->vy + speed->vw * Radius * 0.707107f, 2) + pow(speed->vx + speed->vw * Radius * 0.707107f, 2)) * wheel_rpm_ratio;

    for (int i = 0; i < 4; i++)
    {
        out_speed[i] = dir * helm_wheel_rpm[i] * 5;
    }
}

/**
 * @brief  计算底盘航向电机的目标角度
 * @param  speed 底盘坐标的速度
 * @param  out_angle 2006目标角度
 * @retval void
 * @attention
 */
fp32 temp_x[4] = {0, 0, 0, 0}, temp_y[4] = {0, 0, 0, 0};
fp32 helm_wheel_angle[4] = {0, 0, 0, 0}; // 2006编码器目标角度
fp32 helm_wheel_angle_temp[4] = {0, 0, 0, 0};
fp32 helm_wheel_angle_last[4] = {0, 0, 0, 0};
fp32 helm_atan_angle[4] = {0, 0, 0, 0}; //非静态变量定义时，报警告可以修改为volatile fp32 atan_angle[4];
fp32 helm_round_cnt = 0;;
void helm_wheel_angle_calc(Chassis_Speed_t *speed, fp32 *out_angle)
{
    // 2006目标角度计算
    if (!(speed->vx == 0 && speed->vy == 0 && speed->vw == 0)) //防止除数为零
    {
        temp_x[0] = speed->vx + speed->vw * Radius * 0.707107f;
        temp_y[0] = speed->vy + speed->vw * Radius * 0.707107f;
        temp_x[1] = speed->vx - speed->vw * Radius * 0.707107f;
        temp_y[1] = speed->vy + speed->vw * Radius * 0.707107f;
        temp_x[2] = speed->vx + speed->vw * Radius * 0.707107f;
        temp_y[2] = speed->vy + speed->vw * Radius * 0.707107f;
        temp_x[3] = speed->vx - speed->vw * Radius * 0.707107f;
        temp_y[3] = speed->vy + speed->vw * Radius * 0.707107f;

        for (int i = 0; i < 4; i++)
        {
            helm_atan_angle[i] = atan2_angle_calc(temp_x[i], temp_y[i]);
            helm_atan_angle[i] = rad2deg(helm_atan_angle[i]);
        }
    }
    calc_min_angle_round(helm_atan_angle, helm_wheel_angle_last, helm_wheel_angle_temp, &helm_round_cnt, &dir);

    if (speed->vx == 0 && speed->vy == 0 && speed->vw == 0) //摇杆回中时
    {
        for (int i = 0; i < 4; i++) // memcpy狗都不用
                                    //            out_angle[i] = 0; = helm_wheel_angle_last[i];
            out_angle[i] = 0;
    }
    else //不回中时
    {
        for (int i = 0; i < 4; i++)
        {
            out_angle[i] = (helm_wheel_angle[i] / 360.0f * 8191.0f) * 36.0f * (67.0f / 20.0f);
            //            helm_wheel_angle_last[i] = out_angle[i];
        }
    }
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

    M2006_Target[0] = pid_call_2(Angle_Helm_Target[0], 1);
    M2006_Target[1] = pid_call_2(Angle_Helm_Target[1], 2);
    M2006_Target[2] = pid_call_2(Angle_Helm_Target[2], 3);
    M2006_Target[3] = pid_call_2(Angle_Helm_Target[3], 4);

    CAN1_CMD_1(M3508_Target[0], M3508_Target[1], M3508_Target[2], M3508_Target[3]);
    CAN2_CMD_1(M2006_Target[0], M2006_Target[1], M2006_Target[2], M2006_Target[3]);
}

/**
 * @brief  解算出遥杆到舵电机的角度值
 * @param  x x轴参数的地址
 * @param  y y轴参数的地址
 * @retval temp 解算后的角度
 * @attention
 */
fp32 atan2_angle_calc(fp32 x, fp32 y)
{
    volatile fp32 temp;
    temp = atan2(-x, y); //舵电机放置问题决定
                         //    if(y >= 0)
                         //    {
                         //        temp = atan2(x,y);
                         //    }
                         //    else if(y < 0)
                         //    {
                         //        temp = atan2(x,-y);
                         //    }
    return temp;
}

/**
 * @brief  优化舵轮转动角度，优化就近原则
 * @param  set_angle 即将要转到的角度
 * @param  last_angle 上一次的角度
 * @retval target_angle 输出正确就近角度值
 * @attention
 */
void calc_min_angle_round(fp32 *set_angle, fp32 *previous_angle, fp32 *angle_temp, fp32 *round_cnt, int8_t *direction_coefficient)
{
    for (int i = 0; i < 4; i++)
    {
        static fp32 round1 = 0.0f, round2 = 0.0f;
        /*****************舵轮就近原则*******************/
        if (fabs(set_angle[i] - previous_angle[i]) > 90.0f)
        {
            if (90.0f <= set_angle[i] && set_angle[i] <= 181.0f)
            {
                angle_temp[i] = set_angle[i] - 180.0f;
            }
            if (-181.0f <= set_angle[i] && set_angle[i] <= -90.0f)
            {
                angle_temp[i] = set_angle[i] + 180.0f;
            }
        }
        else
        {
            angle_temp[i] = set_angle[i];
        }

        /*****************舵轮记圈*******************/
        if ((set_angle[i] + angle_temp[i]) - previous_angle[i] > 260)
        {
            
						(*round_cnt)--;
        }
        else if ((set_angle[i] + angle_temp[i]) - previous_angle[i] < -260)
        {
            (*round_cnt)++;
        }

        /*****************舵轮记圈计算*******************/
        round1 = fabs(previous_angle[i] - angle_temp[i]);
        round2 = fabs(set_angle[i] - previous_angle[i]);

        if (round1 >= round2)
        {
            angle_temp[i] = 0.00f;
            *direction_coefficient = 1.00f;
        }
        if (round1 < round2)
        {
            set_angle[i] = 0.00f;
            *direction_coefficient = -1.00f;
        }
        previous_angle[i] = set_angle[i] + angle_temp[i];
    }
}

/**
 * @brief  角度转换为弧度
 * @param  deg 角度值
 * @retval rad 弧度值
 * @attention
 */
fp32 deg2rad(fp32 deg)
{
    fp32 rad;
    rad = deg * (PI / 180.0f);
    return rad;
}

/**
 * @brief  弧度转换为角度
 * @param  rad 弧度值
 * @retval deg 角度值
 * @attention
 */
fp32 rad2deg(fp32 rad)
{
    fp32 deg;
    deg = rad * (180.0f / PI);
    return deg;
}

/**
 * @brief  优化舵轮转动角度，就近原则
 * @param  set_angle 即将要转到的角度
 * @param  last_angle 上一次的角度
 * @retval target_angle 输出正确就近角度值
 * @attention
 */
/* void calc_min_angle(fp32* set_angle,fp32* last_angle,fp32* target_angle)
{
    fp32 temp_angle;
    if(*set_angle > 91.0f)
    {
        temp_angle = *set_angle - 180.0f;
    }
    else if(*set_angle < -89.0f)
    {
        temp_angle = *set_angle + 180.0f;
    }
    else
    {
        temp_angle = *set_angle;
    }

    if(fabs(temp_angle - *last_angle) > 90.0f)
    {
        if(-89.0f < temp_angle && temp_angle < 1.0f)
        {
            *target_angle = temp_angle + 180.0f;
        }
        else if(1.0f < temp_angle < 91.0f)
        {
            *target_angle = temp_angle -180.0f;
        }
    }
    else
    {
        *target_angle = temp_angle;
    }
} */
