#ifndef __TASK_DATA_PACK_H_
#define __TASK_DATA_PACK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

void SendByte(uint8_t data);
void Send(const uint8_t *data, uint8_t len);
uint16_t CRC16_Check(const uint8_t *data, uint8_t len);
uint8_t __SUMCRC(uint8_t *puchMsg, uint16_t usDataLen);
void Send_Cmd_Data(uint8_t cmd, const uint8_t *datas, uint8_t len);
void Data_Analysis(uint8_t cmd, const uint8_t *datas, uint8_t len);
bool Receive(uint8_t rx_buffer[]);



#ifdef __cplusplus
}
#endif

#endif

