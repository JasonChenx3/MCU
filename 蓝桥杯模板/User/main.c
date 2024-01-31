#include "main.h"

unsigned int Proc_Slow_Down;
unsigned char Proc_Flag;
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char ucLed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float float_temperature;
int int_temperature;

void Key_Proc()
{
	if (Proc_Flag != 1) 
		return;
	Proc_Flag = 0;
	
	Key_Val = Key_Read();
	Key_Down = Key_Val & (Key_Val ^ Key_Old);
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
	Key_Old = Key_Val;
	
	
}

void Seg_Proc() 
{
	if (Proc_Flag != 2) 
		return;
	Proc_Flag = 0;
	
}

void Led_Proc() 
{
	
}

void Timer0_Isr() interrupt 1 
{
	static unsigned int Pos = 0;
	
	Proc_Slow_Down = (Proc_Slow_Down + 1) % 500;
	if (Proc_Slow_Down % 9 == 0)
		Proc_Flag = 1;
	if (Proc_Slow_Down % 49 == 0)
		Proc_Flag = 2;
	
	Pos = (Pos + 1) % 8;
	Seg_Disp(Pos, Seg_Buf[Pos], Seg_Point[Pos]);
	Led_Disp(Pos, ucLed[Pos]);
	
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