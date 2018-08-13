#ifndef _TOOL_H
#define _TOOL_H
#include "sys.h"
#include "usart.h"
#include "my_usart.h"
#include "string.h"

int match(u8 *a,u8 *b);
void clear(u8 a[]);
int deal(u8 *buff,u8 argv[][10]);

#endif

