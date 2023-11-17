#include <STC15F2K60S2.H>
#include <AT24C02.H>
#include <IIC.H>

void At24c02_Byte_Write(unsigned char addr, dat)
{
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	I2CSendByte(dat);
	I2CWaitAck();
	I2CStop();
}

unsigned char At24c02_Byte_Read(unsigned char addr)
{
	unsigned char dat;
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	I2CStart();
	I2CSendByte(0xA1);
	I2CWaitAck();
	dat = I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	return dat;
}

/*
R/W'
1¶Á
0Ð´
*/