#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

void ADC1_Init(void);
//u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 times); 
#endif 
