#include <LED.H>
#include <INIT.H>

void Led_Disp(unsigned char pos, enable)
{
	static unsigned char Led_Now = 0x00;
	static unsigned char Led_Old = 0xFF;
	
	if (enable) Led_Now |= (0x01 << pos);
	else Led_Now &= ~(0x01 << pos);
	
	if (Led_Now != Led_Old)
	{
		P0 = Led_Now;
		HC138(4);
		HC138(0);
		Led_Old = Led_Now;
	}
}