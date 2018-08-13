#include "lcddisplay.h"
#include "chfont.h"
#include "delay.h"
LCD_POINT LCD_Point;
LCD_POINT LCD_POINT_DB;
LCD_POINT LCD_POINT_HZ;
LCD_POINT LCD_POINT_AV;
LCD_POINT LCD_POINT_XX;
/*
显示汉字
tfont:要显示汉字的数组
p:显示汉字的初始位置
return：显示汉字的结束位置
*/
LCD_POINT LCD_show(u8 tfont[][32],LCD_POINT *p,u8 length){
		p->prex = p->x;
		p->prey = p->y;
   // LCD_POINT end;
		//printf("length:%d\r\n",sizeof(tfont));
    LCD_ShowFont(p->x,p->y,tfont,length,POINT_COLOR,BACK_COLOR);
    p->x = p->x+length*16;
    p->y = p->y;
    return *p;
}
/*
    在下一行显示汉字
*/
LCD_POINT LCD_show_nextLine(u8 tfont[][32],LCD_POINT *p,u8 length,u8 isAlign){
		p->prey = p->y;
	
		p->x = isAlign?p->prex:POINTX_START;
    p->y = p->y+16+8;
    return LCD_show(tfont,p,length);
}
/*
    在一行的中间显示汉字
*/
LCD_POINT LCD_show_center(u8 tfont[][32],LCD_POINT *p,u8 length){
		p->prex = p->x;
		p->prey = p->y;
    p->x = lcddev.width/2 - length/2*16;
    *p = LCD_show(tfont,p,length);
    p->y = p->y+16+8;
    p->prex=16;
    return *p;
}
/*
	显示字符串
*/
LCD_POINT LCD_show_string(u8 *s,LCD_POINT *p){
	p->prex=p->x;
	p->prey=p->y;
	u8 length = strlen((const char *)s)*8;
	LCD_ShowString(p->x,p->y,length,16,16,s);
	p->x = p->x+length;
	return *p;
}
/*
	显示数值加单位
*/
LCD_POINT LCD_show_int(u32 integer,u8 *units,LCD_POINT *p){
	u8 buff[20];
	sprintf(buff,"%d",integer);
	strcat((char *)buff,(const char*)units);
	LCD_show_string(buff,p);
	p->x=p->prex;
	p->y=p->prey;
	return *p;
}
/*
	 初始化
*/
void LCD_Show_Init(){
		LCD_Init();
//		LCD_ShowBmp(85,14,71,66,(u16*)zzu);
		LCD_Point.x=POINTX_START;
		LCD_Point.y=POINTY_START;
		LCD_Point.prex=POINTX_START;
		LCD_Point.prey=POINTY_START;
		LCD_show_center(ZZU,&LCD_Point,LENGTHOF(ZZU));
	
		LCD_show_nextLine(AV,&LCD_Point,LENGTHOF(AV),0);
		LCD_POINT_AV =  LCD_show_string(":",&LCD_Point);
		LCD_show_nextLine(PL,&LCD_Point,LENGTHOF(PL),0);
		LCD_POINT_HZ = LCD_show_string(":",&LCD_Point);
		LCD_show_nextLine(SHJ,&LCD_Point,LENGTHOF(SHJ),0);
		LCD_POINT_DB = LCD_show_string(":",&LCD_Point);
		LCD_show_nextLine(XX,&LCD_Point,LENGTHOF(XX),0);
		LCD_POINT_XX = LCD_show_string(":",&LCD_Point);
}
