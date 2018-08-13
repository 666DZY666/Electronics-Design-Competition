#include "usart4_wifi.h"
 

u8 MODE[]="AT+CWMODE=3\r\n";
u8 Router[]="AT+CWJAP=\"ds2017\",\"ds1234567\"\r\n";  //配置成路由器 名字为ds2017 密码 ds1234567
u8 RST[]="AT+RST\r\n";
u8 M_Connection[]="AT+CIPMUX=0\r\n";
u8 SERVER[]="AT+CIPSTART=\"TCP\",\"192.168.31.3\",8088\r\n";  //端口号5000
u8 SEND[]="AT+CIPSEND=\r\n";  //AT+CIPSEND= 发送数据

/****************************************************************************
* 名    称: void UART4_init(u32 bound)
* 功    能：LTE_UART4初始化
* 入口参数：bound：波特率   
* 返回参数：无
* 说    明： 
****************************************************************************/
void UART4_init(u32 bound)
{   
//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(UART4, &USART_InitStructure); //初始化串口1
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(UART4, ENABLE);                    //使能串口1 

}



//串口1发送一个字符
void USART4SendChar(u8 ch)
{      
	while((UART4->SR&0x40)==0);  
    UART4->DR = (u8) ch;      
}
/****************************************************************************
* 名    称: void uart1SendChars(u8 *str, u16 strlen)
* 功    能：串口1发送一字符串
* 入口参数：*str：发送的字符串
            strlen：字符串长度
* 返回参数：无
* 说    明： 
****************************************************************************/
void UART4SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { USART4SendChar(*(str + k)); k++; }   //循环发送,直到发送完毕   
    while (k < strlen);
} 
void WIFI_Send(u8 *str){
	UART4SendChars(str,strlen((const char*)str));
	delay_ms(10);
}
void WIFI_Send_FFT(u8 freq,u16 am){
	u8 buffer[15];
	WIFI_Send("AT+CIPSEND=10\r\n");
	sprintf(buffer,"1 %d %d 8\r\n",freq,am);
	WIFI_Send(buffer);
}
void WIFI_Server_Init(void)
{
	UART4_init(115200);	    
	
	//UART4SendChars(MODE,sizeof(MODE));   
//	delay_ms(1000);
	
	//UART4SendChars(RST,sizeof(RST));     //重启模块
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	//UART4SendChars(Router,sizeof(Router));   //连接服务器
//	delay_ms(1000);                          
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);

//	UART4SendChars(M_Connection,sizeof(M_Connection));  //开启多连接
//	delay_ms(1000);
//	
 
	//UART4SendChars(SERVER,sizeof(SERVER));  
	delay_ms(1000);  
	WIFI_Send(SERVER);

}

