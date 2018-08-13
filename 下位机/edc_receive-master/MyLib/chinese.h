#ifndef __CHINESE_H
#define __CHINESE_H	 
#include "sys.h"   


//void TEST_FONT(void);
void LCD_ShowOne(u16 x,u16 y,u8* tfont,u16 pcolor,u16 bcolor);
//void Test_Show_CH_Font24(u16 x,u16 y,u8 index,u16 color);		  
void LCD_ShowFont(u16 x,u16 y,u8 tfont[][32],u8 len,u16 pcolor,u16 bcolor);
void LCD_ShowBmp(u16 x,u16 y,u16 bmp_length,u16 bmp_height,u16* buf);

#endif
