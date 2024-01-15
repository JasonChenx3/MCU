#include "main.h"

unsigned char Key_Slow_Down, Seg_Slow_Down, Led_Slow_Down;  	// 减速变量
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  						// 按键扫描
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // 数码管显示控制
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  			// 小数点控制: 0-熄灭 1-点亮


void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  											// 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  	// 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  	// 读取上升沿
	Key_Old = Key_Val;  												// 辅助变量
	
	
}

void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	
}

void Led_Proc() 
{
	unsigned char i;
	if (Led_Slow_Down) return;
	Led_Slow_Down = 1;
	
	for (i = 0; i < 8; i ++ )
	{
		Led_Disp(i, 0);
	}

	
}

void Timer0_Isr() interrupt 1 
{
	static unsigned int Pos = 0;  // 位置指针
	
	Key_Slow_Down = (Key_Slow_Down + 1) % 10;  // Key更新频率 10ms
	Seg_Slow_Down = (Seg_Slow_Down + 1) % 2;   // Seg更新频率  2ms
	Led_Slow_Down = (Led_Slow_Down + 1) % 2;   // LED更新频率  2ms
	Pos = (Pos + 1) % 8;
	
	Seg_Disp(Pos, Seg_Buf[Pos], Seg_Point[Pos]);
	
	
}

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