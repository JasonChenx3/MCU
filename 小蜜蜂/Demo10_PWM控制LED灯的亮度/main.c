#include <STC15F2K60S2.H>
#define uchar unsigned char
#define uint unsigned int

sbit L1 = P0 ^ 0;
sbit S7 = P3 ^ 0;

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

// ===============定时器相关函数===============
uchar count = 0;
uchar pwm_duty = 0;
void InitTimer0()
{
	// 设置定时器工作模式，注意只能字节赋值
	TMOD = 0x01;
	// 设置计数初值
	// 每1us产生一次中断，一共产生一百次
	TH0 = (65535 - 100) / 256;
	TL0 = (65535 - 100) % 256;
	// 打开中断允许
	ET0 = 1;
	EA = 1;
}

void ServiceTimer0() interrupt 1
{
	// 16位不会自动重装，需要手动重装
	TH0 = (65535 - 100) / 256;
	TL0 = (65535 - 100) % 256;
	
	count ++ ;
	
	if (count == pwm_duty)
	{
		L1 = 1;
	}
	else if (count == 100)
	{
		L1 = 0;
		count = 0;
	}
}
// ============================================

// ===============按键相关函数===============
void DelayK(uint t)  // 软件去抖动延时函数
{
	while (t -- ) ;
}

uchar stat = 0;
void ScanKeys()  // 按键扫描函数
{
	if (S7 == 0)
	{
		DelayK(500);
		if (S7 == 0)
		{
			while (S7 == 0) ;
			switch(stat)
			{
				case 0:
					L1 = 0;
					TR0 = 1;
					pwm_duty = 10;
					stat = 1;
				break;
				
				case 1:
					pwm_duty = 50;
					stat = 2;
				break;
				
				case 2:
					pwm_duty = 90;
					stat = 3;
				break;
				
				case 3:
					L1 = 1;
					TR0 = 0;
					stat = 0;
				break;
			}
		}
	}
}
// ==========================================

void main()
{
	HC138(4);
	L1 = 1;
	InitTimer0();
	while(1)
	{
		ScanKeys();
	}
}