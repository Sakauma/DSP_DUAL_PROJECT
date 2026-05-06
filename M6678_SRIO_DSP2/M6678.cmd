/****************************************************************************/
/*                                                                          */
/*                               M6678.cmd                                  */
/*                         Copyright (c): NUDT                              */
/*                                                                          */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an M6678              */
/*                 device.  Use it as a guideline.  You will want to        */
/*                 change the memory layout to match your specific C6xxx    */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

-stack	0x5000
-heap	0x5000

MEMORY
{
        BOOT:       o=0x00800000   l=0x00000200//中断向量表
	    INITIAL:    o=0x00800200   l=0x00000200//初始化寄存器，搬移4KB以外的程序
//	    CONFIG:     o=0x00800400   l=0x00001000//关闭L1D cache（MSMC的ram默认可被L1 cache）；配置外存等，主要是ddr
	    DATA:       o=0x00800400   l=0x00000400//原始数据，作为程序数据搬移、测试的源
	    L2:         o=0x00800800   l=0x0007f800//主程序
	    MSMCSRAM:   o=0x0c000000   l=0x01000000  // MSMC 区域（1MB）
//	    MSMC0:	    o=0x0c000000   l=0x00100000
//	    MSMC1:	    o=0x0c100000   l=0x00100000
	    /*MSMC0:	o=0x80000000   l=0x00100000
	    MSMC1:	    o=0x80100000   l=0x00100000*/
//	    MSMC2:	    o=0x0c200000   l=0x00100000
//	    MSMC3:	    o=0x0c300000   l=0x00100000
//	    EMIFCE0:    o=0x70000400   l=0x00100000
//	    DDR:		o=0x80000000   l=0x01000000
}

SECTIONS
{
   		vecs        >   BOOT
   		initial     >   INITIAL
		config	    >   L2
		data        >   DATA
		.test       >   L2
		.fardata	>	L2
		.stack		>	L2
		.text		>	L2
		.cinit		>	L2
		.bss		>	L2
		.cio     	> 	L2
		.const   	> 	L2
		.far     	> 	L2
		.sysmem  	> 	L2
		.switch		>	L2
	   .neardata	>	L2
	   .msmc 		> 	MSMCSRAM
//	   .MSMC0		>	MSMC0
//	   .MSMC1		>	MSMC1
//	   .MSMC2		>	MSMC2
//	   .MSMC3		>	MSMC3
//	   .DDR3		>	DDR
}
