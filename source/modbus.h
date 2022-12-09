#ifndef __MODBUS__H___
#define __MODBUS__H___
#include "sys.h"
typedef struct _mmodbus
{
	u8 SlaveAddr;		// �ӻ���ַ
	u8 Order;			// modbus����
	u8 Length;			// ��д���ݳ���
	u8 mode;			// 0��ʾ����ҳ�涼������1��ʾָ��ҳ�津����2��ʾVP��������0x5aʱִ�и�����
	u16 PageIDorVpAddr; // ģʽ1��ģʽ2��Ч
	u16 waitTime;		// ����ָ��ִ��ʱ��
	u16 VPaddr;			// ����ָ��VP��ʼ��ַ��������ֽ���0XFF�����ֽ�Ϊ����ͨ�������ȡ������ֱ��д�����߻�����
	u16 ModbusReg;		// ����ָ��Modbus�Ĵ�����ʼ��ַ
} MMODBUS;

#define START_TREAT_LENGTH 8
#define READ_REGISTER 0x03
#define WRITE_REGISTER 0x10
#define UART485 2

void Uart485RxTreat(void);
void ModbusCommuMintor(void);
#endif