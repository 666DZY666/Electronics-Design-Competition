#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
#include "delay.h"
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KEY4  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY5  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY6  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)
#define KEY7  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)

#define KEY1_PRES	1		 
#define KEY2_PRES	2		 
#define KEY3_PRES	3		 
#define KEY4_PRES	4		 
#define KEY5_PRES	5		 
#define KEY6_PRES	6		 
#define KEY7_PRES	7		 
void Key_Init(void);
u8 KEY_Scan(u8 mode);
extern void KEY_LCD(void);
extern u8  a;
extern u8  b;
extern int c;
extern u8  d;
#endif
