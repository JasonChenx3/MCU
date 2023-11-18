#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

#include <STC15F2K60S2.H>
#include <INTRINS.H>

void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20();
bit init_ds18b20();
float read_t();
void Delay750ms();

#endif