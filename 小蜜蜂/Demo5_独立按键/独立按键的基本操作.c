#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int

sbit S7 = P3 ^ 0;
sbit S6 = P3 ^ 1;
sbit S5 = P3 ^ 2;
sbit S4 = P3 ^ 3;

sbit L1 = P0 ^ 0;
sbit L2 = P0 ^ 1;
sbit L3 = P0 ^ 2;
sbit L4 = P0 ^ 3;
sbit L5 = P0 ^ 4;
sbit L6 = P0 ^ 5;

void HC138(uchar channel)
{
	switch (channel)
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

void OutPutP0(uchar channel, uchar dat)
{
	HC138(channel);
	P0 = dat;
}

void DelayK(uchar t)
{
	while(t--);
}

void ScanKeys_Alone()
{
	OutPutP0(4, 0xff);
	if (S7 == 0)
	{
		DelayK(100);
		if (S7 == 0) 
		{
			L1 = 0;
			while (S7 == 0);
			L1 = 1;
		}
	}
	if (S6 == 0)
	{
		DelayK(100);
		if (S6 == 0)
		{
			L2 = 0;
			while (S6 == 0);
			L2 = 1;
		}
	}
	if (S5 == 0)
	{
		DelayK(100);
		if (S5 == 0)
		{
			L3 = 0;
			while (S5 == 0);
			L3 = 1;
		}
	}
	if (S4 == 0)
	{
		DelayK(100);
		if (S4 == 0)
		{
			L4 = 0;
			while (S4 == 0);
			L4 = 1;
		}
	}
}

void main()
{
	while(1)
	{
		ScanKeys_Alone();
	}
}