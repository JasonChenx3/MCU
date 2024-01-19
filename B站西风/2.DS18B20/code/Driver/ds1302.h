#ifndef __DS1302_H__
#define __DS1302_H__

#include <STC15F2K60S2.H>
#include <INTRINS.H>

#define SCK P17
#define SDA P23
#define RST P13

void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte ( unsigned char address );

#endif