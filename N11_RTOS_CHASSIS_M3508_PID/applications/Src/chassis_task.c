#include "chassis_task.h"

float Target_1,Target_2,Target_3,Target_4;
float Speed_Motor_Target_1,Speed_Motor_Target_2,Speed_Motor_Target_3,Speed_Motor_Target_4;
float Position_Motor_Target_1,Position_Motor_Target_2,Position_Motor_Target_3,Position_Motor_Target_4;

float Vcx,Vcy,Wcr;
float a,b;

extern RC_ctrl_t rc_ctrl;
extern motor_measure_t motor_can1[8];




void chassis_task(void const * argument)
{
  //wait a time 
  //空闲一段时间
  vTaskDelay(CHASSIS_TASK_INIT_TIME);
	
	//底盘初始化 
	PID_devices_Init();
	
  //make sure all chassis motor is online,
  //判断底盘电机是否都在线
  while (toe_is_error(CHASSIS_MOTOR1_TOE) || toe_is_error(CHASSIS_MOTOR2_TOE) || toe_is_error(CHASSIS_MOTOR3_TOE) || toe_is_error(CHASSIS_MOTOR4_TOE) || toe_is_error(DBUS_TOE))
  {
		//chassis task control time  2ms
		//底盘任务控制间隔 2ms
		vTaskDelay(CHASSIS_CONTROL_TIME_MS);
  }
  while(1)
  {
		//chassis data update
    //底盘数据更新
		chassis_feedback_update();
    //set chassis control set-point 
    //底盘控制量设置
    chassis_set_contorl();
    //chassis control pid calculate
		Speed_Motor_Target_1 = 1000;    //测试电机闭环是否可用的代码，正式使用时请注释该行代码
    //底盘控制PID计算
    chassis_control_loop();
		
		//make sure  one motor is online at least, so that the control CAN message can be received
    //确保至少一个电机在线， 这样CAN控制包可以被接收到
        if (!(toe_is_error(CHASSIS_MOTOR1_TOE) && toe_is_error(CHASSIS_MOTOR2_TOE) && toe_is_error(CHASSIS_MOTOR3_TOE) && toe_is_error(CHASSIS_MOTOR4_TOE)))
        {
            //when remote control is offline, chassis motor should receive zero current. 
            //当遥控器掉线的时候，发送给底盘电机零电流.
            if (toe_is_error(DBUS_TOE))
            {
                CAN1_CMD_1(0, 0, 0, 0);  //有松弛风险（待定）
            }
            else
            {
							//send control current
							//发送控制电流
							CAN1_CMD_1(Target_1,0,0,0);
            }
        }
		//os delay
    //系统延时
	  vTaskDelay(CHASSIS_CONTROL_TIME_MS);	
//    osDelay(2);
  }

}






	/**
  * @brief          chassis some measure data updata, such as motor speed, euler angle， robot speed
  * @param[out]     chassis_move_update: "chassis_move" valiable point
  * @retval         none
  */
/**
  * @brief          底盘测量数据更新，包括电机速度，欧拉角度，机器人速度
  * @param[out]     chassis_move_update:"chassis_move"变量指针.
  * @retval         none
  */
static void chassis_feedback_update(void)
{
//    if (chassis_move_update == NULL)
//    {
//        return;
//    }

//    uint8_t i = 0;
//    for (i = 0; i < 4; i++)
//    {
//        //update motor speed, accel is differential of speed PID
//        //更新电机速度，加速度是速度的PID微分
//        chassis_move_update->motor_chassis[i].speed = motor_chassis[i].chassis_motor_measure->speed_rpm;
//        chassis_move_update->motor_chassis[i].accel = chassis_move_update->motor_speed_pid[i].Dbuf[0] * CHASSIS_CONTROL_FREQUENCE;
//    }
}



	/**
  * @brief          set chassis control set-point, three movement control value is set by "chassis_behaviour_control_set".
  * @param[out]     chassis_move_update: "chassis_move" valiable point
  * @retval         none
  */
/**
  * @brief          设置底盘控制设置值, 三运动控制值是通过chassis_behaviour_control_set函数设置的
  * @param[out]     chassis_move_update:"chassis_move"变量指针.
  * @retval         none
  */
static void chassis_set_contorl(void)
{

//    if (chassis_move_control == NULL)
//    {
//        return;
//    }
		
		RC_speed_chassis_data();
		
		Speed_Motor_Target_1 = - Vcx + Vcy + Wcr * ( a + b ) ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		Speed_Motor_Target_2 =   Vcx + Vcy - Wcr * ( a + b ) ;
		Speed_Motor_Target_3 = - Vcx + Vcy - Wcr * ( a + b ) ;
		Speed_Motor_Target_4 =   Vcx + Vcy + Wcr * ( a + b ) ;
}


/*
麦轮速度解析式

V1 = - Vcx + Vcy + Wcr * ( a + b )        (c是Chassis)
V2 =   Vcx + Vcy - Wcr * ( a + b )
V3 = - Vcx + Vcy - Wcr * ( a + b )
V4 =   Vcx + Vcy + Wcr * ( a + b )

*/



/**
  * @brief          control loop, according to control set-point, calculate motor current, 
  *                 motor current will be sentto motor
  * @param[out]     chassis_move_control_loop: "chassis_move" valiable point
  * @retval         none
  */
/**
  * @brief          控制循环，根据控制设定值，计算电机电流值，进行控制
  * @param[out]     chassis_move_control_loop:"chassis_move"变量指针.
  * @retval         none
  */
static void chassis_control_loop(void)
{
	Target_1 = PID_velocity_realize_1(Speed_Motor_Target_1,1);
	Target_2 = PID_velocity_realize_1(Speed_Motor_Target_2,2);
	Target_3 = PID_velocity_realize_1(Speed_Motor_Target_3,3);
	Target_4 = PID_velocity_realize_1(Speed_Motor_Target_4,4);
}




void RC_speed_chassis_data(void)
{
	Vcx = 0.707106f * rc_ctrl.rc.ch[2];
	Vcy = 0.707106f * rc_ctrl.rc.ch[3];
	Wcr = 0.707106f * rc_ctrl.rc.ch[4];
	
	Vcx *= 17;
	Vcy *= 17;
	Wcr *= -17;
	
	a =  0.085;           //轮子到底盘中心的 X轴距离
	b =  0.065;					 //轮子到底盘中心的 Y轴距离
}












