#include "C6x.h"
#include "stdio.h"
#include "InitPLL.h"

void SysInit(){
	unsigned int i;
	//disable cache
	*(unsigned int volatile *)(0x01845044) = 1;
	do{
	i = (*(unsigned int volatile *)(0x01845044)&0x1);
	}while(i!=0);
	*(unsigned int volatile *)(0x01840040) = 0;

	printf("Test begin\n");
	printf("main PLL 1GHz\n");
	MainPLL(40,1,1,1);//1GHz
	PSC_Open_Clk("Timer");
}
