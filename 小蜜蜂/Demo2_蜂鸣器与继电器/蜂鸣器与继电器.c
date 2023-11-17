#include "reg52.h"
#define uint unsigned int

//sbit HC138_A = P2^5;
//sbit HC138_B = P2^6;
//sbit HC138_C = P2^7;

void HC138(uint n) 
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
	HC138(channel);
	P0 = dat;
}

void Delay(uint t)
{
	while (t--);
	while (t--);
}

void LEDRunning()
{
	unsigned char i;
	
	for (i = 0; i < 3; i++)
	{
		OutPutP0(4, 0x00);
		Delay(60000);
		Delay(60000);
		OutPutP0(4, 0xff);
		Delay(60000);
		Delay(60000);
	}
	
	
	// LED 亮
	for (i = 1; i <= 8; i++)
	{
		OutPutP0(4, 0xff << i);
		Delay(60000);
		Delay(60000);
	}
	
//	// 继电器
//	OutPutP0(5, 0x10);  // 选通Y5，继电器吸上
//	Delay(60000);
//	Delay(60000);
//	OutPutP0(5, 0x00);  // 选通Y5，继电器断开
//	
//	// 蜂鸣器
//	OutPutP0(5, 0x40);  // 选通Y5，蜂鸣器打开
//	Delay(60000);
//	Delay(60000);
//	OutPutP0(5, 0x00);  // 选通Y5，蜂鸣器关闭
	
	// LED 灭
	for (i = 1; i <= 8; i ++)
	{
		OutPutP0(4, ~(0xff << i));
		Delay(60000);
		Delay(60000);
	}
}

void InitSystem()
{
	OutPutP0(5, 0x00);
}

void main()
{
	InitSystem();
	while (1)
	{
		LEDRunning();
	}
}
