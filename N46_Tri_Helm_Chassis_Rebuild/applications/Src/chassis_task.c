#include "chassis_task.h"
#include "chassis_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

eChassisAction actChassis = CHASSIS_NORMAL; //底盘默认遥控行走

Chassis_Orientation_t chassisOri;

void chassis_task(void const * argument)
{
	// wait a time
	//空闲一段时间
	vTaskDelay(CHASSIS_TASK_INIT_TIME);

	//PID parameter initialization
	//PID参数初始化
	PID_devices_Init();

	while(true)
	{
		//set chassis motion mode
		//设定底盘运动模式
		Remote_Control_Chassis_Set_Mode(&actChassis);
		
		//chassis data update
		//底盘数据更新
		Remote_Control_Chassis_Mode(&actChassis,&chassisOri);
		
		//chassis pose analysis
		//底盘姿态分析
		Robot_Orientation_Analysis();
		
		//calculate the speed of the chassis
		//底盘速度解算
		Chassis_Sports_Calc(&chassisOri);

		//chassis control pid calculate
		//底盘控制PID计算与数据发送
		Chassis_Loop_Out();
		
		//chassis task control time
		//底盘任务控制间隔
		osDelay(CHASSIS_CONTROL_TIME_MS);
	}
}




#ifdef __cplusplus
}
#endif
