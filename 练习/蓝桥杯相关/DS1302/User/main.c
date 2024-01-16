#include "main.h"

unsigned char Key_Slow_Down, Seg_Slow_Down, Led_Slow_Down;
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;
unsigned char Seg_Buf[8];
unsigned char Seg_Point[8];

unsigned char Seg_Disp_Mode;  // 0-时间和星期 2-年月日
unsigned char Date[] = {0x24, 0x01, 0x16, 0x20, 0x00, 0x00, 0x02};  // 年 月 日 时 分 秒 星期
unsigned char Date_Write_Addr[] = {0x8C, 0x88, 0x86, 0x84, 0x82, 0x80, 0x8A};  // 年 月 日 时 分 秒 星期 -> 写地址
unsigned char Date_Read_Addr[] = {0x8D, 0x89, 0x87, 0x85, 0x83, 0x81, 0x8B};  // 年 月 日 时 分 秒 星期 -> 读地址


void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  											// 实时读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  	// 读取下降沿
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  	// 读取上升沿
	Key_Old = Key_Val;  												// 辅助变量
	
	if (Key_Old == 4)
	{
		Seg_Disp_Mode = 1;
	}
	else
	{
		Seg_Disp_Mode = 0;
	}
}

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
	
	for (i = 0; i < 7; i ++ )
	{
		Date[i] = Read_Ds1302_Byte(Date_Read_Addr[i]);
	}
	
	switch (Seg_Disp_Mode)
	{
		case 0:  // 显示时间和星期
		{
			Seg_Buf[0] = Date[6];
			
			Seg_Buf[2] = Date[3] >> 4;
			Seg_Buf[3] = Date[3] & 0x0F;
			
			Seg_Buf[4] = Date[4] >> 4;
			Seg_Buf[5] = Date[4] & 0x0F;
			
			Seg_Buf[6] = Date[5] >> 4;
			Seg_Buf[7] = Date[5] & 0x0F;
			break;
		}
		case 1:  // 显示年月日
		{
			Seg_Buf[0] = 2;
			Seg_Buf[1] = 0;
			Seg_Buf[2] = Date[0] >> 4;
			Seg_Buf[3] = Date[0] & 0x0F;
			Seg_Buf[4] = Date[1] >> 4;
			Seg_Buf[5] = Date[1] & 0x0F;
			Seg_Buf[6] = Date[2] >> 4;
			Seg_Buf[7] = Date[2] & 0x0F;
			break; 
		}
	}
}

void Led_Proc() 
{
	unsigned char i;
	if (Led_Slow_Down) return;
	Led_Slow_Down = 1;
	
	for (i = 0; i < 8; i ++ )
	{
		Led_Disp(i, 0);
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
	
	
}

void main()
{
	unsigned char i;
	Sys_Init();
	Timer0_Init();
	
	// Off DS1302 write protected
	Write_Ds1302_Byte(0x8E, 0x00);
	for (i = 0; i < 7; i ++ )
	{
		Write_Ds1302_Byte(Date_Write_Addr[i], Date[i]);
	}
	// On DS1302 write protected
	Write_Ds1302_Byte(0x8E, 0x80);
	
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}