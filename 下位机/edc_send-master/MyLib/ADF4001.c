#include "ADF4001.h"
#include "delay.h"


void delay(unsigned int t)
{

}
void Send_Data(unsigned long int datanumber)   // x为后8位，y为前八位
{
	unsigned long int temp;
    unsigned int i;
  	temp=0x800000;
	A4001_LE=0;
	A4001_CLK = 0;
	for(i = 0; i < 24; i++)
	{
		if(datanumber & temp)
		{
			A4001_DATA = 1;
		}
		else
		{
			A4001_DATA = 0;
		}
		temp >>= 1;
		A4001_CLK = 1;
		delay_us(20);
		A4001_CLK = 0;
		delay_us(20);
	}
	delay(20);
	A4001_LE=1;
}
void ADF4001_Control(unsigned long int REF_divide,
					 unsigned long int N_divide)
{
	unsigned long int m,n;
	Send_Data(0x1ff843);
	delay(200);
	m=REF_divide<<2;
	Send_Data(0x000000|m);
	delay(200);
	n=N_divide<<8;
	Send_Data(0x000001|n);
	delay(200);
	Send_Data(0x1ff842);	
}
void ADF4001_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	A4001_LE=0;
  	A4001_CLK=0;
  	A4001_DATA=0;
}
