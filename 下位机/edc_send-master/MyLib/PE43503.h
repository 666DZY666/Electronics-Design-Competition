#ifndef _PE43503_H
#define _PE43503_H
#include "sys.h"
#include "delay.h"
#define  PE43503_LE  PBout(4)
#define  PE43503_DATA PBout(3)
#define  PE43503_CLK PBout(2)
void PE43503_Init(void);
u8 PE43503_Send(unsigned char data);
#endif
