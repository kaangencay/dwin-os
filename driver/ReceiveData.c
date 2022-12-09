#include "ReceiveData.h"
#include "timer.h"
#include "ui.h"
#include "uart.h"
#include "MYModBus.h"
#include "vars.h"

void Receive_Modbus(void)
{
	u16 iRxReadPt;
	u16 RxHead_PC;
	u16 Check_Nodbus;
	u16 CheckNumber;

	u8 i;
	u8 Length;
	u8 buf[12];
	u16 RxCommon;

	u16 Dat1;
	u16 Dat2; 
	u16 Dat3;
	u16 Dat4;
	u16 Dat5;
	u16 Dat6;

	u16 Addr;

	while ((u16)(RxWritePt - RxReadPt) >= 6)
	{
		iRxReadPt = RxReadPt;

		RxHead_PC = RxBuf[(iRxReadPt++) & RxBufMask]; // ��⵽֡ͷ

		if (RxHead_PC == 0x01)
		{

			if ((RxBuf[(iRxReadPt)&RxBufMask] == 0x03) || (RxBuf[(iRxReadPt)&RxBufMask] == 0x05) || (RxBuf[(iRxReadPt)&RxBufMask] == 0x10)) // ��⵽������
			{
				RxCommon = RxBuf[(iRxReadPt++) & RxBufMask];
			}
			else
			{

				RxReadPt++;
				continue;
			}
		}
		else
		{

			RxReadPt++;
			continue;
		}

		RxReadPt = iRxReadPt;
		delay_ms(5);

		switch (RxCommon)
		{
		case 0x03:
		{
			Length = RxBuf[(iRxReadPt++) & RxBufMask];
			if (Length == 8)
			{
				delay_ms(3);
			}
			switch (Length)
			{
			case 0x02:
			{
				if ((u16)(RxWritePt - RxReadPt) < 4) //
				{
					UART4_Sendbyte16(0xDD03);
					RxReadPt -= 2;
					break;
				}
				buf[0] = 0x01;
				buf[1] = RxCommon;
				buf[2] = Length;						   // �����ֽ���
				buf[3] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����1
				buf[4] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����2

				Check_Nodbus = (RxBuf[(iRxReadPt++) & RxBufMask] << 8) + RxBuf[(iRxReadPt++) & RxBufMask]; // ����У��λ
				CheckNumber = crc16(buf, RxWritePt - RxReadPt);

				if (Check_Nodbus == CheckNumber)
				{

					// ����
					if (HOST_Addr == HOST_Shake_hands)
					{

						gCtrlPara.Shake_handFlag = 1;
						Send_ModBus_10(0x10, HOST_Power_On_Reset, 0x1, 0x0001, 0x02); // �·���λ

						gCtrlPara.Cartoon = 1;							   // ����
						sys_write_vp(0x100E, (u8 *)&gCtrlPara.Cartoon, 1); // ����ֹͣ
						delay_ms(1000);
						break;
					}
					if (HOST_Addr == HOST_Power_On_Reset)
					{
						if (buf[4] == 1)
						{
							gCtrlPara.PowerOn_ResetFlag = 1; // ��λ��־

							gCtrlPara.SendFlag = 1; // ����������ݱ�־

							gCtrlPara.Cartoon = 1;							   // ����
							sys_write_vp(0x100A, (u8 *)&gCtrlPara.Cartoon, 1); // ����ֹͣ
							sys_write_vp(0x100C, (u8 *)&gCtrlPara.Cartoon, 1); // ����ֹͣ

							// ��ת��������
							Page_Change(0x01);
						}
						break;
					}
					// ����״̬����04����
					if (HOST_Addr == HMI_WorkSta)
					{
						gCtrlPara.WorkSta = buf[4];

						// ����״̬��ʾ��100ms����һ�Σ�
						//																										if(gCtrlPara.WorkSta != gCtrlPara.WorkStaHis)
						//																										{
						//																											gCtrlPara.WorkStaHis =gCtrlPara.WorkSta;

						if (gCtrlPara.WorkSta == 0x0000) // IDLE
						{
							gCtrlPara.Text1 = 0x4944;
							gCtrlPara.Text2 = 0x4C45;

							sys_write_vp(HMI_WorkSta, (u8 *)&gCtrlPara.Text1, 1);	  // ��ʾ����״̬
							sys_write_vp(HMI_WorkSta + 1, (u8 *)&gCtrlPara.Text2, 1); // ��ʾ����״̬

							gCtrlPara.Start_Stop = 0;
							sys_write_vp(HMI_Start_Stop, (u8 *)&gCtrlPara.Start_Stop, 1); // ͼ����ʾ
						}
						else if (gCtrlPara.WorkSta == 0x0001) // RUN
						{

							gCtrlPara.Text1 = 0x4255;
							gCtrlPara.Text2 = 0x5359;
							sys_write_vp(HMI_WorkSta, (u8 *)&gCtrlPara.Text1, 1);	  // ��ʾ����״̬
							sys_write_vp(HMI_WorkSta + 1, (u8 *)&gCtrlPara.Text2, 1); // ��ʾ����״̬
							gCtrlPara.Start_Stop = 1;
							sys_write_vp(HMI_Start_Stop, (u8 *)&gCtrlPara.Start_Stop, 1); // ͼ����ʾ
						}
						else if (gCtrlPara.WorkSta == 0x0003) // PAUSE
						{

							gCtrlPara.Text1 = 0x5041;
							gCtrlPara.Text2 = 0x5553;
							gCtrlPara.Text3 = 0x4500;
							sys_write_vp(HMI_WorkSta, (u8 *)&gCtrlPara.Text1, 1);	  // ��ʾ����״̬
							sys_write_vp(HMI_WorkSta + 1, (u8 *)&gCtrlPara.Text2, 1); // ��ʾ����״̬
							sys_write_vp(HMI_WorkSta + 2, (u8 *)&gCtrlPara.Text3, 1); // ��ʾ����״̬
							gCtrlPara.Start_Stop = 0;
							sys_write_vp(HMI_Start_Stop, (u8 *)&gCtrlPara.Start_Stop, 1); // ͼ����ʾ
						}
						else if (gCtrlPara.WorkSta == 0x0005) // IDLE
						{
							gCtrlPara.Text1 = 0x4944;
							gCtrlPara.Text2 = 0x4C45;
							sys_write_vp(HMI_WorkSta, (u8 *)&gCtrlPara.Text1, 1);	  // ��ʾ����״̬
							sys_write_vp(HMI_WorkSta + 1, (u8 *)&gCtrlPara.Text2, 1); // ��ʾ����״̬

							gCtrlPara.Start_Stop = 0;
							sys_write_vp(HMI_Start_Stop, (u8 *)&gCtrlPara.Start_Stop, 1); // ͼ����ʾ
						}
						//																											gCtrlPara.WorkSta =gCtrlPara.WorkStaHis;
						//																										}
						gTime4 = 0;
						gCtrlPara.Flag = 1;
						gCtrlPara.FreeFlag = 1; // ����
						break;
					}

					////��λ�ȱ�����Ϣ
					if (HOST_Addr == HOST_Limit_Infor)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						gCtrlPara.Alarm_Value = Dat1; // ����ֵ

						if (Dat1 == 0x0000)
						{
							gCtrlPara.Text1 = 0x0000;
							gCtrlPara.Text2 = 0x0000;
							gCtrlPara.Text3 = 0x0000;
							gCtrlPara.Text4 = 0x0000;
							gCtrlPara.Text5 = 0x0000;
							gCtrlPara.Text6 = 0x0000;
							gCtrlPara.Text7 = 0x0000;

							sys_write_vp(HMI_Warning_Text, (u8 *)&gCtrlPara.Text1, 1);
							sys_write_vp(HMI_Warning_Text + 1, (u8 *)&gCtrlPara.Text2, 1);
							sys_write_vp(HMI_Warning_Text + 2, (u8 *)&gCtrlPara.Text3, 1);
							sys_write_vp(HMI_Warning_Text + 3, (u8 *)&gCtrlPara.Text4, 1);
							sys_write_vp(HMI_Warning_Text + 4, (u8 *)&gCtrlPara.Text5, 1);
							sys_write_vp(HMI_Warning_Text + 5, (u8 *)&gCtrlPara.Text6, 1);
							sys_write_vp(HMI_Warning_Text + 6, (u8 *)&gCtrlPara.Text7, 1);
						}
						else
						{
							ALm_Init(gCtrlPara.Alarm_Value);
						}

						gTime4 = 0;
						gCtrlPara.Flag = 5;
						gCtrlPara.FreeFlag = 9; // ����
						break;
					}

					// �߱߿�
					if (HOST_Addr == HMI_Go_Border)
					{
						Dat1 = (buf[3] << 8) + buf[4];

						if (Dat1 == 1)
						{
						}
						else if (Dat1 == 2)
						{

							gCtrlPara.SendFlag = 2;
							gCtrlPara.GO_Focusing = 0; // �߱߿�-Ѱ��
						}
						gCtrlPara.FreeFlag = 7;
						break;
					}
					// �ӹ�ʱ�䣨ʱ��
					if (HOST_Addr == HMI_Work_TimeC)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						gCtrlPara.WorkTimeH = Dat1;
						sys_write_vp(HMI_Work_TimeC, (u8 *)&Dat1, 1);

						gTime4 = 0;
						gCtrlPara.Flag = 6;
						break;
					}

					// �ӹ�ʱ�䣨�֣�
					if (HOST_Addr == HMI_Work_TimeD)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						gCtrlPara.WorkTimeM = Dat1;
						sys_write_vp(HMI_Work_TimeD, (u8 *)&Dat1, 1);

						gTime4 = 0;
						gCtrlPara.Flag = 7;
						break;
					}

					// �ӹ�ʱ�䣨�룩
					if (HOST_Addr == HMI_Work_TimeE)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						gCtrlPara.WorkTimeS = Dat1;
						sys_write_vp(HMI_Work_TimeE, (u8 *)&Dat1, 1);

						gTime4 = 0;
						gCtrlPara.Flag = 8;
						break;
					}

					// �ӹ��ٶ�0-1000
					if (HOST_Addr == HMI_Processing_Speed)
					{
						Dat1 = (buf[3] << 8) + buf[4];

						if (gCtrlPara.WorkSta == 0x0001)
						{
							if (Dat1 < 100)
							{
								gCtrlPara.Processing_Speed = 0;
							}
							else if ((Dat1 >= 100) && (Dat1 < 200))
							{
								gCtrlPara.Processing_Speed = 1;
							}
							else if ((Dat1 >= 200) && (Dat1 < 300))
							{
								gCtrlPara.Processing_Speed = 2;
							}
							else if ((Dat1 >= 300) && (Dat1 < 400))
							{
								gCtrlPara.Processing_Speed = 3;
							}
							else if ((Dat1 >= 400) && (Dat1 < 500))
							{
								gCtrlPara.Processing_Speed = 4;
							}
							else if ((Dat1 >= 500) && (Dat1 < 600))
							{
								gCtrlPara.Processing_Speed = 5;
							}
							else if ((Dat1 >= 600) && (Dat1 < 700))
							{
								gCtrlPara.Processing_Speed = 6;
							}
							else if ((Dat1 >= 700) && (Dat1 < 800))
							{
								gCtrlPara.Processing_Speed = 7;
							}
							else if ((Dat1 >= 800) && (Dat1 < 900))
							{
								gCtrlPara.Processing_Speed = 8;
							}
							else if ((Dat1 >= 900) && (Dat1 < 960))
							{
								gCtrlPara.Processing_Speed = 9;
							}
							else if (Dat1 >= 960)
							{
								gCtrlPara.Processing_Speed = 10;
							}

							sys_write_vp(HMI_Processing_Speed, (u8 *)&gCtrlPara.Processing_Speed, 1);
						}
						else if (gCtrlPara.WorkSta == 0x0000)
						{
							gCtrlPara.Processing_Speed = 0;
							sys_write_vp(HMI_Processing_Speed, (u8 *)&gCtrlPara.Processing_Speed, 1); // �ӹ��ٶ���ʾ
						}
						else if (gCtrlPara.WorkSta == 0x0005)
						{
							gCtrlPara.Processing_Speed = 10;
							sys_write_vp(HMI_Processing_Speed, (u8 *)&gCtrlPara.Processing_Speed, 1); // �ӹ��ٶ���ʾ
						}
						gTime4 = 0;
						gCtrlPara.Flag = 9;
						break;
					}

					// �ļ�����
					if (HOST_Addr == HMI_Total_Files)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						gCtrlPara.Total_Files = Dat1; // �ļ�����

						gCtrlPara.Remainder = gCtrlPara.Total_Files % 7; // ����
						gCtrlPara.Integer = gCtrlPara.Total_Files / 7;	 // ����

						sys_write_vp(0x1031, (u8 *)&gCtrlPara.Total_Files, 1); // �ļ�������ʾ
						gCtrlPara.FileCount = 1;							   // �ļ���ȡ����ֵ

						delay_ms(10);

						break;
					}

					if (HOST_Addr == 0x0051)
					{
						Dat1 = (buf[3] << 8) + buf[4];

						sys_write_vp(0x10A1, (u8 *)&Dat1, 1);

						gCtrlPara.IconCount = 1;
					}
					if (HOST_Addr == 0x0052)
					{
						Dat1 = (buf[3] << 8) + buf[4];

						sys_write_vp(0x10A2, (u8 *)&Dat1, 1);

						gCtrlPara.IconCount = 2;
					}
					if (HOST_Addr == 0x0053)
					{
						Dat1 = (buf[3] << 8) + buf[4];

						sys_write_vp(0x10A3, (u8 *)&Dat1, 1);

						gCtrlPara.IconCount = 3;
					}
					if (HOST_Addr == 0x0054)
					{
						Dat1 = (buf[3] << 8) + buf[4];

						sys_write_vp(0x10A4, (u8 *)&Dat1, 1);

						gCtrlPara.IconCount = 4;
					}
					if (HOST_Addr == 0x0055)
					{
						Dat1 = (buf[3] << 8) + buf[4];

						sys_write_vp(0x10A5, (u8 *)&Dat1, 1);

						gCtrlPara.IconCount = 5;
					}
					if (HOST_Addr == 0x0056)
					{
						Dat1 = (buf[3] << 8) + buf[4];

						sys_write_vp(0x10A6, (u8 *)&Dat1, 1);

						gCtrlPara.IconCount = 6;
					}
				}
				for (i = 0; i < 12; i++)
				{
					buf[i] = 0;
				}
			}
			break;

			case 0x04:
			{
				if ((u16)(RxWritePt - RxReadPt) < 6) //
				{
					UART4_Sendbyte16(0xDD04);
					RxReadPt -= 2;
					break;
				}
				buf[0] = 0x01;
				buf[1] = RxCommon;
				buf[2] = Length;						   // �����ֽ���
				buf[3] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����1
				buf[4] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����2
				buf[5] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����3
				buf[6] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����4

				Check_Nodbus = (RxBuf[(iRxReadPt++) & RxBufMask] << 8) + RxBuf[(iRxReadPt++) & RxBufMask]; // ����У��λ
				CheckNumber = crc16(buf, RxWritePt - RxReadPt);

				if (Check_Nodbus == CheckNumber)
				{

					// X����
					if (HOST_Addr == HMI_Read_X_Value)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];

						sys_write_vp(HMI_Read_X_Value, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_Read_X_Value + 1, (u8 *)&Dat2, 1);

						gTime4 = 0;
						gCtrlPara.Flag = 2;
						gCtrlPara.FreeFlag = 3; // ����
						break;
					}

					// Y����
					if (HOST_Addr == HMI_Read_Y_Value)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];

						sys_write_vp(HMI_Read_Y_Value, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_Read_Y_Value + 1, (u8 *)&Dat2, 1);

						gTime4 = 0;
						gCtrlPara.Flag = 3;
						gCtrlPara.FreeFlag = 4; // ����
						break;
					}

					// Z����
					if (HOST_Addr == HMI_Read_Z_Value)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];

						sys_write_vp(HMI_Read_Z_Value, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_Read_Z_Value + 1, (u8 *)&Dat2, 1);

						gTime4 = 0;
						gCtrlPara.Flag = 4;
						gCtrlPara.FreeFlag = 5; // ����
						break;
					}

					// U����
					if (HOST_Addr == HMI_U_Coordinate)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];

						sys_write_vp(HMI_U_Coordinate, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_U_Coordinate + 1, (u8 *)&Dat2, 1);

						gTime4 = 0;
						gCtrlPara.Flag = 10;
						gCtrlPara.FreeFlag = 8; // ����
					}

					// ������Ϣ
					if (HOST_Addr == 0x0032)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];

						sys_write_vp(HMI_System_InformationShow, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_System_InformationShow + 1, (u8 *)&Dat2, 1);

						gCtrlPara.IconCount = 1;
						break;
					}

					// �ۼƿ���ʱ��
					if (HOST_Addr == 0x0465)
					{
						Dat5 = (buf[3] << 8) + buf[4];
						Dat6 = (buf[5] << 8) + buf[6];

						gCtrlPara.Times = (Dat5 << 16) | Dat6;

						Dat1 = (gCtrlPara.Times >> 16) & 0xFFFF;
						Dat2 = (gCtrlPara.Times >> 12) & 0xF;
						Dat3 = (gCtrlPara.Times >> 6) & 0x3F;
						Dat4 = gCtrlPara.Times & 0x3F;

						sys_write_vp(0x400B, (u8 *)&Dat1, 1);
						sys_write_vp(0x400C, (u8 *)&Dat2, 1);
						sys_write_vp(0x400D, (u8 *)&Dat3, 1);
						sys_write_vp(0x400E, (u8 *)&Dat4, 1);

						gCtrlPara.IconCount = 2;

						break;
					}

					// �ۼƼӹ�ʱ��
					if (HOST_Addr == 0x0467)
					{
						Dat5 = (buf[3] << 8) + buf[4];
						Dat6 = (buf[5] << 8) + buf[6];

						gCtrlPara.Times = (Dat5 << 16) | Dat6;

						Dat1 = (gCtrlPara.Times >> 16) & 0xFFFF;
						Dat2 = (gCtrlPara.Times >> 12) & 0xF;
						Dat3 = (gCtrlPara.Times >> 6) & 0x3F;
						Dat4 = gCtrlPara.Times & 0x3F;

						sys_write_vp(0x4002, (u8 *)&Dat1, 1);
						sys_write_vp(0x4003, (u8 *)&Dat2, 1);
						sys_write_vp(0x4004, (u8 *)&Dat3, 1);
						sys_write_vp(0x4005, (u8 *)&Dat4, 1);

						gCtrlPara.IconCount = 3;

						break;
					}

					// ǰ�μӹ�ʱ��
					if (HOST_Addr == 0x0469)
					{
						Dat5 = (buf[3] << 8) + buf[4];
						Dat6 = (buf[5] << 8) + buf[6];

						gCtrlPara.Times = (Dat5 << 16) | Dat6;

						Dat1 = (gCtrlPara.Times >> 16) & 0xFFFF;
						Dat2 = (gCtrlPara.Times >> 12) & 0xF;
						Dat3 = (gCtrlPara.Times >> 6) & 0x3F;
						Dat4 = gCtrlPara.Times & 0x3F;

						sys_write_vp(0x400F, (u8 *)&Dat1, 1);
						sys_write_vp(0x4010, (u8 *)&Dat2, 1);
						sys_write_vp(0x4011, (u8 *)&Dat3, 1);
						sys_write_vp(0x4012, (u8 *)&Dat4, 1);

						gCtrlPara.IconCount = 4;

						break;
					}

					// �ۼƳ���ʱ��
					if (HOST_Addr == 0x046B)
					{
						Dat5 = (buf[3] << 8) + buf[4];
						Dat6 = (buf[5] << 8) + buf[6];

						gCtrlPara.Times = (Dat5 << 16) | Dat6;

						Dat1 = (gCtrlPara.Times >> 16) & 0xFFFF;
						Dat2 = (gCtrlPara.Times >> 12) & 0xF;
						Dat3 = (gCtrlPara.Times >> 6) & 0x3F;
						Dat4 = gCtrlPara.Times & 0x3F;

						sys_write_vp(0x4013, (u8 *)&Dat1, 1);
						sys_write_vp(0x4014, (u8 *)&Dat2, 1);
						sys_write_vp(0x4015, (u8 *)&Dat3, 1);
						sys_write_vp(0x4016, (u8 *)&Dat4, 1);

						gCtrlPara.IconCount = 5;

						break;
					}

					// �ۼƼӹ�����
					if (HOST_Addr == 0x046D)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];

						sys_write_vp(0x4006, (u8 *)&Dat1, 1);
						sys_write_vp(0x4007, (u8 *)&Dat2, 1);

						gCtrlPara.IconCount = 6;

						break;
					}

					// X���ۼ��г�
					if (HOST_Addr == 0x046F)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];

						sys_write_vp(0x4017, (u8 *)&Dat1, 1);
						sys_write_vp(0x4018, (u8 *)&Dat2, 1);

						gCtrlPara.IconCount = 7;

						break;
					}

					// Y���ۼ��г�
					if (HOST_Addr == 0x0471)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];

						sys_write_vp(0x4019, (u8 *)&Dat1, 1);
						sys_write_vp(0x401A, (u8 *)&Dat2, 1);

						gCtrlPara.IconCount = 8;

						break;
					}
				}
				for (i = 0; i < 12; i++)
				{
					buf[i] = 0;
				}
			}
			break;

			case 0x08:
			{
				if ((u16)(RxWritePt - RxReadPt) < 10) //
				{
					UART4_Sendbyte16(0xDD08);
					RxReadPt -= 2;
					break;
				}
				buf[0] = 0x01;
				buf[1] = RxCommon;
				buf[2] = Length;							// �����ֽ���
				buf[3] = RxBuf[(iRxReadPt++) & RxBufMask];	// ����1
				buf[4] = RxBuf[(iRxReadPt++) & RxBufMask];	// ����2
				buf[5] = RxBuf[(iRxReadPt++) & RxBufMask];	// ����3
				buf[6] = RxBuf[(iRxReadPt++) & RxBufMask];	// ����4
				buf[7] = RxBuf[(iRxReadPt++) & RxBufMask];	// ����5
				buf[8] = RxBuf[(iRxReadPt++) & RxBufMask];	// ����6
				buf[9] = RxBuf[(iRxReadPt++) & RxBufMask];	// ����7
				buf[10] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����8

				Check_Nodbus = (RxBuf[(iRxReadPt++) & RxBufMask] << 8) + RxBuf[(iRxReadPt++) & RxBufMask]; // ����У��λ
				CheckNumber = crc16(buf, RxWritePt - RxReadPt);

				if (Check_Nodbus == CheckNumber)
				{

					// ��ǰ�ļ���0x0010-0x0013
					if (HOST_Addr == HMI_Current_File_Name)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_Current_File_Name, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_Current_File_Name + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_Current_File_Name + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_Current_File_Name + 3, (u8 *)&Dat4, 1);

						gTime4 = 0;
						gCtrlPara.FreeFlag = 6; // ����

						break;
					}

					// ��IP
					if (HOST_Addr == HMI_Read_WIFI_IP)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_WIFI_IP1_Addr, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_WIFI_IP2_Addr, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_WIFI_IP3_Addr, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_WIFI_IP4_Addr, (u8 *)&Dat4, 1);

						break;
					}

					// һ���ļ�  0x302b-0x302e
					if (HOST_Addr == HMI_NowFile1)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile1, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile1 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile1 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile1 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 2; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}

					// �����ļ� 0x302f-0x3032
					if (HOST_Addr == HMI_NowFile2)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile2, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile2 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile2 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile2 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 3; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}

					// �����ļ�  0x3033-0x3036
					if (HOST_Addr == HMI_NowFile3)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile3, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile3 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile3 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile3 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 4; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}

					// �ĺ��ļ� 0x3037-0x303A
					if (HOST_Addr == HMI_NowFile4)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile4, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile4 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile4 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile4 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 5; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}

					// ����ļ�  0x303B-0x303E
					if (HOST_Addr == HMI_NowFile5)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile5, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile5 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile5 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile5 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 6; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}

					// �����ļ�  0x303F-0x3042
					if (HOST_Addr == HMI_NowFile6)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile6, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile6 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile6 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile6 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 7; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}

					// �ߺ��ļ�  0x3043-0x3046
					if (HOST_Addr == HMI_NowFile7)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile7, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile7 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile7 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile7 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 8; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}

					// �˺��ļ�  0x3047-0x304A
					if (HOST_Addr == HMI_NowFile8)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile8, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile8 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile8 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile8 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 9; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}

					// �ź��ļ�  0x304B-0x304E
					if (HOST_Addr == HMI_NowFile9)
					{
						Dat1 = (buf[3] << 8) + buf[4];
						Dat2 = (buf[5] << 8) + buf[6];
						Dat3 = (buf[7] << 8) + buf[8];
						Dat4 = (buf[9] << 8) + buf[10];

						sys_write_vp(HMI_NowFile9, (u8 *)&Dat1, 1);
						sys_write_vp(HMI_NowFile9 + 1, (u8 *)&Dat2, 1);
						sys_write_vp(HMI_NowFile9 + 2, (u8 *)&Dat3, 1);
						sys_write_vp(HMI_NowFile9 + 3, (u8 *)&Dat4, 1);

						gCtrlPara.FileCount = 10; // �ļ���ȡ����ֵ
						gTime9 = 0;

						break;
					}
				}
				for (i = 0; i < 12; i++)
				{
					buf[i] = 0;
				}
			}
			break;
			}
		}
		break;

		case 0x05:
		{

			Addr = (RxBuf[(iRxReadPt++) & RxBufMask] << 8) + RxBuf[(iRxReadPt++) & RxBufMask]; // ��ַ
			switch (Addr)
			{
			// ��
			case HOST_On:
			{
				if ((u16)(RxWritePt - RxReadPt) < 5) //
				{
					RxReadPt -= 2;
					break;
				}
				buf[0] = 0x01;
				buf[1] = RxCommon;
				buf[2] = (Addr >> 8) && 0xFF;
				buf[3] = Addr && 0xFF;
				buf[4] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����1
				buf[5] = RxBuf[(iRxReadPt++) & RxBufMask]; // ����2

				Check_Nodbus = (RxBuf[(iRxReadPt++) & RxBufMask] << 8) + RxBuf[(iRxReadPt++) & RxBufMask]; // ����У��λ
				CheckNumber = crc16(buf, RxWritePt - RxReadPt);

				if (Check_Nodbus == CheckNumber)
				{
					//																		UART4_Sendbyte16(0x0505);
					//																		gCtrlPara.TouchFlag = 0;
				}

				for (i = 0; i < 12; i++)
				{
					buf[i] = 0;
				}
			}
			break;
			}
		}
		break;
		}

		RxReadPt = iRxReadPt; // ָ����ȷ�����󣬽�ָ��RxReadPtͬ��
	}
}

void ALm_Init(u16 Dat1)
{
	if (Dat1 & 0x1) // X��Ӳ��λ
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0xD6E1;
			gCtrlPara.Text2 = 0xCFDE;
			gCtrlPara.Text3 = 0xCEBB;
			gCtrlPara.Text4 = 0xB4A5;
			gCtrlPara.Text5 = 0xB7A2;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0X5820;
			gCtrlPara.Text2 = 0x4C69;
			gCtrlPara.Text3 = 0x6d69;
			gCtrlPara.Text4 = 0x7420;
			gCtrlPara.Text5 = 0x7472;
			gCtrlPara.Text6 = 0x6967;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x2) // Y��Ӳ��λ
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5920;
			gCtrlPara.Text2 = 0xCFDE;
			gCtrlPara.Text3 = 0xCEBB;
			gCtrlPara.Text4 = 0xB4A5;
			gCtrlPara.Text5 = 0xB7A2;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{

			gCtrlPara.Text1 = 0X5920;
			gCtrlPara.Text2 = 0x4C69;
			gCtrlPara.Text3 = 0x6d69;
			gCtrlPara.Text4 = 0x7420;
			gCtrlPara.Text5 = 0x7472;
			gCtrlPara.Text6 = 0x6967;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x4) // Z��Ӳ��λ
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5A20;
			gCtrlPara.Text2 = 0xCFDE;
			gCtrlPara.Text3 = 0xCEBB;
			gCtrlPara.Text4 = 0xB4A5;
			gCtrlPara.Text5 = 0xB7A2;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x5A20;
			gCtrlPara.Text2 = 0x4C69;
			gCtrlPara.Text3 = 0x6d69;
			gCtrlPara.Text4 = 0x7420;
			gCtrlPara.Text5 = 0x7472;
			gCtrlPara.Text6 = 0x6967;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x8) // U��Ӳ��λ
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5520;
			gCtrlPara.Text2 = 0xCFDE;
			gCtrlPara.Text3 = 0xCEBB;
			gCtrlPara.Text4 = 0xB4A5;
			gCtrlPara.Text5 = 0xB7A2;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x5A20;
			gCtrlPara.Text2 = 0x4C69;
			gCtrlPara.Text3 = 0x6d69;
			gCtrlPara.Text4 = 0x7420;
			gCtrlPara.Text5 = 0x7472;
			gCtrlPara.Text6 = 0x6967;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x100) // X�߿�Խ��
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5820;
			gCtrlPara.Text2 = 0xB1DF;
			gCtrlPara.Text3 = 0xBFF2;
			gCtrlPara.Text4 = 0xD4BD;
			gCtrlPara.Text5 = 0xBDE7;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x5820;
			gCtrlPara.Text2 = 0x4F76;
			gCtrlPara.Text3 = 0x6572;
			gCtrlPara.Text4 = 0x7363;
			gCtrlPara.Text5 = 0x616C;
			gCtrlPara.Text6 = 0x6500;
			gCtrlPara.Text7 = 0x0000;
		}

		//			if(gCtrlPara.Language == 0)
		//			{
		//				Change_Page(0x43);
		//				Send_ModBus(0x05,0x0003,0xFF00,0x00);	//�·���ͣ
		//			}
		//			else
		//			{
		//				Change_Page(0x1B);
		//				Send_ModBus(0x05,0x0003,0xFF00,0x00);	//�·���ͣ
		//			}
	}
	else if (Dat1 & 0x200) // Y�߿�Խ��
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5920;
			gCtrlPara.Text2 = 0xB1DF;
			gCtrlPara.Text3 = 0xBFF2;
			gCtrlPara.Text4 = 0xD4BD;
			gCtrlPara.Text5 = 0xBDE7;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x5920;
			gCtrlPara.Text2 = 0x4F76;
			gCtrlPara.Text3 = 0x6572;
			gCtrlPara.Text4 = 0x7363;
			gCtrlPara.Text5 = 0x616C;
			gCtrlPara.Text6 = 0x6500;
			gCtrlPara.Text7 = 0x0000;
		}

		//			if(gCtrlPara.Language == 0)
		//			{
		//				Change_Page(0x43);
		//				Send_ModBus(0x05,0x0003,0xFF00,0x00);	//�·���ͣ
		//			}
		//			else
		//			{
		//				Change_Page(0x1B);
		//				Send_ModBus(0x05,0x0003,0xFF00,0x00);	//�·���ͣ
		//			}
	}
	else if (Dat1 & 0x400) // ����������
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0xBBFA;
			gCtrlPara.Text2 = 0xC6F7;
			gCtrlPara.Text3 = 0xB1BB;
			gCtrlPara.Text4 = 0xB1A3;
			gCtrlPara.Text5 = 0xBBA4;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x5072;
			gCtrlPara.Text2 = 0x6F74;
			gCtrlPara.Text3 = 0x6563;
			gCtrlPara.Text4 = 0x7469;
			gCtrlPara.Text5 = 0x6F6E;
			gCtrlPara.Text6 = 0x2100;
			gCtrlPara.Text7 = 0x0000;
		}

		//			if(gCtrlPara.Language == 0)
		//			{
		//				Change_Page(0x42);
		//				Send_ModBus(0x05,0x0003,0xFF00,0x00);	//�·���ͣ
		//			}
		//			else
		//			{
		//				Change_Page(0x1A);
		//				Send_ModBus(0x05,0x0003,0xFF00,0x00);	//�·���ͣ
		//			}
	}
	else if (Dat1 & 0x800) // ˮ����
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0xCBAE;
			gCtrlPara.Text2 = 0xB1A3;
			gCtrlPara.Text3 = 0xBBA4;
			gCtrlPara.Text4 = 0xB4A5;
			gCtrlPara.Text5 = 0xB7A2;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x5761;
			gCtrlPara.Text2 = 0x7465;
			gCtrlPara.Text3 = 0x7220;
			gCtrlPara.Text4 = 0x616c;
			gCtrlPara.Text5 = 0x6172;
			gCtrlPara.Text6 = 0x6D00;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x1000) // ����δ��λ����ֹ�ӹ����������ļ�
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0xBBFA;
			gCtrlPara.Text2 = 0xC6F7;
			gCtrlPara.Text3 = 0xCEB4;
			gCtrlPara.Text4 = 0xB8B4;
			gCtrlPara.Text5 = 0xCEBB;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x4E6F;
			gCtrlPara.Text2 = 0x2048;
			gCtrlPara.Text3 = 0x6F6D;
			gCtrlPara.Text4 = 0x696E;
			gCtrlPara.Text5 = 0x6721;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x10) // ���±���
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0xB8DF;
			gCtrlPara.Text2 = 0xCEC2;
			gCtrlPara.Text3 = 0xB1A8;
			gCtrlPara.Text4 = 0xBEAF;
			gCtrlPara.Text5 = 0x0000;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x4869;
			gCtrlPara.Text2 = 0x6768;
			gCtrlPara.Text3 = 0x2074;
			gCtrlPara.Text4 = 0x656D;
			gCtrlPara.Text5 = 0x7020;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x20) // ��������
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0xD1CC;
			gCtrlPara.Text2 = 0xCEED;
			gCtrlPara.Text3 = 0xB1A8;
			gCtrlPara.Text4 = 0xBEAF;
			gCtrlPara.Text5 = 0x0000;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x536D;
			gCtrlPara.Text2 = 0x6F6B;
			gCtrlPara.Text3 = 0x6520;
			gCtrlPara.Text4 = 0x616C;
			gCtrlPara.Text5 = 0x6172;
			gCtrlPara.Text6 = 0x6D00;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x40) // ��Ȩ����
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0xCADA;
			gCtrlPara.Text2 = 0xC8A8;
			gCtrlPara.Text3 = 0xB5BD;
			gCtrlPara.Text4 = 0xC6DA;
			gCtrlPara.Text5 = 0x0000;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x4D61;
			gCtrlPara.Text2 = 0xCADA;
			gCtrlPara.Text3 = 0x7475;
			gCtrlPara.Text4 = 0x7269;
			gCtrlPara.Text5 = 0x7479;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x80) // XY�߿�Խ��
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5859;
			gCtrlPara.Text2 = 0xB1DF;
			gCtrlPara.Text3 = 0xBFF2;
			gCtrlPara.Text4 = 0xD4BD;
			gCtrlPara.Text5 = 0xBDE7;
			gCtrlPara.Text6 = 0x0000;
			gCtrlPara.Text7 = 0x0000;
		}
		else
		{
			gCtrlPara.Text1 = 0x5859;
			gCtrlPara.Text2 = 0x4F76;
			gCtrlPara.Text3 = 0x6572;
			gCtrlPara.Text4 = 0x7363;
			gCtrlPara.Text5 = 0x616C;
			gCtrlPara.Text6 = 0x6500;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x2000) // X������벻��
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5820;
			gCtrlPara.Text2 = 0xBBBA;
			gCtrlPara.Text3 = 0xB3E5;
			gCtrlPara.Text4 = 0xBEE0;
			gCtrlPara.Text5 = 0xC0EB;
			gCtrlPara.Text6 = 0xB2BB;
			gCtrlPara.Text7 = 0xD7E3;
		}
		else
		{
			gCtrlPara.Text1 = 0x5820;
			gCtrlPara.Text2 = 0x6E6F;
			gCtrlPara.Text3 = 0x7420;
			gCtrlPara.Text4 = 0x6275;
			gCtrlPara.Text5 = 0x6666;
			gCtrlPara.Text6 = 0x6572;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x4000) // Y������벻��
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5920;
			gCtrlPara.Text2 = 0xBBBA;
			gCtrlPara.Text3 = 0xB3E5;
			gCtrlPara.Text4 = 0xBEE0;
			gCtrlPara.Text5 = 0xC0EB;
			gCtrlPara.Text6 = 0xB2BB;
			gCtrlPara.Text7 = 0xD7E3;
		}
		else
		{
			gCtrlPara.Text1 = 0x5920;
			gCtrlPara.Text2 = 0x6E6F;
			gCtrlPara.Text3 = 0x7420;
			gCtrlPara.Text4 = 0x6275;
			gCtrlPara.Text5 = 0x6666;
			gCtrlPara.Text6 = 0x6572;
			gCtrlPara.Text7 = 0x0000;
		}
	}
	else if (Dat1 & 0x8000) // XY������벻��
	{
		if (gCtrlPara.Language == 0)
		{
			gCtrlPara.Text1 = 0x5859;
			gCtrlPara.Text2 = 0xBBBA;
			gCtrlPara.Text3 = 0xB3E5;
			gCtrlPara.Text4 = 0xBEE0;
			gCtrlPara.Text5 = 0xC0EB;
			gCtrlPara.Text6 = 0xB2BB;
			gCtrlPara.Text7 = 0xD7E3;
		}
		else
		{
			gCtrlPara.Text1 = 0x5859;
			gCtrlPara.Text2 = 0x206E;
			gCtrlPara.Text3 = 0x6F74;
			gCtrlPara.Text4 = 0x2062;
			gCtrlPara.Text5 = 0x7566;
			gCtrlPara.Text6 = 0x6665;
			gCtrlPara.Text7 = 0x7200;
		}
	}

	sys_write_vp(HMI_Warning_Text, (u8 *)&gCtrlPara.Text1, 1);
	sys_write_vp(HMI_Warning_Text + 1, (u8 *)&gCtrlPara.Text2, 1);
	sys_write_vp(HMI_Warning_Text + 2, (u8 *)&gCtrlPara.Text3, 1);
	sys_write_vp(HMI_Warning_Text + 3, (u8 *)&gCtrlPara.Text4, 1);
	sys_write_vp(HMI_Warning_Text + 4, (u8 *)&gCtrlPara.Text5, 1);
	sys_write_vp(HMI_Warning_Text + 5, (u8 *)&gCtrlPara.Text6, 1);
	sys_write_vp(HMI_Warning_Text + 6, (u8 *)&gCtrlPara.Text7, 1);

	Page_Change(0x18);
	Send_ModBus(0x05, 0x0003, 0xFF00, 0x00); // �·���ͣ
}