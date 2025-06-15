#ifndef __IO_H
#define	__IO_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


/******************
控制和中断按键
******************/
#define right GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)
#define left  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)
#define Stop  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)


void GPIO_Config(void);

#endif 

