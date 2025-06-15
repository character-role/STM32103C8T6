#ifndef __TIM_H
#define	__TIM_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#define Delay_us 35
#define Delay_ms 35999

void TIM3_Configuration(unsigned int TIM_Period,unsigned int TIM_Prescaler);
void TIM2_Configuration(unsigned int TIM_Period,unsigned int TIM_Prescaler);
	
#endif 
