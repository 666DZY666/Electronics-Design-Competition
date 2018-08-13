#include "my_usart.h"
//M_USART 为使用的串口号 1-4分别为1-4号串口
#define M_USART 2
#if M_USART==1
    #define UART USART1
    #define UART_GPIO_TX GPIOA
    #define UART_GPIO_RX UART_GPIO_TX
    #define RCC_GPIO RCC_APB2Periph_GPIOA
    #define RCC_UART RCC_APB2Periph_USART1
    #define UART_PIN_TX  GPIO_Pin_9
    #define UART_PIN_RX  GPIO_Pin_10
    #define UART_IRQ    USART1_IRQn
    #define UARTX_IRQHandler     USART1_IRQHandler
#elif M_USART==2
    #define UART USART2
    #define UART_GPIO_TX GPIOA
    #define UART_GPIO_RX UART_GPIO_TX
    #define RCC_GPIO RCC_APB2Periph_GPIOA
    #define RCC_UART RCC_APB1Periph_USART2
    #define UART_PIN_TX  GPIO_Pin_2
    #define UART_PIN_RX  GPIO_Pin_3
    #define UART_IRQ    USART2_IRQn
    #define UARTX_IRQHandler     USART2_IRQHandler
#elif M_USART==3
    #define UART USART3
    #define UART_GPIO_TX GPIOB
    #define UART_GPIO_RX UART_GPIO_TX
    #define RCC_GPIO RCC_APB2Periph_GPIOB
    #define RCC_UART RCC_APB1Periph_USART3
    #define UART_PIN_TX  GPIO_Pin_10
    #define UART_PIN_RX  GPIO_Pin_11
    #define UART_IRQ    USART3_IRQn
    #define UARTX_IRQHandler     USART3_IRQHandler
#elif M_USART==4
    #define UART UART4
    #define UART_GPIO_TX GPIOC
    #define UART_GPIO_RX UART_GPIO_TX
    #define RCC_GPIO RCC_APB2Periph_GPIOC
    #define RCC_UART RCC_APB1Periph_UART4
    #define UART_PIN_TX  GPIO_Pin_10
    #define UART_PIN_RX  GPIO_Pin_11
    #define UART_IRQ UART4_IRQn
    #define UARTX_IRQHandler     UART4_IRQHandler
#elif my_usart==5
    #define UART UART5
    #define UART_GPIO_TX GPIOC
    #define UART_GPIO_RX GPIOD
    #define RCC_GPIO (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
    #define RCC_UART RCC_APB1Periph_UART5
    #define UART_PIN_TX  GPIO_Pin_12
    #define UART_PIN_RX  GPIO_Pin_2
    #define UART_IRQ UART5_IRQn
    #define UARTX_IRQHandler     UART5_IRQHandler
#endif

u8 USARTX_RX_BUF[USARTX_REC_LEN];     //接收缓冲,最大USARTX_REC_LEN个字节.
u16 USARTX_RX_STA=0;       //接收状态标记
void USARTX_Init(u32 bound){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
        
    RCC_APB2PeriphClockCmd(RCC_GPIO, ENABLE);	//使能PORTC时钟
    RCC_APB1PeriphClockCmd(RCC_UART,ENABLE); //使能UARTX
		RCC_APB2PeriphClockCmd(RCC_UART,ENABLE); //使能UARTX
    USART_DeInit(UART);  //复位串口
        //UARTX_TX   
    GPIO_InitStructure.GPIO_Pin = UART_PIN_TX; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(UART_GPIO_TX, &GPIO_InitStructure); 

    //UARTX_RX	 
    GPIO_InitStructure.GPIO_Pin = UART_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(UART_GPIO_RX, &GPIO_InitStructure);     

    //UARTX NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = UART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART, &USART_InitStructure);
    USART_ITConfig(UART, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(UART, ENABLE);                    //使能串口 
}
///重定向c库函数printf到USARTx
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到UARTX */
	USART_SendData(UART, (uint8_t) ch);
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);
	return (ch);
}
 
void UARTX_IRQHandler(){
	u8 Res;
	if(USART_GetITStatus(UART, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(UART);	//读取接收到的数据
		if((USARTX_RX_STA&0x8000)==0)//接收未完成
        {
			if(USARTX_RX_STA&0x4000)//接收到了0x0d
            {
				if(Res!=0x0a)USARTX_RX_STA=0;//接收错误,重新开始
				else USARTX_RX_STA|=0x8000;	//接收完成了 
            }
			else //还没收到0X0D
            {	
				if(Res==0x0d)USARTX_RX_STA|=0x4000;
				else
                {
					USARTX_RX_BUF[USARTX_RX_STA&0X3FFF]=Res ; //并不记录 0x0d 0x0a
					USARTX_RX_STA++;
					if(USARTX_RX_STA>(USARTX_REC_LEN-1))USARTX_RX_STA=0;//接收数据错误,重新开始接收	  
                }		 
            }
        }   		 
    } 
}
/*
	rec 是接收到字符串
	callbackRec 是回调函数 void*(u8 *receive)
	函数功能：
				如果接收到的字符串和rec相等，则执行回调函数，需要usart.h的支持
*/
void receiveMatch(callbackRec callback){
		if((USARTX_RX_STA&0x8000)!=0){
			(*callback)(USARTX_RX_BUF);
			//清零
			USARTX_RX_STA=0;
			clear(USARTX_RX_BUF);
		}
}
/*
	接受处理终极函数
	rec:发送的原始字符串
	Data:返回字符串的结构体
*/
Data afterDeal(u8 *rec) {
	Data data= {0};
	u8 buff[20]= {0};
	sscanf((const char*)rec,"%s %[^;]",data.command,buff); //分号结束
	data.argc=deal(buff,data.argv);
//	printf("buff:%s\r\n",buff);
	return data;
}

