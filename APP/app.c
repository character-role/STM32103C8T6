#include "Header.h"

u8 SendBuff[15]={'A','7',0};//上报上位机缓存
u16 IRQ_Time=0;//定时器中断计数
int WindowSet=0;//晾衣架设置
int WindowNow=0;//晾衣架状态
u8  Window_JumpEdge=0;//控制晾衣架单位时间
u16 Light=0;//灯光亮度变量
short Temp=0, Humi=0;//温湿度变量
u8  Mode=0;//模式，默认为自动模式
u8  Send_JumpEdge=0;//发送数据时间记录
u8  Dis[10]={0};

int main()	  
{
	SystemInit(0X00000);	//参数为向量表的偏移地址
	delay_init();//初始化延时
	GPIO_Config();//初始化IO口
	ADC_Configuration();//初始化ADC
	SMotor_Init();//步进电机初始化
	WiFi_Init(115200);//初始化WiFi模块
	SysTick_Init();//初始化多功能定时器
	DHT11_Init();//初始化温湿度
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
	Connect_WiFi();//连接WiFi热点
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	
	while(1)
  {	
    DHT11_Read_Data(&Temp, &Humi);
		Display2_1(Temp,Dis);//计算温度
		SendBuff[2]=Dis[0];
		SendBuff[3]=Dis[1];
		SendBuff[4]=Dis[2];
		SendBuff[5]=Dis[3];
		
		Display2_1(Humi,Dis);//计算湿度
		SendBuff[7]=Dis[0];
		SendBuff[8]=Dis[1];
		SendBuff[9]=Dis[2];
		SendBuff[10]=Dis[3];		
		
		ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		Light=(4096-ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1))/4.2;//读取光强度值
    Display3(Light,Dis);//计算光照强度
		SendBuff[11]=Dis[0];
		SendBuff[12]=Dis[1];
		SendBuff[13]=Dis[2];
		
		if(Mode==0)//自动模式
		{
			if(Light>700)//光强足够
			{
				WindowSet=20;
				SendBuff[14]='1';
			}	
			else
			{
				WindowSet=0;
				SendBuff[14]='0';
			}
		}
		
		if(SysTick_JumpEdge(&Send_JumpEdge, &SysTickBit.Time1s))//数据上报上位机
    {
			WiFi_Send_Data(SendBuff,15);
		}

		if(SysTick_JumpEdge(&Window_JumpEdge, &SysTickBit.Time1s))//控制晾衣架开关
		{
		  if(WindowSet-WindowNow>0)
			{
			  WindowNow++;
				SMotor_FFW(0);//步进电机正转
			}
			else if(WindowSet-WindowNow<0)
		  {
			  WindowNow--;
				SMotor_FFW(1);//步进电机反转
			}
			else
			{
			  SMotor_FFW(2);//步进电机停转
			}
		}		

//*************************************//	
//*************************************//	
//获取APP上位机指令					
		if(WiFi_RX_STA&0x8000)//接收到数据
		{
			WiFi_RX_Len=WiFi_RX_STA&0x3FFF;//得到此次接收到的数据长度

			if(WiFi_RX_BUF[0]=='A'&&WiFi_RX_BUF[1]=='7'&&WiFi_RX_BUF[2]=='Z')//自动模式
			{
				Mode=0;
			}
			else if(WiFi_RX_BUF[0]=='A'&&WiFi_RX_BUF[1]=='7'&&WiFi_RX_BUF[2]=='S')//手动模式
			{
				Mode=1;
			}
			
			if(Mode==1)
			{
				if(WiFi_RX_BUF[0]=='A'&&WiFi_RX_BUF[1]=='7'&&WiFi_RX_BUF[2]=='K')//升
				{
					SendBuff[14]='1';
					WindowSet=20;
				}
				else if(WiFi_RX_BUF[0]=='A'&&WiFi_RX_BUF[1]=='7'&&WiFi_RX_BUF[2]=='G')//降
				{
					SendBuff[14]='0';
					WindowSet=0;
				}			
		  }
			
			WiFi_RX_STA=0;
		}		
//*************************************//		
	}
}
