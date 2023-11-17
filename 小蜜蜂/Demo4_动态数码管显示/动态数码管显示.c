#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char

unsigned char month = 1;
unsigned char code SMG_duanma[18] = 
	{0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
	 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
	 0xbf, 0x7f};
	 
void InitHC138(unsigned char n)
{
	switch(n)
	{
		case 4:
			P2 = (P2 & 0x1f) | 0x80;
		break;
		case 5:
			P2 = (P2 & 0x1f) | 0xa0;
		break;
		case 6:
			P2 = (P2 & 0x1f) | 0xc0;
		break;
		case 7:
			P2 = (P2 & 0x1f) | 0xe0;
		break;
	}
}

void OutPutP0(unsigned char channel, unsigned char dat)
{
	InitHC138(channel);
	P0 = dat;
}

void ShowSMG_Bit(unsigned char dat, unsigned char pos)
{
	OutPutP0(6, 0x01 << pos);  // 数码管的位置
	OutPutP0(7, dat);  // 数码管的内容
}

void Delay_SMG(unsigned int t)
{
	while (t--);
}

void SMG_Dynamic()
{
	ShowSMG_Bit(SMG_duanma[2], 0);
	Delay_SMG(500);
	ShowSMG_Bit(SMG_duanma[0], 1);
	Delay_SMG(500);
	ShowSMG_Bit(SMG_duanma[2], 2);
	Delay_SMG(500);
	ShowSMG_Bit(SMG_duanma[3], 3);
	Delay_SMG(500);
	
	ShowSMG_Bit(SMG_duanma[16], 4);
	Delay_SMG(500);
	ShowSMG_Bit(SMG_duanma[16], 5);
	Delay_SMG(500);
	ShowSMG_Bit(SMG_duanma[month / 10], 6);
	Delay_SMG(500);
	ShowSMG_Bit(SMG_duanma[month % 10], 7);
	Delay_SMG(500);
}

void Delay(unsigned char t)
{
	while (t--)
	{
		SMG_Dynamic();
	}
}
	 
void InitSystem()
{
	OutPutP0(4, 0xff);  // 关闭LEd
	OutPutP0(5, 0x00);  // 关闭蜂鸣器
}

void main()
{
	InitSystem();
	while (1)
	{
		SMG_Dynamic();
		month ++ ;
		month %= 13;
		if (month == 0)
		{
			month ++ ;
		}
		Delay(100);
	}
}