#include "menu.h"
#include "chfont.h"
extern LCD_POINT LCD_Point;
MENU *first=NULL;
MENU *last=NULL;
void menu_insert(LCD_POINT coord,u8 name[][32] ,u32 value,u8 units[5]){
    MENU *buff = last;
    last=(MENU*)malloc(sizeof(MENU));
	if(buff==NULL){
		last->coord.x=POINTX_START;
		last->coord.y=POINTY_START;
	}else{
		last->coord.y=buff->coord.y+24;
		last->coord.x=POINTX_START;
	}
	last->name=name;
	last->value=value;
	last->units=units;
	if(last->size==0){
		first=last;
	}else{
		buff->next=last;
	}
	last->next=first;
	first->size=first->size+1;
	printf("x:%d\ty:%d\tvalue%d\tunits:%s\tsize:%d\r\n", last->coord.x, last->coord.y,last->value,last->units,first->size);
}
void menu_init(){
	LCD_show_nextLine(PL,&LCD_Point,LENGTHOF(PL),0);
	LCD_show_string(":",&LCD_Point);
	menu_insert(LCD_Point,PL,122,(u8*)"HZ");

	LCD_show_nextLine(SHJ,&LCD_Point,LENGTHOF(SHJ),0);
	LCD_show_string(":",&LCD_Point);
	menu_insert(LCD_Point,SHJ,12,(u8*)"DB");
	//menu_insert(LCD_Point,PL,122,(u8*)"HZ");
}
void menu_delete(MENU *M){

}
