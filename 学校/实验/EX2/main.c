#include "reg52.h"
#include "absacc.h"
#include "intrins.h"
void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 4;
	j = 129;
	k = 119;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
	unsigned char tem;
	while(1)
	{
		
		tem=XBYTE[0x8000]&0x03;
		switch(tem)
		{
			case 0x00:XBYTE[0x9000]=0xff;break;
			case 0x01:XBYTE[0x9000]=0xfc;break;
			case 0x02:XBYTE[0x9000]=0x3f;break;
			case 0x03:
				XBYTE[0x9000]=0x3c|0x;
				break;
		}
		Delay500ms();
		XBYTE[0x9000]=0xff;
		Delay500ms();
	}
}
