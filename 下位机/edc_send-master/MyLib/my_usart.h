#ifndef _MY_USART_H
#define _MY_USART_H
#include "sys.h"
#include "Tool.h"
#include <stdio.h>
typedef void (*callbackRec)(u8 *receiveBuff); //将callbackRec定义为 void（void）的函数指针
typedef struct {
    u8 command[5]; //接收的指令
    u8 argv[10][10];//接收指令的参数
    u8 argc; //接收参数的个数
}Data;
void USARTX_Init(u32 bound);
void receiveMatch(callbackRec callback);//接受数据的结构体
Data afterDeal(u8 *rec);
#define USARTX_REC_LEN 200
extern u8  USARTX_RX_BUF[USARTX_REC_LEN]; 
extern u16 USARTX_RX_STA;   
extern Data UART_DATA;
#endif
