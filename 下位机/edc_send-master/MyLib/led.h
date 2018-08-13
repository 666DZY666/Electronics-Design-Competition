#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED1 PDout(3)
#define LED2 PDout(6)
#define LED3 PDout(12)
#define LED4 PDout(13)
void LED_Init(void);

#endif
