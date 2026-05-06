
/*******************************************************************************
*文件名：InitPLL.c
*作    用： 用于配置时钟的接口函数。
*版    本：Version 1.0  创建于2018.03.16
*******************************************************************************/
#include <stdio.h>
#include "InitPLL.h"
#include "PSC.h"

/*******************************************************************************
*函数名：pll_wait(unsigned int i)
*功    能： 延时函数
*参    数：设置循环执行nop 5的次数。注意该延时函数只是粗略的延时。
*******************************************************************************/
void pll_wait(unsigned int i)
{
	volatile unsigned int c=0;
	for(c=0;c<i;c++)
	{
		asm("	nop 5");
	}
}

/*******************************************************************************
*函数名：MainPLL(unsigned int PLLM, unsigned int PLLD,  unsigned int POSTDIV2,unsigned int POSTDIV1)
*功    能：主时钟配置，配置主PLL主要用来给CorePacs和部分外设部件提供时钟
*参    数：PLLM为倍频系数，PLLD、POSTDIV2和POSTDIV1为分频系数
*******************************************************************************/
// +---------------------+---------------+--------+--------+------------+-----------+
// | Desired             | (CLK) Input   |        |        |            |           |
// | Device Speed (MHz)  |  Clock (MHz)  | PLLM   | PLLD   | POSTDIV2   | POSTDIV1  |
// +---------------------+---------------+--------+--------+------------+-----------+
// | 1000                | 25(DSK)       | 40     | 1      |       1    |     1     |
// | 800                 |               | 32     | 1      |       1    |     1     |
// | 600                 |               | 48     | 1      |       1    |     2     |
// | 100                 |               | 48     | 1      |       2    |     6     |
// +---------------------+---------------+--------+--------+------------+-----------+
/* FOUTPOSTDIV=(CLK/PLLD*PLLM)/POSTDIV2/POSTDIV1
**在参数配置时需要注意参数的限定条件：1MHz<=CLK<=2GHz
**						1MHz<=CLK/PLLD<=40MHz
**						800MHz<=CLK/PLLD*PLLM<=3.2GHz**/
void MainPLL(unsigned int PLLM, unsigned int PLLD,  unsigned int POSTDIV2,unsigned int POSTDIV1)
{
	volatile unsigned int temp=0;
	volatile unsigned int	Main_PLLM, Main_PLLD,Main_POSTDIV2,Main_POSTDIV1;
	 if (PLLM == 0)			Main_PLLM = 1;
	 else 							Main_PLLM=PLLM;
	 if (PLLD == 0) 		Main_PLLD = 1;
	 else 							Main_PLLD=PLLD;
	 if (POSTDIV2 == 0)	Main_POSTDIV2 = 1;
	 else 							Main_POSTDIV2=POSTDIV2;
	 if (POSTDIV1 == 0)	Main_POSTDIV1 = 1;
	 else 							Main_POSTDIV1=POSTDIV1;

	//CLC寄存器保护机制解锁
	/*对Kicker机制解锁时，需要通过对两个MMR对（KICK0和KICK1）寄存器写入特定数据（KICK0为0x83e70b13，KICK1为0x95a4f1e0）。
	对任意一个Kicker MMRs写入其他数据将锁定kick机制，此时，CLC MMRs只可读。*/
	KICK0=0x83e70b13;
	KICK1=0x95a4f1e0;

	/*配置主PLL系统时钟*/
	MainPLLCMD=0x20;//将MainPLLC输出时钟模式切换为bypass模式
	//至少等待输入时钟CLK四个时钟周期，来保证系统时钟切换到BYPASS模式；
	pll_wait(20);

	//将MainPLLCMD中的PD写为1，关断PLL；
	MainPLLCMD=0x2;

 /** MAINPLLCTL0 Register.*****************************************
    * |  31...18  | 17...6   |5...0     |                         *
    * |Reserved   |  PLLM    |PLLD      |**************************/
	MainPLLCTL0=(Main_PLLM<<6) | Main_PLLD;//设置MainPLL的输入控制信号

/** MAINPLLCTL1 Register.******************************************
   * |31...9  |  8  |   7      |   6   |   5...3 | 2...0  | 			*
   * |Reserved|ENSAT| PLLOUTCTL| BYPASS| POSTDIV2|POSTDIV1|********/
	MainPLLCTL1=(MainPLLCTL1 & 0xffffff40) | (Main_POSTDIV2<<3)|Main_POSTDIV1;//主PLL输出时钟使能
																																			  //配置PLL的POSTDIV2、POSTDIV1系数
	pll_wait(100);	//至少等待1us后将MainPLLCMD中的PD位写为0
	MainPLLCMD=0x0;

	//查询MainPLLCMD中的LOCK是否为1，判断时钟是否稳定
	temp=0x40 &	MainPLLCMD ;
	while(!temp){
		temp=0x40 &	MainPLLCMD ;
	}
	MainPLLCMD=0x1;//将MainPLL控制器时钟切换到PLL模式
	pll_wait(200);
}

/*******************************************************************************
*函数名：DDRpll(unsigned int PLLM, unsigned int PLLD,  unsigned int POSTDIV2,unsigned int POSTDIV1)
*功    能：DDR时钟配置，配置DDRPLL用来专门给DDR部件提供参考时钟DDR_REFCLK、DDR_REFCLK2。
*参    数：PLLM为倍频系数，PLLD、POSTDIV2和POSTDIV1为分频系数
*******************************************************************************/
// +--------------------+---------------+--------+--------+------------+-----------+
// | DDR3 PLL VCO       | (DDRCLK) Input|        |        |            |           |
// | Rate (MHz)         | Clock (MHz)   | PLLM   | PLLD   | POSTDIV2   | POSTDIV1  |
// +--------------------+---------------+--------+--------+------------+-----------+
// | 400                | 25(DSK)       | 32     | 1      |       1    |     2     |
// | 666                |               | 133    | 5      |       1    |     4     |
// | 800                |               | 32     | 1      |       1    |     1     |
// +--------------------+---------------+--------+--------+------------+-----------+

/* DDRPLL_FOUTPOSTDIV=(DDRCLK/PLLD*PLLM)/POSTDIV2/POSTDIV1
**在参数配置时需要注意参数的限定条件：1MHz<=DDRCLK<=2GHz
**						1MHz<=DDRCLK/PLLD<=40MHz
**						800MHz<=DDRCLK/PLLD*PLLM<=3.2GHz**/
void DDRpll(unsigned int PLLM, unsigned int PLLD,  unsigned int POSTDIV2,unsigned int POSTDIV1)
{
	volatile unsigned int temp;
	volatile unsigned int	 DDR_PLLM, DDR_PLLD,DDR_POSTDIV2,DDR_POSTDIV1;
	 if (PLLM == 0)			DDR_PLLM = 1;
	 else								DDR_PLLM=PLLM;
	 if (PLLD == 0) 		DDR_PLLD = 1;
	 else								DDR_PLLD=PLLD;
	 if (POSTDIV2 == 0)	DDR_POSTDIV2 = 1;
	 else								DDR_POSTDIV2=POSTDIV2;
	 if (POSTDIV1 == 0)	DDR_POSTDIV1 = 1;
	 else								DDR_POSTDIV1=POSTDIV1;
	//将DDRPLLC输出时钟模式切换为bypass模式
	DDRPLLC_En=0x1;
	//至少等待DDRPLL输入时钟DDRCLK四个时钟周期，来保证DDR_REFCLK时钟切换到BYPASS模式
	pll_wait(20);
	//DDRPLLCMD中的PD写为1，关断PLL
	DDRPLLCMD=0x1;

	//设置DDRPLL的输入控制信号
/** DDRPLLCTL0 Register.******************************************
   * |31...24  |   23   |  22...18  |17...6|5...0 |              *
   * |Reserved | BYPASS |  Reserved | PLLM |PLLD  |***************/
	DDRPLLCTL0=(DDRPLLCTL0 & 0xfffc0000) | (DDR_PLLM<<6) | DDR_PLLD;//配置PLL的倍频分频系数

/** DDRPLLCTL1 Register.*****************************************
   * |31...7  |      6     |  5...3  | 2...0  | 								*
   * |Reserved|  PLLOUTCTL | POSTDIV2|POSTDIV1| *****************/
	DDRPLLCTL1=(DDRPLLCTL1 & 0xffffffc0) | (DDR_POSTDIV2<<3)|DDR_POSTDIV1; //配置PLL的POSTDIV2、POSTDIV1系数

    //至少等待1us后将DDRPLLCMD中的PD写为0
	pll_wait(100);
	DDRPLLCMD=0;
	//查询DDRPLLCMD中的LOCK是否为1，判断时钟是否稳定
	temp=0x2 &	DDRPLLCMD ;
	while(!temp)
	{
		temp=0x2 &	DDRPLLCMD ;
	}
	//;;;;将DDRPLLC_En中的PLLC_BYPASS写为0，将DDRPLL控制器时钟切换到PLL模式；*******
	DDRPLLC_En=0;
}

/*******************************************************************************
*函数名：ECLKOUT(unsigned int RATIO)
*功    能：ECLKOUT时钟配置，ECLKOUT=SYSCLK1/(RATIO*2)。
*参    数：RATIO为分频器系数。
*******************************************************************************/
void ECLKOUT(unsigned int RATIO)
{
	volatile unsigned int temp=0;

	PSC_Open_Clk("EMIF32");

	if(RATIO == 0)	RATIO=1;
	//1.  查看PLLSTAT寄存器中GOSTAT位，确保是否有GO操作正在进行；
	temp=PLLSTAT & 0x1;
	while(temp)
	{
		temp=PLLSTAT & 0x1;
	}
	//2.	通过编程将PLLDIV_V中的RATIO域改变为预定值
/** PLLDIV_V  Register.**************************************
   * | 31...16  |   15  |  14...8  | 3...0|                 *
   * |Reserved  |  DEN  | Reserved | RATIO| *****************/
	PLLDIV_V=0x00008000 |  RATIO;
	//3.	通过将PLLCTLCMD中GOSET位置为1执行GO操作，此时会根据新的分频系数产生时钟；时钟分频系数为RATIO*2。
	PLLCTLCMD=0x1;
	//4.	查看PLLSTAT寄存器中GOSTAT位是否为0，来标识GO操作是否完成。
	temp=PLLSTAT	&	0x1;
	while(temp)
	{
		temp=PLLSTAT	&	0x1;
	}
	DCHANGE=0x0;
}

/*******************************************************************************
*函数名：PASSPLL()
*功    能：PASSPLL主要用来给PCIe、SRIO、GMAC三大外设部件提供时钟
*参    数：无。
*******************************************************************************/
// +--------------------+---------------+--------+--------+------------+-----------+
// | PASSPLL VCO        |(PASSCLK) Input|        |        |            |           |
// | Rate (MHz)         | Clock (MHz)   |   PLLM | PLLD   | POSTDIV2   | POSTDIV1  |
// +--------------------+---------------+--------+--------+------------+-----------+
// | FOUT3              | 25(DSK)       |   80   |   1    |    1       |    1      |
// | FOUT4              |               |   80   |   1    |    1       |    1      |
// +-------------------+----------------+--------+--------+------------+-----------+
/* FOUT3=(PASSCLK/PLLD*PLLM)/POSTDIV2/POSTDIV1/6
**FOUT4=(PASSCLK/PLLD*PLLM)/POSTDIV2/POSTDIV1/8
**在参数配置时需要注意参数的限定条件：1MHz<=PASSCLK<=2GHz
**						1MHz<=PASSCLK/PLLD<=40MHz
**						800MHz<=PASSCLK/PLLD*PLLM<=3.2GHz**/
void PASSPLL()
{
	volatile unsigned int temp;
	PASSPLLC_En=0x4;//将PASSPLLC输出时钟模式切换为bypass模式
	//至少等待输入时钟CLK四个时钟周期，来保证PASSCLK时钟切换到BYPASS模式；
	pll_wait(20);
	//将PASSPLLCMD中的PD写为1，关断PLL；
	PASSPLLCMD=0x1;
	//设置PASSPLL的输入控制信号

/** PASSPLLCTL0 Register.******************************************
   * | 31...24|     23     |  22...18 |  17...6     |  5...0 |		*
   * |Reserved|    BYPASS  | Reserved |    PLLM     | PLLD   |*****/
	PASSPLLCTL0=0x0001400;	//配置PLL的倍频分频系数

/** PASSPLLCTL1 Register.********************************************************
   * | 31...9 |   8     |      7     |       6        |  5...3    |   2...0    |*
   * |Reserved|  ENSAT  | FPHASPDCTL |  PASSPLLOUTCTL | POSTDIV2  |  POSTDIV1  |*/
	PASSPLLCTL1=0x0000109;	//配置PLL的倍频分频系数
	//至少等待1us后将PASSPLLCMD	中的PD写为0
	pll_wait(100);
	PASSPLLCMD=0x0;
	//查询PASSPLLCMD中的LOCK是否为1，判断时钟是否稳定
	temp=0x2	&	PASSPLLCMD ;
	while(!temp)
	{
		temp=0x2 &	PASSPLLCMD ;
	}
	/*将PASSPLLC_En中的PLLC_BYPASS写为0，将PASSPLL控制器时钟切换到PLL模式
	PASSPLLC_En中，第二位为PASSPLL_SysClk125M,PASSPLL_SysClk333M时钟切换控制位，1为bypass，0为125M或333M时钟；*/
	PASSPLLC_En=0x0;
}
