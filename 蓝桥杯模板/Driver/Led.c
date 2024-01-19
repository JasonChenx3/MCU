#include <LED.H>

void Led_Disp(unsigned char pos, enable) 
{
	static unsigned char tmp = 0x00;
	static unsigned char tmp_old = 0xFF;
	
	if (enable) tmp |= 0x01 << pos;
	else tmp &= ~(0x01 << pos);
	
	if (tmp != tmp_old) 
	{
		P0 = ~tmp;
		P2 = P2 & 0x1F | 0x80;
		P2 &= 0x1F;
		tmp_old = tmp;
	}
}

//void Beep(unsigned char flag)
//{
//	static unsigned char temp = 0x00;
//	static unsigned char temp_old = 0xFF;
//	if(flag)
//		temp |= 0x40;
//	else
//		temp &= ~0x40;
//	if(temp != temp_old)
//	{
//		P0 = temp;
//		P2 = P2 & 0x1f | 0xA0;
//		P2 &= 0x1F;
//		temp_old = temp;		
//	}
//}

//void Relay(unsigned char flag)
//{
//	static unsigned char temp = 0x00;
//	static unsigned char temp_old = 0xFF;
//	if(flag)
//		temp |= 0x10;
//	else
//		temp &= ~0x10;
//	if(temp != temp_old)
//	{
//		P0 = temp;
//		P2 = P2 & 0x1f | 0xA0;
//		P2 &= 0x1F;
//		temp_old = temp;		
//	}	
//}