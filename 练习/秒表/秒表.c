#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char



uchar t_m = 0;
uchar t_s = 0;
uchar t_005s = 0;

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

void Delay(uint t)
{
	while (t -- ) ;
}

void ShowSMG_Bit(uchar value, uchar pos)
{
	HC138(6);
	P0 = 0x01 << pos;
	HC138(7);
	P0 = value;
}

void ShowSMG() 
{
	ShowSMG_Bit();
}

void InitTimer0()
{
	TMOD = 0x01;
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void ServiceTimer0() interrupt 1
{
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	t_005s ++ ;
	if (t_005s == 20)
	{
		t_005s = 0;
		t_s ++ ;
		if (t_s == 60)
		{
			t_m ++ ;
			if (t_m == 100)
			{
				t_m = 0;
			}
		}
	}
}

void main()
{
	while(1)
	{
		
	}
}