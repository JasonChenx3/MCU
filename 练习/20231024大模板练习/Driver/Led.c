#include <LED.H>

void Led_Disp(unsigned char addr, enable)
{
	static unsigned char temp = 0x00;
	static unsigned char temp_old = 0xFF;
	
	if (enable) temp |= 0x01 << addr;
	else temp &= ~(0x01 << addr);
	
	// 产生了数据刷新
	if (temp != temp_old)
	{
		P0 = ~temp;  // 点亮对应的LED
		P2 = P2 & 0x1F | 0x80;  // 选中LED通道
		P2 &= 0x1F;  // 关闭LED通道 
		temp_old = temp;  // 避免出现电流声
	}
}