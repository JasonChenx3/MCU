// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>
#include <ONEWIRE.H>

// 变量
unsigned char Key_Slow_Down;  // 按键扫描减速变量
unsigned int Seg_Slow_Down;  // 数码管扫描减速变量
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // 按键扫描专用变量
unsigned int Seg_Pos; // 数码管扫描变量
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // 数码管显示数组，默认全熄灭
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // // 数码管小数点显示数组，默认全熄灭
unsigned char work_mode;  // 风扇工作模式
unsigned int work_time;  // 风扇工作时间
unsigned char time_cnt;  // 按下了几次
bit show_mode;  // 显示模式，0显示风扇，1显示温度
unsigned int ms;  // 1ms计数
int temper;  // 温度（去除小数后的值）
unsigned int temper_read_cnt;
unsigned char PWM, timer1_cnt;

// 按键
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // 读取上升沿
	Key_Old = Key_Val;  // 辅助变量
	
	if (Key_Down == 4)  // 切换工作模式
	{
		if (show_mode == 0)  // 显示风扇参数有效
		{
			work_mode = (work_mode + 1) % 3;
			work_time = 0;
			time_cnt = 0;
			timer1_cnt = 0;
			switch (work_mode)
			{
				case 0:
					PWM = 20;
					break;
				case 1:
					PWM = 30;
					break;
				case 2:
					PWM = 70;
					break;
			}
		}
	}
	else if (Key_Down == 5)  // 切换定时
	{
		if (show_mode == 0)  // 显示风扇参数有效
		{
			ms = 0;
			time_cnt ++ ;
			work_time = time_cnt * 60;
			if (work_time > 120)
			{
				work_time = 0;
				time_cnt = 0;
			}
		}
	}
	else if (Key_Down == 6)  // 定时清零
	{
		if (show_mode == 0)  // 显示风扇参数有效
		{
			work_time = 0;
		}
	}
	else if (Key_Down == 7)  // 显示切换
	{
		show_mode = ~show_mode;
	}
}

// 数码管
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	if (show_mode)  // 显示温度参数
	{
		Seg_Buf[0] = 11;
		Seg_Buf[1] = 4;
		Seg_Buf[2] = 11;
		Seg_Buf[3] = 10;
		Seg_Buf[4] = 10;
		Seg_Buf[5] = temper >= 10 ? temper / 10 % 10 : 0;
		Seg_Buf[6] = temper >= 1 ? temper % 10 : 0;
		Seg_Buf[7] = 12;
	}
	else  // 显示风扇参数
	{
		Seg_Buf[0] = 11;
		Seg_Buf[1] = work_mode + 1;
		Seg_Buf[2] = 11;
		Seg_Buf[3] = 10;
		Seg_Buf[4] = 0;
		Seg_Buf[5] = work_time >= 100 ? work_time / 100 : 0;
		Seg_Buf[6] = work_time >= 10 ? work_time / 10 % 10 : 0;
		Seg_Buf[7] = work_time >= 1 ? work_time % 10 : 0;
	}
}

// LED
void Led_Proc() 
{
	unsigned char i;
	if (show_mode == 0)
	{
		if (work_time == 0) 
		{
			for (i = 0; i < 8; i ++ )
			{
				Led_Disp(i, 0);
			}
			return;
		}
		Led_Disp(work_mode, 1);
	}
	else
	{
		for (i = 0; i < 8; i ++ )
		{
			Led_Disp(i, 0);
		}
	}
}

// 定时器中断服务
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;  // 按键检测10ms一次
	if ( ++ Seg_Slow_Down == 2) Seg_Slow_Down = 0;  // 数码管更新2ms一次
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	if (work_time > 0)
	{
		if ( ++ ms == 1000)
		{
			ms = 0;
			work_time -= 1;
		}
	}
	else
	{
		ms = 0;
	}
	if ( ++ temper_read_cnt == 1000)
	{
		temper_read_cnt = 0;
	}
}

void Timer1_Isr() interrupt 3
{
	++ timer1_cnt;
	if (timer1_cnt <= PWM)
	{
		P34 = 1;
	}
	else 
	{
		P34 = 0;
	}
}

// 定时器初始化
void Timer0_Init()		//1毫秒@12.000MHz
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

void Timer1_Init()		//10微秒@11.0592MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x91;				//设置定时初始值
	TH1 = 0xFF;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
}



// Main
void main()
{
	Sys_Init();
	Timer0_Init();
	Timer1_Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		if (temper_read_cnt == 0)
		{
			temper = (int)read_t();
		}
	}
}