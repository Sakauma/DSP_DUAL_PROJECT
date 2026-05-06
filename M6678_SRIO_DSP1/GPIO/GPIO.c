
/*******************************************************************************
*文件名：GPIO_Test.c
*作    用： 用于GPIO的基本控制，包括方向设置、输出控制等部分。
*版    本：Version 1.0  创建于2018.01.29
*******************************************************************************/
#include "GPIO.h"

/*******************************************************************************
*函数名：GPIO_BINTEN
*功    能： 全局中断使能配置
*参    数：GPIO模块中断使能，1表示使能，0表示不使能。
*******************************************************************************/
void GPIO_BINTEN(unsigned int  enable)
{
	//设置“部件中断使能寄存器 ”
	*(unsigned int *)BINTEN = (unsigned int)enable & 0x1;
	asm("	MFENCE ");
}

/*******************************************************************************
*函数名：GPIO_RISTRIG
*功    能： 配置上升沿触发中断方式
*参    数：二进制展开代表GPIO的每一位，1表示上升沿检测使能，0表示上升沿检测关闭。
*******************************************************************************/
void GPIO_RISTRIG(unsigned int  ris)
{
	//设置“设置上升沿触发中断寄存器 ”和“清除上升沿触发中断寄存器”
	*(unsigned int *)SET_RIS_TRIG =  (unsigned int)ris;
	*(unsigned int *)CLR_RIS_TRIG = ~(unsigned int)ris;
	asm("	MFENCE ");
}

/*******************************************************************************
*函数名：GPIO_FALTRIG
*功    能： 配置下降沿触发中断方式
*参    数：二进制展开代表GPIO的每一位，1表示下降沿检测使能，0表示下降沿检测关闭。
*******************************************************************************/
void GPIO_FALTRIG(unsigned int  fal)
{
	//设置“设置下降沿触发中断寄存器 ”和“设置下降沿触发中断寄存器”
	*(unsigned int *)SET_FAL_TRIG =  (unsigned int)fal;
	*(unsigned int *)CLR_FAL_TRIG = ~(unsigned int)fal;
	asm("	MFENCE ");
}

/*******************************************************************************
*函数名：GPIO_DirectionSet
*功    能： 配置方向函数
*参    数：二进制展开代表GPIO的每一位，1表示输入，0表示输出。
*注    意：1、设置此寄存器时，就已经将所有的输入输出规定好了，所以需要提前规划好相关引脚的性质。
*     2、 若不设置该寄存器，缺省状态下，所有的GPIO PIN都配置为输入。
*******************************************************************************/
void GPIO_DirectionSet(unsigned int  direction)
{
	//设置“方向寄存器 ”
	*(unsigned int *)DIR = (unsigned int)direction;
	asm("	MFENCE ");
}

/*******************************************************************************
*函数名：GPIO_DateOutnWriteX
*功    能： 将固定的某一位置0或置1，对应相关引脚的高低电平。
*参    数：Numb表示具体的某一位，polarity可以设置为1或0。
*******************************************************************************/
void GPIO_DateOutnWriteX(unsigned int Numb,unsigned int polarity)
{
	unsigned int outdata;
	unsigned int * outdata_addr;
	outdata = 1 << Numb;
	outdata_addr = (unsigned int *)OUT_DATA;
	//对"输出数据寄存器"进行赋值。
	if(polarity)
		*outdata_addr = *outdata_addr | outdata;
	else
		*outdata_addr = *outdata_addr & ~outdata;
	asm("	MFENCE ");
}

/*******************************************************************************
*函数名：GPIO_DataOutWrite
*功    能： 写输出值函数。
*参    数：DataOut这个数，表示一次将所有的输出数值设定好。
*******************************************************************************/
void GPIO_DataOutWrite(unsigned int DataOut)
{
	*(unsigned int *)OUT_DATA = (unsigned int)DataOut;
	asm("	MFENCE ");
}

/*******************************************************************************
*函数名：GPIO_DataInputGet
*功    能： 读取输入的GPIO数值。
*参    数：1、DataOut这个数，表示一次将所有的输入数值读取进来。
*     2、返回GPIO的PIN脚状态，无论该位在方向和输出数据寄存器的状态如何。
*******************************************************************************/
unsigned int GPIO_DataInputGet()
{
	//设置“输入数据寄存器”
	unsigned int inputdata1;
	inputdata1 = *(unsigned int *)IN_DATA;
	return inputdata1;

}

/*******************************************************************************
*函数名：GPIO_DataOutReverse
*功    能： GPIO输出翻转函数。
*参    数：1、Numb这个数对应的二进制中，1表示需要翻转的GPIO位，。
*     2、注意此函数可以一次操作多个GPIO的数值，使用时要计算好Numb数值，再进行配置。
*******************************************************************************/
void GPIO_DataOutReverse(unsigned int Numb)
{
	unsigned int outdata,invoutp[3];
	invoutp[0] = (unsigned int)Numb;
	//1.提取输出为1的反转位
	invoutp[1] = invoutp[0] & *(unsigned int *)OUT_DATA;
	//2.提取输出为0的反转位
	invoutp[2] = invoutp[0] & ~invoutp[1];
	//3.将1反转为0
	outdata = *(unsigned int *)OUT_DATA & ~invoutp[1];
	//4.将翻转的位合并进来
	outdata = outdata | invoutp[2];
	//5.赋值输出
	*(unsigned int *)OUT_DATA = outdata;
	asm("	MFENCE ");
}
