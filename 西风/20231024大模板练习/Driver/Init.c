#include <INIT.h>

void Sys_Init()  // 关闭外设子函数
{
	P0 = 0xFF;  // 关闭LED灯
	P2 = P2 & 0x1F | 0x80;  // 选中LED通道
	P2 &= 0x1F;  // 关闭LED通道 
	
	P0 = 0x00;  // 关闭蜂鸣器、继电器
	P2 = P2 & 0x1F | 0xA0;  // 选中外设通道
	P2 &= 0x1F;  // 关闭外设通道
}