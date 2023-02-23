#include <REGX52.H>
#include <absacc.h>
#include "onewire.h"
unsigned int temp=0x0000;//温度变量
/*========数码管段码定义========*/
unsigned char code SMG_Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char code SMG_Dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
/*========数码管控制函数========*/
void Display_SMG_byte(unsigned char position,unsigned char dat)
{
	XBYTE[0xc000]=0x00;
	XBYTE[0xc000]=0x01<<position;
	XBYTE[0xe000]=dat;
}
void Display_SMG_ALL(unsigned char dat)
{
	XBYTE[0xc000]=0xff;
	XBYTE[0xe000]=dat;
}
void SMG_Delay(unsigned int i)
{
	while(i--);
}
void Display_temp()
{
	Display_SMG_byte(5,SMG_Nodot_duanma[(temp/100)%10]);
	SMG_Delay(100);
	Display_SMG_byte(6,SMG_Dot_duanma[(temp/10)%10]);
	SMG_Delay(100);
	Display_SMG_byte(7,SMG_Nodot_duanma[temp%10]);
	SMG_Delay(100);
	Display_SMG_ALL(0xff);
}
/*========温度获取========*/
void Delay_ds18b02(unsigned int i)
{
	while(i--)
	{
		Display_temp();
	}
}
void Read_ds18b02_temp()
{
/*	unsigned int LSB,MSB;
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	
	Delay_ds18b02(1000);
	
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	LSB=Read_DS18B20();
	MSB=Read_DS18B20();
	Init_DS18B20();
	
	temp=MSB;
	temp<<=8;
	temp=temp|LSB;
	
	if((temp&0xf800)==0x0000)
	{
		temp>>=4;
		temp=temp*10;
		temp=temp+(LSB&0x0f)*0.625;
	}
	*/
		unsigned char LSB,MSB ;
	
	Init_DS18B20();			
	Write_DS18B20(0xCC);			
	Write_DS18B20(0x44);
	Delay_ds18b02(1000);
	Init_DS18B20();		
	Write_DS18B20(0xCC);		
	Write_DS18B20(0xBE);			
	LSB = Read_DS18B20();		
	MSB = Read_DS18B20();			
	Init_DS18B20();		

	temp = 0x0000;
	temp = MSB;
	temp <<= 8;
	temp = temp | LSB;
	
	if((temp & 0xf800) == 0x0000)
	{
		temp >>= 4;
		temp = temp*10;
		temp = temp + (LSB&0x0f)*0.625;
	}
	
}
/*========主函数========*/
void main()
{
	XBYTE[0x8000]=0xff;
	while(1)
	{
		Read_ds18b02_temp();
		Display_temp();
	}
}