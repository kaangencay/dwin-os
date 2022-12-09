
#include "sys.h"
//#include "common.h"
#include "ssd2828.h"
//#include "interrupts.h"

sbit csx = P1^0;		//p1.0
sbit sdo = P1^1;		//p1.1
sbit sdi = P1^2;		//p1.2
sbit sck = P1^3;		//p1.3
sbit sdc = P1^4;		//p1.4


bit ssd2828_cfg_ok=0;


code const u8 pa_tab1[]={0xFF,0x98,0x82,0x01};
code const u8 pa_tab2[]={0x00,0x4a};
code const u8 pa_tab3[]={0x01,0x33};
code const u8 pa_tab4[]={0x02,0x00};
code const u8 pa_tab5[]={0x03,0x00};
code const u8 pa_tab6[]={0x04,0xce};
code const u8 pa_tab7[]={0x05,0x13};
code const u8 pa_tab8[]={0x06,0x00};
code const u8 pa_tab9[]={0x07,0x00};
code const u8 pa_tab10[]={0x08,0x86};
code const u8 pa_tab11[]={0x09,0x01};
code const u8 pa_tab12[]={0x0a,0x73};
code const u8 pa_tab13[]={0x0b,0x00};
code const u8 pa_tab14[]={0x0c,0x13};     	//focal????,48.63%
code const u8 pa_tab15[]={0x0d,0x13};              	//focal????,48.63%
code const u8 pa_tab16[]={0x0e,0x00};
code const u8 pa_tab17[]={0x0f,0x00};
code const u8 pa_tab18[]={0x24,0xce};
code const u8 pa_tab19[]={0x25,0x0b};
code const u8 pa_tab20[]={0x26,0x00};
code const u8 pa_tab21[]={0x27,0x00};
code const u8 pa_tab22[]={0x31,0x0d};
code const u8 pa_tab23[]={0x32,0x21};
code const u8 pa_tab24[]={0x33,0x02};
code const u8 pa_tab25[]={0x34,0x02};
code const u8 pa_tab26[]={0x35,0x02};
code const u8 pa_tab27[]={0x36,0x00};
code const u8 pa_tab28[]={0x37,0x01};
code const u8 pa_tab29[]={0x38,0x09};
code const u8 pa_tab30[]={0x39,0x0b};
code const u8 pa_tab31[]={0x3a,0x13};
code const u8 pa_tab32[]={0x3b,0x11};
code const u8 pa_tab33[]={0x3c,0x17};
code const u8 pa_tab34[]={0x3d,0x15};
code const u8 pa_tab35[]={0x3e,0x07};
code const u8 pa_tab36[]={0x3f,0x07};
code const u8 pa_tab37[]={0x40,0x07};
code const u8 pa_tab38[]={0x41,0x07};
code const u8 pa_tab39[]={0x42,0x07};
code const u8 pa_tab40[]={0x43,0x07};
code const u8 pa_tab41[]={0x44,0x07};
code const u8 pa_tab42[]={0x45,0x07};
code const u8 pa_tab43[]={0x46,0x07};
code const u8 pa_tab44[]={0x47,0x0c};
code const u8 pa_tab45[]={0x48,0x20};
code const u8 pa_tab46[]={0x49,0x02};
code const u8 pa_tab47[]={0x4a,0x02};
code const u8 pa_tab48[]={0x4b,0x02};
code const u8 pa_tab49[]={0x4c,0x00};
code const u8 pa_tab50[]={0x4d,0x01};
code const u8 pa_tab51[]={0x4e,0x08};
code const u8 pa_tab52[]={0x4f,0x0a};
code const u8 pa_tab53[]={0x50,0x12};
code const u8 pa_tab54[]={0x51,0x10};
code const u8 pa_tab55[]={0x52,0x16};
code const u8 pa_tab56[]={0x53,0x14};
code const u8 pa_tab57[]={0x54,0x07};
code const u8 pa_tab58[]={0x55,0x07};
code const u8 pa_tab59[]={0x56,0x07};
code const u8 pa_tab60[]={0x57,0x07};
code const u8 pa_tab61[]={0x58,0x07};
code const u8 pa_tab62[]={0x59,0x07};
code const u8 pa_tab63[]={0x5a,0x07};
code const u8 pa_tab64[]={0x5b,0x07};
code const u8 pa_tab65[]={0x5c,0x07};
code const u8 pa_tab66[]={0x61,0x0c};
code const u8 pa_tab67[]={0x62,0x20};
code const u8 pa_tab68[]={0x63,0x02};
code const u8 pa_tab69[]={0x64,0x02};
code const u8 pa_tab70[]={0x65,0x02};
code const u8 pa_tab71[]={0x66,0x00};
code const u8 pa_tab72[]={0x67,0x01};
code const u8 pa_tab73[]={0x68,0x0a};
code const u8 pa_tab74[]={0x69,0x08};
code const u8 pa_tab75[]={0x6a,0x14};
code const u8 pa_tab76[]={0x6b,0x16};
code const u8 pa_tab77[]={0x6c,0x10};
code const u8 pa_tab78[]={0x6d,0x12};
code const u8 pa_tab79[]={0x6e,0x07};
code const u8 pa_tab80[]={0x6f,0x07};
code const u8 pa_tab81[]={0x70,0x07};
code const u8 pa_tab82[]={0x71,0x07};
code const u8 pa_tab83[]={0x72,0x07};
code const u8 pa_tab84[]={0x73,0x07};
code const u8 pa_tab85[]={0x74,0x07};
code const u8 pa_tab86[]={0x75,0x07};
code const u8 pa_tab87[]={0x76,0x07};
code const u8 pa_tab88[]={0x77,0x0d};
code const u8 pa_tab89[]={0x78,0x21};
code const u8 pa_tab90[]={0x79,0x02};
code const u8 pa_tab91[]={0x7a,0x02};
code const u8 pa_tab92[]={0x7b,0x02};
code const u8 pa_tab93[]={0x7c,0x00};
code const u8 pa_tab94[]={0x7d,0x01};
code const u8 pa_tab95[]={0x7e,0x0b};
code const u8 pa_tab96[]={0x7f,0x09};
code const u8 pa_tab97[]={0x80,0x15};
code const u8 pa_tab98[]={0x81,0x17};
code const u8 pa_tab99[]={0x82,0x11};
code const u8 pa_tab100[]={0x83,0x13};
code const u8 pa_tab101[]={0x84,0x07};
code const u8 pa_tab102[]={0x85,0x07};
code const u8 pa_tab103[]={0x86,0x07};
code const u8 pa_tab104[]={0x87,0x07};
code const u8 pa_tab105[]={0x88,0x07};
code const u8 pa_tab106[]={0x89,0x07};
code const u8 pa_tab107[]={0x8a,0x07};
code const u8 pa_tab108[]={0x8b,0x07};
code const u8 pa_tab109[]={0x8c,0x07};
code const u8 pa_tab110[]={0xb8,0x20};
code const u8 pa_tab111[]={0xd0,0x01};
code const u8 pa_tab112[]={0xd1,0x00};
code const u8 pa_tab113[]={0xd5,0x00};
code const u8 pa_tab114[]={0xe2,0x00};
code const u8 pa_tab115[]={0xe6,0x22};
code const u8 pa_tab116[]={0xe7,0x54};

// RTN. Internal VBP,0x Internal VFP
code const u8 pa_tab117[]={0xFF,0x98,0x82,0x02};
code const u8 pa_tab118[]={0xF1,0x1C};    // Tcon ESD option
code const u8 pa_tab119[]={0x4B,0x5A};    // line_chopper
code const u8 pa_tab120[]={0x50,0xCA};    // line_chopper
code const u8 pa_tab121[]={0x51,0x00};     // line_chopper
code const u8 pa_tab122[]={0x06,0xB1};     // Internal Line Time (RTN)
code const u8 pa_tab123[]={0x0B,0xA0};     // Internal VFP[9]
code const u8 pa_tab124[]={0x0C,0x00};     // Internal VFP[8]
code const u8 pa_tab125[]={0x0D,0x14};     // Internal VBP
code const u8 pa_tab126[]={0x0E,0xBE};     // Internal VFP
code const u8 pa_tab127[]={0x4E,0x11};     // SRC BIAS

// Power Setting
code const u8 pa_tab128[]={0xFF,0x98,0x82,0x05};
code const u8 pa_tab129[]={0x03,0x00};    // Vcom
code const u8 pa_tab130[]={0x04,0xE1};    // Vcom
code const u8 pa_tab131[]={0x58,0x61};    // VGL 2x
code const u8 pa_tab132[]={0x63,0x83};     // GVDDN = -5.1V
code const u8 pa_tab133[]={0x64,0x83};     // GVDDP = 5.1V
code const u8 pa_tab134[]={0x68,0xA1};     // VGHO = 15V
code const u8 pa_tab135[]={0x69,0xA7};     // VGH = 16V
code const u8 pa_tab136[]={0x6A,0x79};     // VGLO = -10V
code const u8 pa_tab137[]={0x6B,0x6B};     // VGL = -11V
code const u8 pa_tab138[]={0x85,0x37};      // HW RESET option
//code const u8 pa_tab1[]={0x46,0x01,0x03};      // LVD HVREG option

// Resolution
code const u8 pa_tab139[]={0xFF,0x98,0x82,0x06};
code const u8 pa_tab140[]={0xD9,0x1F};     // 4Lane
code const u8 pa_tab141[]={0xC0,0x00};     // NL = 1280
code const u8 pa_tab142[]={0xC1,0x15};     // NL = 1280

// Gamma Register

code const u8 pa_tab143[]={0xFF,0x98,0x82,0x08};
code const u8 pa_tab144[]={0xE0,0x00,0x24,0x58,0x81,0xB7,0x54,0xE6,0x0B,0x37,0x5B,0x95,0x95,0xC3,0xED,0x16,0xAA,0x41,0x77,0x99,0xC6,0xFE,0xEC,0x1D,0x5A,0x8D,0x03,0xEC};
code const u8 pa_tab145[]={0xE1,0x00,0x24,0x58,0x81,0xB7,0x54,0xE6,0x0B,0x37,0x5B,0x95,0x95,0xC3,0xED,0x16,0xAA,0x41,0x77,0x99,0xC6,0xFE,0xEC,0x1D,0x5A,0x8D,0x03,0xEC};


// OSC Auto Trim Setting
code const u8 pa_tab146[]={0xFF,0x98,0x82,0x0B};
code const u8 pa_tab147[]={0x9A,0x45};
code const u8 pa_tab148[]={0x9B,0x96};
code const u8 pa_tab149[]={0x9C,0x04};
code const u8 pa_tab150[]={0x9D,0x04};
code const u8 pa_tab151[]={0x9E,0x8B};
code const u8 pa_tab152[]={0x9F,0x8B};
code const u8 pa_tab153[]={0xAB,0xE0};     // AutoTrimType

code const u8 pa_tab154[]={0xFF,0x98,0x82,0x0E};
code const u8 pa_tab155[]={0x11,0x10};     // TSVD Rise position
code const u8 pa_tab156[]={0x13,0x10};     // LV mode TSHD Rise position
code const u8 pa_tab157[]={0x00,0xA0};      // LV mode

code const u8 pa_tab158[]={0xFF,0x98,0x82,0x00};
code const u8 pa_tab159[]={0x35,0x00};
code const u8 pa_tab160[]={0x11,0x00};
//DELAY,0x120
code const u8 pa_tab161[]={0x29,0x00}; 
//DELAY,0x20

#if 0
void delay_us(u8 t){
	u8 i,j;
	for(i=0;i<t;i++){
		for(j=0;j<4;j++);
	}
}
#endif

u16 ssd2828_readID(void){
	u16 ssd2828_id=0,reg_temp=0xb0fa;
	u8 i;
	csx=0;
	sdc=0;		//??
	delay_us(5);
	for(i=0;i<16;i++){
		
		if(reg_temp&0x8000)		//msb
			sdi=1;
		else
			sdi=0;
		sck=0;
		delay_us(5);
		sck=1;
		delay_us(5);
		reg_temp<<=1;			//???
	}
	sdc=1;
	delay_us(5);
	for(i=0;i<16;i++){
		sck=0;
		delay_us(5);
		sck=1;
		delay_us(5);
		ssd2828_id<<=1;
		if(sdo)
			ssd2828_id|=1;
	}
	csx=1;
	delay_us(5);
	return ssd2828_id;
}

u16 ssd2828_readReg(u8 addr_reg){
	u16 reg_value=0,reg_temp;
	u8 i;
	reg_temp=0x00fa|(addr_reg<<8);
	csx=0;
	sdc=0;		//??
	delay_us(5);
	for(i=0;i<16;i++){
		
		if(reg_temp&0x8000)		//msb
			sdi=1;
		else
			sdi=0;
		sck=0;
		delay_us(5);
		sck=1;
		delay_us(5);
		reg_temp<<=1;			//???
	}
	sdc=1;
	delay_us(5);
	for(i=0;i<16;i++){
		sck=0;
		delay_us(5);
		sck=1;
		delay_us(5);
		reg_value<<=1;
		if(sdo)
			reg_value|=1;
	}
	csx=1;
	delay_us(5);
	return reg_value;	
}

void ssd2828_writecmd(u8 cmd){
	u8 i,ssd2828_cmd;
	ssd2828_cmd=cmd;
	csx=0;
	sdc=0;
	delay_us(5);
	for(i=0;i<8;i++){
		if(ssd2828_cmd&0x80)
			sdi=1;
		else
			sdi=0;
		sck=0;
		delay_us(5);
		sck=1;
		delay_us(5);
		ssd2828_cmd<<=1;	
	}
	csx=1;
	delay_us(5);
}

void ssd2828_regconfig(u8 reg_addr,u16 reg_data){
	u8 i,addr_temp,data_H,data_L;
	addr_temp=reg_addr;
	data_H=reg_data>>8;
	data_L=reg_data;
	csx=0;
	sdc=0;
	delay_us(5);
	for(i=0;i<8;i++){		//???????
		if(addr_temp&0x80)
			sdi=1;
		else
			sdi=0;
		sck=0;
		delay_us(5);
		sck=1;
		delay_us(5);
		addr_temp<<=1;
	}
	sdc=1;
	delay_us(5);
	for(i=0;i<8;i++){		//lsb
		if(data_L&0x80)
			sdi=1;
		else
			sdi=0;
		sck=0;
		delay_us(5);
		sck=1;
		delay_us(5);
		data_L<<=1;
	}
	for(i=0;i<8;i++){
		if(data_H&0x80)
			sdi=1;
		else
			sdi=0;
		sck=0;
		delay_us(5);
		sck=1;
		delay_us(5);
		data_H<<=1;
	}
	csx=1;
	delay_us(5);
}

void ssd2828_writepackage(u8 *pbuf,u16 len){
	u16 lenth;
	u8 i,temp;
	lenth=len;
	ssd2828_regconfig(0xbc,lenth);
	ssd2828_regconfig(0xbe,lenth);
	ssd2828_writecmd(0xbf);
	csx=0;
	sdc=1;
	delay_us(5);
	while(lenth--){
		temp=*pbuf++;
		for(i=0;i<8;i++){
			if(temp&0x80)
				sdi=1;
			else
				sdi=0;
			sck=0;
			delay_us(5);
			sck=1;
			delay_us(5);
			temp<<=1;
		}
	}
	csx=1;
	delay_us(5);
}

//void W_C(int val)
//{
//	int i;
//	csx = 1;
//	sdi = 1;
//	sck = 1;
//	delay_us(5);
//	csx = 0;
//	delay_us(5);
//	sdi = 0;
//	delay_us(5);
//	sck = 0;
//	delay_us(5);
//	sck = 1;
//	for(i = 0;i < 8;i++)
//	{
//		if(val&0x80)
//			sdi = 1;
//		else
//			sdi = 0;		
//		val<<=1;
//		sck = 0;
//		delay_us(5);
//		sck = 1;
//		delay_us(5);
//	}
//	csx = 1;
//}

//void W_D(int val)
//{
//	int i;
//	csx = 1;
//	sdi = 1;
//	sck = 1;
//	delay_us(5);
//	csx = 0;
//	delay_us(5);
//	sdi = 1;
//	delay_us(5);
//	sck = 0;
//	delay_us(5);
//	sck = 1;
//	for(i = 0;i < 8;i++)
//	{
//		if(val&0x80)
//			sdi = 1;
//		else
//			sdi = 0;		
//		val<<=1;
//		sck = 0;
//		delay_us(5);
//		sck = 1;
//		delay_us(5);
//	}
//	csx = 1;
//}
//void ssd2828_writepackage(unsigned char *Data1)
//{
//	unsigned int i;
//	unsigned char PCS;
//	PCS = *Data1;
//	W_C(0xB7);	
//	W_D(0x02);
//	W_D(0x50);
//	
//	W_C(0xBD);
//	W_D(0x00);
//	W_D(0x00);
//	
//	W_C(0xBC);	
//	W_D(0x00);
//	W_D(PCS);
//	
//	W_C(0xBF);
//	for(i = 0;i < PCS;i++)
//	{
//		W_D(*(Data1+i+1));
//	}
//}

void W_COM(u8 cmd,u8 val)
{
	u8 i;
	ssd2828_regconfig(0xb7,0x0250);
	ssd2828_regconfig(0xbc,0x0002);
	ssd2828_regconfig(0xbd,0x0000);
	ssd2828_writecmd(0xbf);
	csx=0;
	sdc=1;
	for(i=0;i<8;i++)
	{
		if(cmd&0x80)
			sdi=1;
		else
			sdi=0;
		sck=0;
		delay_us(15);
		sck=1;
		delay_us(15);
		cmd<<=1;
	}
	csx=1;
	delay_us(15);
	for(i=0;i<8;i++)
	{
		if(val&0x80)
			sdi=1;
		else
			sdi=0;
		sck=0;
		delay_us(15);
		sck=1;
		delay_us(15);
		val<<=1;
	}
	csx=1;
	delay_us(15);
	
	delay_ms(10);
}

/******************************************
	ssd2828配置为non-burst mode，NVD=0,BLLP=0
	ssd2828仅在垂直消隐期间发送非视频数据包
*******************************************/
u16 ssd2828_read_mipi(u16 mipi_reg){		//仅读取一个字
	u16 reg_value=0,mipi_reg_value=0;
	u8 mipi_read_cnt=0;
	do{
		ssd2828_regconfig(0xb7,0x02CB);		//REN=1
//		ssd2828_regconfig(0xbb,0x0002);		//设定接收速率，必须比送图速率小
		ssd2828_regconfig(0xc1,0x0001);		//设置读取mipi slave数据的最大长度
//		ssd2828_regconfig(0xc0,0x0100);		//停止送图，准备读取mipi slave寄存器
		ssd2828_regconfig(0xbc,0x0001);		//设置带读取数据包数量
		ssd2828_regconfig(0xbf,mipi_reg);		//读取mipi slave寄存器
		ssd2828_regconfig(0xd4,0x00fa);		//复位SPI读寄存器
		reg_value=ssd2828_readReg(0xc6);
		reg_value&=0x01;		//RDR=0xc6.0
		mipi_read_cnt++;
	} while((reg_value==0)&&(mipi_read_cnt<3));
	//RDR=1，则表明mipi slave已回复
	mipi_reg_value=ssd2828_readReg(0xff);	//mipi slave返回的数据存储在0xff寄存器地址
	ssd2828_regconfig(0xb7,0x024b);				//恢复正常显示
	return mipi_reg_value;
}

void W_write()
{
W_COM(0xB0,0x5A);

W_COM(0xB1,0x00);
W_COM(0x89,0x01);
W_COM(0x91,0x17);
W_COM(0xB1,0x03);
W_COM(0x2C,0x28);

W_COM(0x00,0x28);
W_COM(0x01,0x14);
W_COM(0x02,0x0A);
W_COM(0x03,0x85);
W_COM(0x04,0x42);
W_COM(0x05,0xA1);
W_COM(0x06,0x50);
W_COM(0x07,0x28);
W_COM(0x08,0x14);
W_COM(0x09,0x00);
W_COM(0x0A,0x29);
W_COM(0x0B,0x3C);
W_COM(0x0C,0x00);
W_COM(0x0D,0x00);
W_COM(0x0E,0x24);
W_COM(0x0F,0x1C);
W_COM(0x10,0xC9);
W_COM(0x11,0x60);
W_COM(0x12,0x70);
W_COM(0x13,0x01);
W_COM(0x14,0xE3);//E3:4lane,E2:3lane,E1:2lane
W_COM(0x15,0xFF);
W_COM(0x16,0x3D);
W_COM(0x17,0x0E);
W_COM(0x18,0x01);
W_COM(0x19,0x00);
W_COM(0x1A,0x00);
W_COM(0x1B,0xFC);
W_COM(0x1C,0x0B);
W_COM(0x1D,0xA0);
W_COM(0x1E,0x03);
W_COM(0x1F,0x04);
W_COM(0x20,0x0C);
W_COM(0x21,0x00);
W_COM(0x22,0x04);
W_COM(0x23,0x81);
W_COM(0x24,0x1F);
W_COM(0x25,0x10);
W_COM(0x26,0x9B);
W_COM(0x2D,0x01);
W_COM(0x2E,0x84);
W_COM(0x2F,0x00);
W_COM(0x30,0x02);
W_COM(0x31,0x08);
W_COM(0x32,0x01);
W_COM(0x33,0x1C);
W_COM(0x34,0x70);
W_COM(0x35,0xFF);
W_COM(0x36,0xFF);
W_COM(0x37,0xFF);
W_COM(0x38,0xFF);
W_COM(0x39,0xFF);
W_COM(0x3A,0x05);
W_COM(0x3B,0x00);
W_COM(0x3C,0x00);
W_COM(0x3D,0x00);
W_COM(0x3E,0x0F);
W_COM(0x3F,0x8C);
W_COM(0x40,0x28);
W_COM(0x41,0xFC);
W_COM(0x42,0x01);
W_COM(0x43,0x40);
W_COM(0x44,0x05);
W_COM(0x45,0xE8);
W_COM(0x46,0x16);
W_COM(0x47,0x00);
W_COM(0x48,0x00);
W_COM(0x49,0x88);
W_COM(0x4A,0x08);
W_COM(0x4B,0x05);
W_COM(0x4C,0x03);
W_COM(0x4D,0xD0);
W_COM(0x4E,0x13);
W_COM(0x4F,0xFF);
W_COM(0x50,0x0A);
W_COM(0x51,0x53);
W_COM(0x52,0x26);
W_COM(0x53,0x22);
W_COM(0x54,0x09);
W_COM(0x55,0x22);
W_COM(0x56,0x00);
W_COM(0x57,0x1C);
W_COM(0x58,0x03);
W_COM(0x59,0x3F);
W_COM(0x5A,0x28);
W_COM(0x5B,0x01);
W_COM(0x5C,0xCC);
W_COM(0x5D,0x20);
W_COM(0x5E,0xE8);
W_COM(0x5F,0x9D);
W_COM(0x60,0x6F);
W_COM(0x61,0x9D);
W_COM(0x62,0xF1);
W_COM(0x63,0xB5);
W_COM(0x64,0x35);
W_COM(0x65,0x42);
W_COM(0x66,0x09);
W_COM(0x67,0x21);
W_COM(0x68,0x84);
W_COM(0x69,0x00);
W_COM(0x6A,0x82);
W_COM(0x6B,0xDE);
W_COM(0x6C,0xD8);
W_COM(0x6D,0x52);
W_COM(0x6E,0x09);
W_COM(0x6F,0x1D);
W_COM(0x70,0x53);
W_COM(0x71,0x22);
W_COM(0x72,0x90);
W_COM(0x73,0x10);
W_COM(0x74,0x42);
W_COM(0x75,0x08);
W_COM(0x76,0x00);
W_COM(0x77,0x00);
W_COM(0x78,0x0F);
W_COM(0x79,0xE0);
W_COM(0x7A,0x01);
W_COM(0x7B,0xFF);
W_COM(0x7C,0xFF);
W_COM(0x7D,0x10);
W_COM(0x7E,0x41);
W_COM(0x7F,0xFE);

W_COM(0xB1,0x02);

W_COM(0x00,0xFF);
W_COM(0x01,0x05);
W_COM(0x02,0xF0);
W_COM(0x03,0x00);
W_COM(0x04,0x5E);
W_COM(0x05,0x47);
W_COM(0x06,0x78);
W_COM(0x07,0x0F);
W_COM(0x08,0xC0);
W_COM(0x09,0x04);
W_COM(0x0A,0x00);
W_COM(0x0B,0x14);
W_COM(0x0C,0xE6);
W_COM(0x0D,0x0D);
W_COM(0x0F,0x00);
W_COM(0x10,0xF9);
W_COM(0x11,0x37);
W_COM(0x12,0x73);
W_COM(0x13,0xB3);
W_COM(0x14,0x29);
W_COM(0x15,0x80);
W_COM(0x16,0x07);
W_COM(0x17,0x8A);
W_COM(0x18,0x8D);
W_COM(0x19,0xBF);
W_COM(0x1A,0x69);
W_COM(0x1B,0x0E);
W_COM(0x1C,0xFF);
W_COM(0x1D,0xFF);
W_COM(0x1E,0xFF);
W_COM(0x1F,0xFF);
W_COM(0x20,0xFF);
W_COM(0x21,0xFF);
W_COM(0x22,0xFF);
W_COM(0x23,0xFF);
W_COM(0x24,0xFF);
W_COM(0x25,0xFF);
W_COM(0x26,0xFF);
W_COM(0x27,0x1F);
W_COM(0x28,0xFF);
W_COM(0x29,0xFF);
W_COM(0x2A,0xFF);
W_COM(0x2B,0xFF);
W_COM(0x2C,0xFF);
W_COM(0x2D,0x07);
W_COM(0x33,0x3F);
W_COM(0x35,0x7F);
W_COM(0x36,0x3F);
W_COM(0x38,0x7F);
W_COM(0x3A,0x80);
W_COM(0x3B,0x01);
W_COM(0x3C,0x00);
W_COM(0x3D,0x2F);
W_COM(0x3E,0x00);
W_COM(0x3F,0xE0);
W_COM(0x40,0x05);
W_COM(0x41,0x00);
W_COM(0x42,0xBC);
W_COM(0x43,0x00);
W_COM(0x44,0x80);
W_COM(0x45,0x07);
W_COM(0x46,0x00);
W_COM(0x47,0x00);
W_COM(0x48,0x9B);
W_COM(0x49,0xD2);
W_COM(0x4A,0xC1);
W_COM(0x4B,0x83);
W_COM(0x4C,0x17);
W_COM(0x4D,0xC0);
W_COM(0x4E,0x0F);
W_COM(0x4F,0xF1);
W_COM(0x50,0x78);
W_COM(0x51,0x6A);
W_COM(0x52,0x34);
W_COM(0x53,0x79);
W_COM(0x54,0x9A);
W_COM(0x55,0x02);
W_COM(0x56,0x74);
W_COM(0x57,0x02);
W_COM(0x58,0x00);
W_COM(0x59,0xD0);
W_COM(0x5A,0xEF);
W_COM(0x5B,0xF7);
W_COM(0x5C,0xFB);
W_COM(0x5D,0x00);
W_COM(0x5E,0x00);
W_COM(0x5F,0x00);
W_COM(0x60,0x00);
W_COM(0x61,0x05);
W_COM(0x62,0x85);
W_COM(0x63,0x82);
W_COM(0x64,0x02);
W_COM(0x65,0x00);
W_COM(0x66,0x00);
W_COM(0x67,0x00);
W_COM(0x68,0xDC);
W_COM(0x69,0x00);
W_COM(0x6A,0x01);
W_COM(0x6B,0xF4);
W_COM(0x6C,0xF5);
W_COM(0x6D,0xF5);
W_COM(0x6E,0xF5);
W_COM(0x6F,0xF5);
W_COM(0x70,0xFB);
W_COM(0x71,0xFD);
W_COM(0x72,0x7E);
W_COM(0x73,0x5F);
W_COM(0x74,0x5F);
W_COM(0x75,0x5F);
W_COM(0x76,0xBF);
W_COM(0x77,0xDF);
W_COM(0x78,0xEF);
W_COM(0x79,0xF7);
W_COM(0x7A,0xA1);
W_COM(0x7B,0xA0);
W_COM(0x7C,0xA0);
W_COM(0x7D,0xA0);
W_COM(0x7E,0xA0);
W_COM(0x7F,0x50);
W_COM(0x0B,0x04);

W_COM(0xB1,0x03);
W_COM(0x2C,0x2C);

W_COM(0xB1,0x00);
W_COM(0x89,0x03);
}


void C055_code_download(void)
{
	ssd2828_writepackage(pa_tab1,sizeof(pa_tab1));
	ssd2828_writepackage(pa_tab2,sizeof(pa_tab2));
	ssd2828_writepackage(pa_tab3,sizeof(pa_tab3));
	ssd2828_writepackage(pa_tab4,sizeof(pa_tab4));
	ssd2828_writepackage(pa_tab5,sizeof(pa_tab5));
	ssd2828_writepackage(pa_tab6,sizeof(pa_tab6));
  ssd2828_writepackage(pa_tab7,sizeof(pa_tab7));
	ssd2828_writepackage(pa_tab8,sizeof(pa_tab8));
	ssd2828_writepackage(pa_tab9,sizeof(pa_tab9));
	ssd2828_writepackage(pa_tab10,sizeof(pa_tab10));
	ssd2828_writepackage(pa_tab11,sizeof(pa_tab11));
	ssd2828_writepackage(pa_tab12,sizeof(pa_tab12));
	ssd2828_writepackage(pa_tab13,sizeof(pa_tab13));
	ssd2828_writepackage(pa_tab14,sizeof(pa_tab14));
	ssd2828_writepackage(pa_tab15,sizeof(pa_tab15));
	ssd2828_writepackage(pa_tab16,sizeof(pa_tab16));
	ssd2828_writepackage(pa_tab17,sizeof(pa_tab17));
	ssd2828_writepackage(pa_tab18,sizeof(pa_tab18));
	ssd2828_writepackage(pa_tab19,sizeof(pa_tab19));
	ssd2828_writepackage(pa_tab20,sizeof(pa_tab20));
	ssd2828_writepackage(pa_tab21,sizeof(pa_tab21));
	ssd2828_writepackage(pa_tab22,sizeof(pa_tab22));
	ssd2828_writepackage(pa_tab23,sizeof(pa_tab23));
  ssd2828_writepackage(pa_tab24,sizeof(pa_tab24));
	ssd2828_writepackage(pa_tab25,sizeof(pa_tab25));
	ssd2828_writepackage(pa_tab26,sizeof(pa_tab26));
	ssd2828_writepackage(pa_tab27,sizeof(pa_tab27));
	ssd2828_writepackage(pa_tab28,sizeof(pa_tab28));
	ssd2828_writepackage(pa_tab29,sizeof(pa_tab29));
	ssd2828_writepackage(pa_tab30,sizeof(pa_tab30));
	ssd2828_writepackage(pa_tab31,sizeof(pa_tab31));
	ssd2828_writepackage(pa_tab32,sizeof(pa_tab32));
	ssd2828_writepackage(pa_tab33,sizeof(pa_tab33));
	ssd2828_writepackage(pa_tab34,sizeof(pa_tab34));
	ssd2828_writepackage(pa_tab35,sizeof(pa_tab35));
	ssd2828_writepackage(pa_tab36,sizeof(pa_tab36));
	ssd2828_writepackage(pa_tab37,sizeof(pa_tab37));
	ssd2828_writepackage(pa_tab38,sizeof(pa_tab38));
	ssd2828_writepackage(pa_tab39,sizeof(pa_tab39));
	ssd2828_writepackage(pa_tab40,sizeof(pa_tab40));
  ssd2828_writepackage(pa_tab41,sizeof(pa_tab41));
	ssd2828_writepackage(pa_tab42,sizeof(pa_tab42));
	ssd2828_writepackage(pa_tab43,sizeof(pa_tab43));
	ssd2828_writepackage(pa_tab44,sizeof(pa_tab44));
	ssd2828_writepackage(pa_tab45,sizeof(pa_tab45));
	ssd2828_writepackage(pa_tab46,sizeof(pa_tab46));
	ssd2828_writepackage(pa_tab47,sizeof(pa_tab47));
	ssd2828_writepackage(pa_tab48,sizeof(pa_tab48));
	ssd2828_writepackage(pa_tab49,sizeof(pa_tab49));
	ssd2828_writepackage(pa_tab50,sizeof(pa_tab50));
	ssd2828_writepackage(pa_tab51,sizeof(pa_tab51));
	ssd2828_writepackage(pa_tab52,sizeof(pa_tab52));
	ssd2828_writepackage(pa_tab53,sizeof(pa_tab53));
	ssd2828_writepackage(pa_tab54,sizeof(pa_tab54));
	ssd2828_writepackage(pa_tab55,sizeof(pa_tab55));
	ssd2828_writepackage(pa_tab56,sizeof(pa_tab56));
	ssd2828_writepackage(pa_tab57,sizeof(pa_tab57));
	ssd2828_writepackage(pa_tab58,sizeof(pa_tab58));
	ssd2828_writepackage(pa_tab59,sizeof(pa_tab59));
	ssd2828_writepackage(pa_tab60,sizeof(pa_tab60));
	ssd2828_writepackage(pa_tab61,sizeof(pa_tab61));
	ssd2828_writepackage(pa_tab62,sizeof(pa_tab62));
	ssd2828_writepackage(pa_tab63,sizeof(pa_tab63));
	ssd2828_writepackage(pa_tab64,sizeof(pa_tab64));
	ssd2828_writepackage(pa_tab65,sizeof(pa_tab65));
	ssd2828_writepackage(pa_tab66,sizeof(pa_tab66));
	ssd2828_writepackage(pa_tab67,sizeof(pa_tab67));
  ssd2828_writepackage(pa_tab68,sizeof(pa_tab68));
	ssd2828_writepackage(pa_tab69,sizeof(pa_tab69));
	ssd2828_writepackage(pa_tab70,sizeof(pa_tab70));
	ssd2828_writepackage(pa_tab71,sizeof(pa_tab71));
	ssd2828_writepackage(pa_tab72,sizeof(pa_tab72));
	ssd2828_writepackage(pa_tab73,sizeof(pa_tab73));
	ssd2828_writepackage(pa_tab74,sizeof(pa_tab74));
	ssd2828_writepackage(pa_tab75,sizeof(pa_tab75));
	ssd2828_writepackage(pa_tab76,sizeof(pa_tab76));
	ssd2828_writepackage(pa_tab77,sizeof(pa_tab77));
	ssd2828_writepackage(pa_tab78,sizeof(pa_tab78));
	ssd2828_writepackage(pa_tab79,sizeof(pa_tab79));
	ssd2828_writepackage(pa_tab80,sizeof(pa_tab80));
	ssd2828_writepackage(pa_tab81,sizeof(pa_tab81));
	ssd2828_writepackage(pa_tab82,sizeof(pa_tab82));
	ssd2828_writepackage(pa_tab83,sizeof(pa_tab83));
	ssd2828_writepackage(pa_tab84,sizeof(pa_tab84));
  ssd2828_writepackage(pa_tab85,sizeof(pa_tab85));
	ssd2828_writepackage(pa_tab86,sizeof(pa_tab86));
	ssd2828_writepackage(pa_tab87,sizeof(pa_tab87));
	ssd2828_writepackage(pa_tab88,sizeof(pa_tab88));
	ssd2828_writepackage(pa_tab89,sizeof(pa_tab89));
	ssd2828_writepackage(pa_tab90,sizeof(pa_tab90));
	ssd2828_writepackage(pa_tab91,sizeof(pa_tab91));
	ssd2828_writepackage(pa_tab92,sizeof(pa_tab92));
	ssd2828_writepackage(pa_tab93,sizeof(pa_tab93));
	ssd2828_writepackage(pa_tab94,sizeof(pa_tab94));
	ssd2828_writepackage(pa_tab95,sizeof(pa_tab95));
	ssd2828_writepackage(pa_tab96,sizeof(pa_tab96));
	ssd2828_writepackage(pa_tab97,sizeof(pa_tab97));
	ssd2828_writepackage(pa_tab98,sizeof(pa_tab98));
	ssd2828_writepackage(pa_tab99,sizeof(pa_tab99));
	ssd2828_writepackage(pa_tab100,sizeof(pa_tab100));
	ssd2828_writepackage(pa_tab101,sizeof(pa_tab101));
	ssd2828_writepackage(pa_tab102,sizeof(pa_tab102));
	ssd2828_writepackage(pa_tab103,sizeof(pa_tab103));
	ssd2828_writepackage(pa_tab104,sizeof(pa_tab104));
	ssd2828_writepackage(pa_tab105,sizeof(pa_tab105));
	ssd2828_writepackage(pa_tab106,sizeof(pa_tab106));
	ssd2828_writepackage(pa_tab107,sizeof(pa_tab107));
	ssd2828_writepackage(pa_tab108,sizeof(pa_tab108));
	ssd2828_writepackage(pa_tab109,sizeof(pa_tab109));
	ssd2828_writepackage(pa_tab110,sizeof(pa_tab110));
	ssd2828_writepackage(pa_tab111,sizeof(pa_tab111));
	ssd2828_writepackage(pa_tab112,sizeof(pa_tab112));
	ssd2828_writepackage(pa_tab113,sizeof(pa_tab113));
	ssd2828_writepackage(pa_tab114,sizeof(pa_tab114));
	ssd2828_writepackage(pa_tab115,sizeof(pa_tab115));
	ssd2828_writepackage(pa_tab116,sizeof(pa_tab116));
	ssd2828_writepackage(pa_tab117,sizeof(pa_tab117));
	ssd2828_writepackage(pa_tab118,sizeof(pa_tab118));
	ssd2828_writepackage(pa_tab119,sizeof(pa_tab119));
	ssd2828_writepackage(pa_tab120,sizeof(pa_tab120));
  ssd2828_writepackage(pa_tab121,sizeof(pa_tab121));
	ssd2828_writepackage(pa_tab122,sizeof(pa_tab122));
	ssd2828_writepackage(pa_tab123,sizeof(pa_tab123));
	ssd2828_writepackage(pa_tab124,sizeof(pa_tab124));
	ssd2828_writepackage(pa_tab125,sizeof(pa_tab125));
	ssd2828_writepackage(pa_tab126,sizeof(pa_tab126));
	ssd2828_writepackage(pa_tab127,sizeof(pa_tab127));
	ssd2828_writepackage(pa_tab128,sizeof(pa_tab128));
	ssd2828_writepackage(pa_tab129,sizeof(pa_tab129));
	ssd2828_writepackage(pa_tab130,sizeof(pa_tab130));
	ssd2828_writepackage(pa_tab131,sizeof(pa_tab131));
	ssd2828_writepackage(pa_tab132,sizeof(pa_tab132));
	ssd2828_writepackage(pa_tab133,sizeof(pa_tab133));
	ssd2828_writepackage(pa_tab134,sizeof(pa_tab134));
	ssd2828_writepackage(pa_tab135,sizeof(pa_tab135));
	ssd2828_writepackage(pa_tab136,sizeof(pa_tab136));
	ssd2828_writepackage(pa_tab137,sizeof(pa_tab137));
	ssd2828_writepackage(pa_tab138,sizeof(pa_tab138));
	ssd2828_writepackage(pa_tab139,sizeof(pa_tab139));
	ssd2828_writepackage(pa_tab140,sizeof(pa_tab140));
	ssd2828_writepackage(pa_tab141,sizeof(pa_tab141));
	ssd2828_writepackage(pa_tab142,sizeof(pa_tab142));
	ssd2828_writepackage(pa_tab143,sizeof(pa_tab143));
	ssd2828_writepackage(pa_tab144,sizeof(pa_tab144));
	ssd2828_writepackage(pa_tab145,sizeof(pa_tab145));
	ssd2828_writepackage(pa_tab146,sizeof(pa_tab146));
	ssd2828_writepackage(pa_tab147,sizeof(pa_tab147));
	ssd2828_writepackage(pa_tab148,sizeof(pa_tab148));
	ssd2828_writepackage(pa_tab149,sizeof(pa_tab149));
	ssd2828_writepackage(pa_tab150,sizeof(pa_tab150));
	ssd2828_writepackage(pa_tab151,sizeof(pa_tab151));
	ssd2828_writepackage(pa_tab152,sizeof(pa_tab152));
	ssd2828_writepackage(pa_tab153,sizeof(pa_tab153));
	ssd2828_writepackage(pa_tab154,sizeof(pa_tab154));
	ssd2828_writepackage(pa_tab155,sizeof(pa_tab155));
	ssd2828_writepackage(pa_tab156,sizeof(pa_tab156));
	ssd2828_writepackage(pa_tab157,sizeof(pa_tab157));
	ssd2828_writepackage(pa_tab158,sizeof(pa_tab158));
	ssd2828_writepackage(pa_tab159,sizeof(pa_tab159));
	ssd2828_writepackage(pa_tab160,sizeof(pa_tab160));
	delay_ms(120);
	ssd2828_writepackage(pa_tab161,sizeof(pa_tab161));
	delay_ms(20);
}

void lcd_pa_set(void){
	u8 lcd_vs,lcd_hs,lcd_vbp,lcd_hbp,lcd_vfp,lcd_hfp;
	u16 lcd_sw,lcd_bp,lcd_fp;
	lcd_vs=VS;
	lcd_hs=HS;
	lcd_vbp=VBP;
	lcd_hbp=HBP;
	lcd_vfp=VFP;
	lcd_hfp=HFP;
	lcd_sw=(lcd_vs<<8)| lcd_hs;
	lcd_bp=(lcd_vbp<<8)| lcd_hbp;
	lcd_fp=(lcd_vfp<<8)| lcd_hfp;
	ssd2828_regconfig(0xb1,lcd_sw);			//H:VSA,L:HSA
	ssd2828_regconfig(0xb2,lcd_bp);			//H:VBP,L:HBP
	ssd2828_regconfig(0xb3,lcd_fp);			//H:VFP,L:HFP
	ssd2828_regconfig(0xb4,HACT);			//HACT
	ssd2828_regconfig(0xb5,VACT);			//VACT
}

void ssd2828_init(void){
#if 1
	u16 temp=0;
	temp=ssd2828_readID();
	if(temp!=0x2828)
		return;
#endif
	//??ssd2828????mipi??,????mipi??
	ssd2828_regconfig(0xb7,0x0050);
	ssd2828_regconfig(0xb8,0x0000);
	ssd2828_regconfig(0xb9,0x0000);
	//ssd2828_regconfig(0xba,0x4114);
	ssd2828_regconfig(0xba,0x4214);//
	ssd2828_regconfig(0xbb,0x0003);
	ssd2828_regconfig(0xb9,0x0001);
	//ssd2828_regconfig(0xde,0x0003);
	ssd2828_regconfig(0xde,0x0001);//
	ssd2828_regconfig(0xc9,0x2302);
	delay_ms(100);
	//ssd2828_regconfig(0xb7,0x0210);
	ssd2828_regconfig(0xb7,0x0010);//
	//ssd2828_regconfig(0xbd,0x0000);
	delay_ms(10);
	
	//??mipi?????
	C055_code_download();

	
	//??ssd2828?????,????
	ssd2828_regconfig(0xb7,0x0050);
	ssd2828_regconfig(0xb8,0x0000);
	ssd2828_regconfig(0xb9,0x0000);
	//ssd2828_regconfig(0xba,0x8029);			//801B osc=12M,pll=324M;8029 osc=12M,pll=492M
	//ssd2828_regconfig(0xbb,0x0007);
	ssd2828_regconfig(0xba,0x8048);				//0X804B
	//ssd2828_regconfig(0xba,0xC050);			//C050 osc=12M,
	ssd2828_regconfig(0xbb,0x0007);
	ssd2828_regconfig(0xb9,0x0001);
	ssd2828_regconfig(0xc9,0x2302);
	delay_ms(10);
	ssd2828_regconfig(0xca,0x2301);		//MIPI????
	ssd2828_regconfig(0xcb,0x0510);
	ssd2828_regconfig(0xcc,0x1005);
	ssd2828_regconfig(0xd0,0x0000);
	lcd_pa_set();		//??LCD?????
	
	//ssd2828_regconfig(0xb6,0x000B);		//24bit rgb
	ssd2828_regconfig(0xb6,0x0003);	//Non burst mode 24 bit rgb
	ssd2828_regconfig(0xde,0x0003);		//4 lane
	//ssd2828_regconfig(0xde,0x0001);		//2 lane
	ssd2828_regconfig(0xd6,0x0004);		//04=BGR 05=RGB
	//ssd2828_regconfig(0xb7,0x024B);		//024b 0659 0210
	ssd2828_regconfig(0xb7,0x000B);//
	delay_ms(100);
	ssd2828_writecmd(0x2c);
#if bist
	ssd2828_regconfig(0xee,0x0200);
	delay_ms(1);
	ssd2828_regconfig(0xee,0x0600);
#endif
	ssd2828_cfg_ok=1;
}
