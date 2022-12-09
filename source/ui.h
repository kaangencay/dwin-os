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

//-----------------------�忨��ַ---------------------------------
#define HOST_Shake_hands 0x0046    // ��忪������
#define HOST_Power_On_Reset 0x0031 // ������λ

#define HOST_Start_Stop 0x0000 // ����-��ͣ
#define HOST_Stop 0x0003       // ֹͣ
#define HOST_On 0x0004         // ��
#define HOST_Under 0x0005      // ��
#define HOST_Left 0x0006       // ��
#define HOST_Right 0x0007      // ��

// #define  HOST_Anchor_Point					 0x0013			//����
#define HOST_Z_Positive 0x0014   // Z+
#define HOST_Z_Peverse 0x0015    // Z-
#define HOST_U_Positive 0x0016   // U+
#define HOST_U_Peverse 0x0017    // U-
#define HOST_U_Coordinate 0x0400 // U��ʾ

#define HOST_Anchor_Point 0x0025 // ��λ��

#define HOST_Processing_Speed 0x0029  // �ӹ��ٶ�
#define HOST_Current_File_Name 0x0010 // ��ǰ�ļ���0x0010-0x0013

#define HOST_Work_TimeC 0x001C // �ӹ�ʱ��0x001C-0x001E
#define HOST_Work_TimeD 0x001D // �ӹ�ʱ��0x001C-0x001E
#define HOST_Work_TimeE 0x001E // �ӹ�ʱ��0x001C-0x001E

#define HOST_WorkSta 0x001F // ����״̬

#define HOST_Limit_Infor 0x0021 // ��λ�ȱ�����Ϣ

#define HOST_Go_Border 0x0045       // �߱߿�
#define HOST_Start_Go_Border 0x0011 // �����߱߿�

#define HOST_Focusing05 0x0028 // Ѱ����05-FF00��
// #define	 HOST_Focusing              0x0047      //Ѱ��

#define HOST_Restore_Data 0x0027  // �ָ�����
#define HOST_Data_Dackup 0x0027   // ��������
#define HMI_OutPut 0x308D         // ���->���
#define HOST_Read_WIFI_IP 0x0025  // ��IP (03)
#define HOST_Write_WIFI_IP 0x0025 // дIP(10)

#define HOST_Read_Output 0x0024 // �����״̬(03)
#define HOST_Read_Input 0x0023  // ������״̬(03)

#define HOST_Read_X_Value 0x002A // ��X (03)
#define HOST_Read_Y_Value 0x002C // ��Y (03)
#define HOST_Read_Z_Value 0x002E // ��Z (03)

// 01 03 00 43 00 02 35 DF 01 03 00 04 00 01 C5 CB 01 03 00 09 00 02 14 09
#define HOST_Point_Set 0x0004       // �㶯�������ã�03��
#define HOST_Read_Speed 0x0043      // ���ƶ��ٶ�
#define HOST_Read_Remove 0x0009     // �㶯����
#define HOST_Point_Time 0x0007      // ����ʱ��
#define HOST_PulseLaserPower 0x000D // ���书��

#define HOST_System_Information 0x0032 // ϵͳ��Ϣ����

#define HOSTU_Total_Files 0x0034 // U���ļ�������03��
#define HOST_Total_Files 0x000E  // �ڴ��ļ�������03��

#define HOST_NowFile1 0x0063 // һ���ļ�  0x0063-0x0066
#define HOST_NowFile2 0x0067 // �����ļ�  0x0067-0x006A
#define HOST_NowFile3 0x006B // �����ļ�  0x006B-0x006E
#define HOST_NowFile4 0x006F // �ĺ��ļ�
#define HOST_NowFile5 0x0073 // ����ļ�
#define HOST_NowFile6 0x0077 // �����ļ�
#define HOST_NowFile7 0x007B // �ߺ��ļ�
#define HOST_NowFile8 0x007F // �˺��ļ�
#define HOST_NowFile9 0x0083 // �ź��ļ�

#define HOST_NowFile10 0x0087 // ʮ���ļ�
//...............................................................
#define HOST_NowFile100 0X01EF // 100���ļ�  0X01EF-0X01F2

// U���ļ�
#define HOSTU_NowFile1 0x01F3 // U��һ���ļ�  0x01F3-0x01F6
#define HOSTU_NowFile2 0x01F7 // U�̶����ļ�  0x01F7-0x01FA
#define HOSTU_NowFile3 0x01FB // U�������ļ�
#define HOSTU_NowFile4 0x01FF // U���ĺ��ļ�
#define HOSTU_NowFile5 0x0203 // U������ļ�
#define HOSTU_NowFile6 0x0207 // U�������ļ�
#define HOSTU_NowFile7 0x020B // U���ߺ��ļ�
#define HOSTU_NowFile8 0x020F // U�̰˺��ļ�
#define HOSTU_NowFile9 0x0213 // U�̾ź��ļ�

#define HOSTU_NowFile10 0x0217 // U��ʮ���ļ�
//...............................................................
#define HOSTU_NowFile100 0X037F // U��100���ļ�  0X037F-0X0382

#define HOST_UTotal_Files 0x0034 // U���ļ�������03��
#define HOST_UToR 0x0035         // ��u���е�x���ļ��������ڴ�
#define HOST_RToU 0x0036         // ���ڴ��е�x�ļ�������u��
#define HOST_DeleteR 0x0037      // ���ڴ��е�x���ļ�ɾ��
#define HOST_DeleteU 0x0038      // ��u���е�x���ļ�ɾ��

#define HOST_Confirm 0x0000 // ȷ�ϣ�10��

//-----------------------��������ַ---------------------------------
#define RuiDa_ACS 0x2001          // ����-����
#define HMI_System_Restore 0x2030 // ϵͳ�ָ���-��ʾ

// 01 03 08 D4 CB D0 D0 00 00 00 00 31 F6  -����
// 01 03 08 BF D5 CF D0 00 00 00 00 8A 03  -����
// 01 03 08 CF B5 CD B3 CD EA B3 C9 82 F5  -ϵͳ���
// 01 03 08 D4 DD CD A3 00 00 00 00 40 71  -��ͣ

// ������
#define HMI_Start_Stoptouch 0x2000 // ����-��ͣ�����أ�
#define HMI_Start_Stop 0x3010      // ����-��ͣ��ͼ�꣩
#define HMI_WorkSta 0x301C         // ����״̬301C-3020���ı���ʾ�� 0x0000��ϵͳ����  0x0001��ϵͳ���� 0x0003��ϵͳ��ͣ 0x0005��ϵͳ���
#define HMI_Stop 0x1011            // ֹͣ
#define HMI_On 0x1012              // ��
#define HMI_Under 0x1013           // ��
#define HMI_Left 0x1014            // ��
#define HMI_Right 0x1015           // ��
// #define  HMI_Anchor_Point					   0x1016			//����
#define HMI_Z_Positive 0x1017   // Z+
#define HMI_Z_Peverse 0x1018    // Z-
#define HMI_U_Positive 0x1019   // U+
#define HMI_U_Peverse 0x101A    // U-
#define HMI_U_Coordinate 0x3086 // U��ʾ

#define HMI_Anchor_Point 0x101B // ��λ��

#define HMI_Reset 0x20C7       // ��λ
#define HMI_Reset_Enter 0x3200 // ��λȷ��

#define HMI_Warning_Text 0x2023 // ���浯��

#define HMI_Go_BorderIcon 0x20C6 // �߱߿�ͼ��
#define HMI_Go_Border 0x1018     // �߱߿�
#define HMI_Cartoon 0x20E2       // �߱߿򶯻���ַ��0-ֹͣ   1-��ʼ��

#define HMI_FocusingIcon 0x20CB // Ѱ��ͼ��
#define HMI_Focusing 0x20C8     // Ѱ��
#define HMI_Cartoon 0x20E2      // Ѱ��������ַ��0-ֹͣ   1-��ʼ��

#define HMI_Diagnosis 0x3076        // ���
#define HMI_Diagnosis_Output 0x3077 // ���-���
#define HMI_Processing_Speed 0x302A // �ӹ��ٶ�-ͼ��

#define HMI_Current_File_Name 0x101D // ��ǰ�ļ���0x101D-0X1020

#define HMI_Work_TimeC 0x1021 // �ӹ�ʱ��(03)0x001C-0x001E
#define HMI_Work_TimeD 0x1022 // �ӹ�ʱ��0x001C-0x001E
#define HMI_Work_TimeE 0x1023 // �ӹ�ʱ��0x001C-0x001E

#define HMI_Restore_Data 0x1029 // �ָ�����
#define HMI_Data_Dackup 0x1029  // ��������

#define HMI_Read_WIFI_IP 0x102B  // ��IP(03)
#define HMI_Write_WIFI_IP 0x2078 // дIP(10)   01 10 00 25 00 04 08 00 C0 00 A8 00 01 00 64 CA 36

#define HMI_Point_Set 0x102D // �㶯��������

#define HMI_Read_Speed 0x1026      // �ƶ��ٶ�
#define HMI_Read_Remove 0x102E     // �㶯����
#define HMI_Point_Time 0x102F      // ����ʱ��
#define HMI_PulseLaserPower 0x1024 // ���书��

#define HMI_Point_Write 0x201B // д�㶯����
#define HMI_Point_Read 0x2019  // ���㶯����

#define HMI_PointP_Write 0x201C // д��������01 10 00 43 00 02 04 00 00 01 F4 B7 9D
#define HMI_PointP_Read 0x201A  // ����������

#define HMI_System_Information 0x2040     // ϵͳ��Ϣ����
#define HMI_System_InformationShow 0x3078 // ϵͳ��Ϣ��ʾ

#define HMI_WIFI_IP1_Addr 0x3025 // WIFI_IP������-��ʾ��
#define HMI_WIFI_IP2_Addr 0x3026
#define HMI_WIFI_IP3_Addr 0x3027
#define HMI_WIFI_IP4_Addr 0x3028

#define HMI_Overstep 0x0300 // �߿�Խ��
#define HMI_MProtect 0x0400 // ��������

#define HMI_Read_X_Value 0x3080 // ��X (03)
#define HMI_Read_Y_Value 0x3082 // ��Y (03)
#define HMI_Read_Z_Value 0x3084 // ��Z (03)

#define HMI_Read_Language 0x2030       // ���Դ���
#define HMI_Read_Language_Enter 0x2031 // ����ȷ��

#define HMI_Lock_pas_Addr1 0x4030
#define HMI_Lock_pas_Addr2 0x4031
#define HMI_Lock_pas_Addr3 0x401C
#define HMI_Lock_pas_Addr4 0x401D

#define HMI_UnLock_pas_Addr1 0x3086
#define HMI_UnLock_pas_Addr2 0x3087
#define HMI_UnLock_pas_Addr3 0x3088
#define HMI_UnLock_pas_Addr4 0x3089

#define HMI_Keyboard_lock 0x401E // ������ȷ��

#define HMI_Total_Files 0x1031      // �ļ�����
#define HMI_Residual_Files 0x3050   // ���һҳʣ���ļ�
#define HMI_Current_Control 0x3051  // ��ǰ�������
#define HMI_Totalpage_Number 0x304F // �ļ���ҳ��
#define HMI_File_Icon 0x3052        // �ļ�ͼ��  0x3052-305A

#define HMI_File_Num 0x305B     // �ļ����  0x305b-3063
#define HMI_File_Control 0x3064 // �ļ�����  0x3064-0x0306c

#define HMI_Key_Control 0x306D // �������� 0x306d-0x03075

// ��ʾ�ļ�
#define HMI_NowFile1 0x302B // һ���ļ�  0x302b-0x302e
#define HMI_NowFile2 0x302F // �����ļ�  0x302f-0x3032
#define HMI_NowFile3 0x3033 // �����ļ�  0x3033-0x3036
#define HMI_NowFile4 0x3037 // �ĺ��ļ�  0x3037-0x303A
#define HMI_NowFile5 0x303B // ����ļ�  0x303B-0x303E
#define HMI_NowFile6 0x303F // �����ļ�  0x303F-0x3042
#define HMI_NowFile7 0x3043 // �ߺ��ļ�  0x3043-0x3046
#define HMI_NowFile8 0x3047 // �˺��ļ�  0x3047-0x304A
#define HMI_NowFile9 0x304B // �ź��ļ�  0x304B-0x304E

#define HMI_RU_Memory 0x20A8    // �ڴ�U�̴���
#define HMI_R_MemoryIcon 0x20A2 // �ڴ�ͼ��
#define HMI_U_MemoryIcon 0x20A4 // U��ͼ��
#define HMI_Delete 0x20E1       // ɾ��
#define HMI_Copy 0x20A6         // ���ƴ���
#define HMI_Confirm 0x2020      // ȷ��   //01 10 00 00 00 01 02 00 0A 26 57

#define Read_Cmd 0x83  // ������ָ��
#define Write_Cmd 0x82 // д����ָ��

#endif
