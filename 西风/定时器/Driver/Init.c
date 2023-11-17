#include <INIT.H>

unsigned char HC138(unsigned char channel)
{
	switch(channel)
	{
		case 0:
			P2 = (P2 & 0x1f);
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

void Sys_Init() 
{
	// 关闭LED
	HC138(4);
	P0 = 0xFF;
	HC138(0);
	// 关闭蜂鸣器与继电器
	HC138(5);
	P0 = 0x00;
	HC138(0);
}

