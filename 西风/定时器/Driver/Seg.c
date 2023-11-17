#include <SEG.H>
#include <INIT.H>

code unsigned char Seg_Com[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};  // 位码
code unsigned char Seg_Num[12] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0xFF, 0xBF};  // 段码

void Seg_Disp(unsigned char com, num, point)
{
	// 消影
	P0 = 0x00;
	HC138(7);
	HC138(0);
	
	// 位码
	P0 = Seg_Com[com];
	HC138(6);
	HC138(0);
	
	// 段码
	P0 = point ? Seg_Num[num] & 0x7F : Seg_Num[num];
	HC138(7);
	HC138(0);
}
