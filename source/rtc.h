#ifndef __RTC_H__
#define __RTC_H__
#include "t5los8051.h"
#include "sys.h"

sbit RTC_SDA = P3 ^ 3;
sbit RTC_SCL = P3 ^ 2;

extern bit is_time_flag;
extern unsigned char RTCdata[8]; // 时间设为全局变量  0年1月2日 3星期  4时5分6秒

void SDA_IN();
void SDA_OUT();
void delayus(unsigned char t);
void i2cstart();
void i2cstop();
void mack();
void mnak();
void cack();
void i2cbw(unsigned char RTC_Data); // I2C 写入1个字节
unsigned char i2cbr();              // i2c 读取1个字节数据
void init_rtc();                    // 检查8130有没有掉电，掉电则初始化8130，设置时间为2017.01.01 星期一 00:00:00
void rdtime();                      // 把RTC读取并处理，写到DGUS对应的变量空间，主程序中每0.5秒调用一次
u16 Hex_to_BCD(u16 value);

void Write_RTC(u16 time_addr, u16 week_addr); // 把0x9C之后3字节数据写入rtc模块
void Write_wifi_RTC(void);
void RTC_Read_and_Set(void);

#endif