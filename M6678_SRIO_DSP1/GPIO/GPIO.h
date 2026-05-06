/*******************************************************************************
*文件名：GPIO_Test.h
*作    用： GPIO基本接口函数的声明。
*版    本：Version 1.0  创建于2018.01.29
*     Version 1.1   2018.05.08 对程序中的地址进行宏替换
*******************************************************************************/

#ifndef    _GPIO_TEST_H
#define    _GPIO_TEST_H

/**********************GPIO寄存器**************************************************/
#define  BINTEN         0X02320008
#define  DIR            0X02320010
#define  OUT_DATA       0X02320014
#define  IN_DATA        0X02320020

#define  SET_RIS_TRIG   0X02320024
#define  CLR_RIS_TRIG   0X02320028
#define  SET_FAL_TRIG   0X0232002C
#define  CLR_FAL_TRIG   0X02320030

#define	 GPIO(n)		(n)
#define  Val(n)			(n)

extern void GPIO_BINTEN(unsigned int Enable);
extern void GPIO_RISTRIG(unsigned int ris);
extern void GPIO_RISTRIG(unsigned int fal);
extern void GPIO_FALTRIG(unsigned int  fal);
extern void GPIO_DirectionSet(unsigned int direction);
extern void GPIO_DataOutWrite(unsigned int DataOut);
extern void GPIO_DataOutReverse(unsigned int Numb);
extern unsigned int GPIO_DataInputGet();
extern void GPIO_DateOutnWriteX(unsigned int Numb,unsigned int polarity);

#endif
