#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char
	
// P4引脚，头文件没有定义，需要自行定义
sfr P4 = 0xC0;
	
// 根据原理图来定义
sbit R1 = P3 ^ 0;
sbit R2 = P3 ^ 1;
sbit R3 = P3 ^ 2;
sbit R4 = P3 ^ 3;

// 根据原理图来定义
sbit C4 = P3 ^ 4;
sbit C3 = P3 ^ 5;
sbit C2 = P4 ^ 2;
sbit C1 = P4 ^ 4;

// 数码管的段码
uchar code SMG_duanma[18] = 
	{0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
	 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
	 0xbf, 0x7f};

// 选通HC138的某个信道
void HC138(uchar channel) 
{
	switch(channel)
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

// 数码管展示value这个数值
void DisplayKeyNum(uchar value)
{
	HC138(6);
	P0 = 0x01;
	HC138(7);
	P0 = value;
}

// 判断扫描的按键
uchar key_num;
// 扫描键盘函数
void ScanKeysMulti()
{
	// 判断第一行的按键
	R1 = 0;
	R2 = R3 = R4 = 1;
	// 先向引脚输出高电平
	C1 = C2 = C3 = C4 = 1;
	// 然后再检测哪个为低电平
	if (C1 == 0)
	{
		// 等松手之后再进行判断
		while (C1 == 0);
		key_num = 0;
		// 展示数码
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C2 == 0) 
	{
		while(C2 == 0);
		key_num = 1;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C3 == 0)
	{
		while(C3 == 0);
		key_num = 2;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C4 == 0)
	{
		while(C4 == 0);
		key_num = 3;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	
	// 判断第二行的按键
	R2 = 0;
	R1 = R3 = R4 = 1;
	C1 = C2 = C3 = C4 = 1;
	if (C1 == 0)
	{
		while (C1 == 0);
		key_num = 4;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C2 == 0) 
	{
		while(C2 == 0);
		key_num = 5;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C3 == 0)
	{
		while(C3 == 0);
		key_num = 6;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C4 == 0)
	{
		while(C4 == 0);
		key_num = 7;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	
	// 判断第三行的按键
	R3 = 0;
	R1 = R2 = R4 = 1;
	C1 = C2 = C3 = C4 = 1;
	if (C1 == 0)
	{
		while (C1 == 0);
		key_num = 8;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C2 == 0) 
	{
		while(C2 == 0);
		key_num = 9;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C3 == 0)
	{
		while(C3 == 0);
		key_num = 10;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C4 == 0)
	{
		while(C4 == 0);
		key_num = 11;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	
	// 判断第四行的按键
	R4 = 0;
	R2 = R3 = R1 = 1;
	C1 = C2 = C3 = C4 = 1;
	if (C1 == 0)
	{
		while (C1 == 0);
		key_num = 12;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C2 == 0) 
	{
		while(C2 == 0);
		key_num = 13;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C3 == 0)
	{
		while(C3 == 0);
		key_num = 14;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C4 == 0)
	{
		while(C4 == 0);
		key_num = 15;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
}

void main()
{
	while(1)
	{
		ScanKeysMulti();
	}
}