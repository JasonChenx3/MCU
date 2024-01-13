#include <STC15F2K60S2.H>
#include "MYDF.H"

void LED(u8 x, u8 y)
{
	P2 = (P2 & 0x1f) | 0xC0;
	x = 0x01;
	y = 0x01;
}