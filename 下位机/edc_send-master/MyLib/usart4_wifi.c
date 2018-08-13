#include "usart4_wifi.h"
 

u8 MODE[]="AT+CWMODE=3\r\n";
u8 Router[]="AT+CWJAP=\"ds2017\",\"ds1234567\"\r\n";  //���ó�·���� ����Ϊds2017 ���� ds1234567
u8 RST[]="AT+RST\r\n";
u8 M_Connection[]="AT+CIPMUX=0\r\n";
u8 SERVER[]="AT+CIPSTART=\"TCP\",\"192.168.31.3\",8088\r\n";  //�˿ں�5000
u8 SEND[]="AT+CIPSEND=\r\n";  //AT+CIPSEND= ��������

/****************************************************************************
* ��    ��: void UART4_init(u32 bound)
* ��    �ܣ�LTE_UART4��ʼ��
* ��ڲ�����bound��������   
* ���ز�������
* ˵    ���� 
****************************************************************************/
void UART4_init(u32 bound)
{   
//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(UART4, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ���1 

}



//����1����һ���ַ�
void USART4SendChar(u8 ch)
{      
	while((UART4->SR&0x40)==0);  
    UART4->DR = (u8) ch;      
}
/****************************************************************************
* ��    ��: void uart1SendChars(u8 *str, u16 strlen)
* ��    �ܣ�����1����һ�ַ���
* ��ڲ�����*str�����͵��ַ���
            strlen���ַ�������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void UART4SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { USART4SendChar(*(str + k)); k++; }   //ѭ������,ֱ���������   
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
	
	//UART4SendChars(RST,sizeof(RST));     //����ģ��
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	//UART4SendChars(Router,sizeof(Router));   //���ӷ�����
//	delay_ms(1000);                          
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);

//	UART4SendChars(M_Connection,sizeof(M_Connection));  //����������
//	delay_ms(1000);
//	
 
	//UART4SendChars(SERVER,sizeof(SERVER));  
	delay_ms(1000);  
	WIFI_Send(SERVER);

}

