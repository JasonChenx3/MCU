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

void Timer0_Init(void)		// 1毫秒@12.000MHz
{
	AUXR |= 0x80;			// 定时器时钟1T模式
	TMOD &= 0xF0;			// 设置定时器模式
	TL0 = 0x20;				// 设置定时初始值
	TH0 = 0xD1;				// 设置定时初始值
	TF0 = 0;				  // 清除TF0标志
	TR0 = 1;					// 定时器0开始计时
	ET0 = 1;					// 使能定时器0中断
	EA = 1;					  // 总中断开启
}

