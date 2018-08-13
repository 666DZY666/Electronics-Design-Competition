#ifndef _ADF4001_h_
#define _ADF4001_h_
//******************属于鉴相器**************************//
//******************************************************//
#include "sys.h"
#define  A4001_LE  PAout(4)	
#define  A4001_DATA PAout(3)
#define  A4001_CLK PAout(2)

void Send_Data(unsigned long int datanumber);

void ADF4001_Control(unsigned long int REF_divide,
					 unsigned long int N_divide);
void ADF4001_Init(void);

#endif


