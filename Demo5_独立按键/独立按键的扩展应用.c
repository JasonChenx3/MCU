#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int

// 按键
sbit S7 = P3 ^ 0;
sbit S6 = P3 ^ 1;
sbit S5 = P3 ^ 2;
sbit S4 = P3 ^ 3;

// LED灯
sbit L1 = P0 ^ 0;
sbit L2 = P0 ^ 1;
sbit L3 = P0 ^ 2;
sbit L4 = P0 ^ 3;
sbit L5 = P0 ^ 4;
sbit L6 = P0 ^ 5;

// HC138输出Y_channel
void HC138(uchar channel)
{
	switch (channel)
	{
		case 4:
			P2 = (P2 & 0x1f) | 0x80;
		break;
		case 5:
			P2 = (P2 & 0x1f) | 0xa0;
		break;
		case 6:
			P2 = (P2 & 0x1f) | 0xc0;
		break;
		case 7:
			P2 = (P2 & 0x1f) | 0xe0;
		break;
	}
}

// 独立按键的延时
// 因为延时很短，所以用uchar就行
void DelayK(uchar t)
{
	while(t--);
}

// 控制S7和S6的信号量
uchar stat_k = 0;
// 扫描独立按键
void ScanKeys_Alone()
{
	if (S7 == 0)
	{
		// 这里延时的作用：
		// 消除误差，因为S7 ==0可能是波动造成的
		// 所以不需要很长的延时
		DelayK(100);
		
		// 延时之后再进行判断一次
		// 如果S7 == 0，说明不是误差
		if (S7 == 0) 
		{
			// 信号量是0，表示S7和S6均没有按下
			if (stat_k == 0)
			{
				L1 = 0;
				stat_k = 1;
			}
			// 信号量是1，表示S7按下
			else if (stat_k == 1)
			{
				L1 = 1;
				stat_k = 0;
			}
			// 一直到这次按键动作执行完成之后
			// 再进行别的操作
			// 不然会出现闪烁的情况
			while (S7 == 0);
		}
	}
	
	if (S6 == 0)
	{
		DelayK(100);
		if (S6 == 0)
		{
			if (stat_k == 0)
			{
				L2 = 0;
				stat_k = 2;
			}
			else if (stat_k == 2)
			{
				L2 = 1;
				stat_k = 0;
			}
			while (S6 == 0);
		}
	}
	
	if (S5 == 0)
	{
		DelayK(100);
		if (S5 == 0)
		{
			if (stat_k == 1)
			{
				L3 = 0;
				while (S5 == 0);
				L3 = 1;
			} else if (stat_k == 2)
			{
				L5 = 0;
				while (S5 == 0);
				L5 = 1;
			}
		}
	}
	if (S4 == 0)
	{
		DelayK(100);
		if (S4 == 0)
		{
			if (stat_k == 1)
			{
				L4 = 0;
				while (S4 == 0);
				L4 = 1;
			} else if (stat_k == 2)
			{
				L6 = 0;
				while (S4 == 0);
				L6 = 1;
			}
		}
	}
}

void main()
{
	// 选通Y4
	HC138(4);
	while(1)
	{
		// 一直扫描独立键盘的状态
		ScanKeys_Alone();  
	}
}