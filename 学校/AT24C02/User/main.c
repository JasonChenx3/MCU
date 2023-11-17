/* 头文件区 */
#include <STC15F2K60S2.H>
#include <INIT.H>
#include <SEG.H>
#include <AT24C02.H>

/* 变量区 */
unsigned char Seg_Pos;
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
unsigned char addr, dat;

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
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], 0);
	
	
}

/* Main */
void main() 
{
	Sys_Init();
	Timer0_Init();
	addr = 0x00;
	dat = At24c02_Byte_Read(addr);
	At24c02_Byte_Write(addr, dat + 1);
	Seg_Buf[5] = (dat / 100) % 10;
	Seg_Buf[6] = (dat / 10) % 10;
	Seg_Buf[7] = dat % 10;
	while (1) 
	{
		;
	}
}