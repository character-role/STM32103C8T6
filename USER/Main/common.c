#include "common.h"

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

/**********************************
***********************************
����ȡ����ƽ��ֵ    
ע��adcȡADC_1��ADC_2
    star���鿪ʼλ
    end�������λ
***********************************
**********************************/
u8 average(u8 *adc,u16 star,u16 end)
{
	u16 temp;
  u16 sum=0;
	for(temp=star;temp<end;temp++)
	  sum=sum+adc[temp];
  return (sum/(end-star));
}

/**********************************
***********************************
����ȡ����������  
ע��adcȡADC_1��ADC_2
    star���鿪ʼλ
    end�������λ
***********************************
**********************************/
void sort(u8 *adc,u16 star,u16 end)
{
	u16 i,j,t;
	for(j=star;j<end-1;j++)
	{
		for(i=j+1;i<end;i++)
		{
			if(adc[j]>adc[i])
			{
			t=adc[j];
			adc[j]=adc[i];
			adc[i]=t;
			}
		}
	}
}
/**********************************
***********************************
�����λ��ʾ����С   
***********************************
**********************************/
void Display2_1(u32 value,u8 *BUFF)
{
	if(value<1000)
	{
		*(BUFF+0)=value/100+0x30;
		*(BUFF+1)=value%100/10+0x30;
		*(BUFF+2)='.';
		*(BUFF+3)= value%100%10+0x30;
		*(BUFF+4)=0;
	}
	else
	{
		*(BUFF+0)='9';
		*(BUFF+1)='9';
		*(BUFF+2)='.';
		*(BUFF+3)= '9';
		*(BUFF+4)=0;		
	}
}
/**********************************
***********************************
�����λ��ʾ����С   
***********************************
**********************************/
void Display2(u32 value,u8 *BUFF)
{
	if(value<100)
	{
		*(BUFF+0)=value/10+0x30;
		*(BUFF+1)=value%10+0x30;
		*(BUFF+2)=0;
	}
	else
	{
		*(BUFF+0)='9';
		*(BUFF+1)='9';
		*(BUFF+2)=0;	
	}
}
/**********************************
***********************************
�����λ��ʾ����С   
***********************************
**********************************/
void Display3(u32 value,u8 *BUFF)
{
	if(value<1000)
	{
		*(BUFF+0)=value/100+0x30;
		*(BUFF+1)=value%100/10+0x30;
		*(BUFF+2)=value%100%10+0x30;
		*(BUFF+3)=0;
	}
	else
	{
		*(BUFF+0)='9';
		*(BUFF+1)='9';
		*(BUFF+2)='9';
		*(BUFF+3)=0;	
	}
}
/**********************************
***********************************
�����λ��ʾ����С   
***********************************
**********************************/
void Display4(u32 value,u8 *BUFF)
{
	if(value<10000)
	{
		*(BUFF+0)=value/1000+0x30;
		*(BUFF+1)=value%1000/100+0x30;
		*(BUFF+2)=value%1000%100/10+0x30;
		*(BUFF+3)=value%1000%100%10+0x30;
		*(BUFF+4)=0;
	}
	else
	{
		*(BUFF+0)='9';
		*(BUFF+1)='9';
		*(BUFF+2)='9';
		*(BUFF+3)='9';
    *(BUFF+4)=0;		
	}
}
/**********************************
***********************************
�����λ��ʾ����С 
***********************************
**********************************/
void Display1_4(u32 value,u8 *BUFF)
{
	if(value<100000)
	{
		*(BUFF+0)=value/10000+0x30;
		*(BUFF+1)='.';
		*(BUFF+2)=value%10000/1000+0x30;
		*(BUFF+3)=value%10000%1000/100+0x30;
		*(BUFF+4)=value%10000%1000%100/10+0x30;
		*(BUFF+5)=value%10000%1000%100%10+0x30;
		*(BUFF+6)=0;
	}
	else
	{
		*(BUFF+0)='9';
		*(BUFF+1)='.';
		*(BUFF+2)='9';
		*(BUFF+3)='9';
		*(BUFF+4)='9';
		*(BUFF+5)='9';
		*(BUFF+6)=0;		
	}
}

/**********************************
***********************************
��ʱ     
***********************************
**********************************/
void delay(u32 nCount)
{
	u32 count;
	for(;nCount>0;nCount--)
	  for(count=1000;count>0;count--);
}

/********************************************************************
				��ʱus,ms
********************************************************************/
void delayus(unsigned int time)
{
 unsigned int i=0;
	while(time--)
	{
		i=10;
		while(i--);		
	}
}

void delayms(unsigned int time)
{
 unsigned int i=0;
	while(time--)
	{
		i=12000;
		while(i--);		
	}
}


void delay_init()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8  
	fac_ms=(u16)fac_us*1000;					
}	 
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	  	    
} 


/**********************************
***********************************
����������ֵx^y
***********************************
**********************************/	
double Square(double x,int y)
{
	u32 count=0;
	double value=1;
	
	if(y<=0)
		y=0;
  for(count=0;count<y;count++)
	{
	  value=value*x;
	}
	
	return value;
}






