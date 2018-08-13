#include "ad9854.h"

SPI_InitTypeDef  SPI_InitStructure;
/*
	AD9854驱动
	Update_clock---PB5
	~CS---PIN22---PB6
	RESET---PIN17---PB7
	MOSI: PIN19---PA7
	MISO: PIN18---PA6
	SCK: 	PIN21---PA5
	SPI2:
	PB 13 	14 	 15
	   SCK MISO MOSI
*/
void AD9854_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PB5:UPDATE LOCK PB6:CS PB7:RESET
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //PA5:SCK PA6:MISO PA7:MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	//master_reset();
	SPI2_ReadWriteByte(0xff);//启动传输
	SPI2_SetSpeed(SPI_BaudRatePrescaler_16);//4.5M传输速度
	AD9864_Reset();
	//master_reset();
	AD9854_Ctr(0x104a0021);//控制指令
	//一定要写在控制指令后面
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PB5:UPDATE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//AD9854_Ctr(0x10040020);//0x104a0040 0x1040031
	
}
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)

void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet;
  SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}

//SPIx 读写一个字节
	//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
	u8 retry=0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据
}
/*
		FTW = (Desired Output Frequency × 2^N)/SYSCLK N=48
		入口参数：要求的频率hz
		写入控制字
*/
void AD9854_Write_FTW(unsigned long int frequency){
	u8 fwt[6]={0}; //fwt的缓存，一共有48bit，6位保存，1位保存8bit
  u32 freq=0; //计算乘积
	u8 freq_buff[4]={0};
	for(int i=0;i<4;i++){ //将frequency拆分进freq里
		freq_buff[i]=frequency>>(8*i);
	}
	for(int i=0;i<6;i++){ //计算的到频率控制字并拆分到freq中
		if(i<4){
			freq += AD9854_FWT_TEMP * freq_buff[i];
			fwt[i] = freq;
			freq >>= 8;
		}else{
			fwt[i] = freq;
			freq>>=8;
		}
	}
	AD9854_CS = 0;
	SPI2_ReadWriteByte(AD9854_FRE1); //频率1的控制字地址
	for(int i=5;i>=0;i--){ //发送数据，先发高位
		SPI2_ReadWriteByte(fwt[i]);
	}
	AD9854_CS=1;
	AD9854_Update();//更新
}
/*
	设置正弦波
	frequency:频率
	shap:幅度 0-4096
*/
void AD9854_SetSine(u8 f,u16 shape){
	unsigned long int frequency=f*1000000;
	//shape=shape*AM;
	AD9854_Write_FTW(frequency); //设置频率控制字
	AD9854_CS=0;
	SPI2_ReadWriteByte(AD9854_I_MUL); //如果最高位是1则读，最高位是0则写 默认是0
	SPI2_ReadWriteByte(shape>>8); //先传高位
	SPI2_ReadWriteByte(shape&0xff);
	AD9854_CS=1;
	AD9854_Update();//更新
	
	AD9854_CS=0;
	SPI2_ReadWriteByte(AD9854_Q_MUL);
	SPI2_ReadWriteByte(shape>>8);
	SPI2_ReadWriteByte(shape&0xff);
	AD9854_CS=1;
	AD9854_Update();//更新
	
	//发送阶梯电压
	JTB2_Control(f*80);//控制DAC
}
/*
	复位
*/
void AD9864_Reset(void){
	AD9854_RESET=1;
	delay_ms(1);
	AD9854_RESET=0;
	delay_ms(1);
}
/*
	控制指令
	control 16位
*/
void AD9854_Ctr(unsigned long int control){
	AD9854_CS=0;
	SPI2_ReadWriteByte(AD9854_CTR_REG); //传地址
	for(int i=3;i>=0;i--){
		SPI2_ReadWriteByte(control>>(i*8)); //先传高位
	}
	AD9854_CS=1;
	AD9854_Update();//更新
	delay_ms(1);
}
/*
  更新
*/
void AD9854_Update(){
	AD9854_UPDATE_LOCK =1;
	delay_ms(1);
	AD9854_UPDATE_LOCK = 0;
}
//void master_reset(){
//	AD9854_MASTER_RESET=1;
//	delay_ms(1);
//	AD9854_MASTER_RESET=0;
//	delay_ms(1);
//}
