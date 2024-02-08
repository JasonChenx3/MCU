#include "UART.H"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xC7;		//设置定时初始值
	T2H = 0xFE;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	ES = 1;
	EA = 1;
}

/*字节发送*/
void SendByte(unsigned char dat)
{
	SBUF = dat;
	while (TI == 0) ;
	TI = 0;
}

/*字符串发送*/
void Uart_Send_String(unsigned char* str)
{
	while (*str != '\0')
	{
		SendByte(*str ++ );
	}
}