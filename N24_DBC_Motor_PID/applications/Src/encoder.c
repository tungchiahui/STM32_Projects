#include "encoder.h"

volatile int32_t motor_encoder_count;   //溢出次数

Encoder_TypeDef encoder_data;

Motor_TypeDef motor_data;


/**
 * @brief       定时器更新中断回调函数
 * @param        htim:定时器句柄指针
 * @note        此函数会被定时器中断函数共同调用的
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3) == 1)    /* 判断CR1的DIR位 */
		{
			motor_encoder_count--;                           /* DIR位为1，也就是递减计数 */
		}
		else
		{
			motor_encoder_count++;                          /* DIR位为0，也就是递增计数 */
		}
	}
	if(htim->Instance == TIM7)
	{
		encoder_data.encoder_now = get_encoder_value();                             /* 获取编码器值，用于计算速度 */
		
    motor_message_filtering(&encoder_data,&motor_data,1);                       /* 中位平均值滤除编码器抖动数据，50ms计算一次速度*/
	}
	if(htim->Instance == TIM6)
	{
		
	}
}

/**
 * @brief       获取编码器的值
 * @param       无
 * @retval      编码器值
 */
int32_t get_encoder_value(void)
{
	int32_t encoder_value;
	encoder_value = __HAL_TIM_GET_COUNTER(&htim3) + motor_encoder_count * 65536;       /* 当前计数值+之前累计编码器的值=总的编码器值 */
	
	return encoder_value;
}


/**
 * @brief       电机速度计算
 * @param       encode_now：当前编码器总的计数值
 *              ms：计算速度的间隔，中断1ms进入一次，例如ms = 5即5ms计算一次速度
 * @retval      无
 */
void motor_message_filtering(Encoder_TypeDef *encoder,Motor_TypeDef *motor,uint8_t ms)
{
		//低通滤波系数
		fp32 q = 0.9;
	
	   //冒泡排序法
    uint8_t i = 0, j = 0;
		static uint8_t bubble_sort_number = 0;
		//缓存，充当冒泡排序法的交换缓存和冒泡排序法处理后的速度总和缓存
    fp32 temp = 0.0f;
	
    static uint8_t time_count = 0;
    static fp32 speed_arr[10] = {0.0f};                     /* 存储速度进行滤波运算 */

    if (time_count == ms)                                     /* 计算一次速度 */
    {
        /* 计算电机转速 
           第一步 ：计算ms毫秒内计数变化量
           第二步 ；计算1min内计数变化量：g_encode.speed * ((1000 / ms) * 60 ，
           第三步 ：除以编码器旋转一圈的计数次数（倍频倍数 * 编码器分辨率）
           第四步 ：除以减速比即可得出电机转速
        */
			
        encoder->encoder_delta = (encoder->encoder_now - encoder->encoder_pre);    /* 计算编码器计数值的变化量 */
			if(ABS(encoder->encoder_delta) >= 60000.0f)
			{
				encoder->encoder_delta = 0.0f;
			}
        encoder->encoder_delta_sum += encoder->encoder_delta;
        speed_arr[bubble_sort_number++] = (fp32)(encoder->encoder_delta * ((1000 / ms) * 60.0) / (FRE_DOU_RATIO) / (PULSE_PER_REVOLUTION));    /* 保存电机转速 */
        
        encoder->encoder_pre = encoder->encoder_now;          /* 保存当前编码器的值 */

        /* 累计10次速度值，后续进行滤波*/
        if (bubble_sort_number == 10)
        {
            for (i = 10; i >= 1; i--)                       /* 冒泡排序*/
            {
                for (j = 0; j < (i - 1); j++) 
                {
                    if (speed_arr[j] > speed_arr[j + 1])    /* 数值比较 */
                    { 
                        temp = speed_arr[j];                /* 数值换位 */
                        speed_arr[j] = speed_arr[j + 1];
                        speed_arr[j + 1] = temp;
                    }
                }
            }
						
            temp = 0.0;
            
            for (i = 2; i < 8; i++)                         /* 去除两边高低数据 */
            {
                temp += speed_arr[i];                       /* 将中间数值累加 */
            }
            
            temp = (fp32)(temp / 6);                       /*求速度平均值*/
            
            /* 一阶低通滤波
             * 公式为：Y(n)= qX(n) + (1-q)Y(n-1)
             * 其中X(n)为本次采样值；Y(n-1)为上次滤波输出值；Y(n)为本次滤波输出值，q为滤波系数
             * q值越小则上一次输出对本次输出影响越大，整体曲线越平稳，但是对于速度变化的响应也会越慢
             */
            motor->rotor_speed = (fp32)((fp32)(q * temp) + (motor->rotor_speed * (fp32)(1.0f-q)) );  //通过低通滤波计算转子速度
						motor->motor_speed = motor->rotor_speed / (REDUCTION_RATIO);     //计算减速后电机转轴的速度
						
						motor->rotor_position =  encoder->encoder_delta_sum;//电机位置
						motor->motor_position =  motor->rotor_position / (REDUCTION_RATIO);  //计算减速后电机转轴的位置
						
						motor->rotor_round_cnt = motor->rotor_position / (FRE_DOU_RATIO) / (PULSE_PER_REVOLUTION);
						motor->motor_round_cnt = motor->rotor_round_cnt / (REDUCTION_RATIO);  //计算减速后电机转轴的圈数
						
						

						
            bubble_sort_number = 0;
        }
        time_count = 0;
    }
    time_count ++;
}



