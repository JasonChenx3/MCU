#include <STC15F2K60S2.H>
#include <INTRINS.H>

void HC138(unsigned char channel)
{
	switch (channel)
	{
		case 0:
			P2 = (P2 & 0x1F);
			break;
		case 4:
			P2 = (P2 & 0x1F) | 0x80;
			break;
		case 5:
			P2 = (P2 & 0x1F) | 0xA0;
			break;
		case 6:
			P2 = (P2 & 0x1F) | 0xC0;
			break;
		case 7:
			P2 = (P2 & 0x1F) | 0xE0;
			break;
	}
}

void Delay500ms()		//@12.000MHz
{
	unsigned char data i, j, k;

	_nop_();
	_nop_();
	i = 23;
	j = 205;
	k = 120;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void LED()
{
	unsigned char i;
	HC138(4);
	for (i = 0; i < 8; i ++ )
	{
		P0 = ~(1 << i);
		Delay500ms();
	}
}

void main()
{
	while(1)
	{
		LED();
	}
}