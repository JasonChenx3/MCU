#include "ULTRASOUND.H"

sbit Tx = P1 ^ 0;
sbit Rx = P1 ^ 1;

void Delay12us()		//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void Wave_Init() 
{
	unsigned char i;
	for (i = 0; i < 5; i ++ )
	{
		Tx = 1;
		Delay12us();
		Tx = 0;
		Delay12us();
	}
}

unsigned int Wave_Read()
{
	unsigned int time;
	TH1 = TL1 = 0;
	TF1 = 0;
	Wave_Init();
	TR1 = 1;
	while (Rx == 1 && TF1 == 0) ;
	TR1 = 0;
	if (TF1 == 0)
	{
		time = (TH1 << 8) | TL1;
		return time * 0.017;  // 返回值单位：cm
	}
	else
	{
		TF1 = 0;
		return 9999;
	}
}
