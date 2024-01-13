// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>

// 变量
unsigned char Key_Slow_Down, Seg_Slow_Down;  // 减速变量
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // 按键扫描专用变量
unsigned int Seg_Pos; // 数码管扫描变量
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // 数码管显示数组，默认全熄灭
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // // 数码管小数点显示数组，默认全熄灭
unsigned char time_h = 23, time_m = 59, time_s = 59;  // 时钟
unsigned char alarm_h = 0, alarm_m = 0, alarm_s = 0;  // 闹钟
unsigned int ms_cnt;
unsigned int led_time, led_cnt;
bit led_state;

// 按键
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // 读取上升沿
	Key_Old = Key_Val;  // 辅助变量
	
	// 定时到达，按任意键停止闪烁
	if (led_time > 0 && Key_Down != 0)
	{
		led_time = 0;
		Led_Disp(0, 0);
	}
}

// 数码管
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	// 缓冲区更新
	Seg_Buf[0] = time_h / 10;
	Seg_Buf[1] = time_h % 10;
	Seg_Buf[2] = 11;
	Seg_Buf[3] = time_m / 10;
	Seg_Buf[4] = time_m % 10;
	Seg_Buf[5] = 11;
	Seg_Buf[6] = time_s / 10;
	Seg_Buf[7] = time_s % 10;
	
	// 到达定时闹钟，设置定时参数
	if (time_h == alarm_h && time_m == alarm_m && time_s == alarm_s)
	{
		led_time = 5000;
		led_cnt = 0;
	}
}

// LED
void Led_Proc() 
{
	
}

// 定时器0中断服务
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 50) Seg_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	// 更新时间
	if ( ++ ms_cnt == 1000)
	{
		ms_cnt = 0;
		if ( ++ time_s == 60)
		{
			time_s = 0;
			if ( ++ time_m == 60)
			{
				time_m = 0;
				if ( ++ time_h == 24)
				{
					time_h = 0;
				}
			}
		}
	}
	
	// 闹钟LED显示
	if (led_time > 0)
	{
		if (led_cnt == 0)
		{
			led_state = ~led_state;
			Led_Disp(0, led_state);
		}
		led_cnt = (led_cnt + 1) % 200;
		led_time -= 1;
	}
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