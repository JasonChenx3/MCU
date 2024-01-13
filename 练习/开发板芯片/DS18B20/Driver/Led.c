#include <LED.H>

void Led_Disp(unsigned char pos, enable) 
{
	static unsigned char tmp = 0x00;
	static unsigned char tmp_old = 0xFF;
	
	if (enable) tmp |= 0x01 << pos;
	else tmp &= ~(0x01 << pos);
	
	// 产生数据刷新
	if (tmp != tmp_old) 
	{
		P0 = ~tmp;  // 点亮对应的LED灯
		P2 = P2 & 0x1F | 0x80;
		P2 &= 0x1F;
		tmp_old = tmp;  // 避免出现电流声
	}
}