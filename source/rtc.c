#include "rtc.h"
#include "sys.h"
bit is_time_flag = 0;


//**********************RX8130接口程序，SDA 10K上拉到3.3V**************
//上电时运行一次initrtc()，然后0.5秒间隔运行一次rdtime()读取时间到DGUS相应系统接口
unsigned char RTCdata[8];


void SDA_IN()
{	
	P3MDOUT=P3MDOUT&0xF7;
}

void SDA_OUT()
{   
	//P3MDOUT=P3MDOUT|0x0C;
	P3MDOUT=P3MDOUT|0x08;
}

	//delay t uS
void delayus(unsigned char t)
{	
	char i;
	while(t)
	{	
		for(i=0;i<17;i++)
		{
			i=i;
		}
		t--;
	}
}

void i2cstart()
{	
	SDA_OUT();
	RTC_SDA=1;
	RTC_SCL=1;
	delayus(15);
	RTC_SDA=0;
	delayus(15);
	RTC_SCL=0;
	delayus(15);
}

void i2cstop()
{	
	SDA_OUT();
	RTC_SDA=0;
	RTC_SCL=1;
	delayus(15);
	RTC_SDA=1;
	delayus(15);
	SDA_IN();
}

void mack()
{	
	SDA_OUT();
	RTC_SDA=0;
	delayus(5);
	RTC_SCL=1;
	delayus(5);
	RTC_SCL=0;
	delayus(5);
}

void mnak()
{	
	SDA_OUT();
	RTC_SDA=1;
	delayus(5);
	RTC_SCL=1;
	delayus(5);
	RTC_SCL=0;
	delayus(5);
}

void cack()
{	
	unsigned char i;
	SDA_IN();
	RTC_SDA=1;
	delayus(5);
	RTC_SCL=1;
	delayus(5);
	for(i=0;i<50;i++)
	{   
		if(!RTC_SDA) break;
		delayus(5);
	}
	RTC_SCL=0;
	delayus(5);
	SDA_OUT();
}

		//I2C 写入1个字节
void i2cbw(unsigned char dat)
{	char i;
	SDA_OUT();
	for(i=0;i<8;i++)
	{	
		if(dat&0x80) 
			RTC_SDA=1;
		else 
			RTC_SDA=0;
		dat=(dat<<1);
		delayus(5);
		RTC_SCL=1;
		delayus(5);
		RTC_SCL=0;
		delayus(5);
	}
	cack();
}

//i2c 读取1个字节数据
unsigned char i2cbr()
{	
	char i;
	unsigned char dat;
	SDA_IN();
	for(i=0;i<8;i++)
	{	
		delayus(5);
		RTC_SCL=1;
		delayus(5);
		dat=(dat<<1);
		if(RTC_SDA) 
			dat=dat|0x01;
		else 
			dat=dat&0xFE;
		RTC_SCL=0;
		delayus(5);
	}
	return(dat);
}

		//检查8130有没有掉电，掉电则初始化8130，设置时间为2017.01.01 星期一 00:00:00
void init_rtc()
{	
	unsigned char i;
	
	SetPinOut(3,2);//配置相关的引脚
	SetPinOut(3,3);
	
	//检查有没有掉电
	i2cstart();
	i2cbw(0x64);
	i2cbw(0x1d);
	i2cstop();
	i2cstart();
	i2cbw(0x65);
	i=i2cbr();
	mack();
	i2cbr();
	mnak();
	i2cstop();
	if((i&0x02)==0x02)
	{	//重新配置时间
		i2cstart();		//30=00
		i2cbw(0x64);
		i2cbw(0x30);
		i2cbw(0x00);
		i2cstop();
		i2cstart();		//1C-1F=48 00 40 10
		i2cbw(0x64);
		i2cbw(0x1C);
		i2cbw(0x48);
		i2cbw(0x00);
		i2cbw(0x40);
		i2cbw(0x10);
		i2cstop();
		i2cstart();		//10-16=RTC设置值 BCD格式
		i2cbw(0x64);
		i2cbw(0x10);
		i2cbw(0x00);	//秒
		i2cbw(0x00);	//分
		i2cbw(0x00);	//时
		i2cbw(0x01);	//星期
		i2cbw(0x01);	//日
		i2cbw(0x01);	//月
		i2cbw(0x17);	//年
		i2cstop();
		i2cstart();		//1E-1F 00 10
		i2cbw(0x64);
		i2cbw(0x1E);
		i2cbw(0x00);	
		i2cbw(0x10);			
		i2cstop();
	}
}

//把RTC读取并处理，写到DGUS对应的变量空间，主程序中每0.5秒调用一次
void rdtime()
{	
	//unsigned char RTCdata[8];
	unsigned char i,n,m;
	i2cstart();
	i2cbw(0x64);
	i2cbw(0x10);
	i2cstop();
	i2cstart();
	i2cbw(0x65);
	for(i=6;i>0;i--)
	{	
		RTCdata[i]=i2cbr();
		mack();
	}
	RTCdata[0]=i2cbr();
	mnak();
	i2cstop();
	for(i=0;i<3;i++)	//年月日转换成HEX
	{	
		n=RTCdata[i]/16;
		m=RTCdata[i]%16;
		RTCdata[i]=n*10+m;
	}
	for(i=4;i<7;i++)	//时分秒转换成HEX
	{	
		n=RTCdata[i]/16;
		m=RTCdata[i]%16;
		RTCdata[i]=n*10+m;
	}
	#if 0
	//处理星期的数据格式
	n=0;
	m=RTCdata[3];			//bit     7654 3210
	SBUF0 = m;
	for(i=0;i<7;i++)		//星期日  0000 0001
	{   					//星期一  0000 0010
		if(m&0x01)  break;	//星期二  0000 0100
							//星期三  0000 1000
		n++;				//星期四  0001 0000
		m=(m>>1);
	}
	RTCdata[3]=n;		//星期是  0-6   对应   星期日、星期一...星期六
	#endif
	
	RTCdata[7]=0x00;
	write_dgus_vp(0x0010,RTCdata,0x04);	//写入DGUS变量空间
}

u16 Hex_to_BCD(u16 value)		//转化为BCD码
{
	value = (value/10*16) + value%10;
	return value;
}


u8 rtc_get_week(u8 year,u8 month,u8 day)
{	
	u16 tmp,mon,y;
	u8 week;
	if((month == 1) || (month == 2))
	{
		mon = month + 12;
		y = year - 1;
	}
	else 
	{
		mon = month;
		y = year;
	}
	tmp = y + (y / 4) +(((mon + 1) * 26) / 10) + day - 36;
	week = tmp % 7;
	return week;
}


void Write_RTC(u16 time_addr, u16 week_addr) 
{ 
	unsigned char rtcdata[8];
	unsigned char Rtc_Set[6];
	unsigned short week;
	
	read_dgus_vp(time_addr,Rtc_Set,3);			//用数据变量录入 3个字，高低字节 共6字节  009D--009F
	read_dgus_vp(week_addr,(u8*)&week,1);		//星期录入
	
	rtcdata[6]=Hex_to_BCD(Rtc_Set[0]);
	rtcdata[5]=Hex_to_BCD(Rtc_Set[1]);
	rtcdata[4]=Hex_to_BCD(Rtc_Set[2]);
	rtcdata[2]=Hex_to_BCD(Rtc_Set[3]);
	rtcdata[1]=Hex_to_BCD(Rtc_Set[4]);
	rtcdata[0]=Hex_to_BCD(Rtc_Set[5]);
	/*
	if(week == 7)
	{
		rtcdata[3]=(u8)(1<<0);
	}
	else if(week < 7)
	rtcdata[3]=(u8)(1<<week);*/
	
	rtcdata[3] = rtc_get_week(Rtc_Set[0],Rtc_Set[1],Rtc_Set[2]);

	i2cstart(); //30=00 
	i2cbw(0x64); 
	i2cbw(0x30); 
	i2cbw(0x00); 
	i2cstop(); 
	i2cstart(); //1C-1F=48 00 40 10 
	i2cbw(0x64); 
	i2cbw(0x1C); 
	i2cbw(0x48); 
	i2cbw(0x00); 
	i2cbw(0x40); 
	i2cbw(0x10); 
	i2cstop(); 
	i2cstart(); //10-16=RTC设置值 BCD格式 
	i2cbw(0x64); 
	i2cbw(0x10); 
	
	
	i2cbw(rtcdata[0]); //秒      
	i2cbw(rtcdata[1]); //分 
	i2cbw(rtcdata[2]); //时     
	i2cbw(rtcdata[3]); //星期 
	i2cbw(rtcdata[4]); //日 
	i2cbw(rtcdata[5]); //月 
	i2cbw(rtcdata[6]); //年 



	i2cstop(); 
	i2cstart(); //1E-1F 00 10 
	i2cbw(0x64); 
	i2cbw(0x1E); 
	i2cbw(0x00); 
	i2cbw(0x10); 
	i2cstop(); 
} 

void Write_wifi_RTC(void) 
{ 
	unsigned char rtcdata[7];
	unsigned char Rtc_Set[8];
	
	read_dgus_vp(0x04AC,Rtc_Set,4);			//用数据变量录入 3个字，高低字节 共6字节  009D--009F

	
	rtcdata[6]=Hex_to_BCD(Rtc_Set[1]);
	rtcdata[5]=Hex_to_BCD(Rtc_Set[2]);
	rtcdata[4]=Hex_to_BCD(Rtc_Set[3]);
	rtcdata[2]=Hex_to_BCD(Rtc_Set[5]);
	rtcdata[1]=Hex_to_BCD(Rtc_Set[6]);
	rtcdata[0]=Hex_to_BCD(Rtc_Set[7]);

	rtcdata[3]=(u8)(1<<Rtc_Set[4]);

	i2cstart(); //30=00 
	i2cbw(0x64); 
	i2cbw(0x30); 
	i2cbw(0x00); 
	i2cstop(); 
	i2cstart(); //1C-1F=48 00 40 10 
	i2cbw(0x64); 
	i2cbw(0x1C); 
	i2cbw(0x48); 
	i2cbw(0x00); 
	i2cbw(0x40); 
	i2cbw(0x10); 
	i2cstop(); 
	i2cstart(); //10-16=RTC设置值 BCD格式 
	i2cbw(0x64); 
	i2cbw(0x10); 
	
	
	i2cbw(rtcdata[0]); //秒      
	i2cbw(rtcdata[1]); //分 
	i2cbw(rtcdata[2]); //时     
	i2cbw(rtcdata[3]); //星期 
	i2cbw(rtcdata[4]); //日 
	i2cbw(rtcdata[5]); //月 
	i2cbw(rtcdata[6]); //年 



	i2cstop(); 
	i2cstart(); //1E-1F 00 10 
	i2cbw(0x64); 
	i2cbw(0x1E); 
	i2cbw(0x00); 
	i2cbw(0x10); 
	i2cstop(); 
} 


void RTC_Read_and_Set(void)
{
	if(is_time_flag == 1)
	{
		is_time_flag = 0;
		rdtime();
	}
}










