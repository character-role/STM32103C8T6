#include "AD.h"

void Delay(u32 Delayms);
/**********************************
***********************************
ADC初始化  
***********************************
**********************************/
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;			

	//ADC采集口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_None); 
	ADC_InjectedSequencerLengthConfig(ADC1, 1);//注入组个数
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_7 , 1, ADC_SampleTime_239Cycles5);
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_1,0);
	
	ADC_Cmd(ADC1,ENABLE);
}

/**********************************
***********************************
采集AD值存入数组  
注：adc取ADC_1，ADC_2
***********************************
**********************************/
u16 Get_ADC()
{	
	u16 count=0;
	u32 Data_ADC=0; 
	
	for(count=0;count<10;count++)
	{
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)){}	
	  Data_ADC=Data_ADC+ADC_GetConversionValue(ADC1);
	}
	
	return Data_ADC/10;
}






