#include "canbus.h"
#include "timer.h"
// ���ʹ��CAN����ôsys.h  �����������º궨��  #define INTVPACTION

/*CAN����λʱ��������趨�����
1��	ȷ��ʱ��ݶ�
		ʱ��ݶ�����Ϊ8~25
		λʱ��=1/������           ( 250K           4us)
		ʱ��ݶ�=λʱ��/ʱ��ݶ�����
		BRP=ʱ��ݶ�/(2*tclk)=  ʱ��ݶ�/2*FOSC=206*200/2000=40
2��	����ʱ��κͲ�����
		ʱ��ݶ�����=1+T1+T2
		������80%���
		(1+T1)/(1+T1+T2)=0.8
		�Ϳ���ȷ��T1  T2
		����ʱ��κ���λ��������1  =T1
		��λ��������2              =T2
3��	ȷ��ͬ����ת���ȺͲ�������
		ͬ����ת����1~4  ������

*/
CANBUSUNIT CanData;
// https://blog.csdn.net/weixin_44536482/article/details/89030152
// 125K{0x3F,0x40,0x72,0x00},250K{0x1F,0x40,0x72,0x00},500K{0x0F,0x40,0x72,0x00},1M{0x07,0x40,0x72,0x00}
// ����T5LӦ�ÿ���ָ��3.8��CAN�ӿڽ��г�ʼ��
void CanBusInit(u8 *RegCfg)
{
	SetPinOut(0, 2);
	SetPinIn(0, 3);
	PinOutput(0, 2, 1);
	MUX_SEL |= 0x80; // ��CAN�ӿ�������P0.2,P0.3
	ADR_H = 0xFF;
	ADR_M = 0x00;
	ADR_L = 0x60;
	ADR_INC = 1;
	RAMMODE = 0x8F; // д����
	while (!APP_ACK)
		;
#if 0
	DATA3 = 17;
	DATA2 = 0x4c;
	DATA1 = 0x1f;
	DATA0 = 0x00;
#else
	DATA3 = RegCfg[0];
	DATA2 = RegCfg[1];
	DATA1 = RegCfg[2];
	DATA0 = RegCfg[3];
#endif
	APP_EN = 1;
	while (APP_EN)
		;
	DATA3 = 0;
	DATA2 = 0;
	DATA1 = 0;
	DATA0 = 0; // �������ռĴ���ACR
	APP_EN = 1;
	while (APP_EN)
		;
	DATA3 = 0xFF;
	DATA2 = 0xFF;
	DATA1 = 0xFF;
	DATA0 = 0xFF; // ����AMR
	APP_EN = 1;
	while (APP_EN)
		;
	RAMMODE = 0;
	CAN_CR = 0xA0;
	while (CAN_CR & 0x20)
		;	  // ִ������FF0060-FF0062����
	ECAN = 1; // ��CAN�ж�
}

/**************************************************************
D3  1  CAN_RX_BUFFER  [7] IDE ��[6]RTR�� [3:0]��DLC��֡���ݳ��ȡ�
0xFF:0068
D2:D0  3  δ����
ID  ID����չ֡ʱ 29bit ��Ч����׼֡ʱ 11bit ��Ч��
D3  1  ID ��һ���ֽڣ���׼֡����չ֡��
D2  1  ID �ڶ����ֽڣ�[7:5]Ϊ��׼֡�ĸ� 3bit����չ֡�� 2 �ֽڡ�
D1  1  ID �������ֽڣ���׼֡��Ч����չ֡�� 3 �ֽڡ�
0xFF:0069
D0  1  ID ���ĸ��ֽڣ���׼֡��Ч��[7:3]-��չ֡�ĸ� 5bit��
0xFF:006A  D3:D0  4  ����  �������ݣ�DATA1-DATA4��
0xFF:006B  D3:D0  4  ����  �������ݣ�DATA5-DATA8��
******************************************************************/
// ����T5L1��T5L2������main������while(1)�е���
void CanErrorReset(void)
{
	// EA=0;
	if (CAN_ET & 0X20)
	{
		CAN_ET &= 0XDF;
		CAN_CR |= 0X40;
		delay_us(1000);
		CAN_CR &= 0XBF;
		CanData.CanTxFlag = 0;
	}
	// EA=1;
}

void LoadOneFrame(void)
{
	ADR_H = 0xFF;
	ADR_M = 0x00;
	ADR_L = 0x64;
	ADR_INC = 1;
	RAMMODE = 0x8F; // д����
	while (!APP_ACK)
		;
	DATA3 = CanData.BusTXbuf[CanData.CanTxTail].status; // ֡�೤�����Լ�����
	DATA2 = 0;
	DATA1 = 0;
	DATA0 = 0;
	APP_EN = 1;
	while (APP_EN)
		; // д��RTR,IDE,DLC������
	DATA3 = (u8)(CanData.BusTXbuf[CanData.CanTxTail].ID >> 24);
	DATA2 = (u8)(CanData.BusTXbuf[CanData.CanTxTail].ID >> 16);
	DATA1 = (u8)(CanData.BusTXbuf[CanData.CanTxTail].ID >> 8);
	DATA0 = (u8)(CanData.BusTXbuf[CanData.CanTxTail].ID);
	APP_EN = 1;
	while (APP_EN)
		; // д��ID����
	DATA3 = CanData.BusTXbuf[CanData.CanTxTail].candata[0];
	DATA2 = CanData.BusTXbuf[CanData.CanTxTail].candata[1];
	DATA1 = CanData.BusTXbuf[CanData.CanTxTail].candata[2];
	DATA0 = CanData.BusTXbuf[CanData.CanTxTail].candata[3];
	APP_EN = 1;
	while (APP_EN)
		; // д�뷢������ǰ4�ֽ�
	DATA3 = CanData.BusTXbuf[CanData.CanTxTail].candata[4];
	DATA2 = CanData.BusTXbuf[CanData.CanTxTail].candata[5];
	DATA1 = CanData.BusTXbuf[CanData.CanTxTail].candata[6];
	DATA0 = CanData.BusTXbuf[CanData.CanTxTail].candata[7];
	APP_EN = 1;
	while (APP_EN)
		; // д�뷢�����ݺ�4�ֽ�
	CanData.CanTxTail++;
	RAMMODE = 0;
}

// status��Ҫ�����ṩIDE �� RTR״̬��ʵ�ʷ��ͳ�����len�Զ�����������8�ֽڻ��Զ���ֳɶ��
void CanTx(u32 ID, u8 status, u16 len, const u8 *pData)
{
	u8 i, j, k, framnum, framoffset;
	u32 idtmp, statustmp;

	if (len > 2048) // ���ͳ��ȴ��ڶ��г���
		return;
	if (status & 0x80) // ��չ֡
	{
		idtmp = ID << 3;
	}
	else
	{
		idtmp = ID << 21;
	}
	if (CanData.BusTXbuf[CanData.CanTxHead].status & 0x40) // Զ��֡����Ҫ��������
	{
		CanData.BusTXbuf[CanData.CanTxHead].ID = idtmp;
		CanData.BusTXbuf[CanData.CanTxHead].status = status & 0xC0; // Զ��֡���ͳ���ǿ������
		CanData.CanTxHead++;
	}
	else
	{
		framnum = len >> 3;
		framoffset = len % 8;
		k = 0;
		statustmp = status & 0xC0;
		for (i = 0; i < framnum; i++)
		{
			CanData.BusTXbuf[CanData.CanTxHead].ID = idtmp;
			CanData.BusTXbuf[CanData.CanTxHead].status = statustmp | 0x08;
			for (j = 0; j < 8; j++)
			{
				CanData.BusTXbuf[CanData.CanTxHead].candata[j] = pData[k];
				k++;
			}
			CanData.CanTxHead++;
		}
		if (framoffset)
		{
			CanData.BusTXbuf[CanData.CanTxHead].ID = idtmp;
			CanData.BusTXbuf[CanData.CanTxHead].status = statustmp | framoffset;
			for (j = 0; j < framoffset; j++)
			{
				CanData.BusTXbuf[CanData.CanTxHead].candata[j] = pData[k];
				k++;
			}
			for (; j < 8; j++)
				CanData.BusTXbuf[CanData.CanTxHead].candata[j] = 0;
			CanData.CanTxHead++;
		}
	}
	if (0 == CanData.CanTxFlag)
	{
		EA = 0;
		LoadOneFrame();
		EA = 1;
		CanData.CanTxFlag = 1;
		CAN_CR |= 0x04; // ��������
	}
}

void Can_Isr() interrupt 9
{
	u8 status;

	EA = 0;
	if ((CAN_IR & 0x80) == 0x80)
	{
		CAN_IR &= 0x3F; // ���Զ��֡���λ
	}
	if ((CAN_IR & 0x40) == 0x40)
	{
		CAN_IR &= 0xBF; // �������֡���λ
		ADR_H = 0xFF;
		ADR_M = 0x00;
		ADR_L = 0x68;
		ADR_INC = 1;
		RAMMODE = 0xAF; // ������
		while (!APP_ACK)
			;
		APP_EN = 1;
		while (APP_EN)
			;
		status = DATA3;
		CanData.BusRXbuf[CanData.CanRxHead].status = status;
		APP_EN = 1;
		while (APP_EN)
			;
		CanData.BusRXbuf[CanData.CanRxHead].ID <<= 8;
		CanData.BusRXbuf[CanData.CanRxHead].ID |= DATA3;
		CanData.BusRXbuf[CanData.CanRxHead].ID <<= 8;
		CanData.BusRXbuf[CanData.CanRxHead].ID |= DATA2;
		CanData.BusRXbuf[CanData.CanRxHead].ID <<= 8;
		CanData.BusRXbuf[CanData.CanRxHead].ID |= DATA1;
		CanData.BusRXbuf[CanData.CanRxHead].ID <<= 8;
		CanData.BusRXbuf[CanData.CanRxHead].ID |= DATA0;
		CanData.BusRXbuf[CanData.CanRxHead].ID = CanData.BusRXbuf[CanData.CanRxHead].ID >> 3;
		if (0 == (status & 0x80)) // ��׼֡ID����Ҫ����18λ
		{
			CanData.BusRXbuf[CanData.CanRxHead].ID >>= 18;
		}
		if (0 == (status & 0x40)) // ����֡����Ҫ��ȡ����
		{
			APP_EN = 1;
			while (APP_EN)
				;
			CanData.BusRXbuf[CanData.CanRxHead].candata[0] = DATA3;
			CanData.BusRXbuf[CanData.CanRxHead].candata[1] = DATA2;
			CanData.BusRXbuf[CanData.CanRxHead].candata[2] = DATA1;
			CanData.BusRXbuf[CanData.CanRxHead].candata[3] = DATA0;
			APP_EN = 1;
			while (APP_EN)
				;
			CanData.BusRXbuf[CanData.CanRxHead].candata[4] = DATA3;
			CanData.BusRXbuf[CanData.CanRxHead].candata[5] = DATA2;
			CanData.BusRXbuf[CanData.CanRxHead].candata[6] = DATA1;
			CanData.BusRXbuf[CanData.CanRxHead].candata[7] = DATA0;
		}
		RAMMODE = 0;
		CanData.CanRxHead++;
	}
	if ((CAN_IR & 0x20) == 0x20)
	{
		CAN_IR &= ~(0x20); // ��շ���֡���λ
		if (CanData.CanTxTail != CanData.CanTxHead)
		{
			LoadOneFrame();
			CAN_CR |= 0x04; // ��������
		}
		else
		{
			CanData.CanTxFlag = 0; // ��շ��ͱ��λ
		}
	}
	if ((CAN_IR & 0x10) == 0x10)
	{
		CAN_IR &= 0xEF; // ��ս���������λ
	}
	if ((CAN_IR & 0x08) == 0x08)
	{
		CAN_IR &= 0xF7; // ��մ�����λ
	}
	if ((CAN_IR & 0x04) == 0x04)
	{
		CAN_IR &= 0xFB; // ����ٲ�ʧ�ܱ��λ
		CAN_CR |= 0x04; // ������������
	}
	CAN_ET = 0;
	EA = 1;
}

/*��ѭ�����ã�����Ҫ���͵����ݷ��ڻ��������ɣ�ͬʱCAN���ͻ�ռ�ö�ʱ��7������λ��������ʹ��*/
// void CANTx(void)
// {
// 	u32 tmp32;
// 	if((0==CanData.CanTxFlag)&&(CanData.CanTxTail != CanData.CanTxFlag))
// 	{
// 		EA = 0;
// 		tmp32=CanData.BusTXbuf[CanData.CanTxTail].ID;
// 		ADR_H = 0xFF;
// 		ADR_M = 0x00;
// 		ADR_L = 0x64;
// 		ADR_INC = 1;
// 		RAMMODE = 0x8F;		//д����
// 		while(!APP_ACK);
// 		DATA3 = CanData.BusTXbuf[CanData.CanTxTail].status;			//֡�೤�����Լ�����
// 		DATA2 = 0;
// 		DATA1 = 0;
// 		DATA0 = 0;
// 		APP_EN = 1;
// 		while(APP_EN);		//д��RTR,IDE,DLC������
// 		DATA3 = (u8)(tmp32>>24);
// 		DATA2 = (u8)(tmp32>>16);
// 		DATA1 = (u8)(tmp32>>8);
// 		DATA0 = (u8)(tmp32>>0);
// 		APP_EN = 1;
// 		while(APP_EN);		//д��ID����
// 		DATA3 = CanData.BusTXbuf[CanData.CanTxTail].candata[0];
// 		DATA2 = CanData.BusTXbuf[CanData.CanTxTail].candata[1];
// 		DATA1 = CanData.BusTXbuf[CanData.CanTxTail].candata[2];
// 		DATA0 = CanData.BusTXbuf[CanData.CanTxTail].candata[3];
// 		APP_EN = 1;
// 		while(APP_EN);		//д�뷢������ǰ4�ֽ�
// 		DATA3 = CanData.BusTXbuf[CanData.CanTxTail].candata[4];
// 		DATA2 = CanData.BusTXbuf[CanData.CanTxTail].candata[5];
// 		DATA1 = CanData.BusTXbuf[CanData.CanTxTail].candata[6];
// 		DATA0 = CanData.BusTXbuf[CanData.CanTxTail].candata[7];
// 		APP_EN = 1;
// 		while(APP_EN);		//д�뷢�����ݺ�4�ֽ�
// 		CanData.CanTxTail++;
// 		CanData.CanTxFlag = 1;
// 		StartTimer(7,3000);//3S��δ���ͳ�ȥ������շ��ͱ��
// 		EA = 1;
// 		RAMMODE = 0;
// 		CAN_CR |= 0x04;		//��������
// 	}
// 	if(CanData.CanTxFlag!=0)
// 	{
// 		if(GetTimeOutFlag(7))
// 		{
// 			CanData.CanTxFlag = 0;
// 		}
// 	}
// }