/* 头文件区 */
#include <STC15F2K60S2.H>
#include <INIT.H>
#include <LED.H>
#include <KEY.H>
#include <SEG.H>

/* 变量区 */
unsigned char Key_Slow_Down, Seg_Slow_Down;  // 减速变量
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // 按键扫描专用变量
unsigned char Seg_Pos; // 数码管扫描变量
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // 数码管显示数组，默认全熄灭
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // // 数码管小数点显示数组，默认全熄灭

/* 按键处理函数 */
void Key_Proc() 
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read(0);  // 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // 读取上升沿
	Key_Old = Key_Val;  // 辅助变量
	
	if (Key_Down)
	{
		Seg_Buf[7] = Key_Down % 10;
		Seg_Buf[6] = (Key_Down / 10) % 10;
		Led_Disp(0, Key_Down % 2);
	}
}

/* 信息处理函数 */
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	
}

/* 其他显示函数 */
void Led_Proc() 
{
	
}

/* 定时器0初始化函数 */
void Timer0_Init()		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA = 1;
}


/* 定时器0中断服务函数*/
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Key_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	
}

/* Main */
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