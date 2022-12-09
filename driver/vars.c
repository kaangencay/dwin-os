#include "vars.h"


gCtrlTable gCtrlPara; 


char HMI_VERSION[9]; 




unsigned short gTime; //报警清除
unsigned short gTime1;//触控扫描定时
unsigned short gTime2;//联动参数
unsigned short gTime3;
unsigned short gTime4;
unsigned short gTime5;
unsigned short gTime6;
unsigned short gTime7;
unsigned short gTime8;
unsigned short gTime9;
unsigned short Contorlcount;


u16 HOST_Addr = 0;//主机地址（HMI）

u8 	TP_Status_Old[8]={0};
u8 	TP_Status_New[8]={0};
u16  Read_StatusFlag;

void Vars_Initialize(void)
{
	
	gCtrlPara.TouchFlag = 0;
	Read_StatusFlag = 0;
	gCtrlPara.Value_X = 0;
	gCtrlPara.Value_Y = 0;
	
	gCtrlPara.Value = 0;
	gCtrlPara.Language = 1;
	gCtrlPara.Language_Enter = 0;
	
	gCtrlPara.System_Information = 0;
	gCtrlPara.System_InformationShow = 0;
	

	gCtrlPara.VendxCount = 0;
	gCtrlPara.VendxINFO = 0;

	gCtrlPara.Times = 0;
	gCtrlPara.Vondx_H = 0;
	gCtrlPara.Vondx_M = 0;
	gCtrlPara.Vondx_S = 0;	
	
	gTime = 0; 
	gTime1 = 0;
	gTime2 = 0;
	gTime3 = 0;
	gTime4 = 0;
	gTime5 = 0;	
	gTime6 = 0;	
	gTime7 = 0;	
	gTime8 = 0;	
	gTime9 = 0;	
	
	gCtrlPara.System_Restore = 0;
	gCtrlPara.Page = 55;
	
  gCtrlPara.Touch_ScanFlag = 0;
  gCtrlPara.ContorlFlag = 0;
	gCtrlPara.Shake_handFlag = 0;
	gCtrlPara.PowerOn_ResetFlag = 0;
	gCtrlPara.SendFlag = 0;//主界面读数据标志
	
	gCtrlPara.PollingFlag = 0;//轮询标志
	
	gCtrlPara.Flag = 0;
	gCtrlPara.FreeFlag = 0;
	gCtrlPara.Running = 0;
	gCtrlPara.WorkSta = 2;
	gCtrlPara.WorkStaHis = 2;
	
	gCtrlPara.Text1 = 0;
	gCtrlPara.Text2 = 0;
	gCtrlPara.Text3 = 0;
	gCtrlPara.Text4 = 0;
	gCtrlPara.Text5 = 0;
	gCtrlPara.Text6 = 0;
	gCtrlPara.Text7 = 0;
//启动
	gCtrlPara.RuiDa = 0;
	gCtrlPara.Reset = 0;
	
	gCtrlPara.Start_Stoptouch = 0;
	gCtrlPara.Start_Stop = 0;
	gCtrlPara.Stop = 0;
	
	gCtrlPara.Alarm_Value = 0;//报警值
	gCtrlPara.AlarmCount = 0;
	gCtrlPara.Value = 0;
	
	gCtrlPara.Processing_Speed = 0;//加工速度
	gCtrlPara.Point_Shoot = 0;
	
	gCtrlPara.On = 0;
	gCtrlPara.Under = 0;
	gCtrlPara.Left = 0;
	gCtrlPara.Right = 0;
	
	gCtrlPara.Z_Positive = 0;
	gCtrlPara.Z_Peverse = 0;
	gCtrlPara.U_Positive = 0;
	gCtrlPara.U_Peverse = 0;
	
	gCtrlPara.Anchor_Point = 0;
	
	gCtrlPara.Go_Border = 0;//走边框
	gCtrlPara.Cartoon = 0;//动画
	gCtrlPara.Focusing = 0;//寻焦
	gCtrlPara.GO_Focusing = 0;
	

	gCtrlPara.Restore_Data = 0;//恢复参数
	gCtrlPara.Data_Dackup = 0;//参数备份
	
	gCtrlPara.Read_IP = 0;//读IP
	gCtrlPara.Write_IP = 0;//写IP
	
	gCtrlPara.WIFI_IP1 = 0;
	gCtrlPara.WIFI_IP2 = 0;
	gCtrlPara.WIFI_IP3 = 0;
	gCtrlPara.WIFI_IP4 = 0;
	
	gCtrlPara.NumFlag1 = 0;
	
	gCtrlPara.WorkTimeH = 0;
	gCtrlPara.WorkTimeM = 0;
	gCtrlPara.WorkTimeS = 0;
	
	gCtrlPara.Point_WriteFlag = 0;
	gCtrlPara.Point_Write = 0;
	gCtrlPara.Point_ReadFlag = 0;
	gCtrlPara.Point_Read = 0;
	gCtrlPara.Point_Set = 0;
	gCtrlPara.Read_Speed = 0;
	gCtrlPara.Read_SpeedL = 0;
	gCtrlPara.Read_Remove = 0;
	
	gCtrlPara.PointP_ReadFlag = 0;
	gCtrlPara.PointP_Write = 0;
	gCtrlPara.PointP_Read = 0;
	gCtrlPara.Point_Time = 0;
	gCtrlPara.Point_TimeL = 0;
	gCtrlPara.PulseLaserPower = 0;
	
	gCtrlPara.Lock_password1 = 0;
	gCtrlPara.Lock_password2 = 0;
	gCtrlPara.Lock_password3 = 0;
	gCtrlPara.Lock_password4 = 0;
	gCtrlPara.UnLock_password1 = 0;
	gCtrlPara.UnLock_password2 = 0;
	gCtrlPara.UnLock_password3 = 0;
	gCtrlPara.UnLock_password4 = 0;
	gCtrlPara.Keyboard_lock = 0;//键盘锁确认
		
	gCtrlPara.FileFlag = 0;
	gCtrlPara.Total_FilesFlag = 0;
	gCtrlPara.Total_Files = 0;//文件总数
	gCtrlPara.Total_FilesHis = 0;//文件总数
	gCtrlPara.UTotal_FilesHis = 0;//文件总数
	gCtrlPara.Current_Control = 1;//当前界面控制
	gCtrlPara.Current_ControlHis = 1;//当前界面控制
	gCtrlPara.Number_Control = 0;//当前界面控制数
	
	gCtrlPara.Remainder = 0;//余数
	gCtrlPara.Integer = 0;//整数
	gCtrlPara.FileCount = 0;//
	
	
	gCtrlPara.File_Icon = 0;//文件图标
	gCtrlPara.File_IconHis = 0;//文件图标
	gCtrlPara.File_IconNum = 0;//文件显示当前数
	
	gCtrlPara.Number = 0;
	gCtrlPara.NumberHis = 0;
	gCtrlPara.File_Num = 0;//文件标号
	gCtrlPara.File_NumHis = 0;//文件标号
	gCtrlPara.File_Control = 0;//文件控制
	gCtrlPara.NumberHis = 0;
	
	gCtrlPara.Memory = 1;//内存U盘
	gCtrlPara.MemoryHis = 1;
	gCtrlPara.MemoryIcon = 0;
	gCtrlPara.Delete = 0;
	gCtrlPara.Copy = 0;
	gCtrlPara.CopyFlag = 0;
	gCtrlPara.Confirm = 0;//确认
	
	
	gCtrlPara.Icon_InitFlag = 0;
	gCtrlPara.Icon_Init= 0;
	gCtrlPara.Icon_InitHis= 0;
	gCtrlPara.IconCount = 0;
	
	gCtrlPara.LED_NOWFlag = 0;
	
	gCtrlPara.arrowLastPressVal = 0;
	gCtrlPara.arrowContPressFlag = 1;
}

