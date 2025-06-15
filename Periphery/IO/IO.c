#include "IO.h"

/**********************************
***********************************
GPIO初始化  
***********************************
**********************************/
void GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;
		
		//WiFi指示灯//PC13
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
}


