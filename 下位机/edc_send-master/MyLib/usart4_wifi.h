#ifndef __USART4_WIFI_H
#define __USART4_WIFI_H
#include "stdio.h"	
#include "delay.h" 
#include "sys.h"
#include "string.h"
#include "stdlib.h" 

void UART4_init(u32 bound);
void WIFI_Server_Init(void);
void UART4SendChars(u8 *str, u16 strlen);
void WIFI_Send(u8 *str);
void WIFI_Send_FFT(u8 freq,u16 am);
#endif


