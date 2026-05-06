
/*******************************************************************************
*文件名：UART.c
*作    用： 用于实现串口功能的接口函数
*版    本：Version 1.0  创建于2018.05.08
*******************************************************************************/
#include <string.h>
#include "UART.h"
#include "InitPLL.h"
#include "PSC.h"
/*******************************************************************************
*函数名：UART_Init(Uint32 BaudRate)
*功    能： 打开UART时钟,配置波特率。
*参    数：
*	   BaudRate --------- 波特率
*返 回 值：无
*备    注：无
*******************************************************************************/
void UART_Init(Uint32 BaudRate)
{
	unsigned int temp11,temp22;
	PSC_Open_Clk("UART");
	temp11 = BaudRate*8*4;
	temp22= MAIN_PLL/temp11;
	ScalerReg=(temp22*10- 5 )/10;
	ControlReg=(1<<0)|(1<<1)|(1<<2)|(1<<3);	// 接收和发送使能 并且打开接收和发送中断
//	uart_in_clk = main_frq * 1000000;		// MHz to Hz
//	uart_in_clk /= 4;
//	scaler = (((uart_in_clk*10)/(BaudRate*8))-5)/10;
//	printf("UART_Init OK, main_frq:%dMHz, scaler:%d, BaudRate:%d.\r\n",
//			main_frq, scaler, BaudRate);
//	UARTDEV->UARTSCL = scaler;
//	UARTDEV->UARTCTL = (1<<0)|(1<<1)|(1<<2)|(1<<3);	// 接收和发送使能 并且打开接收和发送中断
}
/*******************************************************************************
*函数名：UART_Send(char *SourceData_Addr,Uint32 Count_Data)
*功    能： 串口发送数据
*参    数：SourceData_Addr表示需要发送的数据的起始地址,Count_Data为传输的字节数。
*******************************************************************************/
void  UART_Send(char *SourceData_Addr,Uint32 Count_Data)
{
	unsigned int i;
	unsigned char Tranbuf;
	char *temp_byte;
	temp_byte = (char *) SourceData_Addr;
	// ***********************transmit*****************************************
	for(i=0;i<Count_Data;i++)                  //  循环发送size个数给串口助手
	{
		Tranbuf = *temp_byte++;
		DataReg=Tranbuf;                       //   给数据寄存器里写入要发送的数值
	}
}

/******************************************************************************
*函数名：UART_Receive(char *DestData_Addr)
*功    能： 串口接收数据
*参    数：DestData_Addr表示需要接收的数据存放的地址。
*******************************************************************************/
void  UART_Receive(char *DestData_Addr, unsigned int u32RecvByteCnt)
{
	char *p;
	int i=0;
	p = DestData_Addr;
	while(1)                                          //一直处于接收状态，设置串口助手为发送时，
	{												  //要注意设置发送间隔时间，如>=2毫秒
		while((StatusReg&0xfc000000)<0x04000000){};  //查询状态寄存器是否有有效数据
		*p = DataReg;
		p = p+1;
		i = i+1;
		if(i >= u32RecvByteCnt)
			break;
	}
}

/*******************************************************************************
*函数名：UART_Print(const char *strFmt)
*功    能： 串口打印字符串
*参    数：*strFmt代表要传输的字符串。
*******************************************************************************/
void UART_Print(const char *strFmt)
{
	unsigned int i,length;
    length = strlen(strFmt);
    for (i=0;i<length;i++)
    {
    	DataReg=strFmt[i];
    }
 }

