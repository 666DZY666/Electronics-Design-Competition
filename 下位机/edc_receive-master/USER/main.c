/*
	接收部分
	引脚：
		PD2：测频
		PA4:DAC1——频率
		PA5:DAC2——幅度
		PC11:UART4 RX
		PC1: ADC 测幅度
		分屏：100	
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
void callbackReceive(u8 *rec);
u32 times=0;
u8 TIM3_Count=0;
u16 tt=0;
u32 temp=0;
u32 frequceny=0;
u8 buff_hz[20];
u8 buff_db[20];
float adc_value=0.0;
int main(void){	
	int step=200;	
	delay_init();	    	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	
	USARTX_Init(9600);
	LED_Init();
	Key_Init();
	ADC1_Init();
	DAC_InitAll();
//	Input_Init(10000,7299);//500ms   中断测频 用到TIM15 1-860k
//	TIM3_Cap_Init(5000,7200-1);//10ms  测频 用到TIM4 TIM3  1-10MHZ
	printf("ok!\r\n");
	while(1){
		//adc_value=(float)Get_Adc_Average(100)/4096*3.65/0.6;
//		KEY_LCD();
		receiveMatch(callbackReceive); //接收串口信号
//		DAC2_V_Control((int)(adc_value+1)*673);//测幅度
//		DAC1_F_Control(frequceny/100.7326); //40k-3.2v  1k-0.125v
		//printf("adc:%f\r\n",adc_value);//Get_Adc_Average(100)/4096*3.65/0.6 为
		//receiveMatch(callbackReceive);
	}
}
 /*
 	回调函数
 */
void callbackReceive(u8 *rec){
	printf("receive:%s",rec);
	Data receive = 	afterDeal(rec);
	if(receive.argc>0){
		if(!strcmp((const char*)receive.command,"H")){ //HSMP3816 :0-3299
	//		HSMP3816_Control(atoi((const char*)receive.argv[0]));
			printf("HSMP3816 Received\r\n");
		}else if(!strcmp((const char*)receive.command,"P")){//PE43503  0-31;
	//		PE43503_Send(atoi((const char*)receive.argv[0]));
			printf("PE43503 Received\r\n");
		}else if(!strcmp((const char*)receive.command,"S")){//AD9854 
			if(receive.argc>=2){
//				AD9854_SetSine(atoi((const char*)receive.argv[0]),atoi((const char*)receive.argv[1]));
				DAC1_F_Control(atoi((const char*)receive.argv[0])*80);// 发送的单位是mv
				DAC2_V_Control(atoi((const char*)receive.argv[1])); //发送的单位是hz
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
		TIM3_Count++;
		LED4=!LED4;
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
void TIM4_IRQHandler(void)   //TIM3
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
	{
		TIM_Cmd(TIM3, DISABLE); 
		frequceny = (int)(TIM3->CNT+TIM3_Count*65536)*16*1.04301;
		printf("tim3->cnt:%dHZ\r\n",frequceny); 
		TIM3->CNT=0;
		TIM3_Count=0;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  
		TIM_Cmd(TIM3, ENABLE);
	}
}
