// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>
#include <ONEWIRE.H>

// 变量
unsigned char i, j, k;
unsigned char Key_Slow_Down, Seg_Slow_Down;
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;
unsigned int Seg_Pos;
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int base10[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
int tp;
float ftp;     

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
	
	ftp = read_t();
	tp = ftp * 100;
	for (i = 4; i < 8; i ++ )
	{
		Seg_Buf[i] = (tp / base10[7 - i]) % 10;
	}
	Seg_Point[5] = 1;
}

// LED
void Led_Proc() 
{
	
}

// 定时器0中断服务
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	
}

// 定时器初始化
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x20;				//设置定时初始值
	TH0 = 0xD1;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA = 1;
}

void Delay750ms()		//@11.0592MHz
{
	unsigned char data i, j, k;

	_nop_();
	_nop_();
	i = 32;
	j = 133;
	k = 87;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

// Main
void main()
{
	unsigned char i;
	ftp = read_t();
	Delay750ms();
	Sys_Init();
	Timer0_Init();
	for (i = 0; i < 8; i ++ ) 
	{
		Led_Disp(i, 0);
	}
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}