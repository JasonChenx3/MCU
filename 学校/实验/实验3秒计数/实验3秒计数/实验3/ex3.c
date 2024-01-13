#include "reg52.h"
#include "absacc.h"
unsigned char code duan_code_ca[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff}; //共阳数码管段码表，0~9及全熄，所用数码管虽为共阴，但反向后才接数码管段码引脚，故可用共阳段码
unsigned char disp_buf[6];
void disp();
void Timer0_Isr(void) interrupt 1
{
	TL0 = 0x30;				//设置定时初始值
	TH0 = 0xF8;				//设置定时初始值，此两行功能为重新加载定时常数
	disp();//调用一位数码管显示函数显示一位数码
}

void Timer0_Init(void)		//2毫秒@12.000MHz
{
	//AUXR &= 0x7F;			//定时器时钟12T模式  注意：实验箱所用单片机的定时器时钟只有12T模式，且没有辅助寄存器AURX，所以用STC的下载器产生定时器初始化函数要去掉本行
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式   注意：定时器工作模式1为16位方式，所用单片机机的定时器无16位自动重装载模式，故在定时器中断服务函数中需重装定时常数
	TL0 = 0x30;				//设置定时初始值
	TH0 = 0xF8;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA=1;       //总的中断允许
}

//
void disp_buf_change(unsigned char one,two,three,four,five,six)
{
	disp_buf[0]=duan_code_ca[one];
	disp_buf[1]=duan_code_ca[two];
	disp_buf[2]=duan_code_ca[three];
	disp_buf[3]=duan_code_ca[four];
	disp_buf[4]=duan_code_ca[five];
	disp_buf[5]=duan_code_ca[six];
}

//
void disp()
{
	static unsigned char disp_com;
	P1=0xff;//消隐
	XBYTE[0x9000]=disp_buf[disp_com];
	P1=~(32>>disp_com);//因数码管为共阴，若不反向则会5只数码亮一只不亮，按位反向后输出就能保证只有指定位亮了。;
	disp_com=(disp_com+1)%6;
}

//
void main()
{
	unsigned long int dat;
	Timer0_Init();//初始化T0及中断
	dat=120345;
	if(dat>=1000000)
		dat=dat%1000000;
	disp_buf_change(dat>=100000?dat/100000:10,dat>=10000?dat%100000/10000:10,dat>=1000?dat%10000/1000:10,dat>=100?dat%1000/100:10,dat>=10?dat%100/10:10,dat%10);
	while(1);
	
}
	