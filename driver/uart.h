

/******************************************************************************

                  ��Ȩ���� (C), 2019, �������ĿƼ����޹�˾

 ******************************************************************************
  �� �� ��   : net.c
  �� �� ��   : V1.0
  ��    ��   : chenmeishu
  ��������   : 2019.9.2
  ��������   :
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
******************************************************************************/
#ifndef __UART_H__
#define __UART_H__
#include "sys.h"
#define SERIAL_SIZE 0x400
#define SERIAL_COUNT 0x3ff
#define UART2 0
#define UART3 1
#define UART4 2
#define UART5 3
#define UARTBUFNUM 4

typedef struct databuff
{
  u8 tx_buf[SERIAL_SIZE];
  u8 rx_buf[SERIAL_SIZE];
  u16 tx_head, tx_tail;
  u16 rx_head, rx_tail;
  u8 tx_flag;
} DATABUFF;

extern DATABUFF Uart_Struct[];

void Uatr_Send_Data(u8 str, u16 len, const u8 *buf);
void UartInit(u8 UartPort, u32 bdt);

// ��
#define RxBufLen 512             // 2K���ݻ�����
#define RxBufMask (RxBufLen - 1) //

extern volatile u16 RxWritePt;      // ����дָ��
extern volatile u16 RxReadPt;       // ���ն�ָ��
extern volatile u8 RxBuf[RxBufLen]; // ���ջ�����  (2K�ֽ�)
void UART4_Sendbyte(u8 dat);
void UART4_Sendbyte16(u16 dat);
void UART4_Sendbyte32(u32 dat);

#endif
