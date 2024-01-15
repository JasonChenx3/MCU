#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char

sbit L1 = P0 ^ 0;
sbit L8 = P0 ^ 7;

void Delay(uint t)
{
	while(t -- );
	while(t -- );
	while(t -- );
	while(t -- );
}

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

void Working()
{
	HC138(4);
	L1 = 0;
	Delay(60000);
	L1 = 1;
	Delay(60000);
}

// 中断初始化
void Init_INT1()
{
	IT1 = 1;
	EX1 = 1;
	EA = 1;
}

// 中断函数
void Service_INT1() interrupt 2
{
		L8 = 0;
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		L8 = 1;
}

void main()
{
	Init_INT1();
	while(1)
	{
		Working();
	}
}