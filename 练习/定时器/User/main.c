/* 头文件 */
#include <STC15F2K60S2.H>
#include "INIT.H"
#include "LED.H"
#include "KEY.H"
#include "SEG.H"

/* 变量区 */
unsigned char Key_Slow_Down;  // 按键减速
unsigned char Key_Val, Key_Down, Key_Up, Key_Old;  // 按键相关
unsigned char Seg_Slow_Down;  // 数码管减速
unsigned char Seg_Pos;  // 数码管扫描
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // 数码管显示
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 数码管小数点
unsigned char ms, _50ms, s, min;  // 时间显示
unsigned char run;

/* 按键处理 */
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read(1);  // 读取临时矩阵按键
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // 读取按键下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // 读取按键上升沿
	Key_Old = Key_Val;
	
	switch (Key_Down)
	{
		case 4:
			run ^= 1;
		break;
		case 5:
		{
				if (run == 0)
				{
					ms = 0;
					_50ms = 0;
					s = 0;
					min = 0;
				}
				break;
		}
	}
	
	Seg_Buf[0] = (min / 10) % 10;
	Seg_Buf[1] = (min % 10);
	Seg_Buf[2] = 11;
	Seg_Buf[3] = (s / 10) % 10;
	Seg_Buf[4] = s % 10;
	Seg_Buf[5] = 11;
	Seg_Buf[6] = (_50ms / 10) % 10;
	Seg_Buf[7] = _50ms % 10;
}


/* 信息处理 */
void Seg_Proc()
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	
}


/* 其他显示 */
void Led_Proc()
{
	
}

/* 定时器0初始化 */
void Timer0_Init()		//1毫秒@12.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x20;				//设置定时初始值
	TH0 = 0xD1;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定
	EA = 1;  //打开总中断
}

/* 定时器0中断服务函数 */
void Timer0_Isr() interrupt 1
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	if (run)
	{
		ms ++ ;
		if (ms == 50)
		{
			ms = 0;
			_50ms ++ ;
			if (_50ms == 20)
			{
				_50ms = 0;
				s ++ ;
				if (s == 60)
				{
					s = 0;
					min ++ ;
					if (min == 60)
					{
						min = 0;
					}
				}
			}
		}
	}
}


/* Main */
void main()
{
	Sys_Init();
	Timer0_Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}