/*
 * SrioSwitch1848Cfg.c
 *
 *  Created on: 2022年11月14日
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdint.h>
#include "SrioSwitch1848Cfg.h"
#include "UART.h"
#include "PSC.h"

#define SRIOSWITCH_PORTSTATUSMASK		(uint32_t)0x00020202
#define SRIOSWITCH_PORTSTATUSOK			(uint32_t)0x00000002

//全局参数
static SrioSwitchCardInfo_st g_stModuleInfo = {0,0,0,0};		//私有全局参数-模块硬件配置信息

//拷贝参数
unsigned int TrainAfterValue[10][8]={0};
unsigned int TrainAfterErrRegister[10][6]={0};

static void SrioSwitch_Delay(unsigned int cnt)
{
	int i,j;
	for(i=0;i<cnt;i++)
	{
		for(j=0;j<2000;j++)
			asm(" nop ");
	}
}

/*******************************************************************************
 *      Function Name           :   SrioSwitch_Change_Speed
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   SRIO交换机端口速率配置
 *      Param                   :
 *      unsigned char u8Cps1848Index -- 交换机序号(CPS1848_1、CPS1848_2)
 *      unsigned char speed -- 交换机端口速率(Speed_1_25、Speed_2_5、Speed_3_125、Speed_5、Speed_6_25)
 *
 *      Return Code             :   0-Success/-1-Fault
 *      Global Variable         :
 *      g_stModuleInfo - 模块全局参数
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
void SrioSwitch_Change_Speed(unsigned char speed)
{
	if(speed==Speed_1_25)
	{
		SrioSwitch_WriteRegister(0x00000004,0x00ff0000);   //q0
		SrioSwitch_WriteRegister(0x00000004,0x00ff0040);
		SrioSwitch_WriteRegister(0x00000004,0x00ff0080);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8000);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8100);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8200);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8300);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9000);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9100);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9200);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9300);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa000);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa100);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa200);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa300);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0010);   //q1
		SrioSwitch_WriteRegister(0x00000004,0x00ff0050);
		SrioSwitch_WriteRegister(0x00000004,0x00ff0090);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8400);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8500);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8600);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8700);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9400);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9500);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9600);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9700);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa400);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa500);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa600);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa700);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0020);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff0060);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff00a0);   //q2
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8800);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8900);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8a00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8b00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9800);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9900);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9a00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9b00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa800);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffa900);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffaa00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffab00);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0030);   //q3
		SrioSwitch_WriteRegister(0x00000004,0x00ff0070);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff00b0);   //q2
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8c00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8d00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8e00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff8f00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9c00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9d00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9e00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ff9f00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffac00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffad00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffae00);
		SrioSwitch_WriteRegister(0x00001e80,0x00ffaf00);


		SrioSwitch_WriteRegister(0xbffc0fff,0x00f20300);
	}
	if(speed == Speed_2_5)
	{
		SrioSwitch_WriteRegister(0x00000004,0x00ff0000);   //q0
		SrioSwitch_WriteRegister(0x00000004,0x00ff0040);
		SrioSwitch_WriteRegister(0x00000004,0x00ff0080);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8000);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8100);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8200);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8300);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9000);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9100);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9200);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9300);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa000);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa100);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa200);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa300);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0010);   //q1
		SrioSwitch_WriteRegister(0x00000004,0x00ff0050);
		SrioSwitch_WriteRegister(0x00000004,0x00ff0090);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8400);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8500);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8600);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8700);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9400);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9500);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9600);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9700);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa400);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa500);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa600);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa700);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0020);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff0060);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff00a0);   //q2
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8800);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8900);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8a00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8b00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9800);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9900);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9a00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9b00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa800);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa900);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffaa00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffab00);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0030);   //q3
		SrioSwitch_WriteRegister(0x00000004,0x00ff0070);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff00b0);   //q2
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8c00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8d00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8e00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8f00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9c00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9d00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9e00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9f00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffac00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffad00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffae00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffaf00);


		SrioSwitch_WriteRegister(0xbffc0fff,0x00f20300);

	}

	if(speed==Speed_3_125)
	{
		SrioSwitch_WriteRegister(0x00000005,0x00ff0000);   //q0
		SrioSwitch_WriteRegister(0x00000005,0x00ff0040);
		SrioSwitch_WriteRegister(0x00000005,0x00ff0080);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8000);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8100);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8200);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8300);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9000);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9100);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9200);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9300);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa000);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa100);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa200);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa300);

		SrioSwitch_WriteRegister(0x00000005,0x00ff0010);   //q1
		SrioSwitch_WriteRegister(0x00000005,0x00ff0050);
		SrioSwitch_WriteRegister(0x00000005,0x00ff0090);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8400);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8500);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8600);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8700);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9400);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9500);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9600);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9700);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa400);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa500);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa600);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa700);

		SrioSwitch_WriteRegister(0x00000005,0x00ff0020);   //q2
		SrioSwitch_WriteRegister(0x00000005,0x00ff0060);   //q2
		SrioSwitch_WriteRegister(0x00000005,0x00ff00a0);   //q2
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8800);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8900);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8a00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8b00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9800);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9900);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9a00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9b00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa800);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffa900);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffaa00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffab00);

		SrioSwitch_WriteRegister(0x00000005,0x00ff0030);   //q3
		SrioSwitch_WriteRegister(0x00000005,0x00ff0070);   //q2
		SrioSwitch_WriteRegister(0x00000005,0x00ff00b0);   //q2
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8c00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8d00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8e00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff8f00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9c00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9d00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9e00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ff9f00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffac00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffad00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffae00);
		SrioSwitch_WriteRegister(0x00001e8a,0x00ffaf00);


		SrioSwitch_WriteRegister(0xbffc0fff,0x00f20300);
	}
	if(speed==Speed_5)
	{
		SrioSwitch_WriteRegister(0x00000004,0x00ff0000);   //q0
		SrioSwitch_WriteRegister(0x00000004,0x00ff0040);
		SrioSwitch_WriteRegister(0x00000004,0x00ff0080);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8000);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8100);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8200);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8300);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9000);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9100);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9200);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9300);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa000);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa100);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa200);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa300);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0010);   //q1
		SrioSwitch_WriteRegister(0x00000004,0x00ff0050);
		SrioSwitch_WriteRegister(0x00000004,0x00ff0090);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8400);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8500);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8600);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8700);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9400);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9500);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9600);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9700);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa400);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa500);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa600);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa700);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0020);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff0060);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff00a0);   //q2
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8800);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8900);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8a00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8b00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9800);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9900);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9a00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9b00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa800);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffa900);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffaa00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffab00);

		SrioSwitch_WriteRegister(0x00000004,0x00ff0030);   //q3
		SrioSwitch_WriteRegister(0x00000004,0x00ff0070);   //q2
		SrioSwitch_WriteRegister(0x00000004,0x00ff00b0);   //q2
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8c00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8d00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8e00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff8f00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9c00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9d00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9e00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ff9f00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffac00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffad00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffae00);
		SrioSwitch_WriteRegister(0x00001e94,0x00ffaf00);

		SrioSwitch_WriteRegister(0xbffc0fff,0x00f20300);
	}
}

/*******************************************************************************
 *      Function Name           :   SrioSwitch_InterfaceInit
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   DSP与SRIO交换机1848配置接口(I2C Slave)通信接口的初始化，读取交换机ID判断正确性，此接口需由用户实现；
 *      Param                   :
 *
 *      Return Code             :   0-Success/-1-Fault
 *      Global Variable         :
 *      g_stModuleInfo - 模块全局参数
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
int SrioSwitch_InterfaceInit(unsigned char u8SwitchI2CAddr, unsigned int unBaudRate)
{
	uint32_t u32RegVaule = 0x00;

	//硬件参数赋值
	g_stModuleInfo.u8SwitchI2CAddr = u8SwitchI2CAddr;
	g_stModuleInfo.u32BaudRate = unBaudRate;
	g_stModuleInfo.u8InfoInif = 1;

	//串口端口打开
	PSC_Open_Clk("UART");
	UART_Init(unBaudRate);
	g_stModuleInfo.u8UartisOpen = 1;	//端口打开标志置位

	//读取交换机ID数值---Read/Write接口用户需按照实际硬件接口进行
	SrioSwitch_ReadRegister(&u32RegVaule, 0x000000);
	printf("SrioSwitch1848-ID:%08X \n",u32RegVaule);

	return 1;
}

/*******************************************************************************
 *      Function Name           :   SrioSwitch_ReadRegister
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   SRIO交换机寄存器读接口--此示例DSP通过UART与FPGA进行通信，FPGA解析串口数据后转换为I2C协议进行交换机的配置，该接口与板卡有关联需要用户调整
 *      Param                   :
 *      unsigned char u8Cps1848Index -- 交换机序号(CPS1848_1、CPS1848_2)
 *      unsigned int *pu32RegVaule -- 读取寄存器的数值缓存区指针
 *      unsigned int u32RegAddr -- 寄存器地址
 *
 *      Return Code             :   0-Success/-1-Fault
 *      Global Variable         :
 *      g_stModuleInfo - 模块全局参数
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
int SrioSwitch_ReadRegister(unsigned int *pu32RegVaule, unsigned int u32RegAddr)
{
	unsigned char auchSendBuff[32] = {0x00};
	unsigned char auchRecvBuff[32] = {0x00};
	unsigned char u8SnedBuffIndex = 0x00;
	unsigned char *pu8RegValuePoint = (unsigned char*)pu32RegVaule;

	if(!g_stModuleInfo.u8UartisOpen)
		return -1;

	//构建数据帧
	auchSendBuff[u8SnedBuffIndex++] = 0xEB;
	auchSendBuff[u8SnedBuffIndex++] = 0x90;

	//Byte2-Bit0~Bit3 -- 传输方向
	//Byte2-Bit4~Bit7 -- 交换机序号
	auchSendBuff[u8SnedBuffIndex++] = (((0x01)<<4) | 0x01);

	//Device Address
	auchSendBuff[u8SnedBuffIndex++] = g_stModuleInfo.u8SwitchI2CAddr;

	//Register Address
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)((u32RegAddr & 0x00FF0000) >> 16);
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)((u32RegAddr & 0x0000FF00) >> 8);
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)u32RegAddr;

	//Resigter Data
	auchSendBuff[u8SnedBuffIndex++] = 0x00;
	auchSendBuff[u8SnedBuffIndex++] = 0x00;
	auchSendBuff[u8SnedBuffIndex++] = 0x00;
	auchSendBuff[u8SnedBuffIndex++] = 0x00;

	//FrameLast
	auchSendBuff[u8SnedBuffIndex++] = 0xD5;
	auchSendBuff[u8SnedBuffIndex++] = 0xC8;

	UART_Send((char*)auchSendBuff, u8SnedBuffIndex);	//uart数据发送

	//等待接收
	UART_Receive((char*)auchRecvBuff,8);
	pu8RegValuePoint[0] = auchRecvBuff[5];
	pu8RegValuePoint[1] = auchRecvBuff[4];
	pu8RegValuePoint[2] = auchRecvBuff[3];
	pu8RegValuePoint[3] = auchRecvBuff[2];

	return 1;
}

/*******************************************************************************
 *      Function Name           :   SrioSwitch_WriteRegister
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   SRIO交换机寄存器写接口--此示例DSP通过UART与FPGA进行通信，FPGA解析串口数据后转换为I2C协议进行交换机的配置，该接口与板卡有关联需要用户调整
 *      Param                   :
 *      unsigned char u8Cps1848Index -- 交换机序号(CPS1848_1、CPS1848_2)
 *      unsigned int u32RegVaule -- 待写入寄存器的数值
 *      unsigned int u32RegAddr -- 寄存器地址
 *
 *      Return Code             :   0-Success/-1-Fault
 *      Global Variable         :
 *      g_stModuleInfo - 模块全局参数
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
int SrioSwitch_WriteRegister(unsigned int u32RegVaule, unsigned int u32RegAddr)
{
	unsigned char auchSendBuff[32] = {0x00};
	unsigned char auchRecvBuff[32] = {0x00};
	unsigned char u8SnedBuffIndex = 0x00;

	if(!g_stModuleInfo.u8UartisOpen)
		return -1;

	//构建数据帧
	auchSendBuff[u8SnedBuffIndex++] = 0xEB;
	auchSendBuff[u8SnedBuffIndex++] = 0x90;

	//Byte2-Bit0~Bit3 -- 传输方向
	//Byte2-Bit4~Bit7 -- 交换机序号
	auchSendBuff[u8SnedBuffIndex++] = (((0x01)<<4) | 0x00);

	//Device Address
	auchSendBuff[u8SnedBuffIndex++] = g_stModuleInfo.u8SwitchI2CAddr;

	//Register Address
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)((u32RegAddr & 0x00FF0000) >> 16);
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)((u32RegAddr & 0x0000FF00) >> 8);
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)u32RegAddr;

	//Resigter Data
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)((u32RegVaule & 0xFF000000) >> 24);
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)((u32RegVaule & 0xFF0000) >> 16);
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)((u32RegVaule & 0xFF00) >> 8);
	auchSendBuff[u8SnedBuffIndex++] = (uint8_t)u32RegVaule;

	//FrameLast
	auchSendBuff[u8SnedBuffIndex++] = 0xD5;
	auchSendBuff[u8SnedBuffIndex++] = 0xC8;

	UART_Send((char*)auchSendBuff,u8SnedBuffIndex);	//uart数据发送

	//等待接收
	UART_Receive((char*)auchRecvBuff,8);

	return 1;
}

int SrioSwitch_ShowRegister(unsigned int u32RegAddr)
{
	uint32_t u32RegValue = 0x00;
	SrioSwitch_ReadRegister(&u32RegValue,u32RegAddr);
	printf("Addr:%06X Val:%08X\n",u32RegAddr,u32RegValue);
	return 1;
}

int SrioSwitch_PortStatus(unsigned int unPort)
{
	uint32_t u32RegValue = 0x00;
	SrioSwitch_ReadRegister(&u32RegValue,0x158+unPort*0x20);

	return u32RegValue;
}

int SrioSwitch_NMS1800SerdesInit()
{
	int i = 0;

	for(i = 0; i<12; i++)
	{
		//Reg:PORT_0_ERR_STAT_CSR -- Addr:0x000158 + (0x20 * port_num)
		SrioSwitch_WriteRegister(0x88000000, 0x158+i*0x20);
		SrioSwitch_WriteRegister(0x1e1557a8, 0xFF1000+i*0x10);
		SrioSwitch_WriteRegister(0x6a1b6400, 0xFF1008+i*0x10);
	}

	for(i = 0; i<48; i++)
	{
		//Lane {0..47} DFE 1 Register -- Addr:0xFF8028 + (0x100 * lane_num)
		SrioSwitch_WriteRegister(0x00007800, 0xFF8028+i*0x100);
		//Lane {0..47} DFE 2 Register -- 0xFF802C + (0x100 * lane_num)
		SrioSwitch_WriteRegister(0x0004071f, 0xFF802c+i*0x100);
		SrioSwitch_WriteRegister(0x9c422220, 0xFF8034+i*0x100);
		SrioSwitch_WriteRegister(0x45400090, 0xFF8030+i*0x100);
	}
	for(i = 0; i<12; i++)
	{
		// -- Addr:0x00015c + (0x20 * port_num)
		SrioSwitch_WriteRegister(0xd0600000, 0x15c+i*0x20);
	}

	//Reg:PORT_LINK_TO_CTL_CSR--TIMEOUT--Addr:0x120
	SrioSwitch_WriteRegister(0x00008E00,0x120);

	//Device Reset and Control Register -- Addr:0xF20300
	SrioSwitch_WriteRegister(0xbfffffff, 0xF20300);

	return 1;
}

int SrioSwitch_CETC54_1848Init()
{
	int i = 0;

	for(i = 0; i<12; i++)
	{
		SrioSwitch_WriteRegister(0xD0E00001,0x15c+0x20*i);	//端口关闭
	}

	//板块复位
	SrioSwitch_WriteRegister(0xFFFFFFFF,0xF20300);	//芯片复位

	return 1;
}

unsigned int I2C_Read_LanePRBSErrorCounterRegister(unsigned int port,unsigned int (*TrainAfterErrRegister)[6],unsigned int NUM)
{
	unsigned int value=0;

	SrioSwitch_ReadRegister(&value, 0x00ff8008+(port*4)*0x100);  //读出 Lane  PRBS Error Counter Register
    printf("Port%d Lane 0 PRBS Error Counter:%03X\n",port,value);
    TrainAfterErrRegister[NUM][0]=value;

    SrioSwitch_ReadRegister(&value, 0x00ff8008+(port*4)*0x100+0x100); //读出 Lane  PRBS Error Counter Register
    printf("Port%d Lane 1 PRBS Error Counter:%03X\n",port,value);
    TrainAfterErrRegister[NUM][1]=value;

    SrioSwitch_ReadRegister(&value, 0x00ff8008+(port*4)*0x100+0x200); //读出 Lane  PRBS Error Counter Register
    printf("Port%d Lane 2 PRBS Error Counter:%03X\n",port,value);
    TrainAfterErrRegister[NUM][2]=value;

    SrioSwitch_ReadRegister(&value, 0x00ff8008+(port*4)*0x100+0x300); //读出 Lane  PRBS Error Counter Register
    printf("Port%d Lane 3 PRBS Error Counter:%03X\n",port,value);
    TrainAfterErrRegister[NUM][3]=value;
/*
	SrioSwitch_ReadRegister(&value,0x00ff800C+(port*4)*0x100);        //读出 Lane  Error DETECT Register
    TrainAfterErrRegister[NUM][1]=value;

	SrioSwitch_ReadRegister(&value,0x00ff8010+(port*4)*0x100);        //读出 Lane  Error Rate En Register
    TrainAfterErrRegister[NUM][2]=value;

	SrioSwitch_ReadRegister(&value,0x00ff8014+(port*4)*0x100);        //读出 Lane   Attributes Capture Register
    TrainAfterErrRegister[NUM][3]=value;

	SrioSwitch_ReadRegister(&value,0x00ff8018+(port*4)*0x100);        //读出 Lane    Data Capture 0 Register
    TrainAfterErrRegister[NUM][4]=value;

	SrioSwitch_ReadRegister(&value,0x00ff801c+(port*4)*0x100);        //读出 Lane    Data Capture 1 Register
    TrainAfterErrRegister[NUM][5]=value;
*/
    return 1;
}

int SrioSwitch_PRBSModeSet(unsigned int unPort)
{
	unsigned int i,j;
	unsigned int unRegLaneCtlVaule=0;

	for(j=0;j<10;j++)
		for(i=0;i<8;i++)
			TrainAfterValue[j][i]=0;

	//打印LANE参数
	for(i = 0; i<4; i++)
		SrioSwitch_ShowLaneStatus(unPort*4+i);

	SrioSwitch_ReadRegister(&unRegLaneCtlVaule, 0x00ff8000+(unPort*4)*0x100);

	//unRegLaneCtlVaule=0x10800680;
	unRegLaneCtlVaule |= 0x0A000000;  //Bit3~Bit6:PRBS_MODE（0b0101-x7, (只有在x7上才能跑通)
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x200);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x300);

	unRegLaneCtlVaule |= 0x00800000;  //Bit8-PRBS_UNIDIR_BERT_MODE_EN（0x00800000）为10位模式
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x200);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x300);

	unRegLaneCtlVaule |= 0x00100000;  //设置PRBS_TRAIN=1; | 0x00100000;
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x200);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x300);

	//Read_LCR=0x10840680 ;
	unRegLaneCtlVaule |= 0x00040000;     //设置XMIPRBS为1在0x00040000位;
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x200);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x300);
	SrioSwitch_Delay(1000);

	//unRegLaneCtlVaule=0x10860680 & 0xfffdffff;
	unRegLaneCtlVaule |= 0x00020000;    //设置PRBS_RX_CHECRER_MODE为1,0x00020000
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x200);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x300);
	SrioSwitch_Delay(1000);

	//unRegLaneCtlVaule=0x108e0680  ;
	unRegLaneCtlVaule |= 0x00080000;    //设置PRBS_EN=1  0x00080000
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x200);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule, 0x00FF8000+(unPort*4)*0x100+0x300);
/*
	unRegLaneCtlVaule |= 0x00008000;    //设置LPBK_10BIT_EN  0x00008000
	SrioSwitch_WriteRegister(unRegLaneCtlVaule,0x00FF8000+(unPort*4)*0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule,0x00FF8000+(unPort*4)*0x100+0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule,0x00FF8000+(unPort*4)*0x100+0x200);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule,0x00FF8000+(unPort*4)*0x100+0x300);
*/

	SrioSwitch_Delay(10000);
	I2C_Read_LanePRBSErrorCounterRegister(8,TrainAfterErrRegister,0);
	//Read Lane {0..47} Status 1 CSR Bit4 LP_TRAINED


	unRegLaneCtlVaule &= ~0x00100000;    //设置PRBS_TRAIN=0,因为之前设置PRBS_TRAIN=1所以现在减0x00100000
	SrioSwitch_WriteRegister(unRegLaneCtlVaule,0x00FF8000+(unPort*4)*0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule,0x00FF8000+(unPort*4)*0x100+0x100);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule,0x00FF8000+(unPort*4)*0x100+0x200);
	SrioSwitch_WriteRegister(unRegLaneCtlVaule,0x00FF8000+(unPort*4)*0x100+0x300);

	SrioSwitch_Delay(100000);

	//打印LANE参数
	for(i = 0; i<4; i++)
		SrioSwitch_ShowLaneStatus(unPort*4+i);

	return 1;
}


void SrioSwitch_GetPRBSErrorCnt(unsigned int unPort)
{
	unsigned i = 0;

	for(i=0;i<4;i++)
	{
		I2C_Read_LanePRBSErrorCounterRegister(unPort,TrainAfterErrRegister,i+1);   //读书各个Lane的误码率测试错误的个数
		SrioSwitch_Delay(10000);
	}
}

void SrioSwitch_ShowLaneStatus(unsigned char uchLane)
{
	unsigned int unRegAddress = 0x00;
	unsigned int unRegVaule = 0x00;

	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	//Lane {0..47} Status 0 CSR -- 0x002010 + (0x20 * lane_num)
	unRegAddress = 0x002010 + (0x20 * uchLane);
	SrioSwitch_ReadRegister(&unRegVaule,unRegAddress);
	printf("SrioSwitch-Lane%d:Status0:%08X\n",uchLane, unRegVaule);
	//Lane {0..47} Status 1 CSR -- 0x002014 + (0x20 * lane_num)
	unRegAddress = 0x002014 + (0x20 * uchLane);
	SrioSwitch_ReadRegister(&unRegVaule,unRegAddress);
	printf("SrioSwitch-Lane%d:Status1:%08X\n",uchLane, unRegVaule);
	//Lane {0..47} Status 2 CSR -- 0x002018 + (0x20 * lane_num)
	unRegAddress = 0x002018 + (0x20 * uchLane);
	SrioSwitch_ReadRegister(&unRegVaule,unRegAddress);
	printf("SrioSwitch-Lane%d:Status2:%08X\n",uchLane, unRegVaule);
	//Lane {0..47} Status 3 CSR -- 0x002018 + (0x20 * lane_num)
	unRegAddress = 0x00201C + (0x20 * uchLane);
	SrioSwitch_ReadRegister(&unRegVaule,unRegAddress);
	printf("SrioSwitch-Lane%d:Status3:%08X\n",uchLane, unRegVaule);
	//Lane {0..47} Status 4 CSR -- 0x002018 + (0x20 * lane_num)
	unRegAddress = 0x002020 + (0x20 * uchLane);
	SrioSwitch_ReadRegister(&unRegVaule,unRegAddress);
	printf("SrioSwitch-Lane%d:Status4:%08X\n",uchLane, unRegVaule);
}

void SrioSwitch_ShowPortStatus(unsigned char uchPort)
{
	unsigned int unRegAddress = 0x00;
	unsigned int unRegVaule = 0x00;

	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	//PORT_{0..17}_ERR_STAT_CSR
	unRegAddress = 0x000158 + (0x20 * uchPort);
	SrioSwitch_ReadRegister(&unRegVaule,unRegAddress);
	printf("SrioSwitch-Port%d:ERR_STAT_CSR:%08X\n",uchPort, unRegVaule);
}


void SrioSwitch_SerdesSet_AMP(unsigned char u8Lane, unsigned char u8AmpCtl)
{
	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	unsigned int u32RegVaule = 0x00;
	unsigned int u32RegAddress = 0x00;
	//1.Lane{0..47}Status 3 CSR - Bit_2 AMP_PROG_EN
	u32RegAddress = 0x00201C + 0x20*u8Lane;
	SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
	u32RegVaule |= 0x20000000;
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);

	//2.Lane {0..47} Control Register - Bit
	u32RegAddress = 0xFF8000 + 0x100*u8Lane;
	SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
	u8AmpCtl &= 0x3F;	//Bit0~Bit5
	u32RegVaule &= ~(0x000007E0);
	u32RegVaule |= ((uint32_t)u8AmpCtl << 5);
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
}

void SrioSwitch_SerdesSet_NEG(unsigned char u8Lane, unsigned char u8NegCtl)
{
	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	unsigned int u32RegVaule = 0x00;
	unsigned int u32RegAddress = 0x00;
	//1.Lane{0..47}Status 3 CSR - Bit21~Bit25 NEG1_TAP
	u32RegAddress = 0x00201C + 0x20*u8Lane;
	SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
	u8NegCtl &= 0x1F;	//Bit0~Bit4
	u32RegVaule &= ~(0x000007C0);
	u32RegVaule |= ((uint32_t)u8NegCtl<<6);
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
}

void SrioSwitch_SerdesSet_Pos(unsigned char u8Lane, unsigned char u8PosCtl)
{
	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	unsigned int u32RegVaule = 0x00;
	unsigned int u32RegAddress = 0x00;
	//1.Lane{0..47}Status 3 CSR - Bit26~Bit31 POS1_TAP
	u32RegAddress = 0x00201C + 0x20*u8Lane;
	SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
	u8PosCtl &= 0x3F;	//Bit0~Bit5
	u32RegVaule &= ~(0x0000003F);
	u32RegVaule |= ((uint32_t)u8PosCtl);
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
}

/*******************************************************************************
 *      Function Name           :   SrioSwitch_Reset
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   SRIO交换机复位
 *      Param                   :
 *      unsigned char u8Cps1848Index -- 交换机序号(CPS1848_1、CPS1848_2)
 *
 *      Return Code             :   0-Success/-1-Fault
 *      Global Variable         :
 *      g_stModuleInfo - 模块全局参数
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
void SrioSwitch_Reset(unsigned char u8Cps1848Index)
{
	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	unsigned int u32RegVaule = 0x00;
	unsigned int u32RegAddress = 0x00;
	//DEVICE_SOFT_RESET
	u32RegAddress = 0xF20040;
	u32RegVaule = 0x00030097;
	//u32RegVaule = 0x97000300;
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
	SrioSwitch_Delay(10000);
}

/*******************************************************************************
 *      Function Name           :   SrioSwitch_PortReset
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   SRIO交换机端口复位
 *      Param                   :
 *      unsigned char u8Cps1848Index -- 交换机序号(CPS1848_1、CPS1848_2)
 *      unsigned int unPort -- 交换机端口序号
 *
 *      Return Code             :   0-Success/-1-Fault
 *      Global Variable         :
 *      g_stModuleInfo - 模块全局参数
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
void SrioSwitch_PortReset(unsigned int unPort)
{
	if(!g_stModuleInfo.u8UartisOpen)
			return ;
	if(unPort > 17)
		return;

	unsigned int u32RegVaule = 0x00;
	unsigned int u32RegAddress = 0x00;

	//Device Reset and Control Register
	u32RegAddress = 0xF20300;
#if(!SRIOSWITCH_CETC54_1848)
	u32RegVaule = 0x80000000 | (0x01 << unPort);
#else
	u32RegVaule = 0x80000000 | (0x40001 << unPort); //Bit0-DO_RESET、Bit2~Bit13-PLL_SEL、Bit14~Bit31-PORT_SEL;
#endif
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
	SrioSwitch_Delay(1000);
}

void SrioSwitch_PortWidthOvrd(unsigned int unPort, unsigned char uchWidth)
{
	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	unsigned int u32RegVaule = 0x00;
	unsigned int u32RegAddress = 0x00;

	//Port{0..17} Control 1 CSR Bit5~Bit7
	u32RegAddress = 0x15C + (0x20*unPort);
	SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
	u32RegVaule &= ~(0x07000000);	//Clear Bit5~Bit7
	switch(uchWidth)
	{
	case 1:
		u32RegVaule |= 0x02000000;
		SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
		break;
	case 2:
		u32RegVaule |= 0x05000000;
		SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
		break;
	case 4:
		u32RegVaule |= 0x07000000;
		SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
		break;
	default:
		break;
	}
	SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
}


void SrioSwitch_PortForward(unsigned int unPort)
{
	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	unsigned int u32RegVaule = 0x00;
	unsigned int u32RegAddress = 0x00;
	//DEVICE_CTL_1
	u32RegAddress =0x00015C + 0x20*unPort;
	SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
	u32RegVaule |= 0x00200000;	//Bit10-INPUT_PORT_EN - 1
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);
	SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
}

void SrioSwitch_RoutTableSet(unsigned char u8SrcPort,unsigned char u8DestID,unsigned char u8DesPort)
{
	if(!g_stModuleInfo.u8UartisOpen)
			return ;

	unsigned int u32RegVaule = 0x00;
	unsigned int u32RegAddress = 0x00;

	//间接配置
	//RTE_PORT_SEL
	u32RegAddress = 0x010070;
	u32RegVaule = u8SrcPort + 0x1;
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);

	//RTE_DESTID_CSR
	u32RegAddress = 0x70;
	u32RegVaule = (uint32_t)u8DestID;
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);

	//RTE_PORT_CSR
	u32RegAddress = 0x74;
	u32RegVaule = (uint32_t)u8DesPort;
	SrioSwitch_WriteRegister(u32RegVaule, u32RegAddress);

	/*
	//直接配置寄存器地址
	u32RegAddress = 0xE10000 + (0x1000 * u8SrcPort) + (0x4 * u8DestID);
	u32RegVaule = u8DesPort;
	SrioSwitch_WriteRegister(u32RegVaule,u32RegAddress);
	 */

	//ReadRoutVaule
	//SrioSwitch_ReadRegister(&u32RegVaule, u32RegAddress);
	//printf("SrioSwitchRoutCfg:Port%d DestID:0x%X Vaule:%08X\n",u8SrcPort,u8DestID,u32RegVaule);
}

