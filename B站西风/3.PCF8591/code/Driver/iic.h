#ifndef __IIC_H__
#define __IIC_H__

#include <STC15F2K60S2.H>
#include <INTRINS.H>

void AT24C02_Write_Byte(unsigned char addr, dat);
unsigned char AT24C02_Read_Byte(unsigned char addr);
unsigned char PCF8591_ADC();
void PCF8591_DAC(unsigned char dat);

#endif