#include "main.h"

unsigned char Key_Slow_Down, Seg_Slow_Down, Led_Slow_Down;
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;
unsigned char Seg_Buf[8];
unsigned char Seg_Point[8];


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
	unsigned char i;
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	for (i = 0; i < 8; i ++ )
	{
		Seg_Buf[i] = 10;
		Seg_Point[i] = 0;
	}
	
	
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