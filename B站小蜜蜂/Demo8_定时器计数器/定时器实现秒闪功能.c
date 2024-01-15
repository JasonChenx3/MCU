#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char

sbit L1 = P0 ^ 0;
sbit L8 = P0 ^ 7;

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

void InitTimer0()
{
	TMOD = 0x01;
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	ET0 = 1;
	EA = 1;
	// 由TR0来启动
	TR0 = 1;
}

uchar count = 0;
void ServiceTImer0() interrupt 1
{
	// 没有自动重装功能，需要重新写入计数初值
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	count ++ ;
	if (count % 10 == 0)
	{
		L1 = ~L1;
	}
	
	if (count == 100)
	{
		L8 = ~L8;
		count = 0;
	}
}

void main()
{
	HC138(4);
	InitTimer0();
	while(1)
	{
		
	}
}