#include "ui.h"
#include "uart.h"
#include "vars.h"
#include "timer.h"
#include "MYModBus.h"
#include "string.h"
#include "stdlib.h"

void P0Func(void)
{
		Contorl();
		Touch_Scan_0();

}

void P1Func(void)//1 - 6
{	
		Polling1_6();
		Touch_Scan_1_2_6();
}
void P2Func(void)
{
  	Polling1_6();
		Touch_Scan_1_2_6();
}

void P3Func(void)
{
		Touch_Scan_3();
		Coordinate_Control();//删除和确认
								
		File_Control();//读文件操作		
}
void P4Func(void)
{
		Touch_Scan_4();
}

void P5Func(void)
{
		Touch_Scan_5();
  	Touch_Scan_1_2_6();
}
void P9Func(void)
{
		gCtrlPara.Icon_InitFlag = 0;;//读状态标志
					
		gCtrlPara.VendxCount = 0;//读Vendx标志

		gCtrlPara.LED_NOWFlag = 1;//读亮度标志
}

void P10_12Func(void)
{
		Polling10_12();
		Touch_Scan_10_12();
}

void P13Func(void)
{
	Touch_Scan_13();
}

void P14Func(void)
{
	Touch_Scan_14();
}

//密码
void P16Func(void)
{
	Touch_Scan_16();
}

//锁-时间
void P18Func(void)
{
//	Touch_Scan_18();
}

void P15_17Func(void)
{
		Polling15_17();

}
code void (*PagePointer[])(u16 pageid)={
					P0Func,P1Func,P2Func,P3Func,P4Func,P5Func,P1Func,0,0,P9Func,P10_12Func,
					0,P10_12Func,P13Func,P14Func,P15_17Func,P16Func,P15_17Func,P18Func
};

void PageFunction(void)
{
	u16 pageid;

	EA = 0;
	ADR_H = 0x00;
	ADR_M = 0x00;
	ADR_L = 0x0a;
	ADR_INC = 1;
	RAMMODE = 0xAF;		//������
	while(!APP_ACK);
	APP_EN = 1;
	while(APP_EN);
	pageid = DATA3;
	pageid <<= 8;
	pageid |= DATA2;
	RAMMODE = 0;
	EA = 1;
	if(PagePointer[pageid] != 0)
	{
		PagePointer[pageid](pageid);
	}	
}

//握手和数据显示
void Contorl(void)
{
			if(gCtrlPara.Shake_handFlag == 0)
			{		
				  gTime3++;
					if(gTime3 >= 50)
					{
							gTime3 = 0;
							HOST_Addr = HOST_Shake_hands;
							Send_ModBus(0x03,HOST_Shake_hands,0x0000,0x01);//握手	
						
							gCtrlPara.Cartoon = 0;//动画
							sys_write_vp(0x100E,(u8*)&gCtrlPara.Cartoon,1);//动画开始
							
							gCtrlPara.Cartoon = 1;//动画
							sys_write_vp(0x100A,(u8*)&gCtrlPara.Cartoon,1);//动画停止
							sys_write_vp(0x100C,(u8*)&gCtrlPara.Cartoon,1);//动画停止
			  	}
		}
		else
		{
				if(gCtrlPara.PowerOn_ResetFlag == 0)
				{
					gTime3++;
					if(gTime3 >= 50)
					{
							gTime3 = 0;
							HOST_Addr = HOST_Power_On_Reset;
							Send_ModBus(0x03,HOST_Power_On_Reset,0x0000,0x01);//复位	
								
								gCtrlPara.Cartoon = 0;//动画						
								sys_write_vp(0x100C,(u8*)&gCtrlPara.Cartoon,1);//动画开始
//					     	sys_write_vp(0x100A,(u8*)&gCtrlPara.Cartoon,1);//动画开始
					}				
				}
		}

}

//轮询读取数据
void Polling1_6(void)
{
		if(gCtrlPara.Shake_handFlag == 1) //握手成功读取数据
		{		
						if(gCtrlPara.WorkSta == 0x0001)//运行
					{									
							switch(gCtrlPara.Flag)
							{
								case 0:
									HOST_Addr = HMI_WorkSta;
									
									Send_ModBus(0x03,HOST_WorkSta,0x0000,0x01);////工作状态
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 1;
										gTime4 = 0;
									}
								break;												
								case 1:
									HOST_Addr = HMI_Read_X_Value;
									Send_ModBus(0x03,HOST_Read_X_Value,0x0000,0x02);//读X坐标
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 2;
										gTime4 = 0;
									}
								break;
								case 2:
									HOST_Addr = HMI_Read_Y_Value;
									Send_ModBus(0x03,HOST_Read_Y_Value,0x0000,0x02);//读y坐标
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 3;
										gTime4 = 0;
									}
								break;
								case 3:
									HOST_Addr = HMI_Read_Z_Value;
									Send_ModBus(0x03,HOST_Read_Z_Value,0x0000,0x02);//读Z坐标
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 4;
										gTime4 = 0;
									}
								break;
								case 4:
									HOST_Addr = HOST_Limit_Infor;
									Send_ModBus(0x03,HOST_Limit_Infor,0x0000,0x01);//限位等报警信息
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 5;
										gTime4 = 0;
									}
								break;
								case 5:
									HOST_Addr = HMI_Work_TimeC;
									Send_ModBus(0x03,HOST_Work_TimeC,0x0000,0x01);//加工时间0x001C-0x001E
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 6;
										gTime4 = 0;
									}
								break;
								case 6:
									HOST_Addr = HMI_Work_TimeD;
									Send_ModBus(0x03,HOST_Work_TimeD,0x0000,0x01);//加工时间0x001C-0x001E
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 7;
										gTime4 = 0;
									}
								break;
								case 7:
									HOST_Addr = HMI_Work_TimeE;
									Send_ModBus(0x03,HOST_Work_TimeE,0x0000,0x01);//加工时间0x001C-0x001E
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 8;
										gTime4 = 0;
									}
								break;
								case 8:
									HOST_Addr = HMI_Processing_Speed;
									Send_ModBus(0x03,HOST_Processing_Speed,0x0000,0x01);//加工速度
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 9;
										gTime4 = 0;
									}
								break;
								case 9:
//												HOST_Addr = HMI_U_Coordinate;   
//												Send_ModBus(0x03,HOST_U_Coordinate,0x0000,0x04);//U显示
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.Flag = 10;
										gTime4 = 0;
									}
																			
								break;
								case 10:
									gCtrlPara.Flag =0;
								break;
							}
					}
					// && (gCtrlPara.SendFlag <= 2)
					else if((gCtrlPara.SendFlag == 1) && (gCtrlPara.WorkSta != 0x0001))//运行
					{
							switch(gCtrlPara.FreeFlag)
							{
								case 0:								
									if(gCtrlPara.GO_Focusing == 1)
									{
											gCtrlPara.FreeFlag = 1;
									}
									else
									{
										HOST_Addr = HMI_WorkSta;
										Send_ModBus(0x03,HOST_WorkSta,0x0000,0x01);////工作状态
										gTime4++;
										if(gTime4 >5)
										{
											gCtrlPara.FreeFlag = 1;
											gTime4 = 0;
										}
									}										
								break;
								case 1:
										gCtrlPara.FreeFlag = 2;
								break;												
								case 2:
									HOST_Addr = HMI_Read_X_Value;
									Send_ModBus(0x03,HOST_Read_X_Value,0x0000,0x02);//读X坐标
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.FreeFlag = 3;
										gTime4 = 0;
									}
								break;
								case 3:
									HOST_Addr = HMI_Read_Y_Value;
									Send_ModBus(0x03,HOST_Read_Y_Value,0x0000,0x02);//读y坐标
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.FreeFlag = 4;
										gTime4 = 0;
									}
								break;
								case 4:
									HOST_Addr = HMI_Read_Z_Value;
									Send_ModBus(0x03,HOST_Read_Z_Value,0x0000,0x02);//读Z坐标
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.FreeFlag = 5;
										gTime4 = 0;
									}
								break;
								case 5:
									if(gCtrlPara.GO_Focusing == 1)
									{
											gCtrlPara.FreeFlag = 6;
									}
									else
									{
										HOST_Addr = HMI_Current_File_Name;
										Send_ModBus(0x03,HOST_Current_File_Name,0x0000,0x04);//当前文件名0x0010-0x0013
										gTime4++;
										if(gTime4 >5)
										{
											gCtrlPara.FreeFlag = 6;
											gTime4 = 0;
										}
									}
								break;
								case 6:
										if(gCtrlPara.GO_Focusing == 1)
										{
											HOST_Addr = HMI_Go_Border;
											Send_ModBus(0x03,HOST_Go_Border,0x0000,0x01);//下发触控		
										}
										else
										{
												gCtrlPara.FreeFlag = 7;
										}
								break;														
								case 7:
//													HOST_Addr = HMI_U_Coordinate;   
//													Send_ModBus(0x03,HOST_U_Coordinate,0x0000,0x04);//U显示
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.FreeFlag = 8;
										gTime4 = 0;
									}
								
								break;
									
								case 8:
									HOST_Addr = HOST_Limit_Infor;
									Send_ModBus(0x03,HOST_Limit_Infor,0x0000,0x01);//限位等报警信息
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.FreeFlag = 9;
										gTime4 = 0;
									}
								break;	
								case 9:
									gCtrlPara.FreeFlag = 0;
									gCtrlPara.SendFlag++;
								break;
							}
					}
					else if(gCtrlPara.SendFlag > 1)
					{
						gCtrlPara.SendFlag++;
						if(gCtrlPara.SendFlag > 50)
						{
							gCtrlPara.SendFlag = 1; 
						}
					}

		}
}

//轮询读取数据
void Polling2(void)
{
		if(gCtrlPara.Shake_handFlag == 1) //握手成功读取数据
		{		
					
		}
}


void Polling10_12(void)
{
		if(gCtrlPara.Shake_handFlag == 1) //握手成功读取数据
		{		
				if(gCtrlPara.VendxCount < 2)
				{
						switch(gCtrlPara.IconCount)
						{		
							case 0:
									HOST_Addr = 0x0051;
									Send_ModBus(0x03,0x0051,0x0000,0x01);//下发触控	
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.IconCount = 1;
										gTime4 = 0;
									}
								break;
							
							case 1:
									HOST_Addr = 0x0052;
									Send_ModBus(0x03,0x0052,0x0000,0x01);//下发触控	
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.IconCount = 2;
										gTime4 = 0;
									}
								break;
							
							case 2:
									HOST_Addr = 0x0053;
									Send_ModBus(0x03,0x0053,0x0000,0x01);//下发触控	
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.IconCount = 3;
										gTime4 = 0;
									}
								break;
							
							case 3:
									HOST_Addr = 0x0054;
									Send_ModBus(0x03,0x0054,0x0000,0x01);//下发触控	
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.IconCount = 4;
										gTime4 = 0;
									}
								break;
							
							case 4:
									HOST_Addr = 0x0055;
									Send_ModBus(0x03,0x0055,0x0000,0x01);//下发触控	
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.IconCount = 5;
										gTime4 = 0;
									}
								break;
							
							case 5:
									HOST_Addr = 0x0056;
									Send_ModBus(0x03,0x0056,0x0000,0x01);//下发触控	
									gTime4++;
									if(gTime4 >5)
									{
										gCtrlPara.IconCount = 6;
										gTime4 = 0;
									}
								break;
							
							case 6:
									gCtrlPara.IconCount = 0;
									gCtrlPara.VendxCount++;
									if(gCtrlPara.VendxCount >1)
									{
											gCtrlPara.VendxCount = 2;
									}
								break;
						}
				}
		}
}


void Polling15_17(void)
{
		
		if(gCtrlPara.Shake_handFlag == 1) //握手成功读取数据
		{		
									if(gCtrlPara.VendxINFO < 2)
									{
											switch(gCtrlPara.IconCount)
											{		
												case 0:
														HOST_Addr = 0x0032;
														Send_ModBus(0x03,0x0032,0x0000,0x02);//下发触控	 主板信息
														gTime4++;
														if(gTime4 >5)
														{
															gCtrlPara.IconCount = 1;
															gTime4 = 0;
														}
													break;
												
												case 1:
														HOST_Addr = 0x0465;
														Send_ModBus(0x03,0x0465,0x0000,0x02);//下发触控	累计开机时间
														gTime4++;
														if(gTime4 >5)
														{
															gCtrlPara.IconCount = 2;
															gTime4 = 0;
														}
													break;
												
												case 2:
														HOST_Addr = 0x0467;
														Send_ModBus(0x03,0x0467,0x0000,0x02);//下发触控	累计加工时间
														gTime4++;
														if(gTime4 >5)
														{
															gCtrlPara.IconCount = 3;
															gTime4 = 0;
														}
													break;
												
												case 3:
														HOST_Addr = 0x0469;
														Send_ModBus(0x03,0x0469,0x0000,0x02);//下发触控	前次加工时间
														gTime4++;
														if(gTime4 >5)
														{
															gCtrlPara.IconCount = 4;
															gTime4 = 0;
														}
													break;
												
												case 4:
														HOST_Addr = 0x046B;
														Send_ModBus(0x03,0x046B,0x0000,0x02);//下发触控	累计出光时间
														gTime4++;
														if(gTime4 >5)
														{
															gCtrlPara.IconCount = 5;
															gTime4 = 0;
														}
													break;
																												
												case 5:
														HOST_Addr = 0x046D;
														Send_ModBus(0x03,0x046D,0x0000,0x02);//下发触控	 累计加工次数
														gTime4++;
														if(gTime4 >5)
														{
															gCtrlPara.IconCount = 6;
															gTime4 = 0;
														}
													break;

													//01 03 04 0A 00 01 A5 38 														
												case 6:
														HOST_Addr = 0x046F;
														Send_ModBus(0x03,0x046F,0x0000,0x02);//下发触控	 X轴累计行程
														gTime4++;
														if(gTime4 >5)
														{
															gCtrlPara.IconCount = 7;
															gTime4 = 0;
														}
													break;

												//01 03 04 0B 00 01 F4 F8														
												case 7:
														HOST_Addr = 0x0471;
														Send_ModBus(0x03,0x0471,0x0000,0x02);//下发触控	Y轴累计行程
														gTime4++;
														if(gTime4 >5)
														{
															gCtrlPara.IconCount = 8;
															gTime4 = 0;
														}
													break;	
 
//												//01 03 04 0C 00 02 05 38														
//												case 8:
//														HOST_Addr = 0x040C;
//														Send_ModBus(0x03,0x040C,0x0000,0x04);//下发触控	 系统锁定时间
//														gTime4++;
//														if(gTime4 >5)
//														{
//															gCtrlPara.IconCount = 9;
//															gTime4 = 0;
//														}
//													break;
											
												case 8:

														gCtrlPara.IconCount = 0;
														gCtrlPara.VendxINFO++;
														if(gCtrlPara.VendxINFO >1)
														{
																gCtrlPara.VendxINFO = 2;
														}
													break;
											}
									}		
									else
									{
										gCtrlPara.VendxINFO++;
										if(gCtrlPara.VendxINFO >200)
										{
												gCtrlPara.VendxINFO = 0;
										}
					
									}
		}
}

void Touch_Scan_0(void)
{
	u8 i;
		//停止复位
		for(i = 0;i < 5;i++)
		{
			sys_read_vp(RuiDa_ACS,(u8*)&gCtrlPara.Stop,1);	//读触控
		}
		if(gCtrlPara.Stop == 0x005A)			
		{
			sys_read_vp(RuiDa_ACS,(u8*)&gCtrlPara.Stop,1);	//读触控
			if(gCtrlPara.Stop == 0x005A)
			{  										
					Send_ModBus(0x05,0x0003,0xFF00,0x00);	//下发触控
					gCtrlPara.Stop = 0;
					sys_write_vp(RuiDa_ACS,(u8*)&gCtrlPara.Stop,1);//触控清零
			}
		}
}


void Touch_Scan_3(void)
{
	u8 i;
	u8 j;			
											//文件图标显示1
										  for(i = 0;i < 7;i++)
											{
												sys_read_vp(HMI_Key_Control+i,(u8*)&gCtrlPara.File_Icon,1);	//读触控
											
													if(gCtrlPara.File_Icon == 0x0001)			
													{
															for(j = 0;j < 7;j++)
															{
																sys_write_vp(HMI_File_Icon + j,(u8*)&gCtrlPara.File_IconHis,1);//图标清零		
															}

															sys_write_vp(HMI_File_Icon + i,(u8*)&gCtrlPara.File_Icon,1);//图标写1	
															gCtrlPara.File_IconNum = i+1;//当前界面文件号
															gCtrlPara.File_Num = gCtrlPara.Number_Control*7 + gCtrlPara.File_IconNum;//选中文件标号
															
															
															sys_write_vp(HMI_Key_Control + i,(u8*)&gCtrlPara.File_IconHis,1);//触控清零		
													}
								  		}
											
											//读当前控制页
											for(i = 0;i < 5;i++)
											{
												sys_read_vp(HMI_Current_Control,(u8*)&gCtrlPara.Current_Control,1);
											}							
											if(gCtrlPara.Current_Control != gCtrlPara.Current_ControlHis)
											{
												gCtrlPara.Number_Control = gCtrlPara.Current_Control -1;
												
												if(gCtrlPara.Number_Control <= gCtrlPara.Integer)
												{
													gCtrlPara.FileFlag = 0;//读文件标志	
												}
												else
												{
													gCtrlPara.Current_ControlHis = gCtrlPara.Current_Control;
												}
												
												gCtrlPara.File_Num = gCtrlPara.Number_Control*7 + gCtrlPara.File_IconNum ;//选中文件标号
																								
																								
											}
									
																
}

void Touch_Scan_4(void)
{
	
	u8 i;
								gCtrlPara.FileFlag = 0;//读文件标志
								gCtrlPara.Icon_InitFlag = 0;;//读状态标志
								gCtrlPara.VendxINFO = 0;//读INFO标志
					
								//复位
								for(i = 0;i < 5;i++)
								{
									sys_read_vp(HMI_Reset_Enter,(u8*)&gCtrlPara.Reset,1);	//读触控
								}
								if(gCtrlPara.Reset == 0x0001)			
								{
									sys_read_vp(HMI_Reset_Enter,(u8*)&gCtrlPara.Reset,1);	//读触控
									if(gCtrlPara.Reset == 0x0001)
									{  							
											gCtrlPara.Shake_handFlag = 0;
											gCtrlPara.PowerOn_ResetFlag = 0;
											Page_Change(0x00);
										
											gCtrlPara.Reset = 0;
											sys_write_vp(HMI_Reset_Enter,(u8*)&gCtrlPara.Reset,1);//触控清零
									}
								}
								
								//3301
								for(i = 0;i < 5;i++)
								{
									sys_read_vp(0x3301,(u8*)&gCtrlPara.Reset,1);	//读触控
								}
								if(gCtrlPara.Reset == 0x0001)			
								{
									sys_read_vp(0x3301,(u8*)&gCtrlPara.Reset,1);	//读触控
									if(gCtrlPara.Reset == 0x0001)
									{  										
											Page_Change(0x08);
										
											gCtrlPara.Reset = 0;
											sys_write_vp(0x3301,(u8*)&gCtrlPara.Reset,1);//触控清零
									}
								}
								
								//读IP
								for(i = 0;i < 5;i++)
								{
									sys_read_vp(HMI_Read_WIFI_IP,(u8*)&gCtrlPara.Read_IP,1);	//读触控
								}
								if(gCtrlPara.Read_IP == 0x005A)			
								{

									sys_read_vp(HMI_Read_WIFI_IP,(u8*)&gCtrlPara.Read_IP,1);	//读触控
									if(gCtrlPara.Read_IP == 0x005A)
									{
											HOST_Addr = HMI_Read_WIFI_IP;
										
											Send_ModBus(0x03,HOST_Read_WIFI_IP,0x0000,0x04);//下发触控
											gCtrlPara.Read_IP = 0;
											sys_write_vp(HMI_Read_WIFI_IP,(u8*)&gCtrlPara.Read_IP,1);//触控清零		
									}				
								}

}

void Touch_Scan_5(void)
{
	u8 i;
									//点射(第一次)
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x200B,(u8*)&gCtrlPara.On,1);	//读触控
									}
									if(gCtrlPara.On == 0x005A)			
									{
										sys_read_vp(0x200B,(u8*)&gCtrlPara.On,1);	//读触控
										if(gCtrlPara.On == 0x005A)
										{
											gCtrlPara.TouchFlag = 1;
												Send_ModBus(0x05,0x0013,0xFF00,0x00);//下发触控
												gCtrlPara.On = 0;
												sys_write_vp(0x200B,(u8*)&gCtrlPara.On,1);//触控清零										
										}				
									}
									
									//点射（松开）
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x200C,(u8*)&gCtrlPara.On,1);	//读触控
									}
									if(gCtrlPara.On == 0x005A)			
									{
										sys_read_vp(0x200C,(u8*)&gCtrlPara.On,1);	//读触控
										if(gCtrlPara.On == 0x005A)
										{
											gCtrlPara.TouchFlag = 1;
												Send_ModBus(0x05,0x0013,0x0000,0x00);//下发触控
												gCtrlPara.On = 0;
												sys_write_vp(0x200C,(u8*)&gCtrlPara.On,1);//触控清零	
										}				
									}
						
}

void Touch_Scan_1_2_6(void)
{
	
	u8 i;
									gCtrlPara.FileFlag = 0;//读文件标志

					
									//走边框	//0x1018
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(HMI_Go_Border,(u8*)&gCtrlPara.Go_Border,1);	//读触控
									}
									if(gCtrlPara.Go_Border == 0x005A)			
									{
										sys_read_vp(HMI_Go_Border,(u8*)&gCtrlPara.Go_Border,1);	//读触控
										if(gCtrlPara.Go_Border == 0x005A)
										{
											gCtrlPara.TouchFlag = 1;
											HOST_Addr = HMI_Go_Border;
											gCtrlPara.GO_Focusing = 1;
											
											Send_ModBus(0x05,HOST_Start_Go_Border,0xFF00,0x00);//下发触控
											gCtrlPara.Go_Border = 0;
											sys_write_vp(HMI_Go_Border,(u8*)&gCtrlPara.Go_Border,1);//触控清零
										}				
									}
									
									//定位点101B
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(HMI_Anchor_Point,(u8*)&gCtrlPara.Anchor_Point,1);	//读触控
									}
									if(gCtrlPara.Anchor_Point == 0x005A)			
									{
										sys_read_vp(HMI_Anchor_Point,(u8*)&gCtrlPara.Anchor_Point,1);	//读触控
										if(gCtrlPara.Anchor_Point == 0x005A)
										{
												Send_ModBus(0x05,HOST_Anchor_Point,0xFF00,0x00);//下发触控
												gCtrlPara.Anchor_Point = 0;
												sys_write_vp(HMI_Anchor_Point,(u8*)&gCtrlPara.Anchor_Point,1);//触控清零		
										}				
									}
												
									//启动/暂停
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(HMI_Start_Stoptouch,(u8*)&gCtrlPara.Start_Stoptouch,1);	//读触控
									}
									if(gCtrlPara.Start_Stoptouch == 0x5A)			
									{
										sys_read_vp(HMI_Start_Stoptouch,(u8*)&gCtrlPara.Start_Stoptouch,1);	//读触控
										if(gCtrlPara.Start_Stoptouch == 0x5A)
										{	
											Send_ModBus(0x05,0x0000,0xFF00,0x00);	//下发触控
											gCtrlPara.Start_Stoptouch = 0;
											sys_write_vp(HMI_Start_Stoptouch,(u8*)&gCtrlPara.Start_Stoptouch,1);//触控清零		
											
											gCtrlPara.SendFlag = 1; 
											if((gCtrlPara.WorkSta == 0) || (gCtrlPara.WorkSta == 3) || (gCtrlPara.WorkSta == 5))
											{
//											gCtrlPara.WorkSta = 1;//运行												
												gCtrlPara.Start_Stop = 1;
												sys_write_vp(HMI_Start_Stop,(u8*)&gCtrlPara.Start_Stop,1);//图标显示
											}
											else
											{
//											gCtrlPara.WorkSta = 3;//暂停											
												gCtrlPara.Start_Stop = 0;
												sys_write_vp(HMI_Start_Stop,(u8*)&gCtrlPara.Start_Stop,1);//图标显示
											}

										}				
									}
									
									
									//停止(工作状态为空闲)
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(HMI_Stop,(u8*)&gCtrlPara.Stop,1);	//读触控
									}
									if(gCtrlPara.Stop == 1)			
									{
										sys_read_vp(HMI_Stop,(u8*)&gCtrlPara.Stop,1);	//读触控
										if(gCtrlPara.Stop == 1)
										{  
												Send_ModBus(0x05,0x0003,0xFF00,0x00);	//下发触控
												gCtrlPara.Stop = 0;
												sys_write_vp(HMI_Stop,(u8*)&gCtrlPara.Stop,1);//触控清零	
												gCtrlPara.SendFlag = 1; 
											
												gCtrlPara.Start_Stop = 0;
												sys_write_vp(HMI_Start_Stop,(u8*)&gCtrlPara.Start_Stop,1);//图标显示
												
												gCtrlPara.WorkTimeH = 0;
												sys_write_vp(HMI_Work_TimeC,(u8*)&gCtrlPara.WorkTimeH,1);
												gCtrlPara.WorkTimeM = 0;
												sys_write_vp(HMI_Work_TimeD,(u8*)&gCtrlPara.WorkTimeM,1);
												gCtrlPara.WorkTimeS = 0;
												sys_write_vp(HMI_Work_TimeE,(u8*)&gCtrlPara.WorkTimeS,1);
											
												gCtrlPara.Processing_Speed = 0;
												sys_write_vp(HMI_Processing_Speed,(u8*)&gCtrlPara.Processing_Speed,1);//加工速度显示
											
												Page_Change(0x01);
	
//										gCtrlPara.Cartoon = 0;//动画
//										sys_write_vp(HMI_Cartoon,(u8*)&gCtrlPara.Cartoon,1);//动画停止
											
											gCtrlPara.GO_Focusing = 0;//走边框-寻焦
										}				
									}
									
									/**********************************************************************************/
								if(gCtrlPara.Shake_handFlag == 1) //握手成功读取数据
								{
									/* Arrow Keys Continuous Press Check */
									if (GetTimeOutFlag(ARROW_KEYS_TIMER_NO) && (gCtrlPara.arrowContPressFlag == 0))
									{
										Send_ModBus(0x05,gCtrlPara.arrowLastPressVal,0xFF00,0x00);//下发触控
										delay_ms(50);
										gCtrlPara.arrowContPressFlag = 1;
									}


									//上(第一次) Up First Press
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2002,(u8*)&gCtrlPara.On,1);	//读触控
									}
									if(gCtrlPara.On == 0x005A)			
									{
										sys_read_vp(0x2002,(u8*)&gCtrlPara.On,1);	//读触控
										if(gCtrlPara.On == 0x005A)
										{											
											Send_ModBus(0x05,0x0004,0xFF00,0x00);//下发触控
											delay_ms(50);
											Send_ModBus(0x05,0x0004,0x0000,0x00);
											gCtrlPara.On = 0;
											sys_write_vp(0x2002,(u8*)&gCtrlPara.On,1);//触控清零		

											gCtrlPara.arrowContPressFlag = 0;
											gCtrlPara.arrowLastPressVal = 0x0004;
											StartTimer(ARROW_KEYS_TIMER_NO, ARROW_KEYS_TIMER_CNT);
											gCtrlPara.SendFlag = 1; 
										}				
									}
									
									//上（松开） Up Release
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2003,(u8*)&gCtrlPara.On,1);	//读触控
									}
									if(gCtrlPara.On == 0x005A)			
									{
										sys_read_vp(0x2003,(u8*)&gCtrlPara.On,1);	//读触控
										if(gCtrlPara.On == 0x005A)
										{
											KillTimer(ARROW_KEYS_TIMER_NO);
											gCtrlPara.arrowLastPressVal = 0;
											gCtrlPara.arrowContPressFlag = 0;
											Send_ModBus(0x05,0x0004,0x0000,0x00);//下发触控
											gCtrlPara.On = 0;
											sys_write_vp(0x2003,(u8*)&gCtrlPara.On,1);//触控清零		
										}				
									}
									
									//下(第一次) Down First Press
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2004,(u8*)&gCtrlPara.Under,1);	//读触控
									}
									if(gCtrlPara.Under == 0x005A)			
									{
										sys_read_vp(0x2004,(u8*)&gCtrlPara.Under,1);	//读触控
										if(gCtrlPara.Under == 0x005A)
										{
											Send_ModBus(0x05,0x0005,0xFF00,0x00);//下发触控
											delay_ms(50);
											Send_ModBus(0x05,0x0005,0x0000,0x00);
											gCtrlPara.Under = 0;
											sys_write_vp(0x2004,(u8*)&gCtrlPara.Under,1);//触控清零	
											
											gCtrlPara.arrowContPressFlag = 0;
											gCtrlPara.arrowLastPressVal = 0x0005;
											StartTimer(ARROW_KEYS_TIMER_NO, ARROW_KEYS_TIMER_CNT);
											gCtrlPara.SendFlag = 1; 
										}				
									}
									
									//下（松开） Down Release
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2005,(u8*)&gCtrlPara.Under,1);	//读触控
									}
									if(gCtrlPara.Under == 0x005A)			
									{
										sys_read_vp(0x2005,(u8*)&gCtrlPara.Under,1);	//读触控
										if(gCtrlPara.Under == 0x005A)
										{
											KillTimer(ARROW_KEYS_TIMER_NO);
											gCtrlPara.arrowLastPressVal = 0;
											gCtrlPara.arrowContPressFlag = 0;
											Send_ModBus(0x05,0x0005,0x0000,0x00);//下发触控
											gCtrlPara.Under = 0;
											sys_write_vp(0x2005,(u8*)&gCtrlPara.Under,1);//触控清零																	
										}				
									}
											
									//左(第一次) Left First Press
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2006,(u8*)&gCtrlPara.Left,1);	//读触控
									}
									if(gCtrlPara.Left == 0x005A)			
									{
										sys_read_vp(0x2006,(u8*)&gCtrlPara.Left,1);	//读触控
										if(gCtrlPara.Left == 0x005A)
										{
											Send_ModBus(0x05,0x0006,0xFF00,0x00);//下发触控
											delay_ms(10);
											Send_ModBus(0x05,0x0006,0x0000,0x00);
											gCtrlPara.Left = 0;
											sys_write_vp(0x2006,(u8*)&gCtrlPara.Left,1);//触控清零		
											
											gCtrlPara.arrowContPressFlag = 0;
											gCtrlPara.arrowLastPressVal = 0x0006;
											StartTimer(ARROW_KEYS_TIMER_NO, ARROW_KEYS_TIMER_CNT);
											gCtrlPara.SendFlag = 1; 
										}				
									}
									

									//左（松开） Left Release
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2007,(u8*)&gCtrlPara.Left,1);	//读触控
									}
									if(gCtrlPara.Left == 0x005A)			
									{
										sys_read_vp(0x2007,(u8*)&gCtrlPara.Left,1);	//读触控
										if(gCtrlPara.Left == 0x005A)
										{
											KillTimer(ARROW_KEYS_TIMER_NO);
											gCtrlPara.arrowLastPressVal = 0;
											gCtrlPara.arrowContPressFlag = 0;
											Send_ModBus(0x05,0x0006,0x0000,0x00);//下发触控
											gCtrlPara.Left = 0;
											sys_write_vp(0x2007,(u8*)&gCtrlPara.Left,1);//触控清零															
										}				
									}
									
									//右(第一次) Right First Press
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2008,(u8*)&gCtrlPara.Right,1);	//读触控
									}
									if(gCtrlPara.Right == 0x005A)			
									{
										sys_read_vp(0x2008,(u8*)&gCtrlPara.Right,1);	//读触控
										if(gCtrlPara.Right == 0x005A)
										{
											Send_ModBus(0x05,0x0007,0xFF00,0x00);//下发触控
											delay_ms(10);
											Send_ModBus(0x05,0x0007,0x0000,0x00);
											gCtrlPara.Right = 0;
											sys_write_vp(0x2008,(u8*)&gCtrlPara.Right,1);//触控清零		
										
											gCtrlPara.arrowContPressFlag = 0;
											gCtrlPara.arrowLastPressVal = 0x0007;
											StartTimer(ARROW_KEYS_TIMER_NO, ARROW_KEYS_TIMER_CNT);
											gCtrlPara.SendFlag = 1; 
										}				
									}
									

									//右（松开） Right Release
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2009,(u8*)&gCtrlPara.Right,1);	//读触控
									}
									if(gCtrlPara.Right == 0x005A)			
									{
										sys_read_vp(0x2009,(u8*)&gCtrlPara.Right,1);	//读触控
										if(gCtrlPara.Right == 0x005A)
										{
											KillTimer(ARROW_KEYS_TIMER_NO);
											gCtrlPara.arrowLastPressVal = 0;
											gCtrlPara.arrowContPressFlag = 0;
											Send_ModBus(0x05,0x0007,0x0000,0x00);//下发触控
											gCtrlPara.Right = 0;
											sys_write_vp(0x2009,(u8*)&gCtrlPara.Right,1);//触控清零		
										}				
									}
									
									//Z轴正(第一次)
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x200D,(u8*)&gCtrlPara.Z_Positive,1);	//读触控
									}
									if(gCtrlPara.Z_Positive == 0x005A)			
									{
										sys_read_vp(0x200D,(u8*)&gCtrlPara.Z_Positive,1);	//读触控
										if(gCtrlPara.Z_Positive == 0x005A)
										{
												Send_ModBus(0x05,0x0014,0xFF00,0x00);//下发触控
												gCtrlPara.Z_Positive = 0;
												sys_write_vp(0x200D,(u8*)&gCtrlPara.Z_Positive,1);//触控清零	
											
											gCtrlPara.SendFlag = 1; 
										}				
									}

									//Z轴正（松开）
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x200E,(u8*)&gCtrlPara.Z_Positive,1);	//读触控
									}
									if(gCtrlPara.Z_Positive == 0x005A)			
									{
										sys_read_vp(0x200E,(u8*)&gCtrlPara.Z_Positive,1);	//读触控
										if(gCtrlPara.Z_Positive == 0x005A)
										{
												Send_ModBus(0x05,0x0014,0x0000,0x00);//下发触控
												gCtrlPara.Z_Positive = 0;
												sys_write_vp(0x200E,(u8*)&gCtrlPara.Z_Positive,1);//触控清零
										}				
									}
									
									//Z轴负(第一次)
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x200F,(u8*)&gCtrlPara.Z_Peverse,1);	//读触控
									}
									if(gCtrlPara.Z_Peverse == 0x005A)			
									{
										sys_read_vp(0x200F,(u8*)&gCtrlPara.Z_Peverse,1);	//读触控
										if(gCtrlPara.Z_Peverse == 0x005A)
										{
												Send_ModBus(0x05,0x0015,0xFF00,0x00);//下发触控
												gCtrlPara.Z_Peverse = 0;
												sys_write_vp(0x200F,(u8*)&gCtrlPara.Z_Peverse,1);//触控清零
											
											gCtrlPara.SendFlag = 1; 
										}				
									}

									//Z轴负（松开）
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2010,(u8*)&gCtrlPara.Z_Peverse,1);	//读触控
									}
									if(gCtrlPara.Z_Peverse == 0x005A)			
									{
										sys_read_vp(0x2010,(u8*)&gCtrlPara.Z_Peverse,1);	//读触控
										if(gCtrlPara.Z_Peverse == 0x005A)
										{
												Send_ModBus(0x05,0x0015,0x0000,0x00);//下发触控
												gCtrlPara.Z_Peverse = 0;
												sys_write_vp(0x2010,(u8*)&gCtrlPara.Z_Peverse,1);//触控清零	
										}				
									}
									
									//U轴正(第一次)
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2011,(u8*)&gCtrlPara.U_Positive,1);	//读触控
									}
									if(gCtrlPara.U_Positive == 0x005A)			
									{
										sys_read_vp(0x2011,(u8*)&gCtrlPara.U_Positive,1);	//读触控
										if(gCtrlPara.U_Positive == 0x005A)
										{
												Send_ModBus(0x05,0x0016,0xFF00,0x00);//下发触控
												gCtrlPara.U_Positive = 0;
												sys_write_vp(0x2011,(u8*)&gCtrlPara.U_Positive,1);//触控清零		
											
											gCtrlPara.SendFlag = 1; 
										}				
									}

									//U轴正（松开）
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2012,(u8*)&gCtrlPara.U_Positive,1);	//读触控
									}
									if(gCtrlPara.U_Positive == 0x005A)			
									{

										sys_read_vp(0x2012,(u8*)&gCtrlPara.U_Positive,1);	//读触控
										if(gCtrlPara.U_Positive == 0x005A)
										{
												Send_ModBus(0x05,0x0016,0x0000,0x00);//下发触控
												gCtrlPara.U_Positive = 0;
												sys_write_vp(0x2012,(u8*)&gCtrlPara.U_Positive,1);//触控清零		
											
										}				
									}
									
									//U轴负(第一次)
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2013,(u8*)&gCtrlPara.U_Peverse,1);	//读触控
									}
									if(gCtrlPara.U_Peverse == 0x005A)			
									{
										sys_read_vp(0x2013,(u8*)&gCtrlPara.U_Peverse,1);	//读触控
										if(gCtrlPara.U_Peverse == 0x005A)
										{
												Send_ModBus(0x05,0x0017,0xFF00,0x00);//下发触控
												gCtrlPara.U_Peverse = 0;
												sys_write_vp(0x2013,(u8*)&gCtrlPara.U_Peverse,1);//触控清零		
											
											gCtrlPara.SendFlag = 1; 
										}				
									}

									//U轴负（松开）
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x2014,(u8*)&gCtrlPara.U_Peverse,1);	//读触控
									}
									if(gCtrlPara.U_Peverse == 0x005A)			
									{
										sys_read_vp(0x2014,(u8*)&gCtrlPara.U_Peverse,1);	//读触控
										if(gCtrlPara.U_Peverse == 0x005A)
										{
												Send_ModBus(0x05,0x0017,0x0000,0x00);//下发触控
												gCtrlPara.U_Peverse = 0;
												sys_write_vp(0x2014,(u8*)&gCtrlPara.U_Peverse,1);//触控清零		
										}				
									}
								}
}
void Touch_Scan_10_12(void)
{
	
	u8 i;
									for(i = 0;i < 5;i++)
									{
										sys_read_vp(0x10B1,(u8*)&gCtrlPara.Icon_Init,1);	//读触控
									}
									if(gCtrlPara.Icon_Init != gCtrlPara.Icon_InitHis)			
									{
										sys_read_vp(0x10B1,(u8*)&gCtrlPara.Icon_Init,1);	//读触控
										if(gCtrlPara.Icon_Init != gCtrlPara.Icon_InitHis)	
										{
											
											if(gCtrlPara.Icon_Init == 1)
											{
													HOST_Addr = 0x0051;
													Send_ModBus_10(0x10,0x0051,0x01,gCtrlPara.Icon_Init,0x2);
													delay_ms(5);
													Send_ModBus(0x03,0x0051,0x0000,0x01);//下发触控	
				
											}
											else if(gCtrlPara.Icon_Init == 2)
											{
													HOST_Addr = 0x0052;
													Send_ModBus_10(0x10,0x0052,0x01,gCtrlPara.Icon_Init,0x2);
												
													delay_ms(5);
													Send_ModBus(0x03,0x0052,0x0000,0x01);//下发触控	
											}
											else if(gCtrlPara.Icon_Init == 3)
											{
													HOST_Addr = 0x0053;
													Send_ModBus_10(0x10,0x0053,0x01,gCtrlPara.Icon_Init,0x2);
												
													delay_ms(5);
													Send_ModBus(0x03,0x0053,0x0000,0x01);//下发触控	
											}
											else if(gCtrlPara.Icon_Init == 4)
											{
													HOST_Addr = 0x0054;
													Send_ModBus_10(0x10,0x0054,0x01,gCtrlPara.Icon_Init,0x2);
												
													delay_ms(5);
													Send_ModBus(0x03,0x0054,0x0000,0x01);//下发触控	
											}
											else if(gCtrlPara.Icon_Init == 5)
											{
													HOST_Addr = 0x0055;
													Send_ModBus_10(0x10,0x0055,0x01,gCtrlPara.Icon_Init,0x2);
												
													delay_ms(5);
													Send_ModBus(0x03,0x0055,0x0000,0x01);//下发触控	
											}
											else if(gCtrlPara.Icon_Init == 6)
											{
													HOST_Addr = 0x0056;
													Send_ModBus_10(0x10,0x0056,0x01,gCtrlPara.Icon_Init,0x2);
												
													delay_ms(5);
													Send_ModBus(0x03,0x0056,0x0000,0x01);//下发触控	
											}
											
										sys_write_vp(0x10B1,(u8*)&gCtrlPara.Icon_InitHis,1);//触控清零			
										}
									}
}

//void Touch_Scan_10_12(void)
//{
//	u8 i;
   					//语言
//						for(i = 0;i < 10;i++)
//						{
//							sys_read_vp(HMI_Read_Language_Enter,(u8*)&gCtrlPara.Language_Enter,1);	//读触控
//						}
//						if(gCtrlPara.Language_Enter == 0x01)			
//						{
//							sys_read_vp(HMI_Read_Language_Enter,(u8*)&gCtrlPara.Language_Enter,1);	//读触控
//							if(gCtrlPara.Language_Enter == 0x01)
//							{
//								sys_read_vp(HMI_Read_Language,(u8*)&gCtrlPara.Language,1);	
//								Write_Nor_Flash();
//								sys_delay_ms(1);
//								if(gCtrlPara.Language == 0)
//									Change_Page(0x06);
//								else
//									Change_Page(0x1F);
//								sys_write_vp(HMI_Read_Language,(u8*)&gCtrlPara.Language,1);
//								
//								gCtrlPara.Language_Enter = 0;
//								sys_write_vp(HMI_Read_Language_Enter,(u8*)&gCtrlPara.Language_Enter,1);//触控清零		
//							}
//						}
//}
void Touch_Scan_13(void)
{	
	u8 i;
										if(gCtrlPara.LED_NOWFlag == 1)
										{
												for(i = 0;i < 5;i++)
												{
													sys_read_vp(LED_NOW,(u8*)&gCtrlPara.Value,1);	//读取当前亮度
												}
												gCtrlPara.Value = gCtrlPara.Value&0x00FF;
												
												if((gCtrlPara.Value>0) && (gCtrlPara.Value<= 0x14))
												{
														gCtrlPara.Data = 0x0001;												
														sys_write_vp(0x10A8,(u8*)&gCtrlPara.Data,1);
												}
												else if((gCtrlPara.Value>0x14) && (gCtrlPara.Value<= 0x32))
												{
														gCtrlPara.Data = 0x0002;												
														sys_write_vp(0x10A8,(u8*)&gCtrlPara.Data,1);
												}
												else if((gCtrlPara.Value>0x32) && (gCtrlPara.Value<= 0x64))
												{
														gCtrlPara.Data = 0x0003;												
														sys_write_vp(0x10A8,(u8*)&gCtrlPara.Data,1);
												}
												
												gCtrlPara.LED_NOWFlag = 0;
										}
			
										for(i = 0;i < 5;i++)
										{
											sys_read_vp(0x20E0,(u8*)&gCtrlPara.Value,1);	//读触控
										}
										if(gCtrlPara.Value == 0x0001)			
										{
											sys_read_vp(0x20E0,(u8*)&gCtrlPara.Value,1);	//读触控
											if(gCtrlPara.Value == 0x0001)
											{

												sys_read_vp(0x10A7,(u8*)&gCtrlPara.Data,1);	//读触控
												if(gCtrlPara.Data == 0)
												{
														sys_write_vp(0x3302,(u8*)&gCtrlPara.Data,1);
												}
												else if(gCtrlPara.Data == 1)
												{
														sys_write_vp(0x3302,(u8*)&gCtrlPara.Data,1);
												}
																							
												sys_read_vp(0x10A8,(u8*)&gCtrlPara.Data,1);	//读触控
												if(gCtrlPara.Data == 0x01)
												{
														gCtrlPara.Data = 0x1414;
														sys_write_vp(LED_CONFIG,(u8*)&gCtrlPara.Data,1);

												}
												else if(gCtrlPara.Data == 2)
												{
														gCtrlPara.Data = 0x3232;
														sys_write_vp(LED_CONFIG,(u8*)&gCtrlPara.Data,1);

												}
												else if(gCtrlPara.Data == 3)
												{
														gCtrlPara.Data = 0x6464;
														sys_write_vp(LED_CONFIG,(u8*)&gCtrlPara.Data,1);

												}
												
													gCtrlPara.Value = 0;
													sys_write_vp(0x20E0,(u8*)&gCtrlPara.Value,1);//触控清零																	
											}				
										}
										
}

void Touch_Scan_14(void)
{
			u8 i;
								//写IP
								for(i = 0;i < 5;i++)
								{
									sys_read_vp(HMI_Write_WIFI_IP,(u8*)&gCtrlPara.Write_IP,1);	//读触控
								}
								if(gCtrlPara.Write_IP == 0x0001)			
								{												
									sys_read_vp(HMI_Write_WIFI_IP,(u8*)&gCtrlPara.Write_IP,1);	//读触控
									if(gCtrlPara.Write_IP == 0x0001)
									{												
											Send_ModBus_IP();//发送IP地址
											delay_ms(1);
											gCtrlPara.Write_IP = 0;
											sys_write_vp(HMI_Write_WIFI_IP,(u8*)&gCtrlPara.Write_IP,1);//触控清零		
									}				
								}
}


void Touch_Scan_16(void)
{
			u8 i;
				//密码输入
				for(i = 0;i < 5;i++)
				{
					sys_read_vp(0x401F,(u8*)&gCtrlPara.Value,1);	//读触控
				}
				if(gCtrlPara.Value == 0x0001)			
				{
					sys_read_vp(0x401F,(u8*)&gCtrlPara.Value,1);	//读触控
					if(gCtrlPara.Value == 0x0001)
					{		
						sys_read_vp(0x4020,(u8*)&gCtrlPara.Data,1);	//读触控
						sys_read_vp(0x4021,(u8*)&gCtrlPara.Data1,1);	//读触控
						sys_read_vp(0x4022,(u8*)&gCtrlPara.Data2,1);	//读触控
						sys_read_vp(0x4023,(u8*)&gCtrlPara.Data3,1);	//读触控
						
						if((gCtrlPara.Data == 1)&&(gCtrlPara.Data1 == 2)&&(gCtrlPara.Data2 == 3)&&(gCtrlPara.Data3 == 3))
						{
								Page_Change(0x011);
						}
					}
				}
}
//void Touch_Scan_18(void)
//{
//			u8 i;
//	
//						for(i = 0;i < 10;i++)
//						{
//							sys_read_vp(HMI_Keyboard_lock,(u8*)&gCtrlPara.Keyboard_lock,1);	//读触控
//						}
//						if(gCtrlPara.Keyboard_lock == 0x01)			
//						{
//							sys_read_vp(HMI_Keyboard_lock,(u8*)&gCtrlPara.Keyboard_lock,1);	//读触控
//							if(gCtrlPara.Keyboard_lock == 0x01)
//							{
//								sys_read_vp(HMI_Lock_pas_Addr1,(u8*)&gCtrlPara.Lock_password1,1);
//								sys_read_vp(HMI_Lock_pas_Addr2,(u8*)&gCtrlPara.Lock_password2,1);
//								sys_read_vp(HMI_Lock_pas_Addr3,(u8*)&gCtrlPara.Lock_password3,1);
//								sys_read_vp(HMI_Lock_pas_Addr4,(u8*)&gCtrlPara.Lock_password4,1);

//								Send_ModBus08_10(0x10,0x0417,0x04,gCtrlPara.Lock_password1,gCtrlPara.Lock_password2,gCtrlPara.Lock_password3,gCtrlPara.Lock_password4,0x08);//下发时
//								
//								gCtrlPara.Keyboard_lock = 0;
//								sys_write_vp(HMI_Keyboard_lock,(u8*)&gCtrlPara.Keyboard_lock,1);//触控清零		
//								
//							}				
//						}
//}

//坐标控制（文件确认和删除）
void Coordinate_Control(void)
{
			sys_read_vp(TP_STATUS,TP_Status_New,4);	//读触控
			Read_StatusFlag=strcmp(TP_Status_New,TP_Status_Old);
			if(Read_StatusFlag)
			{			
				if(TP_Status_New[1] == 0x02)//抬起
				{
						gCtrlPara.Value_X = (TP_Status_New[2]<<8) + TP_Status_New[3];
						gCtrlPara.Value_Y = (TP_Status_New[4]<<8) + TP_Status_New[5];
						
						
						if((gCtrlPara.Value_X<=530)&&(gCtrlPara.Value_X>=460))//删除X(460-530)
						{
									// Y(210-320)110
									if((gCtrlPara.Value_Y >= 210)&&(gCtrlPara.Value_Y <= 320) && (gCtrlPara.File_IconNum == 1))
									{
										HOST_Addr = HMI_Delete;
										Send_ModBus_10(0x10,HOST_DeleteR,0x1,gCtrlPara.File_Num,0x2);//下发文件号	

										delay_ms(5);
										gCtrlPara.FileFlag = 0;//读文件标志
				
									}

									else if((gCtrlPara.Value_Y >= 330)&&(gCtrlPara.Value_Y <= 440) && (gCtrlPara.File_IconNum == 2))
									{
										HOST_Addr = HMI_Delete;
										Send_ModBus_10(0x10,HOST_DeleteR,0x1,gCtrlPara.File_Num,0x2);//下发文件号	

										delay_ms(5);
										gCtrlPara.FileFlag = 0;//读文件标志
									}
									else if((gCtrlPara.Value_Y >= 450)&&(gCtrlPara.Value_Y <= 550) && (gCtrlPara.File_IconNum == 3))
									{
										HOST_Addr = HMI_Delete;
										Send_ModBus_10(0x10,HOST_DeleteR,0x1,gCtrlPara.File_Num,0x2);//下发文件号	
										
										delay_ms(5);
										gCtrlPara.FileFlag = 0;//读文件标志
									}
									else if((gCtrlPara.Value_Y >= 570)&&(gCtrlPara.Value_Y <= 680) && (gCtrlPara.File_IconNum == 4))
									{
										HOST_Addr = HMI_Delete;
										Send_ModBus_10(0x10,HOST_DeleteR,0x1,gCtrlPara.File_Num,0x2);//下发文件号	

										delay_ms(5);
										gCtrlPara.FileFlag = 0;//读文件标志
									}
									else if((gCtrlPara.Value_Y >= 685)&&(gCtrlPara.Value_Y <= 790) && (gCtrlPara.File_IconNum == 5))
									{
										HOST_Addr = HMI_Delete;
										Send_ModBus_10(0x10,HOST_DeleteR,0x1,gCtrlPara.File_Num,0x2);//下发文件号	

										delay_ms(5);
										gCtrlPara.FileFlag = 0;//读文件标志
									}
									else if((gCtrlPara.Value_Y >= 800)&&(gCtrlPara.Value_Y <= 910) && (gCtrlPara.File_IconNum == 6))
									{
										HOST_Addr = HMI_Delete;
										Send_ModBus_10(0x10,HOST_DeleteR,0x1,gCtrlPara.File_Num,0x2);//下发文件号	

										delay_ms(5);
										gCtrlPara.FileFlag = 0;//读文件标志
									}
									else if((gCtrlPara.Value_Y >= 920)&&(gCtrlPara.Value_Y <= 1030) && (gCtrlPara.File_IconNum == 7))
									{
										HOST_Addr = HMI_Delete;
										Send_ModBus_10(0x10,HOST_DeleteR,0x1,gCtrlPara.File_Num,0x2);//下发文件号	

										delay_ms(5);
										gCtrlPara.FileFlag = 0;//读文件标志
									}
							
						}
						else  if((gCtrlPara.Value_X<=680)&&(gCtrlPara.Value_X>570))//X(570-680)确认
						{
									// Y(130-195)
									if((gCtrlPara.Value_Y >= 210)&&(gCtrlPara.Value_Y <= 320) && (gCtrlPara.File_IconNum == 1))
									{
										HOST_Addr = HMI_Confirm;
										Send_ModBus_10(0x10,HOST_Confirm,0x1,gCtrlPara.File_Num,0x2);//下发文件号	
										UART4_Sendbyte(0x01);
										Page_Change(0x01);
									}
									// Y(205-270)
									else if((gCtrlPara.Value_Y >= 330)&&(gCtrlPara.Value_Y <= 440) && (gCtrlPara.File_IconNum == 2))
									{
										HOST_Addr = HMI_Confirm;
										Send_ModBus_10(0x10,HOST_Confirm,0x1,gCtrlPara.File_Num,0x2);//下发文件号	
										
										UART4_Sendbyte(0x02);
										Page_Change(0x01);
									}
									else if((gCtrlPara.Value_Y >= 450)&&(gCtrlPara.Value_Y <= 550) && (gCtrlPara.File_IconNum == 3))
									{
										HOST_Addr = HMI_Confirm;
										Send_ModBus_10(0x10,HOST_Confirm,0x1,gCtrlPara.File_Num,0x2);//下发文件号	
										UART4_Sendbyte(0x03);
										Page_Change(0x01);
									}
									else if((gCtrlPara.Value_Y >= 570)&&(gCtrlPara.Value_Y <= 680) && (gCtrlPara.File_IconNum == 4))
									{
										HOST_Addr = HMI_Confirm;
										Send_ModBus_10(0x10,HOST_Confirm,0x1,gCtrlPara.File_Num,0x2);//下发文件号	
										UART4_Sendbyte(0x01);
										Page_Change(0x01);
									}
									else if((gCtrlPara.Value_Y >= 685)&&(gCtrlPara.Value_Y <= 790) && (gCtrlPara.File_IconNum == 5))
									{
										HOST_Addr = HMI_Confirm;
										Send_ModBus_10(0x10,HOST_Confirm,0x1,gCtrlPara.File_Num,0x2);//下发文件号	
										UART4_Sendbyte(0x01);
										Page_Change(0x01);
									}
									else if((gCtrlPara.Value_Y >= 800)&&(gCtrlPara.Value_Y <= 910) && (gCtrlPara.File_IconNum == 6))
									{
										HOST_Addr = HMI_Confirm;
										Send_ModBus_10(0x10,HOST_Confirm,0x1,gCtrlPara.File_Num,0x2);//下发文件号	
										UART4_Sendbyte(0x01);
										Page_Change(0x01);
									}
									else if((gCtrlPara.Value_Y >= 920)&&(gCtrlPara.Value_Y <= 1030) && (gCtrlPara.File_IconNum == 7))
									{
										HOST_Addr = HMI_Confirm;
										Send_ModBus_10(0x10,HOST_Confirm,0x1,gCtrlPara.File_Num,0x2);//下发文件号	
										UART4_Sendbyte(0x01);
										Page_Change(0x01);
									}
						}

							
						
//					for( i=0;i<9;i++)
//					{
//						UART4_Sendbyte(TP_Status_New[i]);
//					}
				
				}
					memcpy(TP_Status_Old,TP_Status_New,sizeof(TP_Status_New));//
			}
			else
			{

			}
		

}

//内存控制
void File_Control(void)
{
				u8 i;
	
				
				if(gCtrlPara.Memory == 0x0001)//内存
				{

			             if((gCtrlPara.FileFlag == 0) || (gCtrlPara.FileFlag == 1) || (gCtrlPara.Current_Control != gCtrlPara.Current_ControlHis))
									 {			
//											gTime6++;
//											if(gTime6 > 10)
//											{	
//												gTime6 = 0;																

													if(gCtrlPara.FileCount == 0)
													{																						
														//文件总数
														 HOST_Addr = HMI_Total_Files;
														 Send_ModBus(0x03,HOST_Total_Files,0x0000,0x01);													
													}

													if(gCtrlPara.Total_Files > 0)//有文件
													{
															if((gCtrlPara.Current_Control >= 1) && (gCtrlPara.Current_Control <= gCtrlPara.Integer))
															{
																		switch(gCtrlPara.FileCount)
																		{
																			case 1:
																			{
																					//一号文件
																				 gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile1;
																				 Send_ModBus(0x03,HOST_NowFile1+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 2;
																				}
																			}
																			break;
																			case 2:
																			{
																				//二号文件
																				gCtrlPara.Number++;
																				HOST_Addr = HMI_NowFile2;
																				Send_ModBus(0x03,HOST_NowFile2+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 3;
																				}
																			}
																			break;
							
																			case 3:
																			{
																				//三号文件
																				gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile3;
																				 Send_ModBus(0x03,HOST_NowFile3+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 4;
																				}
																			}
																			break;
																			case 4:
																			{
																				//四号文件
																				gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile4;
																				 Send_ModBus(0x03,HOST_NowFile4+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 5;
																				}
																			}
																			break;
																			case 5:
																			{
																				//五号文件
																				gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile5;
																				 Send_ModBus(0x03,HOST_NowFile5+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 6;
																				}
																			}
																			break;
																			case 6:
																			{
																				//六号文件
																				gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile6;
																				 Send_ModBus(0x03,HOST_NowFile6+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 7;
																				}
																			}
																			break;
																			case 7:
																			{
																				//七号文件
																				gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile7;
																				 Send_ModBus(0x03,HOST_NowFile7+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 8;
																				}
																			}
																			break;
																			case 8:
																			{
																					gCtrlPara.Current_ControlHis = gCtrlPara.Current_Control;		
															
																					gCtrlPara.FileFlag++;
																					if(gCtrlPara.FileFlag > 1)
																					{
																						gCtrlPara.FileFlag = 2;
//																						gCtrlPara.Total_FilesHis = gCtrlPara.Total_Files;
																					}
																					
																					gCtrlPara.Number = 0;		
																					gCtrlPara.NumberHis = 0;
																				
																					gCtrlPara.FileCount = 0;
																			}
																			break;
																		}
												    	}
															else if((gCtrlPara.Integer == 0) || ((gCtrlPara.Current_Control >= 1)&&(gCtrlPara.Current_Control == (gCtrlPara.Integer+1))))
															{
																		if(gCtrlPara.FileCount > gCtrlPara.Remainder)//步进值大于余数跳出
																	  {
																			gCtrlPara.Current_ControlHis = gCtrlPara.Current_Control;		
															
																			gCtrlPara.FileFlag++;
																			if(gCtrlPara.FileFlag > 1)
																			{
																				gCtrlPara.FileFlag = 2;
//																				gCtrlPara.Total_FilesHis = gCtrlPara.Total_Files;
																			}
																			
																			gCtrlPara.Number = 0;		
																			gCtrlPara.NumberHis = 0;
																			
																			
																			gCtrlPara.FileCount = 0;
																	  }
																		
																		switch(gCtrlPara.FileCount)
																		{ 
																			case 1:
																			{
																				//一号文件
																				 gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile1;
																				 Send_ModBus(0x03,HOST_NowFile1+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																					gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 2;
																				}
																			}
																			break;
																			case 2:
																			{
																				//二号文件
																				 gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile2;
																				 Send_ModBus(0x03,HOST_NowFile2+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 3;
																				}
																			}
																			break;
																			case 3:
																			{
																				//三号文件
																				 gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile3;
																				 Send_ModBus(0x03,HOST_NowFile3+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 4;
																				}
																			}
																			break;
																			case 4:
																			{
																				//四号文件
																				 gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile4;
																				 Send_ModBus(0x03,HOST_NowFile4+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 5;
																				}
																			}
																			break;												
																			case 5:
																			{
																				//五号文件
																				gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile5;
																				 Send_ModBus(0x03,HOST_NowFile5+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 6;
																				}
																			}
																			break;
																			case 6:
																			{
																				//六号文件
																				gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile6;
																				 Send_ModBus(0x03,HOST_NowFile6+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 7;
																				}
																			}
																			break;
																			case 7:
																			{
																				//七号文件
																				gCtrlPara.Number++;
																				 HOST_Addr = HMI_NowFile7;
																				 Send_ModBus(0x03,HOST_NowFile7+gCtrlPara.Number_Control*4*7,0x0000,0x04);
																				gTime9++;
																				if(gTime9 >= 1)
																				{
																						gTime9 = 0;
																						gCtrlPara.FileCount = 8;
																				}
																			}
																			break;
																		}																																																									
														}
														
														if(gCtrlPara.FileFlag == 0)
														{
																	if(gCtrlPara.NumberHis != gCtrlPara.Number)//有文件
																	{
																			for(i = 0;i<gCtrlPara.Number;i++)
																			{
																					gCtrlPara.File_Control = 0;//文件控制
																					sys_write_vp(HMI_File_Control+i,(u8*)&gCtrlPara.File_Control,1);
																				
																					sys_write_vp(HMI_File_Icon + i,(u8*)&gCtrlPara.File_IconHis,1);//文件图标触控清零																																										
																			}
																			for(i = 6;i>=gCtrlPara.Number;i--)
																			{	
																					gCtrlPara.File_Control = 1;//文件控制
																					sys_write_vp(HMI_File_Control+i,(u8*)&gCtrlPara.File_Control,1);
																			}
																			gCtrlPara.NumberHis = gCtrlPara.Number;
																	}
														}
									}
									else
									{			
													if(gCtrlPara.FileCount == 1)
													{
															//没有文件
															for(i=0;i<7;i++)
															{
																	gCtrlPara.File_Control = 1;//文件控制
																	sys_write_vp(HMI_File_Control + i,(u8*)&gCtrlPara.File_Control,1);
																
																	sys_write_vp(HMI_File_Icon + i,(u8*)&gCtrlPara.File_IconHis,1);//文件图标触控清零	
															}

															gCtrlPara.FileCount = 0;
													}	
									}
									
					

					 }
				}			
}

