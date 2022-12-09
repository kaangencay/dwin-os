#ifndef _VARS_H_
#define _VARS_H_

#include "t5los8051.h"
#include "sys.h"

#define ARROW_KEYS_TIMER_NO 2
#define ARROW_KEYS_TIMER_CNT 500


extern char HMI_VERSION[9];

extern unsigned short gTime;
extern unsigned short gTime1;
extern unsigned short gTime2;
extern unsigned short gTime3;
extern unsigned short gTime4;
extern unsigned short gTime5;
extern unsigned short gTime6;
extern unsigned short gTime7;
extern unsigned short gTime8;
extern unsigned short gTime9;
extern unsigned short Contorlcount;

extern u16 HOST_Addr;

extern u16  Read_StatusFlag;
extern u8  	TP_Status_Old[8];
extern u8  	TP_Status_New[8];

typedef struct 
{
	u16 TouchFlag;
	u16 Value_X;
	u16 Value_Y;
	u16 Value;
	u16 Data;
	u16 Data1;
	u16 Data2;
	u16 Data3;
	u16 Language;
	u16 Language_Enter;
	

	u16 VendxCount;
	u16 VendxINFO;
	
	u32 Times;
	u32 Vondx_H;
	u16 Vondx_M;
	u16 Vondx_S;
	
	u16 System_Information;  //系统信息
	u32 System_InformationShow;
	
	u16 System_Restore;
	u8 Page;//页面号
	
	u16 Reset;
	
	u8 Touch_ScanFlag;
	u8 ContorlFlag;
	u8 Shake_handFlag;//握手
	u8 PowerOn_ResetFlag;//复位
	u8 SendFlag;//主界面读数据标志
	
	u8 PollingFlag;//轮询标志
	
	u16 Flag;
	u16 FreeFlag;
	u8 Running;
	u8 WorkSta;//工作状态  0x0000：系统空闲  0x0001：系统运行 0x0003：系统暂停 0x0005：系统完成
	u8 WorkStaHis;//
	u16 Text1;
	u16 Text2;
	u16 Text3;
	u16 Text4;

	u16 Text5;
	u16 Text6;
	u16 Text7;
//启动
	u16 RuiDa;
	u16 Start_Stoptouch;//触控
	u16 Start_Stop;
	u16 Stop;
	
	u16 Alarm_Value;//报警值
	u16 AlarmCount;

	
	u16 Processing_Speed;//加工速度
	u16 Point_Shoot;//点射
	
	u16 On;
	u16 Under;
	u16 Left;
	u16 Right;
	
	u16 Z_Positive;//Z轴正
	u16 Z_Peverse;
	u16 U_Positive;
	u16 U_Peverse;
	
	u16 Anchor_Point;//定位点
	
	u16 Go_Border;//走边框
	u16 Cartoon;//动画
	u16 Focusing;//寻焦
	u16 GO_Focusing;//
	
	
	
	u16 Restore_Data;//恢复参数
	u16 Data_Dackup;//参数备份
	
  u16 OutPut_Date;
	
	u16 Read_IP;
	u16 Write_IP;
	
	u16 WIFI_IP1;
	u16 WIFI_IP2;
	u16 WIFI_IP3;
	u16 WIFI_IP4;
	
	u16 Which_page;//界面编号
	u16 NumFlag1;
	
	u16 WorkTimeH;
	u16 WorkTimeM;
	u16 WorkTimeS;
	
	u16 Point_WriteFlag;//点动写标志
	u16 Point_ReadFlag;//点动写标志
	u16 Point_Read;
	u16 Point_Write;
	u16 Point_Set;
	u16 Read_Speed;
	u16 Read_SpeedL;
	u16 Read_Remove;
	
	u16 PointP_ReadFlag;
	u16 PointP_Write;
	u16 PointP_Read;
	u16 Point_Time;
	u16 Point_TimeL;
	u16 PulseLaserPower;//点射功率
	
	u16 Lock_password1;
	u16 Lock_password2;
	u16 Lock_password3;
	u16 Lock_password4;
	u16 UnLock_password1;
	u16 UnLock_password2;
	u16 UnLock_password3;
	u16 UnLock_password4;
	u16 Keyboard_lock;//键盘锁确认
	
	u16 FileFlag;
	u16 Total_FilesFlag;
	u16 Total_Files;  //文件总数
	u16 UTotal_FilesHis;  //文件总数
	u16 Total_FilesHis;  //文件总数
	u16 Current_Control;//当前界面控制
	u16 Current_ControlHis;//当前界面控制
	u16 Number_Control;//当前界面控制数
	u16 Remainder;//余数
	u16 Integer;//整数
	
	u16 FileCount;//
	
	u16 File_Icon;
	u16 File_IconHis;
	u16 File_IconNum;//文件显示当前数
	
	u16 Number;
	u16 NumberHis;
	u16 File_Num;//文件标号
	u16 File_NumHis;//文件标号
	u16 File_Control;//文件控制

	
	u16 Memory;//内存U盘
	u16 MemoryHis;
	u16 MemoryIcon;//图标
	u16 Delete; //删除
	u16 Copy;//复制
	u16 CopyFlag;//复制标志
	u16 Confirm;//确认
	
	u16 Icon_InitFlag;
	u16 Icon_Init;
	u16 Icon_InitHis;
	u16 IconCount;
	
	u16 LED_NOWFlag;
	
	u16 arrowLastPressVal;
	u8 arrowContPressFlag;
	
}gCtrlTable;

extern gCtrlTable gCtrlPara; 

void Vars_Initialize(void);

#endif 