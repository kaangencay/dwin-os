#ifndef _MYMODBUS_H_
#define _MYMODBUS_H_

#include "t5los8051.h"
#include "sys.h"

extern void Send_ModBus(unsigned char CMD, u16 Address, u16 dat, unsigned char count);
extern unsigned int crc16(volatile unsigned char *puchMsg, unsigned char usDataLen);

extern void Send_ModBus_10(unsigned char CMD, u16 Address, u16 Register_num, u16 dat, unsigned char count);
extern void Send_ModBus04_10(unsigned char CMD, u16 Address, u16 Register_num, u16 datH, u16 datL, unsigned char count);
extern void Send_ModBus08_10(unsigned char CMD, u16 Address, u16 Register_num, u16 datH, u16 datL, u16 dat1H, u16 dat1L, unsigned char count);

void Send_ModBus_IP(void);

void Receive_Modbus(void);

#endif
