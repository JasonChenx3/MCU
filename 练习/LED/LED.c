#include <regx52.h>
#define uchar unsigned char 
#define uint unsigned int
	
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
	while (t -- ) ;
}

void LED_3()
{
	int i;
	for (i = 0; i < 3; i ++ )
	{
		P0 = 0x00;
		Delay(60000);
		Delay(60000);
		P0 = 0xff;
		Delay(60000);
		Delay(60000);
	}
}

void LED() 
{
	int i;
	LED_3();
	
	for (i = 0; i < 8; i ++ ) 
	{
		P0 = 0xfe << i;
		Delay(60000);
		Delay(60000);
	}
	
	for (i = 0; i < 8; i ++ )
	{
		P0 |= 0x01 << i;
		Delay(60000);
		Delay(60000);
	}
}

void main()
{
	HC138(4);
	while(1)
	{
		LED();
	}
}