/* 头文件声明区域 */
#include <STC15F2K60S2.H>  // 15单片机寄存器专属头文件
#include <KEY.H>  // 按键底层专属头文件
#include <SEG.H>  // 数码管底层专属头文件
#include <INIT.h>  // 初始化底层专属头文件
#include <LED.H>  // LED底层专属头文件

/* 变量声明区域 */
unsigned char Key_Slow_Down;  // 按键减速专用变量
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // 按键扫描专用变量
unsigned int Seg_Slow_Down;  // 数码管减速专用变量
unsigned char Seg_Pos;  // 数码管扫描专用变量
unsigned char Seg_Buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};  // 数码管数据显示数组 默认全部熄灭
unsigned char Seg_Point[8] = {1, 1, 1, 1, 1, 1, 1, 0};  // 数码管小数点数据数组 默认全部熄灭

/* 按键处理函数 */
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;  // 按键减速程序
	
	Key_Val = Key_Read();  // 实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);  // 读取按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);  // 读取按键上升沿
	Key_Old = Key_Val;  // 辅助扫描变量
	
	
}

/* 信息处理函数 */
void Seg_Proc()
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;  // 数码管减速程序
	
	
}

/* 其他显示函数 */
void Led_Proc()
{
	
}

/* 定时器0初始化函数 */
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;
	EA = 1;
}

/* 定时器0中断服务函数 */
void Timer0_Server() interrupt 1
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Seg_Slow_Down = 0; 
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	
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