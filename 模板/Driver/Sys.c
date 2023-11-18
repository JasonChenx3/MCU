#include <SYS.H>

void Sys_Init()
{
	// 关闭LED灯
	P0 = 0xFF;
	P2 = P2 & 0x1F | 0x80;
	P2 &= 0x1F;
	
	// 关闭蜂鸣器和继电器
	P0 = 0x00;
	P2 = P2 & 0x1F | 0xA0;
	P2 &= 0x1F;
}