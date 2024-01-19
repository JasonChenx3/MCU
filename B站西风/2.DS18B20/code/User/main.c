#include "main.h"

unsigned char Key_Slow_Down, Seg_Slow_Down, Led_Slow_Down;
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned char Seg_Disp_Mode;  // 0-温度 1-参数 2-精度
unsigned char T_Accuracy = 4;

unsigned char Config_T[2] = {20, 30};  // 温度值 0-min 1-max
unsigned char Config_T_Set[2];
bit Config_T_Index;  // 0-min 1-max

bit Seg_Flash_Flag;  // 闪烁控制
unsigned int Timer_1ms;

float float_t;  // 实时温度(float)
int int_t;  // 实时温度(int)

unsigned int Key_Timer;
bit Key_Flag;

void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  											// 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  	// 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  	// 读取上升沿
	Key_Old = Key_Val;  												// 辅助变量
	
	if (Seg_Disp_Mode == 1)
	{
		if (Key_Down == 14)
		{
			Key_Flag = 1;
			return;
		}
	}
	
	if (Key_Timer < 500)
	{
		if (Key_Up == 14)  // 短按
		{
			Key_Flag = Key_Timer = 0;
			if ( ++ Config_T_Set[Config_T_Index] == 71)
			{
				Config_T_Set[Config_T_Index] = 10;
			}
		}
	}
	else
	{
		if (Key_Old == 14)
		{
			if ( ++ Config_T_Set[Config_T_Index] == 71)
			{
				Config_T_Set[Config_T_Index] = 10;
			}
		}
		if (Key_Up == 14)
		{
			Key_Flag = Key_Timer = 0;
		}
	}
	
	switch (Key_Down)
	{
		case 12:  // 界面切换
		{
			Seg_Disp_Mode = (Seg_Disp_Mode + 1) % 3;
			
			switch (Seg_Disp_Mode)
			{
				case 0:
				{
					break;
				}
				case 1:
				{
					Config_T_Index = 1;
					Config_T_Set[0] = Config_T[0];
					Config_T_Set[1] = Config_T[1];
					break;
				}
				case 2:
				{
					// 确认设置参数，差错检验
					if (Config_T_Set[1] >= Config_T_Set[0])
					{
							Config_T[0] = Config_T_Set[0];
							Config_T[1] = Config_T_Set[1];
					}
//						Config_T[0] = Config_T_Set[0];
//						Config_T[1] = Config_T_Set[1];
					break;
				}
			}
			break;
		}
		
		case 13:  // 参数切换
		{
			if (Seg_Disp_Mode == 1)  // 参数界面有效
			{
				Config_T_Index ^= 1;
			}
			break;
		}
		
		case 14:  // 参数加
		{
			if (Seg_Disp_Mode == 1)  // 参数界面
			{
				if ( ++ Config_T_Set[Config_T_Index] == 71)
				{
					Config_T_Set[Config_T_Index] = 10;
				}
			}
			break;
		}
		
		case 15:  // 参数减
		{
			if (Seg_Disp_Mode == 1)  // 参数界面
			{
				if ( -- Config_T_Set[Config_T_Index] == 9)
				{
					Config_T_Set[Config_T_Index] = 70;
				}
			}
			break;
		}
		
		case 16:  // 重置
		{
			Seg_Disp_Mode = 0;
			Config_T[0] = 20;
			Config_T[1] = 30;
			Config_T_Index = 0;
			T_Accuracy = 4;
			break;
		}
	}
}

void Seg_Proc() 
{
	unsigned char i;
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	switch (Seg_Disp_Mode)
	{
		case 0:  // 温度
		{
			Seg_Buf[0] = 11;
			Seg_Buf[4] = int_t / 1000 % 10;
			Seg_Buf[5] = int_t / 100 % 10;
			Seg_Buf[6] = int_t / 10 % 10;
			Seg_Buf[7] = 11;
			Seg_Point[5] = 1;
			break;
		}
		
		case 1:  // 参数
		{
			Seg_Buf[0] = 12;
			Seg_Buf[1] = Seg_Buf[2] = 10;
			Seg_Buf[3] = Config_T_Set[1] / 10 % 10;
			Seg_Buf[4] = Config_T_Set[1] % 10;
			Seg_Buf[5] = 13;
			Seg_Buf[6] = Config_T_Set[0] / 10 % 10;
			Seg_Buf[7] = Config_T_Set[0] % 10;
			Seg_Point[5] = 0;
			if (Config_T_Index)
			{
				Seg_Buf[3] = Seg_Flash_Flag ? Config_T_Set[1] / 10 % 10 : 10;
				Seg_Buf[4] = Seg_Flash_Flag ? Config_T_Set[1] % 10 : 10;
			}
			else
			{
				Seg_Buf[6] = Seg_Flash_Flag ? Config_T_Set[0] / 10 % 10 : 10;
				Seg_Buf[7] = Seg_Flash_Flag ? Config_T_Set[0] % 10 : 10;
			}
			break;
		}
		
		case 2:  // 精度
		{
			Seg_Buf[0] = 14;
			for (i = 1; i <= 5; i ++ )
			{
				Seg_Buf[i] = 10;
			}
			Seg_Buf[6] = 13;
			Seg_Buf[7] = T_Accuracy;
			break;
		}
	}
}

void Led_Proc() 
{
	int MIN = (int)Config_T_Set[0] * 100, MAX = (int)Config_T_Set[1] * 100;
	if (Led_Slow_Down) return;
	Led_Slow_Down = 1;
	
	if (MAX < MIN) 
	{
		Led_Disp(4, 1);
		Led_Disp(0, 0);
		Led_Disp(1, 0);
		Led_Disp(2, 0);
		return;
	} 
	else
	{
		Led_Disp(4, 0);
	}
	
	MIN = (int)Config_T[0] * 100;
	MAX = (int)Config_T[1] * 100;
	
	if (MAX < int_t)
	{
		Led_Disp(0, 1);
	}
	else
	{
		Led_Disp(0, 0);
	}
	
	if (int_t >= MIN && int_t <= MAX)
	{
		Led_Disp(1, 1);
	}
	else
	{
		Led_Disp(1, 0);
	}
	
	if (MIN > int_t)
	{
		Led_Disp(2, 1);
	}
	else
	{
		Led_Disp(2, 0);
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
	
	if ( ++ Timer_1ms == 500)
	{
		Timer_1ms = 0;
		Seg_Flash_Flag ^= 1;
	}
	
	if (Key_Flag)
	{
		if ( ++ Key_Timer >= 600)
		{
			Key_Timer = 600;
		}
	}
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


void main()
{
	float_t = read_t();
	Delay750ms();
	
	Sys_Init();
	Timer0_Init();
	while (1)
	{
		float_t = read_t();
		int_t = float_t * 100;
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}