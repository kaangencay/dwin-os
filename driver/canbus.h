#ifndef __CANBUS__H___
#define __CANBUS__H___
// ���ʹ��CAN����ôsys.h  �����������º궨��  #define INTVPACTION
#include "sys.h"

typedef struct _candataunit
{
	u8 status; // bit7   IDE  0��ʾ��׼֡  1��ʾ��չ֡     bit6 RTR  0��ʾ����֡��1��ʾԶ��֡   bit3_0 �������ݳ��ȣ�0-8.
	u32 ID;
	u8 candata[8];
} CANUNIT;

typedef struct _candataunitbuf
{
	CANUNIT BusRXbuf[256];
	CANUNIT BusTXbuf[256];
	u8 CanRxHead, CanRxTail;
	u8 CanTxHead, CanTxTail, CanTxFlag;
} CANBUSUNIT;

typedef struct _can8283
{
	u8 Busbuf[256];
	u16 Can8283RxHead, Can8283RxTail;
} CANBUS8283;

extern CANBUSUNIT CanData;

void CanTx(u32 ID, u8 status, u16 len, const u8 *pData);
void CanBusInit(u8 *RegCfg);
void CanErrorReset(void);

#endif
