#include "WiFi.h"

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

u8 WiFi_RX_BUF[WiFi_REC_LEN];     //���ջ���,���WiFi_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 WiFi_RX_STA=0;       //����״̬���	  
u16 WiFi_RX_Len=0;//WiFi�������ݳ���
/**********************************
***********************************
WiFi��ʼ��  
***********************************
**********************************/
void WiFi_Init(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

/**********************************
***********************************
WiFi�������� 
***********************************
**********************************/
void WiFi_Send_Command(const u8 *com,u8 endflag)
{
  while(*com!=0)
  {
		USART_ClearFlag(USART1,USART_FLAG_TC);
		USART_SendData(USART1,(*com));
		while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
		com++;
  }
	if(endflag==1)
	{
		USART_ClearFlag(USART1,USART_FLAG_TC);
		USART_SendData(USART1,0x0D);
		while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
		USART_ClearFlag(USART1,USART_FLAG_TC);
		USART_SendData(USART1,0x0A);
		while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
	}
}

/**********************************
***********************************
WiFi�������� 
***********************************
**********************************/
void WiFi_Send_Data(u8 *data, u32 len)
{
	u32 count;
  for(count=0;count<len;count++)
	{
		USART_ClearFlag(USART1,USART_FLAG_TC);
		USART_SendData(USART1,(*data));
		while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
		data++;
	}
}

/**********************************
***********************************
����WiFi�ȵ�
***********************************
**********************************/
void Connect_WiFi(void)
{
	u32 tcp_len=0;
	u32 count=0;
	
	WiFi_Send_Command("+++",0);//WiFi��ʼ��ֹͣ����
	delay_ms(1500);
	WiFi_Send_Command("AT+RST",1);//WiFi��ʼ����λ
	delay_ms(1500);delay_ms(1500);
	
	while(1)
	{
		WiFi_Send_Command("AT+CIPSTATUS",1);
		delay_ms(1000);
		count++;
		if(count>15)
		{
			count=0;
			WiFi_Send_Command("+++",0);//WiFi��ʼ��ֹͣ����
			delay_ms(1500);
			WiFi_Send_Command("AT+RST",1);//WiFi��ʼ����λ
			delay_ms(1500);delay_ms(1500);		
		}
		if(WiFi_RX_STA&0x8000)
		{
			tcp_len=WiFi_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			if(WiFi_RX_BUF[tcp_len-1]==0x32)
			{
				WiFi_RX_STA=0;
				break;
			}
			WiFi_RX_STA=0;
		}
	}
	delay_ms(1000);
	
	WiFi_Send_Command("AT+CIPSTART=\"TCP\",\"115.29.109.104\",6534",1);//���ӷ�����
	delay_ms(1000);
	
	WiFi_Send_Command("AT+CIPMODE=1",1);//ʹ��͸��ģʽ
	delay_ms(1000);
	
	WiFi_Send_Command("AT+CIPSEND",1);//��ʼ��������
  delay_ms(1000);
	
  WiFi_Send_Command("CONNECTED",0);//����λ������������
	WiFi_RX_STA=0;//�������״̬
}

/**********************************
***********************************
�жϽ�������
***********************************
**********************************/
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((WiFi_RX_STA&0x8000)==0)//����δ���
		{
			if(WiFi_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res==0x0A||Res=='n')
					WiFi_RX_STA|=0x8000;	//���������	
				else
          WiFi_RX_STA=0;//���մ���,���¿�ʼ				
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0D||Res=='\\')
				{
					WiFi_RX_STA|=0x4000;
				}
				else
				{
					WiFi_RX_BUF[WiFi_RX_STA&0X3FFF]=Res ;
					WiFi_RX_STA++;
					if(WiFi_RX_STA>(WiFi_REC_LEN-1))
						WiFi_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 								 
} 

