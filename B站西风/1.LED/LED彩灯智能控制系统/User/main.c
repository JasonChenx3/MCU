// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>

// 变量
unsigned char Key_Slow_Down = 0, Seg_Slow_Down = 0;  					// 减速变量
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  						// 按键扫描
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // 数码管显示控制
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  			// 小数点控制: 0-熄灭 1-点亮

unsigned int Timer_1ms;
unsigned int Timer_400ms;

unsigned int Led_Time[] = {400, 400, 400, 400};  // Led每个模式的循环时间
unsigned int Led_Time_Set[4];  // LED设置时间
unsigned char Led_Time_Set_Index;  // LED设置时间指针
unsigned char Led_Data;

bit Sys_Run_Flag;  // 系统启动标志 0-未启动 1-启动
bit Set_Flag;  // 0-模式编号 1-流转时间
bit Seg_Flashing_Flag;  // 数码管闪烁标志
bit Data_Disp_Mode;  // 0-显示状态 1-显示数据

unsigned char Led_Mode;  // Led显示模式
unsigned char Seg_Disp_Mode;  // 0-运行状态界面 1-流转时间设置界面 2-数据显示界面

// 按键
void Key_Proc()
{
	unsigned char i;
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // 读取上升沿
	Key_Old = Key_Val;  // 辅助变量
	
	if (Sys_Run_Flag == 0)
	{
		if (Key_Old == 4 && Seg_Disp_Mode == 0)
		{
			Data_Disp_Mode = 1;
		}
		else
		{
			Data_Disp_Mode = 0;
		}
	}
	else
	{
		Data_Disp_Mode = 0;
	}
	
	switch (Key_Down)
	{
		case 4:  // 参数减
		{
			if (Seg_Disp_Mode == 1)
			{
				if (Set_Flag == 0)
				{
					if ( -- Led_Time_Set_Index == 255)
					{
						Led_Time_Set_Index = 3;
					}
				}
				else
				{
					Led_Time_Set[Led_Time_Set_Index] -= 100;
					if (Led_Time_Set[Led_Time_Set_Index] == 300)
					{
						Led_Time_Set[Led_Time_Set_Index] = 1200;
					}
				}
			}
			break;
		}
		case 5:  // 参数加
		{
			if (Seg_Disp_Mode == 1)
			{
				if (Set_Flag == 0)
				{
					if ( ++ Led_Time_Set_Index == 4)
					{
						Led_Time_Set_Index = 0;
					}
				}
				else
				{
					Led_Time_Set[Led_Time_Set_Index] += 100;
					if (Led_Time_Set[Led_Time_Set_Index] == 1300)
					{
						Led_Time_Set[Led_Time_Set_Index] = 400;
					}
				}
			}
			break;
		}
		case 6:  // 设置界面
		{
			if (Sys_Run_Flag == 0)
			{
				if (Seg_Disp_Mode == 0)
				{
					for (i = 0; i < 4; i ++ )
					{
						Led_Time_Set[i] = Led_Time[i];
					}
					Seg_Disp_Mode = 1;
				}
				else if (Seg_Disp_Mode == 1)
				{
					Set_Flag ^= 1;
					if (Set_Flag == 0)
					{
						for (i = 0; i < 4; i ++ )
						{
							Led_Time[i] = Led_Time_Set[i];
						}
						Seg_Disp_Mode = 0;
					}
				}
			}
			break;
		}
		case 7:  // 启动/暂停
		{
			Sys_Run_Flag ^= 1;
			break;
		}
	}
}

// 数码管
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
	
	switch (Seg_Disp_Mode)
	{
		case 0:  // 运行状态界面
		{
			if (Data_Disp_Mode == 0)
			{
				Seg_Buf[0] = Sys_Run_Flag ? 11 : 5;
				Seg_Buf[1] = Led_Mode + 1;
				Seg_Buf[4] = Led_Time[Led_Mode] / 1000 % 10;
				Seg_Buf[5] = Led_Time[Led_Mode] / 100 % 10;
				Seg_Buf[6] = Led_Time[Led_Mode] / 10 % 10;
				Seg_Buf[7] = Led_Time[Led_Mode] % 10;
				Seg_Buf[4] = Seg_Buf[4] == 0 ? 10 : Seg_Buf[4];
			}
			else
			{
				Seg_Buf[0] = 13;
				Seg_Point[0] = 1;
				Seg_Buf[1] = 11;
				Seg_Point[1] = 1;
				
				Seg_Buf[2] = 12;
				Seg_Buf[3] = Led_Mode + 1;
				
				Seg_Buf[4] = Led_Data / 10 % 10;
				Seg_Buf[5] = Led_Data % 10;
			}
			break;
		}
		case 1:  // 流转时间设置界面
		{
			Seg_Buf[0] = 12;
			Seg_Buf[1] = Led_Time_Set_Index + 1;
			Seg_Buf[4] = Led_Time_Set[Led_Time_Set_Index] / 1000 % 10;
			Seg_Buf[5] = Led_Time_Set[Led_Time_Set_Index] / 100 % 10;
			Seg_Buf[6] = Led_Time_Set[Led_Time_Set_Index] / 10 % 10;
			Seg_Buf[7] = Led_Time_Set[Led_Time_Set_Index] % 10;
			Seg_Buf[4] = Seg_Buf[4] == 0 ? 10 : Seg_Buf[4];
			if (Set_Flag == 0)
			{
				Seg_Buf[0] = Seg_Flashing_Flag ? Seg_Buf[0] : 10;
				Seg_Buf[1] = Seg_Flashing_Flag ? Seg_Buf[1] : 10;
			}
			else
			{
				Seg_Buf[4] = Seg_Flashing_Flag ? Seg_Buf[4] : 10;
				Seg_Buf[5] = Seg_Flashing_Flag ? Seg_Buf[5] : 10;
				Seg_Buf[6] = Seg_Flashing_Flag ? Seg_Buf[6] : 10;
				Seg_Buf[7] = Seg_Flashing_Flag ? Seg_Buf[7] : 10;
			}
			break;
		}
	}
}

// LED
void Led_Proc() 
{
	unsigned char i;
	static unsigned char Led_Cnt = 0;
	if (Timer_1ms == Led_Time[Led_Mode])
	{
		Timer_1ms = 0;
		
		if (Led_Mode == 0 || Led_Mode == 1)
		{
			if (Led_Cnt == 8)
			{
				Led_Cnt = 0;
				Led_Mode = (Led_Mode + 1) % 4;
			}
		}
		else
		{
			if (Led_Cnt == 4)
			{
				Led_Cnt = 0;
				Led_Mode = (Led_Mode + 1) % 4;
			}
		}
		
		for (i = 0; i < 8; i ++ )
		{
			Led_Disp(i, 0);
		}
		
		switch (Led_Mode)
		{
			case 0:
			{
				Led_Disp(Led_Cnt, 1);
				Led_Data = Led_Cnt + 1;
				break;
			}
			case 1:
			{
				Led_Disp(7 - Led_Cnt, 1);
				Led_Data = 7 - Led_Cnt + 1;
				break;
			}
			case 2:
			{
				Led_Disp(Led_Cnt, 1);
				Led_Disp(7 - Led_Cnt, 1);
				Led_Data = (Led_Cnt + 1) * 10 + 7 - Led_Cnt + 1;
				break;
			}
			case 3:
			{
				Led_Disp(3 - Led_Cnt, 1);
				Led_Disp(4 + Led_Cnt, 1);
				Led_Data = (3 - Led_Cnt + 1) * 10 + 4 + Led_Cnt + 1;
				break;
			}
		}
		Led_Cnt ++ ;
	}
}

// 定时器0中断服务
void Timer0_Isr() interrupt 1 
{
	static unsigned int Pos = 0;  // 位置指针
	
	Key_Slow_Down = (Key_Slow_Down + 1) % 10;  // 按键扫描 10ms 一次
	Seg_Slow_Down = (Seg_Slow_Down + 1) % 50;  // 数码管扫描 50ms 一次
	Pos = (Pos + 1) % 8;
	
	Seg_Disp(Pos, Seg_Buf[Pos], Seg_Point[Pos]);
	
	if (Sys_Run_Flag)
	{
		Timer_1ms ++ ;
	} 
	else
	{
		Timer_1ms = 0;
	}
	
	if ( ++ Timer_400ms == 400) 
	{
		Timer_400ms = 0;
		Seg_Flashing_Flag ^= 1;
	}
}

// 定时器初始化
void Timer0_Init(void)		// 1毫秒@12.000MHz
{
	AUXR |= 0x80;			// 定时器时钟1T模式
	TMOD &= 0xF0;			// 设置定时器模式
	TL0 = 0x20;				// 设置定时初始值
	TH0 = 0xD1;				// 设置定时初始值
	TF0 = 0;				  // 清除TF0标志
	TR0 = 1;					// 定时器0开始计时
	ET0 = 1;					// 使能定时器0中断
	EA = 1;					  // 总中断开启
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