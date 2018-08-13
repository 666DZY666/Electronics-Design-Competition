#include "menu.h"
#include "chfont.h"
extern LCD_POINT LCD_Point;
#define PL_MAX 46
#define FD_MAX 3299
#define ZY_MAX 4095 

#define PL_MIN 1
#define FD_MIN 1
#define ZY_MIN 980
MENU *first=NULL;
MENU *last=NULL;
u16 amplitude=200;
void menu_insert(LCD_POINT *coord,u8 name[][32] ,u32 value,u8 units[5],u8 length,u8 menu_id){
    MENU *buff = last;
    last=(MENU*)malloc(sizeof(MENU));
	if(buff==NULL){
		last->coord.x=POINTX_START;
		last->coord.y=coord->y;
		
		last->coord.startx=POINTX_START;
		last->coord.starty=coord->y;
	}else{
		last->coord.y=buff->coord.y+24;
		last->coord.x=POINTX_START;
		
		last->coord.startx=POINTX_START;
		last->coord.starty=buff->coord.starty+24;
	}
	last->id = menu_id;
	last->name=name;
	last->value=value;
	last->units=units;
	last->length=length;
	if(first==NULL){
		first=last;
	}else{
		buff->next=last;
		last->pre=buff;
	}
	last->next=first;
	first->pre=last;
	//printf("x:%d\ty:%d\tvalue%d\tunits:%s\tid:%d\r\n", last->coord.x, last->coord.y,last->value,last->units,last->id);
}

void menu_show_item(MENU *last){
	last->coord.x=last->coord.startx;
	last->coord.y=last->coord.starty;
	LCD_show(last->name,&last->coord,last->length);
	//LCD_ShowFont(last->coord.x,last->coord.y,last->name,last->length,POINT_COLOR,BACK_COLOR);
	LCD_show_string(":",&last->coord);
	LCD_show_int(last->value,last->units,&last->coord);
}
void menu_show(MENU *m){
	MENU *buff=m;
	do{
		menu_show_item(m);
		m=m->next;
	}while(m!=buff);
}
void menu_init(void){

	menu_insert(&LCD_Point,PL,5,(u8*)"   ",LENGTHOF(PL),1);
	menu_insert(&LCD_Point,AV,amplitude,(u8*)"    ",LENGTHOF(AV),3);
	menu_insert(&LCD_Point,SHJ,ZY_MIN,(u8*)"    ",LENGTHOF(SHJ),2);
	menu_insert(&LCD_Point,SP,PL_MIN,(u8*)"     ",LENGTHOF(SP),4);

	menu_show(first);
	last=first;
	LCD_ShowString(last->coord.startx-24,last->coord.starty,16,16,16,"->");
	AD9854_SetSine(5,200);
	HSMP3816_Control(980);
	//menu_insert(LCD_Point,PL,122,(u8*)"HZ");
}
u8 sp_flag=0;
u8 wifi_buff[16];
u8 freq=5;
void menu_select(u8 key){


	//上下移动选择菜单和清屏
	if(key==KEY1_PRES){
		delay_ms(200);
		last=last->next;
		LCD_Fill(last->pre->coord.startx-24,last->pre->coord.starty,last->pre->coord.startx,last->pre->coord.starty+16,WHITE);
		LCD_ShowString(last->coord.startx-24,last->coord.starty,16,16,16,"->");
	}else if(key==KEY2_PRES){
		delay_ms(200);
		last=last->pre;
		LCD_Fill(last->next->coord.startx-24,last->next->coord.starty,last->next->coord.startx,last->next->coord.starty+16,WHITE);
		LCD_ShowString(last->coord.startx-24,last->coord.starty,16,16,16,"->");
	}
	switch(key){
		case KEY3_PRES: //按键3 +
			last->value++;
			if(last->id==1){ 
				delay_ms(50);
				last->value = last->value<PL_MAX?last->value:PL_MAX;
				freq=last->value;
				AD9854_SetSine(freq,amplitude);
				printf("A %d %d\r\n",freq,amplitude);
			}else if(last->id==2){
				delay_ms(50);
				last->value = last->value<ZY_MAX?last->value:ZY_MAX;
				HSMP3816_Control(last->value);
			}else if(last->id==3){
				last->value = last->value<FD_MAX?last->value:FD_MAX;
				amplitude=last->value;
				AD9854_SetSine(freq,amplitude);
				printf("A %d %d\r\n",freq,amplitude);
			}
			menu_show(first);
			break;
		case KEY4_PRES://按键4 -
			last->value--;
			if(last->id==1){
				delay_ms(50);
				last->value = last->value>PL_MIN?last->value:PL_MIN;
				freq=last->value;
				AD9854_SetSine(freq,amplitude);
				printf("A %d %d\r\n",freq,amplitude);
			}else if(last->id==2){
				delay_ms(50);
				last->value = last->value>ZY_MIN?last->value:ZY_MIN;
				HSMP3816_Control(last->value);
			}else if(last->id==3){
				delay_ms(50);
				last->value = last->value>FD_MIN?last->value:FD_MIN;
				amplitude=last->value;
				AD9854_SetSine(freq,amplitude);
				printf("A %d %d\r\n",freq,amplitude);
			}
			menu_show(first);
			break;
			case KEY5_PRES: //确定
				if(last->id==4){
					sp_flag=!sp_flag;
					if(sp_flag){
						TIM_Cmd(TIM4, ENABLE);
					}else{
						TIM_Cmd(TIM4, DISABLE);
					}
				}
				break;
				case KEY7_PRES: 
					WIFI_Send_FFT(freq,amplitude);
					break;
	}
//	last=last->next;
	
	//printf("units:%s x:%d\ty:%d\t\r\n",last->units,last->coord.startx-16,last->coord.starty-16);
}
void menu_delete(MENU *M){
	
}
