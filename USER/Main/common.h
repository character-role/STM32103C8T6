#ifndef __common_H
#define __common_H

#include "stm32f10x.h"
#include "misc.h"

u8 average(u8 *adc,u16 star,u16 end);
void sort(u8 *adc,u16 star,u16 end);
void Display2_1(u32 value,u8 *BUFF);
void Display2(u32 value,u8 *BUFF);
void Display3(u32 value,u8 *BUFF);
void Display4(u32 value,u8 *BUFF);
void Display1_4(u32 value,u8 *BUFF);
void delay(u32 nCount);
void delayus(unsigned int time);
void delayms(unsigned int time);
void delay_init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);
double Square(double x,int y);
#endif 
