#include "key.h"
#include "delay.h"
#include "my_usart.h"
u8 a=0;
u8 b=0;
int c=0;
u8 d=0;
u8 f=0;
void Key_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//PD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOD
}
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0||KEY7==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==0)return KEY4_PRES;
		else if(KEY5==0)return KEY5_PRES;
		else if(KEY6==0)return KEY6_PRES;
		else if(KEY7==0)return KEY7_PRES;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1&&KEY6==1&&KEY7==1)key_up=1; 	     
	return 0;// 无按键按下
}
//按键控制LCD
//KEY7—幅度增加
//KEY6—幅度减小
//KEY5—频率增加
//KEY4—频率减小
//KEY3—增益增加
//KEY2—增益减小
//KEY1—模式切换（短按—幅度；长按—XX）
void KEY_LCD(void)
{
	if(KEY2==0)
	{
		delay_ms(100);
		if(KEY2==0)
			c--;
		printf("H %d\r\n",40-c);
	}
	if(KEY3==0)
	{
		delay_ms(100);
		if(KEY3==0)
			c++;
		printf("H %d\r\n",40-c);
	}
	if(KEY4==0)
	{
		delay_ms(100);
		if(KEY4==0)
			b--;
		printf("A %d %d\r\n",b,a);
	}
	if(KEY5==0)
	{
		delay_ms(100);
		if(KEY5==0)
			b++;
		printf("A %d %d\r\n",b,a);
	}
	if(f==0)
	{
		if(KEY7==0)
		{
			delay_ms(100);
			if(KEY7==0)
				a++;
			printf("A %d %d\r\n",b,a);
		}
		if(KEY6==0)
		{
			delay_ms(100);
			if(KEY6==0)
				a--;
			printf("A %d %d\r\n",b,a);
		}
	}
	else if(f==1)
	{
		if(KEY7==0)
		{
			delay_ms(100);
			if(KEY7==0)
				d++;
		}
		if(KEY6==0)
		{
			delay_ms(100);
			if(KEY6==0)
				d--;
		}
	}
	if(KEY1==0)
	{
		f=0;
		delay_ms(1000);
		if(KEY1==0)
		{
			f=1;
			delay_ms(1000);
			if(KEY1==0)
			{
				f=1;
				delay_ms(1000);
				if(KEY1==0)
					f=1;
			}
		}
	}
	if(a<5)
		a=5;
	else if(a>100)
		a=100;
	if(b<1)
		b=1;
	else if(b>40)
		b=40;
	if(c<0)
		c=0;
	else if(c>40)
		c=40;
	if(d<1)
		d=1;
	else if(d>40)
		d=40;
}
