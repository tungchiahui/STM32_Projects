#include "Task_Data_pack.h"
/*
需要发送变长的数据帧。其他场景可在此基础上进行简化。为了发送变长数据帧，使用 帧头+帧长+命令字节+数据字节+校验字节+帧尾 的格式对数据包进行打包，其实帧头和帧长已经足够解决帧对齐问题，帧尾可以去掉，为了适应更复杂的情况，这里保留帧尾。

帧头：使用 0xA5,0x5A两个字节作为帧头，二进制位0与1的个数相同，分布均匀不易出错。
帧长：根据数据帧实际长度确定帧长字节，这里只使用1个字节，故帧长字节最大为255,为提高利用率，规定帧长字节描述的是数据字节的长度，故应重新命名为数据长度字节。
命令字节：利用命令字节指定数据字节的功能，例如命令字节为1表示传输温度，为2表示传输湿度等，1字节命令+n字节数据是工业中比较常用的一种格式。
数据字节：数据字节长度可变，帧长字节为0，表示没有数据，帧长字节为255，表示有255字节数据。
校验字节：比较简单的一种校验方式为和校验，即把校验字节前的所以字节求和，最后保留低8位作为校验字节。在MODBUS协议中常用CRC16循环冗余校验方式，将校验字节前的所以字节加入计算，得到两字节CRC16校验码，本文采用此方式。
帧尾：与帧头相似，这里使用0xFF作为帧尾。

帧头		数据长度	命令		数据		CRC16校验	帧尾
A5 5A		XX		 XX		…XX 		…XX XX		 FF
2字节		1字节		1字节		n字节 …		 2字节		1字节
*/

#define ICC_UART USART1
#define DATA_LENTH 20

uint8_t rx_buffer[DATA_LENTH];
int16_t opencv_tracking_offset;



/**
 * @brief  串口接收回调函数
 * @param  huart 句柄
 * @retval void
 * @attention  弱函数
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == ICC_UART)
	{
		Receive(rx_buffer);
	}
}


/**
 * @brief  发送
 * @param  data 单个数据
 * @retval void
 * @attention
 */
void SendByte(uint8_t data)
{
    while ((ICC_UART->SR & 0X40) == 0);     //等待上一个字符发送完成
    ICC_UART->DR = data;                    //将要发送的字符 ch 写入到DR寄存器
}

/**
 * @brief  发送
 * @param  data 数据数组首地址
 * @param  len 数据字节长度
 * @retval void
 * @attention
 */
void Send(const uint8_t *data, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++)
	{
		SendByte(data[i]); // 发送一个字节
	}
}

/**
 * @brief  16位循环冗余校验
 * @param  data 数据数组首地址
 * @param  len 数据字节长度
 * @retval void
 * @attention
 */
uint16_t CRC16_Check(const uint8_t *data, uint8_t len)
{
	uint16_t CRC16 = 0xFFFF;
	uint8_t state, i, j;
	for (i = 0; i < len; i++)
	{
		CRC16 ^= data[i];
		for (j = 0; j < 8; j++)
		{
			state = CRC16 & 0x01;
			CRC16 >>= 1;
			if (state)
			{
				CRC16 ^= 0xA001;
			}
		}
	}
	return CRC16;
}

/**
 * @brief  求和效验
 * @param  puchMsg 数据数组首地址
 * @param  usDataLen 数据字节长度
 * @retval void
 * @attention
 */
uint8_t __SUMCRC(uint8_t *puchMsg, uint16_t usDataLen)
{
    int16_t i = 0;
		uint8_t uchSUMCRC = 0x00;
    for (; i < usDataLen; i++)
    {
			uchSUMCRC += puchMsg[i];
    }
    return uchSUMCRC;
}



/**
 * @brief  发送数据包
 * @param  cmd 类型标识符
 * @param  datas 数据数组首地址
 * @param  len 数据字节长度
 * @retval void
 * @attention
 */
void Send_Cmd_Data(uint8_t cmd, const uint8_t *datas, uint8_t len)
{
	uint8_t buf[300], i, cnt = 0;
	uint16_t crc16;
	buf[cnt++] = 0xA5;
	buf[cnt++] = 0x5A;
	buf[cnt++] = len;
	buf[cnt++] = cmd;
	for (i = 0; i < len; i++)
	{
		buf[cnt++] = datas[i];
	}
	crc16 = CRC16_Check(buf, len + 4);
	buf[cnt++] = crc16 >> 8;   //高八位
	buf[cnt++] = crc16 & 0xFF;
	buf[cnt++] = 0xFF;
	Send(buf, cnt); // 调用数据帧发送函数将打包好的数据帧发送出去
}

/**
 * @brief  数据解析
 * @param  cmd 类型标识符
 * @param  datas 数据数组首地址
 * @param  len 数据字节长度
 * @retval void
 * @attention
 */
void Data_Analysis(uint8_t cmd, const uint8_t *datas, uint8_t len)
{
	opencv_tracking_offset = (datas[0] << 8 | datas[1]);
}

/**
 * @brief  接收数据包
 * @param  rx_buffer 数据数组首地址
 * @retval 是否接收成功
 * @attention 该函数会调用Data_Analysis()函数
 */
bool Receive(uint8_t rx_buffer[])
{
	uint8_t rx_buffer_pre;
	static uint16_t Real_CRC16_;
	static uint16_t Receive_CRC16_;
	static uint16_t rx_cnt;
	static uint8_t len_; 
	static uint8_t cmd_;
	for(int i = 0;i < DATA_LENTH;i++)
	{
		if(rx_buffer_pre == 0xA5 && rx_buffer[i] == 0x5A)
		{
			rx_cnt = i - 1;
			goto DATA_HEADER_CORRECT;
		}
		rx_buffer_pre = rx_buffer[i];
	}
	DATA_HEADER_CORRECT:
	if(rx_buffer[rx_cnt] != 0xA5)
	{
		return false;
	}
	if(rx_buffer[rx_cnt + 1] != 0x5A)
	{
		return false;
	}
	else
	{
		len_ = rx_buffer[rx_cnt + 2];
		cmd_ = rx_buffer[rx_cnt + 3];
		Real_CRC16_ = CRC16_Check(&(rx_buffer[rx_cnt]),(len_ + 4));
		Receive_CRC16_ = ((rx_buffer[rx_cnt + 4 + len_] << 8) | rx_buffer[rx_cnt + 4 + len_ + 1]);
		if(Real_CRC16_ != Receive_CRC16_)
		{
			return false;
		}
		Data_Analysis(cmd_,&rx_buffer[rx_cnt + 4],len_);
		return true;
	}
}
