#ifndef __PAGE_H__
#define __PAGE_H__
#include "sys.h"


void PageFunction(void);
void Contorl(void);
void Polling1_6(void);
void Polling2(void);
void Polling10_12(void);
void Polling15_17(void);

void Touch_Scan_0(void);
void Touch_Scan_3(void);
void Touch_Scan_4(void);
void Touch_Scan_5(void);
void Touch_Scan_1_2_6(void);
void Touch_Scan_10_12(void);
void Touch_Scan_13(void);
void Touch_Scan_14(void);
void Touch_Scan_16(void);
void Touch_Scan_18(void);


void Coordinate_Control(void);
void File_Control(void);

void Sta_Init(void);
void PageFunction(void);

//-----------------------板卡地址---------------------------------
#define  HOST_Shake_hands      			0x0046    //面板开机握手
#define  HOST_Power_On_Reset      	0x0031    //开机复位

#define  HOST_Start_Stop      			0x0000    //启动-暂停
#define  HOST_Stop      						0x0003     //停止
#define  HOST_On     				        0x0004    //上
#define  HOST_Under      						0x0005    //下
#define  HOST_Left      				 		0x0006     //左
#define  HOST_Right     						0x0007    //右

//#define  HOST_Anchor_Point					 0x0013			//点射
#define  HOST_Z_Positive     				 0x0014    //Z+
#define  HOST_Z_Peverse     				 0x0015    //Z-
#define  HOST_U_Positive      			 0x0016     //U+
#define  HOST_U_Peverse     				 0x0017    //U-
#define  HOST_U_Coordinate           0x0400       //U显示

#define	 HOST_Anchor_Point			    0x0025			//定位点

#define  HOST_Processing_Speed      0x0029    //加工速度
#define  HOST_Current_File_Name     0x0010  //当前文件名0x0010-0x0013

#define  HOST_Work_TimeC            0x001C  //加工时间0x001C-0x001E
#define  HOST_Work_TimeD            0x001D  //加工时间0x001C-0x001E
#define  HOST_Work_TimeE            0x001E  //加工时间0x001C-0x001E

#define  HOST_WorkSta         			0x001F     //工作状态

#define  HOST_Limit_Infor           0x0021    //限位等报警信息

#define	 HOST_Go_Border             0x0045      //走边框
#define	 HOST_Start_Go_Border       0x0011      //启动走边框


#define	 HOST_Focusing05            0x0028      //寻焦（05-FF00）
//#define	 HOST_Focusing              0x0047      //寻焦


#define  HOST_Restore_Data          0x0027    //恢复参数
#define  HOST_Data_Dackup           0x0027    //参数备份
#define  HMI_OutPut			            0x308D    //诊断->输出
#define  HOST_Read_WIFI_IP          0x0025    //读IP (03)
#define  HOST_Write_WIFI_IP         0x0025    //写IP(10)

#define  HOST_Read_Output           0x0024   //读输出状态(03)
#define  HOST_Read_Input          	0x0023   //读输入状态(03)

#define  HOST_Read_X_Value          0x002A    //读X (03)
#define  HOST_Read_Y_Value          0x002C    //读Y (03)
#define  HOST_Read_Z_Value          0x002E    //读Z (03)


//01 03 00 43 00 02 35 DF 01 03 00 04 00 01 C5 CB 01 03 00 09 00 02 14 09 
#define  HOST_Point_Set             0x0004    //点动点射设置（03）
#define  HOST_Read_Speed            0x0043    //读移动速度
#define  HOST_Read_Remove           0x0009    //点动距离
#define  HOST_Point_Time            0x0007    //点射时间
#define  HOST_PulseLaserPower       0x000D    //点射功率

#define  HOST_System_Information    0x0032   //系统信息触控

#define  HOSTU_Total_Files          0x0034      //U盘文件总数（03）
#define  HOST_Total_Files         0x000E      //内存文件总数（03）

#define  HOST_NowFile1            0x0063//一号文件  0x0063-0x0066 
#define  HOST_NowFile2            0x0067//二号文件  0x0067-0x006A
#define  HOST_NowFile3            0x006B//三号文件  0x006B-0x006E
#define  HOST_NowFile4            0x006F//四号文件
#define  HOST_NowFile5            0x0073//五号文件
#define  HOST_NowFile6            0x0077//六号文件
#define  HOST_NowFile7            0x007B//七号文件
#define  HOST_NowFile8            0x007F//八号文件
#define  HOST_NowFile9            0x0083//九号文件

#define  HOST_NowFile10           0x0087//十号文件
//...............................................................
#define  HOST_NowFile100          0X01EF//100号文件  0X01EF-0X01F2 

//U盘文件
#define  HOSTU_NowFile1            0x01F3//U盘一号文件  0x01F3-0x01F6
#define  HOSTU_NowFile2            0x01F7//U盘二号文件  0x01F7-0x01FA
#define  HOSTU_NowFile3            0x01FB//U盘三号文件
#define  HOSTU_NowFile4            0x01FF//U盘四号文件
#define  HOSTU_NowFile5            0x0203//U盘五号文件
#define  HOSTU_NowFile6            0x0207//U盘六号文件
#define  HOSTU_NowFile7            0x020B//U盘七号文件
#define  HOSTU_NowFile8            0x020F//U盘八号文件
#define  HOSTU_NowFile9            0x0213//U盘九号文件

#define  HOSTU_NowFile10           0x0217//U盘十号文件
//...............................................................
#define  HOSTU_NowFile100          0X037F//U盘100号文件  0X037F-0X0382


#define  HOST_UTotal_Files         0x0034     //U盘文件总数（03）
#define  HOST_UToR                 0x0035     //将u盘中的x号文件拷贝到内存
#define  HOST_RToU                 0x0036     //将内存中的x文件拷贝到u盘
#define  HOST_DeleteR              0x0037     //将内存中的x号文件删除
#define  HOST_DeleteU              0x0038     //将u盘中的x号文件删除

#define  HOST_Confirm              0x0000    //确认（10）


//-----------------------触摸屏地址---------------------------------
#define  RuiDa_ACS      				     0x2001     //启动-触控
#define  HMI_System_Restore  				 0x2030     //系统恢复中-显示

//01 03 08 D4 CB D0 D0 00 00 00 00 31 F6  -运行
//01 03 08 BF D5 CF D0 00 00 00 00 8A 03  -空闲
//01 03 08 CF B5 CD B3 CD EA B3 C9 82 F5  -系统完成
//01 03 08 D4 DD CD A3 00 00 00 00 40 71  -暂停

//主界面
#define  HMI_Start_Stoptouch      	 0x2000     //启动-暂停（触控）
#define  HMI_Start_Stop      				 0x3010     //启动-暂停（图标）
#define  HMI_WorkSta         				 0x301C     //工作状态301C-3020（文本显示） 0x0000：系统空闲  0x0001：系统运行 0x0003：系统暂停 0x0005：系统完成
#define  HMI_Stop      							 0x1011      //停止
#define  HMI_On     				         0x1012     //上
#define  HMI_Under      						 0x1013     //下
#define  HMI_Left      				 			 0x1014     //左
#define  HMI_Right     							 0x1015     //右
//#define  HMI_Anchor_Point					   0x1016			//点射
#define  HMI_Z_Positive     				 0x1017    //Z+
#define  HMI_Z_Peverse     					 0x1018    //Z-
#define  HMI_U_Positive      				 0x1019     //U+
#define  HMI_U_Peverse     					 0x101A    //U-
#define  HMI_U_Coordinate           0x3086    //U显示


#define	 HMI_Anchor_Point						 0x101B			//定位点

#define	 HMI_Reset                   0x20C7    //复位
#define	 HMI_Reset_Enter             0x3200    //复位确认

#define  HMI_Warning_Text    				 0x2023				//警告弹窗

#define	 HMI_Go_BorderIcon           0x20C6      //走边框图标
#define	 HMI_Go_Border               0x1018      //走边框
#define	 HMI_Cartoon                 0x20E2      //走边框动画地址（0-停止   1-开始）

#define	 HMI_FocusingIcon            0x20CB      //寻焦图标
#define	 HMI_Focusing                0x20C8      //寻焦
#define	 HMI_Cartoon                 0x20E2      //寻焦动画地址（0-停止   1-开始）


#define	 HMI_Diagnosis							 0x3076			//诊断
#define	 HMI_Diagnosis_Output				 0x3077			//诊断-输出
#define  HMI_Processing_Speed        0x302A    //加工速度-图标

#define  HMI_Current_File_Name       0x101D  //当前文件名0x101D-0X1020


#define  HMI_Work_TimeC            0x1021  //加工时间(03)0x001C-0x001E
#define  HMI_Work_TimeD            0x1022  //加工时间0x001C-0x001E
#define  HMI_Work_TimeE            0x1023  //加工时间0x001C-0x001E


#define  HMI_Restore_Data          0x1029    //恢复参数
#define  HMI_Data_Dackup           0x1029    //参数备份

#define  HMI_Read_WIFI_IP          0x102B    //读IP(03)
#define  HMI_Write_WIFI_IP         0x2078    //写IP(10)   01 10 00 25 00 04 08 00 C0 00 A8 00 01 00 64 CA 36 


#define  HMI_Point_Set             0x102D    //点动点射设置

#define  HMI_Read_Speed            0x1026    //移动速度
#define  HMI_Read_Remove           0x102E    //点动距离
#define  HMI_Point_Time            0x102F    //点射时间
#define  HMI_PulseLaserPower       0x1024    //点射功率

#define  HMI_Point_Write           0x201B    //写点动设置
#define  HMI_Point_Read            0x2019    //读点动设置

#define  HMI_PointP_Write           0x201C   //写点射设置01 10 00 43 00 02 04 00 00 01 F4 B7 9D
#define  HMI_PointP_Read            0x201A   //读点射设置

#define  HMI_System_Information     0x2040   //系统信息触控
#define  HMI_System_InformationShow 0x3078   //系统信息显示

#define  HMI_WIFI_IP1_Addr          0x3025    //WIFI_IP（触控-显示）
#define  HMI_WIFI_IP2_Addr          0x3026    
#define  HMI_WIFI_IP3_Addr          0x3027    
#define  HMI_WIFI_IP4_Addr          0x3028    

#define HMI_Overstep    0x0300    //边框越界
#define HMI_MProtect    0x0400     //机器保护


#define  HMI_Read_X_Value         0x3080    //读X (03)
#define  HMI_Read_Y_Value         0x3082    //读Y (03)
#define  HMI_Read_Z_Value         0x3084    //读Z (03)


#define  HMI_Read_Language        0x2030    //语言触控
#define  HMI_Read_Language_Enter  0x2031    //语言确认


#define  HMI_Lock_pas_Addr1          0x4030    
#define  HMI_Lock_pas_Addr2          0x4031    
#define  HMI_Lock_pas_Addr3          0x401C  
#define  HMI_Lock_pas_Addr4          0x401D  

#define  HMI_UnLock_pas_Addr1          0x3086   
#define  HMI_UnLock_pas_Addr2          0x3087    
#define  HMI_UnLock_pas_Addr3          0x3088    
#define  HMI_UnLock_pas_Addr4          0x3089 

#define  HMI_Keyboard_lock             0x401E    //键盘锁确认


#define  HMI_Total_Files          0x1031      //文件总数
#define  HMI_Residual_Files       0x3050      //最后一页剩余文件
#define  HMI_Current_Control      0x3051      //当前界面控制
#define  HMI_Totalpage_Number     0x304F       //文件总页码
#define  HMI_File_Icon            0x3052       //文件图标  0x3052-305A

#define  HMI_File_Num             0x305B       //文件标号  0x305b-3063
#define  HMI_File_Control         0x3064       //文件控制  0x3064-0x0306c

#define  HMI_Key_Control          0x306D       //按键控制 0x306d-0x03075

//显示文件
#define  HMI_NowFile1            0x302B//一号文件  0x302b-0x302e
#define  HMI_NowFile2            0x302F//二号文件  0x302f-0x3032
#define  HMI_NowFile3            0x3033//三号文件  0x3033-0x3036 
#define  HMI_NowFile4            0x3037//四号文件  0x3037-0x303A 
#define  HMI_NowFile5            0x303B//五号文件  0x303B-0x303E 
#define  HMI_NowFile6            0x303F//六号文件  0x303F-0x3042 
#define  HMI_NowFile7            0x3043//七号文件  0x3043-0x3046
#define  HMI_NowFile8            0x3047//八号文件  0x3047-0x304A
#define  HMI_NowFile9            0x304B//九号文件  0x304B-0x304E 


#define  HMI_RU_Memory           0x20A8    //内存U盘触控
#define  HMI_R_MemoryIcon        0x20A2    //内存图标
#define  HMI_U_MemoryIcon        0x20A4    //U盘图标
#define  HMI_Delete              0x20E1    //删除
#define  HMI_Copy                0x20A6    //复制触控
#define  HMI_Confirm             0x2020    //确认   //01 10 00 00 00 01 02 00 0A 26 57 





#define  Read_Cmd            0x83    //读参数指令
#define  Write_Cmd           0x82    //写参数指令


#endif

