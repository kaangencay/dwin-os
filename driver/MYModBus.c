#include "MYModBus.h"
#include "sys.h"
#include "uart.h"
#include "vars.h"
//#include "function.h"
#include "math.h"
#include "stdio.h"
#include "ui.h"

u8 xdata ID ;
u16 xdata CRC_C;
u8 xdata Send_Buf[20];
 

const u8 code auchCRCHi[]={
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40};


const u8 code auchCRCLo[]={
0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,
0x07,0xC7,0x05,0xC5,0xC4,0x04,0xCC,0x0C,0x0D,0xCD,
0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,
0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,
0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4,
0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,
0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,
0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,
0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29,
0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,
0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60,
0x61,0xA1,0x63,0xA3,0XA2,0x62,0x66,0xA6,0xA7,0x67,
0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,
0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,
0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,
0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,
0x70,0xB0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,
0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4B,0x8B,
0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,
0x43,0x83,0x41,0x81,0x80,0x40};



unsigned int crc16(volatile unsigned char *puchMsg, unsigned char usDataLen) 
{
	unsigned char uchCRCHi = 0xFF;/*��CRC�ֽڳ�ʼ��*/
	unsigned char uchCRCLo = 0xFF; /*��CRC�ֽڳ�ʼ��*/ 
  unsigned long uIndex; /* CRCѭ���е�����*/
	
	while (usDataLen--) /* ������Ϣ������*/
	{
		uIndex = uchCRCHi^ *puchMsg++ ; /*����CRC */
		uchCRCHi = uchCRCLo^(auchCRCHi[uIndex]);
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);

}





void SendString(  unsigned char *str,unsigned char len)
{
	unsigned int k=0;
    do 
    {
        UART4_Sendbyte(  *(str + k) );
        k++;
    } while(len--);
	delay_ms(5);
} 



void Send_ModBus( unsigned char CMD, u16 Address ,u16 dat,unsigned char count) 
{
	
	ID = 0x01;

	switch (CMD){
		case 0x03:
		{
			Send_Buf[0] = ID ;
			Send_Buf[1] = CMD ; 
			Send_Buf[2] = (Address&0xFF00)>>8;
			Send_Buf[3] = Address&0x00FF;
			Send_Buf[4] = 0x00;
			Send_Buf[5] = count;
			
			CRC_C=crc16(Send_Buf, 6);
			Send_Buf[6] = (CRC_C&0xFF00)>>8 ;
			Send_Buf[7] = CRC_C&0xFF ;
			SendString(Send_Buf,7);
		  break;
		}
	  case 0x05:
		{
			Send_Buf[0] = ID ;
			Send_Buf[1] = CMD ; 
			Send_Buf[2] = (Address&0xFF00)>>8;
			Send_Buf[3] = Address&0x00FF;
			Send_Buf[4] =  (dat&0xFF00)>>8;
			Send_Buf[5] =  dat&0x00FF;
					
			CRC_C=crc16(Send_Buf, 6) ;
			Send_Buf[6] = (CRC_C&0xFF00)>>8 ;
			Send_Buf[7] = CRC_C&0x00FF ;
			SendString(Send_Buf,7);
		  break;
		}
	  case 0x10:
		{
			Send_Buf[0] = ID ;
			Send_Buf[1] = CMD ; 
			Send_Buf[2] = Address&0xFF00>>8;
			Send_Buf[3] = Address&0x00FF;
			Send_Buf[4] = (unsigned char)dat&0xFF00>>8;
			Send_Buf[5] = (unsigned char)dat&0x00FF;
					
			CRC_C=crc16(Send_Buf, 6) ;
			Send_Buf[6] = CRC_C&0xFF00>>8 ;
			Send_Buf[7] = CRC_C&0x00FF ;

			SendString(Send_Buf,7);
		  break;
		}
	}
	
}

void Send_ModBus_10( unsigned char CMD, u16 Address , u16 Register_num ,u16 dat,unsigned char count) 
{
	
	ID = 0x01;

	Send_Buf[0] = ID ;
	Send_Buf[1] = CMD ; 
	Send_Buf[2] = (Address&0xFF00)>>8;
	Send_Buf[3] = Address&0x00FF;
	Send_Buf[4] = (Register_num&0xFF00)>>8;
	Send_Buf[5] = Register_num&0x00FF;
	Send_Buf[6] = count;
	Send_Buf[7] =  (dat&0xFF00)>>8;
	Send_Buf[8] =  dat&0x00FF;
			
	CRC_C=crc16(Send_Buf,9);
	Send_Buf[9] = (CRC_C&0xFF00)>>8 ;
	Send_Buf[10] = CRC_C&0x00FF ;
	SendString(Send_Buf,10);
}


void Send_ModBus04_10( unsigned char CMD, u16 Address , u16 Register_num ,u16 datH,u16 datL,unsigned char count) 
{
	
	ID = 0x01;

	Send_Buf[0] = ID ;
	Send_Buf[1] = CMD ; 
	Send_Buf[2] = (Address&0xFF00)>>8;
	Send_Buf[3] = Address&0x00FF;
	Send_Buf[4] = (Register_num&0xFF00)>>8;
	Send_Buf[5] = Register_num&0x00FF;
	Send_Buf[6] = count;
	Send_Buf[7] =  (datH&0xFF00)>>8;
	Send_Buf[8] =  datH&0x00FF;
	Send_Buf[9] =  (datL&0xFF00)>>8;
	Send_Buf[10] =  datL&0x00FF;
			
	CRC_C=crc16(Send_Buf,11);
	Send_Buf[11] = (CRC_C&0xFF00)>>8 ;
	Send_Buf[12] = CRC_C&0x00FF ;
	SendString(Send_Buf,12);
}



void Send_ModBus_IP(void) 
{
	sys_read_vp(HMI_WIFI_IP1_Addr,(u8*)&gCtrlPara.WIFI_IP1,1);
	sys_read_vp(HMI_WIFI_IP2_Addr,(u8*)&gCtrlPara.WIFI_IP2,1);
	sys_read_vp(HMI_WIFI_IP3_Addr,(u8*)&gCtrlPara.WIFI_IP3,1);
	sys_read_vp(HMI_WIFI_IP4_Addr,(u8*)&gCtrlPara.WIFI_IP4,1);

	Send_Buf[0] = 0x01 ;
	Send_Buf[1] = 0x10 ; 
	Send_Buf[2] = (HOST_Write_WIFI_IP&0xFF00)>>8;
	Send_Buf[3] = HOST_Write_WIFI_IP&0x00FF;
	Send_Buf[4] = 0x00;
	Send_Buf[5] = 0x04;
	Send_Buf[6] = 0x08;
	Send_Buf[7] =  (gCtrlPara.WIFI_IP1&0xFF00)>>8;
	Send_Buf[8] =  gCtrlPara.WIFI_IP1&0x00FF;
	Send_Buf[9] =  (gCtrlPara.WIFI_IP2&0xFF00)>>8;
	Send_Buf[10] =  gCtrlPara.WIFI_IP2&0x00FF;
	Send_Buf[11] =  (gCtrlPara.WIFI_IP3&0xFF00)>>8;
	Send_Buf[12] =  gCtrlPara.WIFI_IP3&0x00FF;
	Send_Buf[13] =  (gCtrlPara.WIFI_IP4&0xFF00)>>8;
	Send_Buf[14] =  gCtrlPara.WIFI_IP4&0x00FF;

	CRC_C=crc16(Send_Buf, 15) ;
	Send_Buf[15] = (CRC_C&0xFF00)>>8 ;
	Send_Buf[16] = CRC_C&0x00FF ;
	SendString(Send_Buf,16);
}


void Send_ModBus08_10( unsigned char CMD, u16 Address , u16 Register_num ,u16 datH,u16 datL,u16 dat1H,u16 dat1L,unsigned char count) 
{
	
	ID = 0x01;

	Send_Buf[0] = ID ;
	Send_Buf[1] = CMD ; 
	Send_Buf[2] = (Address&0xFF00)>>8;
	Send_Buf[3] = Address&0x00FF;
	Send_Buf[4] = (Register_num&0xFF00)>>8;
	Send_Buf[5] = Register_num&0x00FF;
	Send_Buf[6] = count;
	Send_Buf[7] =  (datH&0xFF00)>>8;
	Send_Buf[8] =  datH&0x00FF;
	Send_Buf[9] =  (datL&0xFF00)>>8;
	Send_Buf[10] =  datL&0x00FF;
	Send_Buf[11] =  (dat1H&0xFF00)>>8;
	Send_Buf[12] =  dat1H&0x00FF;
	Send_Buf[13] =  (dat1L&0xFF00)>>8;
	Send_Buf[14] =  dat1L&0x00FF;
			
	CRC_C=crc16(Send_Buf,15);
	Send_Buf[15] = (CRC_C&0xFF00)>>8 ;
	Send_Buf[16] = CRC_C&0x00FF ;
	SendString(Send_Buf,16);
}



