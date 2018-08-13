/*	
	引脚:
	LCD:
		PD 0 1 4 5 7 8 9 10 11 14 15 
		PE 7 8 9 10 11 12 13 14 15
		PB 0
*/
#include "sys.h"
#include "delay.h"
#include "string.h"
#include "Tool.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "libincludes.h"
#include <stdarg.h>
#include <stdlib.h>
#include "chinese.h"
extern LCD_POINT LCD_Point;
extern u16 amplitude;
extern MENU *first;
extern MENU *last;
extern u8 freq;
void callbackReceive(u8 *rec);
u32 times=0;
int main(void){	
	delay_init();	    	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	
	USARTX_Init(9600);
	UART4_init(115200); //WIFI
	TIM4_Init(5,72-1);//500ms
	Key_Init();
	LED_Init();
	AD9854_Init();
	HSMP3816_Init();
	LCD_Show_Init();
	ADC1_Init();
	LED1=1;
	WIFI_Server_Init();
	LED1=0;
	menu_init();
	printf("ok!\r\n");
	while(1){
		menu_select(KEY_Scan(1));
		receiveMatch(callbackReceive);//调试用
		//delay_ms(10);
//		LED1 = !LED1;		
//		delay_ms(100);
	}
}
 /*
 	回调函数
 */
void callbackReceive(u8 *rec){
	Data receive = 	afterDeal(rec);
	if(receive.argc>0){
		if(!strcmp((const char*)receive.command,"H")){ //HSMP3816 :0-3299
			HSMP3816_Control(atoi((const char*)receive.argv[0]));
			printf("HSMP3816 Received\r\n");
		}else if(!strcmp((const char*)receive.command,"P")){//PE43503  0-31;
//			PE43503_Send(atoi((const char*)receive.argv[0]));
			printf("PE43503 Received\r\n");
		}else if(!strcmp((const char*)receive.command,"A")){//AD9854 
			if(receive.argc>=2){
				AD9854_SetSine(atoi((const char*)receive.argv[0]),atoi((const char*)receive.argv[1]));
				printf("AD9854 Received\r\n");
			}
		}
	}
}

void EXTI15_10_IRQHandler(void)
{
	LED3 =!LED3;
	times++;
	EXTI_ClearITPendingBit(EXTI_Line15);  
}

void TIM3_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
	//	TIM3_Count++;
	//	LED4=!LED4;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
	}
}
	 
void TIM5_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
	{
		EXTI->IMR&=0XF7FFF;//关15脚中断
		LED2=!LED2;
//		printf("F:%dHZ\r\n",times);
		times=0;
		EXTI->IMR|=1<<15;
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update); 
	}
}
/*
误差补偿: 8s           1.00609
					0.5s         1.04301

*/
//u8 auto_freq=0;
//扫频
void TIM4_IRQHandler(void)   //TIM3
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
	{
		if(last->id==4){
			last->value = last->value<40?last->value:0;
			last->value++;
			AD9854_SetSine(last->value,amplitude);
			printf("S %d %d\r\n",last->value,Get_Adc_Average(100));//串口发送
			WIFI_Send_FFT(last->value,Get_Adc_Average(100));
			//printf("req:%d,am:%d\r\n",last->value,amplitude);
			menu_show(first);
			LED4=!LED4;
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  
	}
}
