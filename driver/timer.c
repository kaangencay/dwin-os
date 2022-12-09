/******************************************************************************

                  ��Ȩ���� (C), 2019, �������ĿƼ����޹�˾

 ******************************************************************************
  �� �� ��   : timer.c
  �� �� ��   : V1.0
  ��    ��   : chenmeishu
  ��������   : 2019.9.2
  ��������   : ʵ���˶�ʱ��    Ӳ����ʱ��0    ʵ����8����ʱ��
  �޸���ʷ   :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : 
******************************************************************************/
#include "timer.h"
#include "sys.h"

//ע�⣺��ֹ˽�Է�����Щ������ֻ���ýӿں�������
//u8  EnableTimer;   //8����ʱ���Ƿ�ʹ��
//u8  OutTimeFlag;   //8����ʱ���Ƿ�ʱ
//u32 TimerTime[8];  //8����ʱ����ʱʱ��
u8  EnablePWM;
u16 PWMTotal[8];
u16 PWMLow[8];
u16 PWMTicks[8];
u8  PWMPort[8];
u8  PWMPin[8];
//ע�⣺��ֹ˽�Է�����Щ������ֻ���ýӿں�������
u8 data EnableTimer;   //8����ʱ���Ƿ�ʹ��
u8 data OutTimeFlag;   //8����ʱ���Ƿ�ʱ
u16 data TimerTime[8];  //8����ʱ����ʱʱ��
u8 data SysTick;

/*****************************************************************************
 �� �� ��  : void T0_Init(void)
 ��������  : ��ʱ��0��ʼ��	��ʱ���1ms
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��9��2��
    ��    ��   :  ������
    �޸�����   : ����
*****************************************************************************/

void T0_Init(void)
{
	  TMOD=0x11;          //16λ��ʱ��
    //T0
    TH0=0x00;
    TL0=0x00;
    TR0=0x00;
	
	  OutTimeFlag=0;
	  EnableTimer=0;
	
    TMOD|=0x01;
    TH0=T1MS>>8;        //1ms��ʱ��
    TL0=T1MS;
    ET0=1;              //������ʱ��0�ж�
//    EA=1;               //�����ж�
    TR0=1;              //������ʱ��
}
/*****************************************************************************
 �� �� ��  : void T0_ISR_PC(void)    interrupt 1
 ��������  : ��ʱ��0����������������
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��9��2��
    ��    ��   : ������
    �޸�����   : ����
*****************************************************************************/
void T0_ISR_PC(void)    interrupt 1
{
	 u8 data i;
	
    EA=0;
    TH0=T1MS>>8;
    TL0=T1MS;
		SysTick++;
	  SysTick_RTC++;
		for(i=0;i<8;i++)
		{
			if(EnableTimer&(0x01<<i))
			{
				TimerTime[i]--;
				if(TimerTime[i]==0)
				{
					OutTimeFlag |= 0x01<<i;
					EnableTimer &= ~(0x01<<i); 					 
				}
			}
		}
    EA=1;
}

void timer2_Isr() interrupt 5
{
	EA = 0;
	TF2 = 0;
	EA = 1;
}


/*****************************************************************************
 �� �� ��  : void StartTimer(u8 ID, u16 nTime)
 ��������  : ����һ����ʱ����
 �������  :	 u8 ID         ��ʱ��ID
               u16 nTime     ��ʱms��

 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��9��2��
    ��    ��   : ������
    �޸�����   : ����
*****************************************************************************/
void StartTimer(u8 ID, u16 nTime)
{
	  EA=0;
	  EnableTimer=EnableTimer|(1<<ID);
	  TimerTime[ID]=nTime;
	  OutTimeFlag&=~(1<<ID);
	  EA=1; 
}


/*****************************************************************************
 �� �� ��  : void KillTimer(u8 ID)
 ��������  : ֹͣһ����ʱ����
 �������  :	u8 ID  ��ʱ��ID
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��9��2��
    ��    ��   : ������
    �޸�����   : ����
*****************************************************************************/
void KillTimer(u8 ID)
{
	  EA=0;
	  EnableTimer&=~(1<<ID);
	  OutTimeFlag&=~(1<<ID);
	  EA=1;
}

/*****************************************************************************
 �� �� ��  : u8 GetTimeOutFlag(u8 ID)
 ��������  : ��ö�ʱ���Ƿ�ʱ
 �������  :	u8 ID  ��ʱ��ID
 �������  :  0  δ��ʱ    ��0  ��ʱ
 �޸���ʷ  :
  1.��    ��   : 2019��9��2��
    ��    ��   : ������
    �޸�����   : ����
*****************************************************************************/
u8 GetTimeOutFlag(u8 ID)
{
  u8 flag;
	EA=0;
	flag=OutTimeFlag&(1<<ID);
	EA=1;
	return flag;
	
}

#ifdef PWMENABLE
void StartPWM(u8 PWMID, u16 VPWMTotal,u16 VPWMLow,u8 Port,u8 Pin )
{
	  EA=0;
	  SetPinOut(Port,Pin);
	  PWMTicks[PWMID]=0;
	  PWMTotal[PWMID]=VPWMTotal;
    PWMLow[PWMID]=VPWMLow;
    PWMPort[PWMID]=Port;
    PWMPin[PWMID]=Pin;	
	  EnablePWM|=(1<<PWMID);
	  EA=1;
}

void StopPWM(u8 PWMID)
{
	  EA=0;
	  EnablePWM&=~(1<<PWMID);	
	  PinOutput(PWMPort[PWMID],PWMPin[PWMID],0);
	  EA=1;
}

void StopAllPWM()
{
	 u8 i;
	 EnablePWM=0;
	 for(i=0;i<8;i++)
	{
		PinOutput(PWMPort[i],PWMPin[i],0);
	}
}
#endif