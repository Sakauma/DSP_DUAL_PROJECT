#include "UARTPrint.h"


#define BaudRate_Value   115200                         //UARTµÄ²¨ÌØÂÊ
//print function
char PrintfBuffer[2048];
unsigned int PrintfLen;

unsigned int PrintfUART(const char *format){

	strcpy(PrintfBuffer,format);
	PrintfLen = strlen(format);
	UART_Send(PrintfBuffer, PrintfLen);
	return 0;
}


