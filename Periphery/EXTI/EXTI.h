#ifndef __EXTI_H
#define	__EXTI_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "IO.h"
#include "TIM.h"

extern u8 star_flag;

void GPIO_Configuration(void);
void RCC_Configuration(void);
void EXTI_Configuration(void);
void NVIC_Exti_Configuration(void);

#endif 
