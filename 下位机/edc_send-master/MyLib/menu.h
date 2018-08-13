#ifndef _MENU_H
#define _MENU_H
#include "lcddisplay.h"
#include "string.h"
#include "stdlib.h"
#include "key.h"
#include "libincludes.h"

typedef struct _menu{
    LCD_POINT coord;//坐标
    u8  (*name)[32];//名字
    int value;//值
    u8 *units;
    u8 length;
		u8 id;
    struct _menu *next;
		struct _menu *pre;
}MENU;
void menu_insert(LCD_POINT *coord,u8 name[][32] ,u32 value,u8 units[5],u8 length,u8 id);
void menu_init(void);
void menu_select(u8 key);
void menu_show(MENU *m);
#endif
