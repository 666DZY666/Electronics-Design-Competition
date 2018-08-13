/*-------------------------------------------------------------------------------
文件名称：led.c
文件描述：根据硬件连接配置LED端口，打开对应的寄存器        
备    注：无
---------------------------------------------------------------------------------*/
#include "led.h"
/*-------------------------------------------------------------------------------
程序名称：LED_Init
程序描述：初始化LED相关端口，打开端口时钟，配置端口输出  
输入参数：无
返回参数：无
备    注：无
---------------------------------------------------------------------------------*/
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}
