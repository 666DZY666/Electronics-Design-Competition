#include "pe43503.h"
void PE43503_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA端口时钟
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	PE43503_LE=0;
	PE43503_CLK=0;
	PE43503_DATA=0;
}
/*
	输入分频位数1-31
	data：十进制数 1-31
*/
u8 PE43503_Send(unsigned char data){
	unsigned char tempt = 0x80; //1000 0000  先传高位
	data = data>31?31:data;
	data = data<1?1:data;
	PE43503_LE = 0;
	PE43503_CLK = 0;
	PE43503_DATA = 0;
	data <<= 2; //处理
	data = data & 0x7c; //0x7c = 0111 1100 把其他三位过滤为0
	delay_us(5);
	for(int i = 0 ;i < 8 ; i++){
		if(tempt & data){
			PE43503_DATA = 1;
		}else{
			PE43503_DATA = 0;
		}
		tempt >>= 1;
		PE43503_CLK = 1;
		delay_us(3);
		PE43503_CLK = 0;
		delay_us(3);
	}
	delay_us(4);
	PE43503_LE=1;
	return 0;
}
