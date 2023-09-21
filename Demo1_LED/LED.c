#include "reg52.h"
#define uint unsigned int

sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;

void HC138(uint n) 
{
	switch(n)
	{
		case 4:
			HC138_C = 1;
			HC138_B = 0;
			HC138_A = 0;
			break;
		case 5:
			HC138_C = 1;
			HC138_B = 0;
			HC138_A = 1;
			break;
		case 6:
			HC138_C = 1;
			HC138_B = 1;
			HC138_A = 0;
			break;
		case 7:
			HC138_C = 1;
			HC138_B = 1;
			HC138_A = 1;
			break;
	}
}

void Delay(uint t)
{
	while (t--);
	while (t--);
}

void LEDRunning()
{
	unsigned char i;
	HC138_C = 1;
	HC138_B = 0;
	HC138_A = 0;
	
	/*
	P0 = 0x00;  // LED全亮
	Delay(60000);
	P0 = 0xff;  // LED全灭
	Delay(60000);
	*/
	
	for (i = 1; i <= 8; i += 2)
	{
		P0 = (0xff << i);
		Delay(60000);
	}
	
	for (i = 1; i <= 8; i += 2)
	{
		P0 = ~(0xff << i);
		Delay(60000);
	}
}

void main()
{
	HC138(5);
	P0 = 0x00;
	while (1)
	{
		LEDRunning();
	}
}
