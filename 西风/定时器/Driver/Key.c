#include <KEY.H>
#include <INIT.H>

unsigned char Key_Read(unsigned char flag)
{
	unsigned char Key_Now = 0;
	P44 = 0, P42 = 1, P35 = 1, P34 = 1;
	if (P33 == 0) Key_Now = 4;
	if (P32 == 0) Key_Now = 5;
	if (P31 == 0) Key_Now = 6;
	if (P30 == 0) Key_Now = 7;
	if (flag)
	{
		P44 = 1, P42 = 0, P35 = 1, P34 = 1;
		if (P33 == 0) Key_Now = 8;
		if (P32 == 0) Key_Now = 9;
		if (P31 == 0) Key_Now = 10;
		if (P30 == 0) Key_Now = 11;
		P44 = 1, P42 = 1, P35 = 0, P34 = 1;
		if (P33 == 0) Key_Now = 12;
		if (P32 == 0) Key_Now = 13;
		if (P31 == 0) Key_Now = 14;
		if (P30 == 0) Key_Now = 15;
		P44 = 1, P42 = 1, P35 = 1, P34 = 0;
		if (P33 == 0) Key_Now = 16;
		if (P32 == 0) Key_Now = 17;
		if (P31 == 0) Key_Now = 18;
		if (P30 == 0) Key_Now = 19;
	}
	return Key_Now;
}