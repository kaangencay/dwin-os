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

//IO引脚初始化
void io_init()
{
	PORTDRV = 0x01;//驱动电流为8mA
	//P1MDOUT |= 0x02;//将P1.1设置为输出,用于驱动LED1灯
	//P1MDOUT &= 0xFE;//将P1.0设置为输入,用于读取引脚的电平变化
	
	//sbit csx = P1^0;		//p1.0
	//sbit sdo = P1^1;		//p1.1
	//sbit sdi = P1^2;		//p1.2
	//sbit sck = P1^3;		//p1.3
	//sbit sdc = P1^4;		//p1.4
	
	P1MDOUT = 0x1D;	
}

void main()
{
//	unsigned char head[2];
	
	INIT_CPU();             		//CPU 初始化，注意初始化过程中会把所有xdata变量初始化成0。如果声明的变量有初始值，需要在该函数后面重新赋值
	T0_Init();						//定时器0初始化
	Pro8283Init();
	init_rtc();
	EA = 1;
	StartTimer(0,20);
	StartTimer(1,10);
	
	//-------------------
	io_init();
	delay_ms(1000);
	ssd2828_init();
	//-------------------
	rdtime();
	
	UartInit(UART4,9600);	
	Vars_Initialize();
	Sta_Init();
	
	while(1)
	{
//		if(SysTick_RTC >= 500)
//		{
//			rdtime();								//更新硬件RTC时间
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
		
		if(GetTimeOutFlag(0))
		{
			PageFunction();//每一页对应自己的一个子函数。有界面的开发使用该框架
			StartTimer(0,20);
		}
		if(GetTimeOutFlag(1))
		{
			Pro8283Deal();
			StartTimer(1,10);
		}	
//		CanErrorReset();
		
		Receive_Modbus();
		
	}
}

//初始化相关参数和图标
void Sta_Init(void)
{
	u16 Dat;
	
	//固定寄存器初始化
	Dat = 0x0000;
	sys_write_vp(0x1000,(u8*)&Dat,1);
	Dat = 0x005A;
	sys_write_vp(0x1001,(u8*)&Dat,1);
	Dat = 0xFF00;
	sys_write_vp(0x1002,(u8*)&Dat,1);
	Dat = 0x0001;
	sys_write_vp(0x1003,(u8*)&Dat,1);
	
	gCtrlPara.Memory = 0x0001;//内存
	
	sys_write_vp(HMI_Current_Control,(u8*)&gCtrlPara.Current_Control,1);	//文件上电读当前控制页1
	

	
	Dat = 0x0001;
	sys_write_vp(0x10A1,(u8*)&Dat,1);
	sys_write_vp(0x10A2,(u8*)&Dat,1);
	sys_write_vp(0x10A3,(u8*)&Dat,1);
	sys_write_vp(0x10A4,(u8*)&Dat,1);
	sys_write_vp(0x10A5,(u8*)&Dat,1);
	sys_write_vp(0x10A6,(u8*)&Dat,1);
	

}


