// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>

// 变量
unsigned char Key_Slow_Down = 0, Seg_Slow_Down = 0;  					// 减速变量
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  						// 按键扫描
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // 数码管显示控制
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  			// 小数点控制: 0-熄灭 1-点亮
unsigned char Led_State[8] = {0, 0, 0, 0, 0, 0, 0, 0};  			// LED控制: 0-熄灭 1-点亮

// 按键
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // 读取上升沿
	Key_Old = Key_Val;  // 辅助变量
	
}

// 数码管
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
}

// LED
void Led_Proc() 
{
	
}

// 定时器0中断服务
void Timer0_Isr() interrupt 1 
{
	static unsigned int Pos = 0;  // 位置指针
	
	Key_Slow_Down = (Key_Slow_Down + 1) % 10;  // 按键扫描 10ms 一次
	Seg_Slow_Down = (Seg_Slow_Down + 1) % 50;  // 数码管扫描 50ms 一次
	Pos = (Pos + 1) % 8;
	
	Seg_Disp(Pos, Seg_Buf[Pos], Seg_Point[Pos]);
	Led_Disp(Pos, Led_State[Pos]);
	
}

// 定时器初始化
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


// Main
void main()
{
	Sys_Init();
	Timer0_Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}