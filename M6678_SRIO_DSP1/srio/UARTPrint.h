#ifndef _UARTPRINT_H_
#define _UARTPRINT_H_

#include "UART.h"
#include "string.h"
#include <stdio.h>
extern char PrintfBuffer[2048];
extern unsigned int PrintfUART(const char *format);


#endif
