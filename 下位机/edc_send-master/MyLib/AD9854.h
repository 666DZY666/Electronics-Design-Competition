#ifndef _AD9854_H
#define _AD9854_H
#include "sys.h"
#include "delay.h"
#include "libincludes.h"
#define AD9854_UPDATE_LOCK PCout(5)
#define AD9854_CS PCout(6)
#define AD9854_RESET PCout(7)

//#define AD9854_MASTER_RESET PAout(4)

#define SPI_MISO PAout(6)
#define SPI_MOSI PAout(7)
#define SPI_SCK  PAout(5)
 //serial address write
#define	  AD9854_PHA1       0x00       // 2 Bytes
#define	  AD9854_PHA2       0x01       // 2 Bytes
#define	  AD9854_FRE1       0x02       // 6 Bytes
#define	  AD9854_FRE2       0x03      // 6 Bytes
#define	  AD9854_DELTA      0x04        // 6 Bytes
#define	  AD9854_UPDATA     0x05        // 4 Bytes
#define	  AD9854_RAMP_CLK   0x06        // 3 Bytes
#define	  AD9854_CTR_REG    0x07       // 4 Bytes
#define	  AD9854_I_MUL      0x08        // 2 Bytes
#define	  AD9854_Q_MUL      0x09        // 2 Bytes
#define	  AD9854_SHAPED     0x0A       // 1 Bytes
#define	  AD9854_Q_DAC      0x0B        // 2 Bytes

#define AD9854_FWT_TEMP 1407375  // 2^48/20M 用来计算fwt 1407374.883 
#define AM 15
void AD9854_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void AD9854_Update(void);
void AD9854_Ctr(unsigned long int control);
void AD9864_Reset(void);
void AD9854_SetSine(u8 f,u16 shape); //设置正弦波幅度和频率
//void master_reset(void);
#endif
