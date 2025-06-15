#include "SMotor.h"

u8 Step=0;
u8 Direction=0;

/**********************************************************************							
步进电机引脚初始化													
***********************************************************************/
void SMotor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(STEP1_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin=STEP1_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(STEP1_GPIO,&GPIO_InitStructure);
	GPIO_WriteBit(STEP1_GPIO,STEP1_PIN,Bit_RESET);//初始化

	RCC_APB2PeriphClockCmd(STEP2_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin=STEP2_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(STEP2_GPIO,&GPIO_InitStructure);
	GPIO_WriteBit(STEP2_GPIO,STEP2_PIN,Bit_RESET);//初始化

	RCC_APB2PeriphClockCmd(STEP3_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin=STEP3_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(STEP3_GPIO,&GPIO_InitStructure);
	GPIO_WriteBit(STEP3_GPIO,STEP3_PIN,Bit_RESET);//初始化
	
	RCC_APB2PeriphClockCmd(STEP4_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin=STEP4_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(STEP4_GPIO,&GPIO_InitStructure);
	GPIO_WriteBit(STEP4_GPIO,STEP4_PIN,Bit_RESET);//初始化
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	
	TIM_TimeBaseStruct.TIM_Period=2000;//初值
	TIM_TimeBaseStruct.TIM_Prescaler=35;//预分频
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

/**********************************************************************							
步进电机驱动														
***********************************************************************/
void SMotor_FFW(u8 direction)
{ 
	if(direction==0) 
	{
    Direction=0;
		TIM_Cmd(TIM3,ENABLE);
	}
	else if(direction==1) 
	{
    Direction=1;
		TIM_Cmd(TIM3,ENABLE);
	}
	else
	{
	  TIM_Cmd(TIM3,DISABLE);
	}
}

/******************************************************
*
*	             定时器中断控制步进电机转动
*
********************************************************/
void TIM3_IRQHandler(void)
{
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
	Step++;
	if(Step>=8)
		Step=0;
	
	if(Direction==0)
	{
	  if(Step==0)
		{
      STEP1_ON;STEP2_OFF;STEP3_OFF;STEP4_OFF;
		}
		else if(Step==1)
		{
			STEP1_ON;STEP2_ON;STEP3_OFF;STEP4_OFF;
		}
		else if(Step==2)
		{
			STEP1_OFF;STEP2_ON;STEP3_OFF;STEP4_OFF;
		}
		else if(Step==3)
		{
			STEP1_OFF;STEP2_ON;STEP3_ON;STEP4_OFF;
		}
		else if(Step==4)
		{
			STEP1_OFF;STEP2_OFF;STEP3_ON;STEP4_OFF;
		}
		else if(Step==5)
		{
			STEP1_OFF;STEP2_OFF;STEP3_ON;STEP4_ON;
		}
		else if(Step==6)
		{
			STEP1_OFF;STEP2_OFF;STEP3_OFF;STEP4_ON;
		}
		else if(Step==7)
		{
			STEP1_ON;STEP2_OFF;STEP3_OFF;STEP4_ON;	
		}			
	}
	else 
	{
		if(Step==0)
		{
			STEP4_ON;STEP3_OFF;STEP2_OFF;STEP1_OFF;
		}
		else if(Step==1)
		{
			STEP4_ON;STEP3_ON;STEP2_OFF;STEP1_OFF;
		}
		else if(Step==2)
		{
			STEP4_OFF;STEP3_ON;STEP2_OFF;STEP1_OFF;
		}
		else if(Step==3)
		{
			STEP4_OFF;STEP3_ON;STEP2_ON;STEP1_OFF;
		}
		else if(Step==4)
		{
			STEP4_OFF;STEP3_OFF;STEP2_ON;STEP1_OFF;
		}
		else if(Step==5)
		{
			STEP4_OFF;STEP3_OFF;STEP2_ON;STEP1_ON;
		}
		else if(Step==6)
		{
			STEP4_OFF;STEP3_OFF;STEP2_OFF;STEP1_ON;
		}
		else if(Step==7)
		{
			STEP4_ON;STEP3_OFF;STEP2_OFF;STEP1_ON;
		}
	}
}
