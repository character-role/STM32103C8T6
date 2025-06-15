#ifndef  __WiFi_H
#define __WiFi_H

#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stdio.h"
#include "common.h"

#define WiFi_REC_LEN 1024

extern u8  WiFi_RX_BUF[WiFi_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 WiFi_RX_STA;         		//接收状态标记	
extern u16 WiFi_RX_Len;//WiFi接收数据长度

void WiFi_Init(u32 bound);
void Wifi_Send_Command(const u8 *com);
void WiFi_Send_Data(u8 *data, u32 len);
void Connect_WiFi(void);
void WiFi_Send_Info(u8 *phone,u32 number);

#endif 
