#ifndef __IIC_H__
#define __IIC_H__

static void I2C_Delay(unsigned char n);
void I2CStart();
void I2CStop();
void I2CSendByte(unsigned char byt);
unsigned char I2CReceiveByte();
unsigned char I2CWaitAck();
void I2CSendAck(unsigned char ackbit);

#endif