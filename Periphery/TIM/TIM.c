#include "TIM.h"

/**********************************
***********************************
基本定时器TIME3初始化  
***********************************
**********************************/
void TIM3_Configuration(unsigned int TIM_Period,unsigned int TIM_Prescaler)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  NVIC_InitTypeDef NVIC_InitStructure;
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	TIM_TimeBaseStruct.TIM_Period=TIM_Period;//初值
	TIM_TimeBaseStruct.TIM_Prescaler=TIM_Prescaler;//预分频
	TIM_TimeBaseStruct.TIM_ClockDivision=0;
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上

	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStruct); 
	TIM_ClearFlag(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}
/**********************************
***********************************
基本定时器TIME5初始化  
***********************************
**********************************/
void TIM2_Configuration(unsigned int TIM_Period,unsigned int TIM_Prescaler)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  NVIC_InitTypeDef NVIC_InitStructure;
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	TIM_TimeBaseStruct.TIM_Period=TIM_Period;//初值
	TIM_TimeBaseStruct.TIM_Prescaler=TIM_Prescaler;//预分频
	TIM_TimeBaseStruct.TIM_ClockDivision=0;
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上

	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruct); 
	TIM_ClearFlag(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}



