#include "EXTI.h"

/**********************************
***********************************
EXTI初始化   作者：刘星洋
***********************************
**********************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

//	EXTI_ClearITPendingBit(EXTI_Line4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource4); 

	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;

	EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Exti_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

///**********************************
//***********************************
//外部中断函数    作者：陈开放
//***********************************
//**********************************/
//void EXTI9_5_IRQHandler(void)
//{

//}
