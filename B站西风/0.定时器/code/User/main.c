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
unsigned char Seg_Disp_Mode;  // 数码管显示模式 0-时钟显示界面 1-时钟设置界面 2-闹钟设置界面
unsigned char Clock_Disp[3] = {23, 59, 55};  // 时钟显示
unsigned char Clock_Set[3];  // 时钟设置显示
unsigned char Clock_Set_Index;  // 时间选择指针 0-小时 1-分钟 2-秒钟
unsigned char Alarm[3] = {0, 0, 0};  // 闹钟时间
unsigned char Alarm_Set[3];  // 闹钟设置显示
unsigned int Timer_1000Ms;
unsigned int Timer_500Ms;
bit Seg_Flag;  // 数码管闪烁控制
bit Alarm_Flag;  // 闹钟使用/禁止控制
bit Alarm_Enable_Flag;  // 闹钟开启/关闭控制

// 按键
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // 读取上升沿
	Key_Old = Key_Val;  // 辅助变量
	if (Key_Down != 0) 
	{
		Alarm_Enable_Flag = 0;
	}
	switch (Key_Down)
	{
		case 4:  // 展示时钟设置界面
		{
			// 时钟设置显示的是按键按下的时间
			Alarm_Set[0] = Alarm[0];
			Alarm_Set[1] = Alarm[1];
			Alarm_Set[2] = Alarm[2];
			Seg_Disp_Mode = 1;
			Clock_Set_Index = 0;
			break;
		}
		case 5:  // 闹钟设置界面
		{
			Clock_Set[0] = Clock_Disp[0];
			Clock_Set[1] = Clock_Disp[1];
			Clock_Set[2] = Clock_Disp[2];
			Seg_Disp_Mode = 2;
			Clock_Set_Index = 0;
			break;
		}
		case 6:  // 时钟设置取消
		{
			if (Seg_Disp_Mode != 0)
			{
				Seg_Disp_Mode = 0;
			}
			break;
		}
		case 7:  // 设置确认，并展示时钟显示界面
		{
			if (Seg_Disp_Mode == 1)
			{
				Clock_Disp[0] = Clock_Set[0];
				Clock_Disp[1] = Clock_Set[1];
				Clock_Disp[2] = Clock_Set[2];
				Seg_Disp_Mode = 0;
			} 
			else if (Seg_Disp_Mode == 2) 
			{
				Alarm[0] = Alarm_Set[0];
				Alarm[1] = Alarm_Set[1];
				Alarm[2] = Alarm_Set[2];
				Seg_Disp_Mode = 0;
			}
			break;
		}
		case 8:  // 时钟设置切换
		{
			if (Seg_Disp_Mode != 0)
			{
				Clock_Set_Index = (Clock_Set_Index + 1) % 3;
			}
			break;
		}
		case 12:
		{
			switch (Seg_Disp_Mode)
			{
				case 1:  // 处于时钟设置界面
				{
					Clock_Set[Clock_Set_Index] ++ ;
					switch (Clock_Set_Index)
					{
						case 0:  // 小时位
						{
							if (Clock_Set[Clock_Set_Index] == 24)
							{
								Clock_Set[Clock_Set_Index] = 0;
							}
							break;
						}
						default:  // 分钟和秒钟位
						{
							if (Clock_Set[Clock_Set_Index] == 60)
							{
								Clock_Set[Clock_Set_Index] = 0;
							}
							break;
						}
					}
					break;
				}
				case 2:  // 处于闹钟设置界面
				{
					Alarm_Set[Clock_Set_Index] ++ ;
					switch (Clock_Set_Index)
					{
						case 0:  // 小时位
						{
							if (Alarm_Set[Clock_Set_Index] == 24)
							{
								Alarm_Set[Clock_Set_Index] = 0;
							}
							break;
						}
						default:  // 分钟和秒钟位
						{
							if (Alarm_Set[Clock_Set_Index] == 60)
							{
								Alarm_Set[Clock_Set_Index] = 0;
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 13: 
		{
			switch (Seg_Disp_Mode)
			{
				case 1:  // 处于时钟设置界面
				{
					Clock_Set[Clock_Set_Index] -- ;
					switch (Clock_Set_Index)
					{
						case 0:  // 小时位
						{
							if (Clock_Set[Clock_Set_Index] == 255)
							{
								Clock_Set[Clock_Set_Index] = 23;
							}
							break;
						}
						default:  // 分钟和秒钟位
						{
							if (Clock_Set[Clock_Set_Index] == 255)
							{
								Clock_Set[Clock_Set_Index] = 59;
							}
							break;
						}
					}
					break;
				}
				case 2:  // 处于闹钟设置界面
				{
					Alarm_Set[Clock_Set_Index] -- ;
					switch (Clock_Set_Index)
					{
						case 0:  // 小时位
						{
							if (Alarm_Set[Clock_Set_Index] == 255)
							{
								Alarm_Set[Clock_Set_Index] = 23;
							}
							break;
						}
						default:  // 分钟和秒钟位
						{
							if (Alarm_Set[Clock_Set_Index] == 255)
							{
								Alarm_Set[Clock_Set_Index] = 59;
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 16:  // 闹钟使能
		{
			Alarm_Flag ^= 1;
			break;
		}
	}
}

// 数码管
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	switch (Seg_Disp_Mode) 
	{
		case 0:  // 时钟显示界面
		{
			// 显示控制
			Seg_Buf[0] = Clock_Disp[0] / 10 % 10;
			Seg_Buf[1] = Clock_Disp[0]% 10;
			Seg_Buf[2] = 11;
			Seg_Buf[3] = Clock_Disp[1] / 10 % 10;
			Seg_Buf[4] = Clock_Disp[1] % 10;
			Seg_Buf[5] = 11;
			Seg_Buf[6] = Clock_Disp[2] / 10 % 10;
			Seg_Buf[7] = Clock_Disp[2] % 10;
			Seg_Point[1] = Seg_Point[4] = Seg_Point[7] = 1;
			break;
		}
		case 1:  // 时钟设置界面
		{
			// 显示控制
			Seg_Buf[0] = Clock_Set[0] / 10 % 10;
			Seg_Buf[1] = Clock_Set[0]% 10;
			Seg_Buf[2] = 11;
			Seg_Buf[3] = Clock_Set[1] / 10 % 10;
			Seg_Buf[4] = Clock_Set[1] % 10;
			Seg_Buf[5] = 11;
			Seg_Buf[6] = Clock_Set[2] / 10 % 10;
			Seg_Buf[7] = Clock_Set[2] % 10;
			Seg_Point[1] = Seg_Point[4] = Seg_Point[7] = 1;
			// 闪烁控制
			Seg_Buf[0 + 3 * Clock_Set_Index] = Seg_Flag ? Clock_Set[Clock_Set_Index] / 10 % 10 : 10;
			Seg_Buf[1 + 3 * Clock_Set_Index] = Seg_Flag ? Clock_Set[Clock_Set_Index] % 10 : 10;
			break;
		}
		case 2:  // 闹钟设置显示界面
		{
			// 显示控制
			Seg_Buf[0] = Alarm_Set[0] / 10 % 10;
			Seg_Buf[1] = Alarm_Set[0]% 10;
			Seg_Buf[2] = 11;
			Seg_Buf[3] = Alarm_Set[1] / 10 % 10;
			Seg_Buf[4] = Alarm_Set[1] % 10;
			Seg_Buf[5] = 11;
			Seg_Buf[6] = Alarm_Set[2] / 10 % 10;
			Seg_Buf[7] = Alarm_Set[2] % 10;
			Seg_Point[1] = Seg_Point[4] = Seg_Point[7] = 1;
			// 闪烁控制
			Seg_Buf[0 + 3 * Clock_Set_Index] = Seg_Flag ? Alarm_Set[Clock_Set_Index] / 10 % 10 : 10;
			Seg_Buf[1 + 3 * Clock_Set_Index] = Seg_Flag ? Alarm_Set[Clock_Set_Index] % 10 : 10;
			break;
		}
	}
}

// LED
void Led_Proc() 
{
	unsigned char i;
	if (Alarm_Flag)
	{
		if (Clock_Disp[0] == Alarm[0] && Clock_Disp[1] == Alarm[1] && Clock_Disp[2] == Alarm[2])
		{
			Alarm_Enable_Flag = 1;
		}
		if (Alarm_Enable_Flag)
		{
			for (i = 0; i < 8; i ++ )
			{
				if (Clock_Disp[0] >= 12)
				{
					if (i >= 4)
					{
						Led_Disp(i, Seg_Flag);
					} 
					else 
					{
						Led_Disp(i, 0);
					}
				}
				else
				{
					if (i < 4)
					{
						Led_Disp(i, Seg_Flag);
					} 
					else 
					{
						Led_Disp(i, 0);
					}
				}
			}
		}
		else
		{
			for (i = 0; i < 8; i ++ )
			{
				Led_Disp(i, 0);
			}
		}
	}
	else
	{
		for (i = 0; i < 8; i ++ )
		{
			Led_Disp(i, 0);
		}
	}
}

// 定时器0中断服务
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	Timer_1000Ms ++ ;
	if (Timer_1000Ms == 1000)
	{
		Timer_1000Ms = 0;
		Clock_Disp[2] ++ ;
		if (Clock_Disp[2] == 60) 
		{
			Clock_Disp[2] = 0;
			Clock_Disp[1] ++ ;
			if (Clock_Disp[1] == 60) 
			{
				Clock_Disp[1] = 0;
				Clock_Disp[0] ++ ;
				if (Clock_Disp[0] == 24) 
				{
					Clock_Disp[0] = 0;
				}
			}
		}
	}
	
	Timer_500Ms ++ ;
	if (Timer_500Ms == 500) 
	{
		Timer_500Ms = 0;
		Seg_Flag ^= 1;
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