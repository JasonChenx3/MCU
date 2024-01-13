#include <SEG.H>

unsigned char Seg_com[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
unsigned char Seg_num[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0xFF, 0xBF, 0xC6};

void Seg_Disp(unsigned char com, num, point) 
{
	// 消影
	P0 = 0x00;
	P2 = P2 & 0x1F | 0xE0;
	P2 &= 0x1F;
	
	// 段选
	P0 = Seg_com[com];
	P2 = P2 & 0x1F | 0xC0;
	P2 &= 0x1F;
	
	// 位选
	P0 = point ? Seg_num[num] & 0x7F : Seg_num[num];
	P2 = P2 & 0x1F | 0xE0;
	P2 &= 0x1F;
}