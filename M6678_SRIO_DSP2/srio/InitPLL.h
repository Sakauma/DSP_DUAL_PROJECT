
/*******************************************************************************
*文件名：InitPLL.h
*作    用： 用于声明时钟配置的接口函数和寄存器宏定义。
*版    本：Version 1.0  创建于2018.03.16
*******************************************************************************/
#ifndef INITPLL_H_
#define INITPLL_H_

#define	KICK0 	*(volatile	unsigned int *)0x02620038
#define	KICK1	*(volatile	unsigned int *)0x0262003C

/*********************MainPLL 寄存器************************************************/
#define	MainPLLCTL0			*(volatile	unsigned int *)0x02620328
#define	MainPLLCTL1			*(volatile	unsigned int *)0x0262032C
#define	MainPLLCMD			*(volatile unsigned int *)0x02310100

/*********************DDR时钟 寄存器**************************************************/
#define	DDRPLLCTL0			*(volatile	unsigned int *)0x02620330
#define	DDRPLLCTL1			*(volatile	unsigned int *)0x02620334
#define	DDRPLLCMD			*(volatile	unsigned int *)0x02310160
#define	DDRPLLC_En			*(volatile	unsigned int *)0x02310164

/****************配置SRIO、PCIe、GMAC系统时钟以及GMAC特有时钟******************************/
#define	PASSPLLCTL0			*(volatile	unsigned int *)0x02620338
#define	PASSPLLCTL1			*(volatile	unsigned int *)0x0262033C
#define	PASSPLLCMD			*(volatile	unsigned int *)0x02310170
#define	PASSPLLC_En			*(volatile	unsigned int *)0x02310174

/*********************ECLKOUT寄存器*************************************************/
#define	PLLDIV_V 			*(volatile	unsigned int *)0x02310118
#define	PLLCTLCMD  			*(volatile	unsigned int *)0x02310138
#define	PLLSTAT				*(volatile	unsigned int *)0x0231013C
#define	DCHANGE 			*(volatile	unsigned int *)0x02310144

/*********************PLL相关函数声明*************************************************/
extern void pll_wait(unsigned int i);
extern void MainPLL(unsigned int PLLM, unsigned int PLLD,  unsigned int POSTDIV2,unsigned int POSTDIV1);
extern void DDRpll(unsigned int PLLM, unsigned int PLLD,  unsigned int POSTDIV2,unsigned int POSTDIV1);
extern void ECLKOUT(unsigned int RATIO);
extern void PASSPLL();

#endif /* INITPLL_H_ */
