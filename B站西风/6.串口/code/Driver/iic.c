/*	#   I2C代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/
#include <IIC.H>
#define DELAY_TIME	5
#define scl P20
#define sda P21

//
static void I2C_Delay(unsigned char n)
{
    do
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();		
    }
    while(n--);      	
}

//
void I2CStart()
{
    sda = 1;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 0;
	I2C_Delay(DELAY_TIME);
    scl = 0;    
}

//
void I2CStop()
{
    sda = 0;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 1;
	I2C_Delay(DELAY_TIME);
}

//
void I2CSendByte(unsigned char byt)
{
    unsigned char i;
	
    for(i=0; i<8; i++){
        scl = 0;
		I2C_Delay(DELAY_TIME);
        if(byt & 0x80){
            sda = 1;
        }
        else{
            sda = 0;
        }
		I2C_Delay(DELAY_TIME);
        scl = 1;
        byt <<= 1;
		I2C_Delay(DELAY_TIME);
    }
	
    scl = 0;  
}

//
unsigned char I2CReceiveByte()
{
	unsigned char da;
	unsigned char i;
	for(i=0;i<8;i++){   
		scl = 1;
		I2C_Delay(DELAY_TIME);
		da <<= 1;
		if(sda) 
			da |= 0x01;
		scl = 0;
		I2C_Delay(DELAY_TIME);
	}
	return da;    
}

//
unsigned char I2CWaitAck()
{
	unsigned char ackbit;
	
    scl = 1;
	I2C_Delay(DELAY_TIME);
    ackbit = sda; 
    scl = 0;
	I2C_Delay(DELAY_TIME);
	
	return ackbit;
}

//
void I2CSendAck(unsigned char ackbit)
{
    scl = 0;
    sda = ackbit; 
	I2C_Delay(DELAY_TIME);
    scl = 1;
	I2C_Delay(DELAY_TIME);
    scl = 0; 
	sda = 1;
	I2C_Delay(DELAY_TIME);
}

// 向AT24C02写
void AT24C02_Write_Byte(unsigned char* write_dat, unsigned char addr, num)
{
	// 选择芯片，确定写模式
	do {
		I2CStart();
		I2CSendByte(0xA0);
	} while (I2CWaitAck());
	
	// 写入地址帧
	I2CSendByte(addr);
	I2CWaitAck();
	
	while (num -- )
	{
		// 将数组的数据写入芯片
		I2CSendByte(*write_dat ++ );
		I2CWaitAck();
	}
	
	I2CStop();
}

// 从AT24C02读
void AT24C02_Read_Byte(unsigned char* read_dat, unsigned char addr, num)
{

	// 选择芯片，确定写模式
	do {
		I2CStart();
		I2CSendByte(0xA0);
	} while (I2CWaitAck());
	
	// 写入地址帧
	I2CSendByte(addr);
	I2CWaitAck();
	
	// 选择芯片，确定读模式
	do {
		I2CStart();
		I2CSendByte(0xA1);
	} while (I2CWaitAck());
	
	while (num -- )
	{
		// 将读取的数据写入数组
		*read_dat ++  = I2CReceiveByte();
		// 根据剩余字节数发送应答信号
		if (num)
			I2CSendAck(0);
		else
			I2CSendAck(1);
	}
	
	I2CStop();
}

// AD
unsigned char PCF8591_ADC() //因为PCF8591是8位AD转换，所以返回值是char型即可
{
	unsigned char dat;
	do {
		I2CStart();
		I2CSendByte(0x90);  // 发送寻址+写控制字节
	} while (I2CWaitAck());
	I2CSendByte(0x03);  // 控制字：00000011B，选择模拟信号输入通道3，禁止模拟输出，禁止通道号自增
	I2CWaitAck();
	do {
		I2CStart();
		I2CSendByte(0x91);  // 发送地址+读控制
	} while (I2CWaitAck());
	dat = I2CReceiveByte();  // 读取ADC结果
	I2CSendAck(1);
	I2CStop();
	return dat;
}

// DA
void PCF8591_DAC(unsigned char dat) // dat为待转换数据
{
	do {
		I2CStart();
		I2CSendByte(0x90); // 发送寻址+写命令帧
	} while (I2CWaitAck());
	I2CSendByte(0x40);  // 发送控制命令字，模拟输出允许
	I2CWaitAck();
	I2CSendByte(dat);  // 发送待转换数据
	I2CWaitAck();
	I2CStop();
}

