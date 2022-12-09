#include "timer.h"
#include "ui.h"
#include "uart.h"
#include "dwin8283.h"
#include "canbus.h"
#include "ssd2828.h"
#include "rtc.h"

#include "vars.h"
#include "ReceiveData.h"
#include "MYModBus.h"

// IO���ų�ʼ��
void io_init()
{
	PORTDRV = 0x01; // ��������Ϊ8mA
	// P1MDOUT |= 0x02;//��P1.1����Ϊ���,��������LED1��
	// P1MDOUT &= 0xFE;//��P1.0����Ϊ����,���ڶ�ȡ���ŵĵ�ƽ�仯

	// sbit csx = P1^0;		//p1.0
	// sbit sdo = P1^1;		//p1.1
	// sbit sdi = P1^2;		//p1.2
	// sbit sck = P1^3;		//p1.3
	// sbit sdc = P1^4;		//p1.4

	P1MDOUT = 0x1D;
}

void main()
{
	//	unsigned char head[2];

	INIT_CPU(); // CPU ��ʼ����ע���ʼ�������л������xdata������ʼ����0����������ı����г�ʼֵ����Ҫ�ڸú����������¸�ֵ
	T0_Init();	// ��ʱ��0��ʼ��
	Pro8283Init();
	init_rtc();
	EA = 1;
	StartTimer(0, 20);
	StartTimer(1, 10);

	//-------------------
	io_init();
	delay_ms(1000);
	ssd2828_init();
	//-------------------
	rdtime();

	UartInit(UART4, 9600);
	Vars_Initialize();
	Sta_Init();

	while (1)
	{
		//		if(SysTick_RTC >= 500)
		//		{
		//			rdtime();								//����Ӳ��RTCʱ��
		//			SysTick_RTC = 0;
		//		}
		//
		//		read_dgus_vp(0x9C,head,1);
		//		if(head[0] == 0x5A && head[1] == 0xA5)
		//		{
		//			//SBUF0 = temp[0];
		//			Write_RTC(0x9D,0x9E);
		//		}
		//		head[0] = 0;
		//		head[1] = 0;
		//		write_dgus_vp(0x9C,head,1);

		if (GetTimeOutFlag(0))
		{
			PageFunction(); // ÿһҳ��Ӧ�Լ���һ���Ӻ������н���Ŀ���ʹ�øÿ��
			StartTimer(0, 20);
		}
		if (GetTimeOutFlag(1))
		{
			Pro8283Deal();
			StartTimer(1, 10);
		}
		//		CanErrorReset();

		Receive_Modbus();
	}
}

// ��ʼ����ز�����ͼ��
void Sta_Init(void)
{
	u16 Dat;

	// �̶��Ĵ�����ʼ��
	Dat = 0x0000;
	sys_write_vp(0x1000, (u8 *)&Dat, 1);
	Dat = 0x005A;
	sys_write_vp(0x1001, (u8 *)&Dat, 1);
	Dat = 0xFF00;
	sys_write_vp(0x1002, (u8 *)&Dat, 1);
	Dat = 0x0001;
	sys_write_vp(0x1003, (u8 *)&Dat, 1);

	gCtrlPara.Memory = 0x0001; // �ڴ�

	sys_write_vp(HMI_Current_Control, (u8 *)&gCtrlPara.Current_Control, 1); // �ļ��ϵ����ǰ����ҳ1

	Dat = 0x0001;
	sys_write_vp(0x10A1, (u8 *)&Dat, 1);
	sys_write_vp(0x10A2, (u8 *)&Dat, 1);
	sys_write_vp(0x10A3, (u8 *)&Dat, 1);
	sys_write_vp(0x10A4, (u8 *)&Dat, 1);
	sys_write_vp(0x10A5, (u8 *)&Dat, 1);
	sys_write_vp(0x10A6, (u8 *)&Dat, 1);
}
