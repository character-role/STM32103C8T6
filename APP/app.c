#include "Header.h"

u8 SendBuff[15]={'A','7',0};//�ϱ���λ������
u16 IRQ_Time=0;//��ʱ���жϼ���
int WindowSet=0;//���¼�����
int WindowNow=0;//���¼�״̬
u8  Window_JumpEdge=0;//�������¼ܵ�λʱ��
u16 Light=0;//�ƹ����ȱ���
short Temp=0, Humi=0;//��ʪ�ȱ���
u8  Mode=0;//ģʽ��Ĭ��Ϊ�Զ�ģʽ
u8  Send_JumpEdge=0;//��������ʱ���¼
u8  Dis[10]={0};

int main()	  
{
	SystemInit(0X00000);	//����Ϊ�������ƫ�Ƶ�ַ
	delay_init();//��ʼ����ʱ
	GPIO_Config();//��ʼ��IO��
	ADC_Configuration();//��ʼ��ADC
	SMotor_Init();//���������ʼ��
	WiFi_Init(115200);//��ʼ��WiFiģ��
	SysTick_Init();//��ʼ���๦�ܶ�ʱ��
	DHT11_Init();//��ʼ����ʪ��
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
	Connect_WiFi();//����WiFi�ȵ�
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	
	while(1)
  {	
    DHT11_Read_Data(&Temp, &Humi);
		Display2_1(Temp,Dis);//�����¶�
		SendBuff[2]=Dis[0];
		SendBuff[3]=Dis[1];
		SendBuff[4]=Dis[2];
		SendBuff[5]=Dis[3];
		
		Display2_1(Humi,Dis);//����ʪ��
		SendBuff[7]=Dis[0];
		SendBuff[8]=Dis[1];
		SendBuff[9]=Dis[2];
		SendBuff[10]=Dis[3];		
		
		ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		Light=(4096-ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1))/4.2;//��ȡ��ǿ��ֵ
    Display3(Light,Dis);//�������ǿ��
		SendBuff[11]=Dis[0];
		SendBuff[12]=Dis[1];
		SendBuff[13]=Dis[2];
		
		if(Mode==0)//�Զ�ģʽ
		{
			if(Light>700)//��ǿ�㹻
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
		
		if(SysTick_JumpEdge(&Send_JumpEdge, &SysTickBit.Time1s))//�����ϱ���λ��
    {
			WiFi_Send_Data(SendBuff,15);
		}

		if(SysTick_JumpEdge(&Window_JumpEdge, &SysTickBit.Time1s))//�������¼ܿ���
		{
		  if(WindowSet-WindowNow>0)
			{
			  WindowNow++;
				SMotor_FFW(0);//���������ת
			}
			else if(WindowSet-WindowNow<0)
		  {
			  WindowNow--;
				SMotor_FFW(1);//���������ת
			}
			else
			{
			  SMotor_FFW(2);//�������ͣת
			}
		}		

//*************************************//	
//*************************************//	
//��ȡAPP��λ��ָ��					
		if(WiFi_RX_STA&0x8000)//���յ�����
		{
			WiFi_RX_Len=WiFi_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���

			if(WiFi_RX_BUF[0]=='A'&&WiFi_RX_BUF[1]=='7'&&WiFi_RX_BUF[2]=='Z')//�Զ�ģʽ
			{
				Mode=0;
			}
			else if(WiFi_RX_BUF[0]=='A'&&WiFi_RX_BUF[1]=='7'&&WiFi_RX_BUF[2]=='S')//�ֶ�ģʽ
			{
				Mode=1;
			}
			
			if(Mode==1)
			{
				if(WiFi_RX_BUF[0]=='A'&&WiFi_RX_BUF[1]=='7'&&WiFi_RX_BUF[2]=='K')//��
				{
					SendBuff[14]='1';
					WindowSet=20;
				}
				else if(WiFi_RX_BUF[0]=='A'&&WiFi_RX_BUF[1]=='7'&&WiFi_RX_BUF[2]=='G')//��
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
