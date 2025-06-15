#include "WiFi.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

u8 WiFi_RX_BUF[WiFi_REC_LEN];     //接收缓冲,最大WiFi_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 WiFi_RX_STA=0;       //接收状态标记	  
u16 WiFi_RX_Len=0;//WiFi接收数据长度
/**********************************
***********************************
WiFi初始化  
***********************************
**********************************/
void WiFi_Init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

/**********************************
***********************************
WiFi发送数据 
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
WiFi发送数据 
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
连接WiFi热点
***********************************
**********************************/
void Connect_WiFi(void)
{
	u32 tcp_len=0;
	u32 count=0;
	
	WiFi_Send_Command("+++",0);//WiFi初始化停止传输
	delay_ms(1500);
	WiFi_Send_Command("AT+RST",1);//WiFi初始化复位
	delay_ms(1500);delay_ms(1500);
	
	while(1)
	{
		WiFi_Send_Command("AT+CIPSTATUS",1);
		delay_ms(1000);
		count++;
		if(count>15)
		{
			count=0;
			WiFi_Send_Command("+++",0);//WiFi初始化停止传输
			delay_ms(1500);
			WiFi_Send_Command("AT+RST",1);//WiFi初始化复位
			delay_ms(1500);delay_ms(1500);		
		}
		if(WiFi_RX_STA&0x8000)
		{
			tcp_len=WiFi_RX_STA&0x3fff;//得到此次接收到的数据长度
			if(WiFi_RX_BUF[tcp_len-1]==0x32)
			{
				WiFi_RX_STA=0;
				break;
			}
			WiFi_RX_STA=0;
		}
	}
	delay_ms(1000);
	
	WiFi_Send_Command("AT+CIPSTART=\"TCP\",\"115.29.109.104\",6534",1);//连接服务器
	delay_ms(1000);
	
	WiFi_Send_Command("AT+CIPMODE=1",1);//使能透传模式
	delay_ms(1000);
	
	WiFi_Send_Command("AT+CIPSEND",1);//开始传输数据
  delay_ms(1000);
	
  WiFi_Send_Command("CONNECTED",0);//向上位机发送已连接
	WiFi_RX_STA=0;//清除接受状态
}

/**********************************
***********************************
中断接收数据
***********************************
**********************************/
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((WiFi_RX_STA&0x8000)==0)//接收未完成
		{
			if(WiFi_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res==0x0A||Res=='n')
					WiFi_RX_STA|=0x8000;	//接收完成了	
				else
          WiFi_RX_STA=0;//接收错误,重新开始				
			}
			else //还没收到0X0D
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
						WiFi_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 								 
} 

