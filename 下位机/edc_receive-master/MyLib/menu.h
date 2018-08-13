#ifndef _MENU_H
#define _MENU_H
#include "lcddisplay.h"
#include "string.h"
#include "stdlib.h"
typedef struct _menu{
    LCD_POINT coord;//坐标
    u8  (*name)[32];//名字
    u32 value;//值
    u8 *units;
    u8 size;
    struct _menu *next;
}MENU;
void menu_insert(LCD_POINT coord,u8 name[][32] ,u32 value,u8 units[5]);
void menu_init(void);
#endif
