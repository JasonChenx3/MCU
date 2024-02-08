#include <STC15F2K60S2.H>
#include <stdio.h>
#include <string.h>
#include "SYS.H"
#include "KEY.H"
#include "LED.H"
#include "SEG.H"
//#include "DS1302.H"
//#include "IIC.H"
#include "ONEWIRE.H"
//#include "ULTRASOUND.H"
#include "UART.H"

unsigned char Key_Timer, Seg_Timer;
bit Key_Valid, Seg_Valid;
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char ucLed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char Uart_Receive[10];
unsigned char Uart_Receive_Index;
unsigned char Uart_Send[10];
float t;
unsigned char cnt;

void Key_Proc()
{
	if (Key_Timer) { return; }
	Key_Timer = 1;
	
	Key_Val = Key_Read();
	Key_Down = Key_Val & (Key_Val ^ Key_Old);
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
	Key_Old = Key_Val;
	
	switch (Key_Down)
	{
		case 4:
		{
			sprintf(Uart_Send, "T = %.2f\r\n", t);
			Uart_Send_String(Uart_Send);
			cnt ++ ;
			break;
		}
	}
}

void Seg_Proc() 
{
	if (!Seg_Valid) { return; }
	Seg_Valid = 0;
	
	t = read_t();
	Seg_Buf[7] = cnt % 10;
}

void Led_Proc() 
{
	
}

void Timer0_Isr() interrupt 1 
{
	static unsigned int Pos = 0;
	
	if ( ++ Key_Timer == 10)
	{
		Key_Timer = 0;
	}
		
	if ( ++ Seg_Timer == 50)
	{
		Seg_Valid = 1;
		Seg_Timer = 0;
	}
		
	
	Pos = (Pos + 1) % 8;
	Seg_Disp(Pos, Seg_Buf[Pos], Seg_Point[Pos]);
	Led_Disp(Pos, ucLed[Pos]);
	
	
}

void Uart1_Isr() interrupt 4 
{
	if (RI == 1)
	{
		Uart_Receive[Uart_Receive_Index ++ ] = SBUF;
		RI = 0;
	}
}

void main()
{
	Sys_Init();
	Timer0_Init();
	UartInit();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}