#ifndef _LCDDISPLAY_H
#define _LCDDISPLAY_H
#include "lcd.h"
#include "chinese.h"
#include "string.h"
#include "stdio.h"

#define LENGTHOF(A) sizeof(A)/(sizeof(u8)*32)
#define POINTX_START 84
#define POINTY_START 92
typedef struct lcd_point{
    u16 x;
    u16 y;
		u16 prex;
		u16 prey;
}LCD_POINT;

LCD_POINT LCD_show(u8 tfont[][32],LCD_POINT *p,u8 length);
LCD_POINT LCD_show_nextLine(u8 tfont[][32],LCD_POINT *p,u8 length,u8 isAlign);
LCD_POINT LCD_show_center(u8 tfont[][32],LCD_POINT *p,u8 length);
LCD_POINT LCD_show_string(u8 *s,LCD_POINT *p);
LCD_POINT LCD_show_int(u32 integer,u8 *units,LCD_POINT *p);
void LCD_Show_Init(void);

#endif
