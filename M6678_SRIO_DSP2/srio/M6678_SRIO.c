/*
 * M6678_SRIO.c
 *
 *  Created on: 2017-1-17
 */

#include "M6678_SRIO.h"
#include "SrioPhyReg.h"
#include "stdio.h"
#include "UART.h"
volatile unsigned int SrioVersion = 2;
unsigned int aaa=0;

unsigned int rignt_times=0;
unsigned int u32RstCnt_Srio0 = 0,u32RstCnt_Srio1 = 0;
void DelaySRIO(unsigned int cnt){
	int i,j;
	for(i=0;i<cnt;i++)
	{
		for(j=0;j<2000;j++)
			asm(" nop ");
	}
}

//按字节进行大小端转换
unsigned int SrioChangeEnding(unsigned int Input){
	return (
			 ((Input&0xff)<<24)
			 |((Input&0xff00)<<8)
			 |((Input&0xff0000)>>8)
			 |((Input&0xff000000)>>24)
			 );
}

/*
 * 指定一个srio部件建立一个apio窗口
 * Instance:SrioInstance0,SrioInstance1
 * SRIOAPIOStruct:初始化参数
 */
unsigned int SrioCreatAPIOWindow(unsigned int Instance,struct SRIOAPIOStruct * SRIOAPIOStruct){
	unsigned int volatile * CtrlPoint;

	if(SRIOAPIOStruct->WindowNub>15)
		return 1;
	//计算需要操作的APIO映射寄存器组基址
	if(Instance){
		CtrlPoint = (unsigned int volatile *)((unsigned int)&SRIO1_RAB_APIO_AMAP_0_CTRL+ (SRIOAPIOStruct->WindowNub & 0xf)*0x10);
	}else{
		CtrlPoint = (unsigned int volatile *)((unsigned int)&RAB_APIO_AMAP_0_CTRL+ (SRIOAPIOStruct->WindowNub & 0xf)*0x10);
	}

	//初始化某组APIO映射CTRL寄存器
	CtrlPoint[0] = 	((SRIOAPIOStruct->DstID & 0xffff)<<16)
					| ((SRIOAPIOStruct->CRF&0x1)<<7)
					| (((SRIOAPIOStruct->RIOBase>>32) & 0x3)<<5)
					| ((SRIOAPIOStruct->Priority&0x3)<<3)
					| ((SRIOAPIOStruct->WindowTYPE & 0x3)<<1)
					| 0x1;
	//初始化某组APIO映射SIZE寄存器
	CtrlPoint[1] = SRIOAPIOStruct->WindowSize;
	//初始化某组APIO映射ABAR寄存器
	CtrlPoint[2] = SRIOAPIOStruct->AXIBase>>10;
	//初始化某组APIO映射RBAR寄存器
	if(SRIOAPIOStruct->WindowTYPE)
		CtrlPoint[3] = (SRIOAPIOStruct->RIOBase & 0xffffffff)>>10;
	else
		CtrlPoint[3] = (SRIOAPIOStruct->hopcount << 14) | (SRIOAPIOStruct->RIOBase & 0xffffff)>>10;

	return 0;
}

void SrioRPIOMap(unsigned int Instance){
	/*
	配置srio0 srio1 RPIO，使用SRIO数据包中地址[31:28]来选择查找表。
	配置查找表，使SRIO的包地址与DSP内部地址一一映射，注意超过32位的地址将重复映射值低32位空间
	LUT(look up table) register
	bit 0:使能
	bit [4:1]:窗口大小
	Encoding 	Size 	addr，注意addr共38位
	0000 		1M 		{LUTN[31:14], RIO_Address[19:0]}
	0001 		2M 		{LUTN[31:15], RIO_Address[20:0]}
	0010 		4M 		{LUTN[31:16], RIO_Address[21:0]}
	0011 		8M 		{LUTN[31:17], RIO_Address[22:0]}
	0100 		16M 	{LUTN[31:18], RIO_Address[23:0]}
	0101 		32M 	{LUTN[31:19], RIO_Address[24:0]}
	0110 		64M 	{LUTN[31:20], RIO_Address[25:0]}
	0111 		128M 	{LUTN[31:21], RIO_Address[26:0]}
	1000 		256M 	{LUTN[31:22], RIO_Address[27:0]}
	1001 		512M 	{LUTN[31:23], RIO_Address[28:0]}
	1010 		1G 		{LUTN[31:24], RIO_Address[29:0]}
	1011 		2G 		{LUTN[31:25], RIO_Address[30:0]}
	最终使用addr[31:0]来映射至DSP内部地址
	 */
	if(Instance){
		SRIO1_RAB_RIO0_AMAP_LUT0  = 0x00000011;//window size 256M,address:0x00000000-0x0fffffff
		SRIO1_RAB_RIO0_AMAP_LUT1  = 0x00400011;//window size 256M,address:0x10000000-0x1fffffff
		SRIO1_RAB_RIO0_AMAP_LUT2  = 0x00800011;//window size 256M,address:0x20000000-0x2fffffff
		SRIO1_RAB_RIO0_AMAP_LUT3  = 0x00C00011;//window size 256M,address:0x30000000-0x3fffffff
		SRIO1_RAB_RIO0_AMAP_LUT4  = 0x01000011;//window size 256M,address:0x40000000-0x4fffffff
		SRIO1_RAB_RIO0_AMAP_LUT5  = 0x01400011;//window size 256M,address:0x50000000-0x5fffffff
		SRIO1_RAB_RIO0_AMAP_LUT6  = 0x01800011;//window size 256M,address:0x60000000-0x6fffffff
		SRIO1_RAB_RIO0_AMAP_LUT7  = 0x01C00011;//window size 256M,address:0x70000000-0x7fffffff
		SRIO1_RAB_RIO0_AMAP_LUT8  = 0x02000011;//window size 256M,address:0x80000000-0x8fffffff
		SRIO1_RAB_RIO0_AMAP_LUT9  = 0x02400011;//window size 256M,address:0x90000000-0x9fffffff
		SRIO1_RAB_RIO0_AMAP_LUT10 = 0x02800011;//window size 256M,address:0xA0000000-0xAfffffff
		SRIO1_RAB_RIO0_AMAP_LUT11 = 0x02C00011;//window size 256M,address:0xB0000000-0xBfffffff
		SRIO1_RAB_RIO0_AMAP_LUT12 = 0x03000011;//window size 256M,address:0xC0000000-0xCfffffff
		SRIO1_RAB_RIO0_AMAP_LUT13 = 0x03400011;//window size 256M,address:0xD0000000-0xDfffffff
		SRIO1_RAB_RIO0_AMAP_LUT14 = 0x03800011;//window size 256M,address:0xE0000000-0xEfffffff
		SRIO1_RAB_RIO0_AMAP_LUT15 = 0x03C00011;//window size 256M,address:0xF0000000-0xFfffffff
		SRIO1_RAB_RIO_AMAP_IDSL = 0x4;//source address bit[31:28] to select table
		SRIO1_RAB_RIO_AMAP_BYPS = 0x1;//1:source add,0:source id
	}else{
		RAB_RIO0_AMAP_LUT0  = 0x00000011;//window size 256M,address:0x00000000-0x0fffffff
		RAB_RIO0_AMAP_LUT1  = 0x00400011;//window size 256M,address:0x10000000-0x1fffffff
		RAB_RIO0_AMAP_LUT2  = 0x00800011;//window size 256M,address:0x20000000-0x2fffffff
		RAB_RIO0_AMAP_LUT3  = 0x00C00011;//window size 256M,address:0x30000000-0x3fffffff
		RAB_RIO0_AMAP_LUT4  = 0x01000011;//window size 256M,address:0x40000000-0x4fffffff
		RAB_RIO0_AMAP_LUT5  = 0x01400011;//window size 256M,address:0x50000000-0x5fffffff
		RAB_RIO0_AMAP_LUT6  = 0x01800011;//window size 256M,address:0x60000000-0x6fffffff
		RAB_RIO0_AMAP_LUT7  = 0x01C00011;//window size 256M,address:0x70000000-0x7fffffff
		RAB_RIO0_AMAP_LUT8  = 0x02000011;//window size 256M,address:0x80000000-0x8fffffff
		RAB_RIO0_AMAP_LUT9  = 0x02400011;//window size 256M,address:0x90000000-0x9fffffff
		RAB_RIO0_AMAP_LUT10 = 0x02800011;//window size 256M,address:0xA0000000-0xAfffffff
		RAB_RIO0_AMAP_LUT11 = 0x02C00011;//window size 256M,address:0xB0000000-0xBfffffff
		RAB_RIO0_AMAP_LUT12 = 0x03000011;//window size 256M,address:0xC0000000-0xCfffffff
		RAB_RIO0_AMAP_LUT13 = 0x03400011;//window size 256M,address:0xD0000000-0xDfffffff
		RAB_RIO0_AMAP_LUT14 = 0x03800011;//window size 256M,address:0xE0000000-0xEfffffff
		RAB_RIO0_AMAP_LUT15 = 0x03C00011;//window size 256M,address:0xF0000000-0xFfffffff
		RAB_RIO_AMAP_IDSL = 0x4;//choose source address bit[31:28] to select table
		RAB_RIO_AMAP_BYPS = 0x1;//1:source add,0:source id
	}
}
//打开SRIO PSC控制，使指定的srio部件获得电源及时钟
unsigned int SrioClkOpen(unsigned int SrioInstance){
	if(SrioInstance){
		PSC_MDCTL13 |= 0x00000003;
		PTCMD = 0x1;
		while( (PSC_MDSTAT13 & 0x00001c03)!=0x00001c03 );
	}else{
		PSC_MDCTL11 |= 0x00000003;
		PTCMD = 0x1;
		while( (PSC_MDSTAT11 & 0x00001c03)!=0x00001c03 );
	}
	return 0;
}

//关闭SRIO PSC控制，使指定的srio部件电源及时钟关闭
unsigned int SrioClkClose(unsigned int SrioInstance){
	if(SrioInstance){
		PSC_MDCTL13 = 0x00000000;
		PTCMD = 0x1;
		while( (PSC_MDSTAT13 & 0x00000800)!=0x00000800 );
	}else{
		PSC_MDCTL11 = 0x00000000;
		PTCMD = 0x1;
		while( (PSC_MDSTAT11 & 0x00000800)!=0x00000800 );
	}
	return 0;
}

//初始化指定srio phy部件，使用指定参数与对端进行链接
//SrioInstance:SrioInstance0 or SrioInstance1，指定srio0或者srio1
//Speed:speed,Speed1250 Speed2500 Speed3125 Speed5000 Speed6250，运行速率
//work mode:lane1xl0 lane1xl1 lane1xl2 lane1xl3 lane4x lane2xl0l1，width设置
unsigned int SrioPhyInit(unsigned int SrioInstance,unsigned int Speed,unsigned Lane){
	unsigned int Buffer = /*0xb3f50001*/ 0xA7D50001;
	switch(Lane){
		case lane4x:	Buffer += 0x00;break;
		case lane2xl0l1:Buffer += 0x10;break;
		case lane2xl2l3:Buffer += 0x20;break;
		case lane1xl0:	Buffer += 0x30;break;
		case lane1xl1:	Buffer += 0x40;break;
		case lane1xl2:	Buffer += 0x50;break;
		case lane1xl3:	Buffer += 0x60;break;
		default:		Buffer += 0x00;break;
	}

	switch(Speed){
		case Speed1250:	Buffer += 0x000;break;
		case Speed2500:	Buffer += 0x100;break;
		case Speed3125:	Buffer += 0x200;break;
		case Speed5000:	Buffer += 0x300;break;
		case Speed6250:	Buffer += 0x400;break;
		default:		Buffer += 0x000;break;
	}

	if(SrioInstance){
		SRIO1_PBUS_ctrl_5 = Buffer;
	}else{
		PBUS_ctrl_5 = Buffer;
	}
	return 0;
}

unsigned int SrioCheckRx(unsigned int SrioInstance,unsigned Lane,unsigned int L2L3){
	unsigned int volatile * S0CSR_Point;
	unsigned int volatile * LANE_PWR_CTRL_Point;
	unsigned int volatile * RX_CFG_0_Point;
	unsigned int volatile j;
	unsigned int volatile k;
	unsigned int aaa;
//    aaa=10000;
	S0CSR_Point = 			(unsigned int volatile *)((unsigned int)&L0S0CSR 			+ (SrioInstance * 0x40000) + 0x20  * (Lane-L2L3*2));	//L0S0CSR
	LANE_PWR_CTRL_Point = 	(unsigned int volatile *)((unsigned int)&S0L0_LANE_PWR_CTRL + (SrioInstance * 0x40000) + 0x400 * Lane);				//S0L0_LANE_PWR_CTRL
	RX_CFG_0_Point = 		(unsigned int volatile *)((unsigned int)&S0L0_RX_CFG_0 		+ (SrioInstance * 0x40000) + 0x400 * Lane);				//S0L0_RX_CFG_0

	if(
	   ((*S0CSR_Point & 0x00300000) != 0x00300000)//检测Receiver lane sync  Receiver lane ready只对1lane进行检测 4x时间检测1、3lane
	   )
	{
		j = *LANE_PWR_CTRL_Point;
		*LANE_PWR_CTRL_Point = j | 0x100;//从rx_cfg0的13bit get reset value 此时为复位rx控制逻辑
//		while (aaa--);
		k = *RX_CFG_0_Point;
		*RX_CFG_0_Point = k | (1<<13);//对rx_cfg0的13bit 赋1， 表示以后脱离复位
		*RX_CFG_0_Point = k;//此句无意义  对cfg0  将原来值赋给自己
		*LANE_PWR_CTRL_Point = j;
		return 1;
	}

	return 0;
}

//检查phy链接状态
//SrioInstance:SrioInstance0 or SrioInstance1，指定srio0或者srio1
//work mode:lane1xl0 lane1xl1 lane1xl2 lane1xl3 lane4x lane2xl0l1，需校验的width设置
//return 0,link完成
//return 1,link超时
//return 2,width状态不对
unsigned int SrioPhyCheck(unsigned int SrioInstance,unsigned Lane){
	unsigned int i=0;
	volatile unsigned int k;
	volatile unsigned int j;
	volatile unsigned int *RAB_APB_CSR_Point = (volatile unsigned int *)((unsigned int)&RAB_APB_CSR + SrioInstance*0x40000);
	volatile unsigned int *PnESCSR_Point = (volatile unsigned int *)((unsigned int)&PnESCSR + SrioInstance*0x40000);
	volatile unsigned int *PnCCSR_Point = (volatile unsigned int *)((unsigned int)&PnCCSR + SrioInstance*0x40000);

	*RAB_APB_CSR_Point = 0;
	for(i=0;i<PhyCheckTimeOut;i++)
	{
		switch(Lane)
		{
		case lane4x:
			if(i)
			{
				//SrioCheckRx(SrioInstance,0,0);
				SrioCheckRx(SrioInstance,1,0);
				//SrioCheckRx(SrioInstance,2,0);
				SrioCheckRx(SrioInstance,3,0);
			}
			break;
		case lane2xl0l1:
			if(i)
			{
				SrioCheckRx(SrioInstance,1,0);
			}
			break;
		case lane2xl2l3:
			if(i)
			{
				SrioCheckRx(SrioInstance,3,1);
			}

		    default:
			break;
		}
		//printf("PnESCSR_Point is xxxxxxxx 0x%x\n",*PnESCSR_Point);
		aaa++;
		//if( ((*PnESCSR_Point & 0x02000000)==0x02000000) & ((*PnESCSR_Point & 0x00010100)==0x00000000))//检测158寄存器
		if( ((*PnESCSR_Point & 0x02000000)==0x02000000))//检测158寄存器
		{
			//printf("PnESCSR_Point is ttttttt 0x%x\n",*PnESCSR_Point);
			j = (*PnCCSR_Point>>3)&0x7;
			if(Lane==lane4x)
			{
				if(j!=2)
					return 2;
			}
			else
			{
				if((Lane==lane2xl0l1)||(Lane==lane2xl2l3))
				{
					if(j!=3)
						return 2;
				}
				else
				{
					if((j!=0)&&(j!=1))
						return 2;
				}
			}
			return 0;
		}
		DelaySRIO(10);
	}
	return 1;
}

#define threshold_value		2000000
unsigned int Srio_read_PnESCSR(unsigned int SrioInstance)
{
	unsigned int cnt;
	volatile unsigned int *RAB_APB_CSR_Point = (volatile unsigned int *)((unsigned int)&RAB_APB_CSR + SrioInstance*0x40000);
	volatile unsigned int *PnESCSR_Point = (volatile unsigned int *)((unsigned int)&PnESCSR + SrioInstance*0x40000);
	*RAB_APB_CSR_Point = 0;
	rignt_times=0;
	cnt=threshold_value;
    while(cnt--)
    {
		if( ((*PnESCSR_Point & 0x02000000)==0x02000000) & ((*PnESCSR_Point & 0x00010100)==0x00000000))
		{
			rignt_times++;
		}
		else
		{
			rignt_times=rignt_times;
			return 1;
		}
		if(rignt_times==threshold_value)
		return 0;
    }
    return 0;
}
//设置指定srio端口ID工作方式（8位ID或者16位ID），以及本端口ID值
unsigned int SrioCtl(unsigned int SrioInstance,struct SRIOinit * SRIOinit){
	if(SrioInstance){
		SRIO1_RAB_CTRL = 0x0000008f;

		//id
		SRIO1_RAB_APB_CSR = 0x0;//page 0
		if(SRIOinit->IDLen)
			SRIO1_PBUS_ctrl_1 = 0xb3f10004;
		else
			SRIO1_PBUS_ctrl_1 = 0xb3f10000;
		SRIO1_BDIDCSR = (
						  ((SRIOinit->SelfID<<24)&0xff000000)	//16bit lsb 8bit
						| ((SRIOinit->SelfID<<8) &0x00ff0000)	//16bit msb 8bit
						| ((SRIOinit->SelfID<<8) &0x0000ff00)	//8bit id value
								);
		SRIO1_RAB_APIO_N_CTRL = 0x7;
		SRIO1_RAB_RIO_PIO_CTRLn = 0x3;
	}else{
		RAB_CTRL = 0x0000008f;

		//id
		RAB_APB_CSR = 0x0;//page 0
		if(SRIOinit->IDLen)
			PBUS_ctrl_1 = 0xb3f10004;
		else
			PBUS_ctrl_1 = 0xb3f10000;
		BDIDCSR = (
					  ((SRIOinit->SelfID<<24)&0xff000000)	//16bit lsb 8bit
					| ((SRIOinit->SelfID<<8) &0x00ff0000)	//16bit msb 8bit
					| ((SRIOinit->SelfID<<8) &0x0000ff00)	//8bit id value
					);
		RAB_APIO_N_CTRL = 0x7;
		RAB_RIO_PIO_CTRLn = 0x3;
	}
	return 0;
}

//匹配ACKID,主要将本端口inbound与对端outbound进行匹配，将本端口outbound与对端inbound进行匹配
//SrioInstance，指定srio0或者srio1
//DstID，目的ID
//RemotePort，对端端口号,非交换机时，填0
unsigned int SrioMatchACKID(unsigned int SrioInstance,unsigned int DstID,unsigned int RemotePort){
	unsigned int RemoteInBoundID;
	unsigned int RemoteOutBoundID;
	unsigned int LocalInBoundID;
	unsigned int buffer=0;
	unsigned int i;
	unsigned int volatile *LMREQCSRPoint;//Link Maintenance Request CSR
	unsigned int volatile *LMRESPCSRPoint;//Link Maintenance Response CSR
	unsigned int volatile *LASCSRPoint;//Local Ackid Status CSR
	unsigned int volatile AXIBaseAddress;
	struct SRIOAPIOStruct SRIOAPIOStruct;

	SRIOAPIOStruct.WindowNub = 0;				//使用的窗口号，每个部件最多可使用16个窗口（0-15）
	SRIOAPIOStruct.DstID = DstID;				//目的ID
	SRIOAPIOStruct.WindowTYPE = 0;				//窗口类型
	SRIOAPIOStruct.WindowSize = 0x400;			//窗口大小
	SRIOAPIOStruct.RIOBase = 0;			//对端器件RIO基址
	SRIOAPIOStruct.Priority = 0;				//优先级
	SRIOAPIOStruct.hopcount = 0;				//跳数
	SRIOAPIOStruct.CRF = 0;						//CRF值，默认为0
	//初始化寄存器地址及apio窗口映射
	if(SrioInstance){
		LMREQCSRPoint = &SRIO1_PnLMREQCSR;
		LMRESPCSRPoint = &SRIO1_PnLMRESPCSR;
		LASCSRPoint = &SRIO1_PnLASCSR;
		SRIOAPIOStruct.AXIBase = 0x029c0000;		//DSP内axi基址
		AXIBaseAddress = 0x029c0000;
		SrioCreatAPIOWindow(SrioInstance,&SRIOAPIOStruct);
		SRIO1_RAB_APB_CSR = 0x0;
	}else{
		LMREQCSRPoint = &PnLMREQCSR;
		LMRESPCSRPoint = &PnLMRESPCSR;
		LASCSRPoint = &PnLASCSR;
		SRIOAPIOStruct.AXIBase = 0x02980000;		//DSP内axi基址
		AXIBaseAddress = 0x02980000;
		SrioCreatAPIOWindow(SrioInstance,&SRIOAPIOStruct);
		RAB_APB_CSR = 0x0;
	}

	//发送"input-status（restart-from-error）"控制符，请求对端ACK_ID（inbound ackID）
	*LMREQCSRPoint = SrioChangeEnding(0x04);
	i=0;
	//等待对端回传response_valid标志，超时则重新发送"input-status（restart-from-error）"控制符，直到请求到对端ACK_ID
	while(!buffer){
		buffer = *LMRESPCSRPoint& 0x80;
		i++;
		if(i>100000){
			*LMREQCSRPoint = SrioChangeEnding(0x04);
			i=0;
		}
	}

	//获得对端ACKID并将其写至本部件Local ackID CSRs寄存器outbound域以进行本端outbound匹配
	RemoteInBoundID = (SrioChangeEnding(*LMRESPCSRPoint)>>5)&0x3f;//RemoteACKID
	*LASCSRPoint = SrioChangeEnding(RemoteInBoundID);

	//将本端inbound写入对端outbound以进行本端inbound匹配
	LocalInBoundID = SrioChangeEnding(*LASCSRPoint)>>24;//
	//生成对端Local Ackid Status CSR寄存器需填入的值
	//byte0:outbound ackID   byte1:Outstanding port unacknowledged ackID status   byte3:inbound ackID
	buffer = SrioChangeEnding(LocalInBoundID + ((RemoteInBoundID+1)<<24));
	*(unsigned int volatile*)(AXIBaseAddress + 0x148 + RemotePort * 0x20) = buffer;
	DelaySRIO(100);

	return RemoteInBoundID;
}

//SRIO DMA写函，向对端写数据，数传输长度最大值(0x100000/4-1)，单位字（32位）
unsigned int SrioWDMA(unsigned int SrioInstance,struct SRIODMA * SRIODMA){
	if(SrioInstance){
		SRIO1_RAB_APB_CSR = 0x00410000;
		SRIO1_RAB_WDMA_0_CTRL = ((SRIODMA->DstID & 0xffff)<<16)
								| (SRIODMA->EnSendDB & 0x1) <<6
								| ((SRIODMA->CRF & 0x1)<<5)
								| ((SRIODMA->Priority & 0x3)<<3)
								| (!(SRIODMA->EnDescriptor & 0x1)<<2);
		SRIO1_RAB_DMA_IADDR_DESC_SEL = 0x1;
		SRIO1_RAB_WDMA_0_ADDR = SRIODMA->FirstDescriptorAdd>>2;//设置描述符起始地址
		SRIO1_RAB_WDMA_0_ADDR_EXT = 0;
		SRIO1_RAB_DMA_IADDR_DESC_CTRL = ((SRIODMA->DataLen& 0x3ffff)<<5)+0x3;
		SRIO1_RAB_DMA_IADDR_DESC_SRC_ADDR = (SRIODMA->SrcAdd>>2);
		SRIO1_RAB_DMA_IADDR_DESC_DEST_ADDR = (SRIODMA->DstAdd>>2);
		SRIO1_RAB_DMA_IADDR_DESC_NEXT_ADDR = 0x0;
		SRIO1_RAB_WDMA_0_CTRL = ((SRIODMA->DstID & 0xffff)<<16)
								| (SRIODMA->EnSendDB & 0x1) <<6
								| ((SRIODMA->CRF & 0x1)<<5)
								| ((SRIODMA->Priority & 0x3)<<3)
								| (!(SRIODMA->EnDescriptor & 0x1)<<2)
								| 0x1;
	}else{
		RAB_APB_CSR = 0x00410000;

		RAB_WDMA_0_CTRL = ((SRIODMA->DstID & 0xffff)<<16)
						| (SRIODMA->EnSendDB & 0x1) <<6
						| ((SRIODMA->CRF & 0x1)<<5)
						| ((SRIODMA->Priority & 0x3)<<3)
						| (!(SRIODMA->EnDescriptor & 0x1)<<2);
		RAB_DMA_IADDR_DESC_SEL = 0x1;
		RAB_WDMA_0_ADDR = SRIODMA->FirstDescriptorAdd>>2;//设置描述符起始地址
		RAB_WDMA_0_ADDR_EXT = 0;
		RAB_DMA_IADDR_DESC_CTRL = ((SRIODMA->DataLen& 0x3ffff)<<5)+0x3;
		RAB_DMA_IADDR_DESC_SRC_ADDR = (SRIODMA->SrcAdd>>2);
		RAB_DMA_IADDR_DESC_DEST_ADDR = (SRIODMA->DstAdd>>2);
		RAB_DMA_IADDR_DESC_NEXT_ADDR = 0x0;
		RAB_WDMA_0_CTRL = ((SRIODMA->DstID & 0xffff)<<16)
						| (SRIODMA->EnSendDB & 0x1) <<6
						| ((SRIODMA->CRF & 0x1)<<5)
						| ((SRIODMA->Priority & 0x3)<<3)
						| (!(SRIODMA->EnDescriptor & 0x1)<<2)
						| 0x1;
	}
	return 0;
}

//SRIO DMA读函，从对端读数据，数传输长度最大值(0x100000/4-1)，单位字（32位）
unsigned int SrioRDMA(unsigned int SrioInstance,struct SRIODMA * SRIODMA){
	if(SrioInstance){
		SRIO1_RAB_APB_CSR = 0x00410000;
		SRIO1_RAB_RDMA_0_CTRL = (SRIODMA->DstID<<16)
								| ((SRIODMA->CRF & 0x1)<<5)
								| ((SRIODMA->Priority & 0x3)<<3)
								| (!(SRIODMA->EnDescriptor & 0x1)<<2);
		SRIO1_RAB_DMA_IADDR_DESC_SEL = 0;
		SRIO1_RAB_RDMA_0_ADDR = SRIODMA->FirstDescriptorAdd>>2;//设置描述符起始地址
		SRIO1_RAB_RDMA_0_ADDR_EXT = 0x0;
		SRIO1_RAB_DMA_IADDR_DESC_CTRL = ((SRIODMA->DataLen& 0x3ffff)<<5)+0x3;
		SRIO1_RAB_DMA_IADDR_DESC_SRC_ADDR = (SRIODMA->SrcAdd>>2);
		SRIO1_RAB_DMA_IADDR_DESC_DEST_ADDR = (SRIODMA->DstAdd>>2);
		SRIO1_RAB_DMA_IADDR_DESC_NEXT_ADDR = 0;
		SRIO1_RAB_RDMA_0_CTRL = (SRIODMA->DstID<<16)
								| ((SRIODMA->CRF & 0x1)<<5)
								| ((SRIODMA->Priority & 0x3)<<3)
								| (!(SRIODMA->EnDescriptor & 0x1)<<2)
								| 0x1;
	}else{
		RAB_APB_CSR = 0x00410000;
		RAB_RDMA_0_CTRL = (SRIODMA->DstID<<16)
						| ((SRIODMA->CRF & 0x1)<<5)
						| ((SRIODMA->Priority & 0x3)<<3)
						| (!(SRIODMA->EnDescriptor & 0x1)<<2);
		RAB_DMA_IADDR_DESC_SEL = 0;
		RAB_RDMA_0_ADDR = SRIODMA->FirstDescriptorAdd>>2;//设置描述符起始地址
		RAB_RDMA_0_ADDR_EXT = 0x0;
		RAB_DMA_IADDR_DESC_CTRL = ((SRIODMA->DataLen& 0x3ffff)<<5)+0x3;
		RAB_DMA_IADDR_DESC_SRC_ADDR = (SRIODMA->SrcAdd>>2);
		RAB_DMA_IADDR_DESC_DEST_ADDR = (SRIODMA->DstAdd>>2);
		RAB_DMA_IADDR_DESC_NEXT_ADDR = 0;
		RAB_RDMA_0_CTRL = (SRIODMA->DstID<<16)
						| ((SRIODMA->CRF & 0x1)<<5)
						| ((SRIODMA->Priority & 0x3)<<3)
						| (!(SRIODMA->EnDescriptor & 0x1)<<2)
						| 0x1;
	}
	return 0;
}

void SrioConfigSendDB(unsigned int SrioInstance,struct SRIODoorBellStruct * SRIODoorBellStruct){
	unsigned int volatile *RAB_OB_DB_N_CSR;//门铃控制寄存器指针
	unsigned int volatile *RAB_OB_DB_N_INFO;//门铃消息寄存器指针

	RAB_OB_DB_N_CSR = (unsigned int volatile *)((unsigned int)&RAB_OB_DB_0_CSR + 0x8*SRIODoorBellStruct->Nub + SrioInstance * 0x40000);
	RAB_OB_DB_N_INFO = (unsigned int volatile *)((unsigned int)&RAB_OB_DB_0_INFO + 0x8*SRIODoorBellStruct->Nub + SrioInstance * 0x40000);

	*RAB_OB_DB_N_INFO = SRIODoorBellStruct->Info & 0xffff;
	*RAB_OB_DB_N_CSR = 	((SRIODoorBellStruct->DstID & 0xffff)<<16)
						+ ((SRIODoorBellStruct->CRF & 0x1)<<6)
						+ ((SRIODoorBellStruct->Priority & 0x3)<<4);
	if(SRIODoorBellStruct->Send)
		*RAB_OB_DB_N_CSR |= 0x1;
	else
		*RAB_OB_DB_N_CSR |= 0x80;
}

void SrioEnRecvDB(unsigned int SrioInstance){
	//Receive DB config
	//[0]enable
	//[21:16]number of Inbound messages
	if(SrioInstance)
		SRIO1_RAB_IB_DB_CSR = 0x1;
	else
		RAB_IB_DB_CSR = 1;
}

void SrioSetDescriptor(struct SrioDescriptorReg * SrioDescriptorReg,struct SrioDescriptorStruct SrioDescriptorStruct){
	SrioDescriptorReg->Ctrl =
			 ((SrioDescriptorStruct.Len& 0x3ffff)<<5)
			|(1<<1)
			|SrioDescriptorStruct.En;
	SrioDescriptorReg->SrcAdd = (SrioDescriptorStruct.SrcAdd>>2);
	SrioDescriptorReg->DestAdd = (SrioDescriptorStruct.DstAdd>>2);
	SrioDescriptorReg->NextAdd = (SrioDescriptorStruct.NextAdd>>3);
}

void LBERT_Config(struct SrioLBERTStruct *SrioLBERTStruct){
	unsigned int volatile * BasePoint;
			//base address is PAT_CFG
	unsigned int temp;
	unsigned int ConfigValue;
	unsigned int PG_WIDTH;
	unsigned int PM_WIDTH;

	BasePoint = (unsigned int volatile *)(0x0291E00C+SrioLBERTStruct->Instance*0x40000+SrioLBERTStruct->Lane*0x400);//指向PCS_LBERT_PAT_CFG

	if(SrioVersion == 1){
		switch(SrioLBERTStruct->Speed){
			case Speed1250:	PG_WIDTH = 1;PM_WIDTH = 1;break;
			case Speed2500:	PG_WIDTH = 1;PM_WIDTH = 1;break;
			case Speed3125:	PG_WIDTH = 1;PM_WIDTH = 1;break;
			case Speed5000:	PG_WIDTH = 3;PM_WIDTH = 3;break;
			case Speed6250:	PG_WIDTH = 3;PM_WIDTH = 3;break;
			default:		PG_WIDTH = 1;PM_WIDTH = 1;break;
		}
	}else if(SrioVersion == 2){
		PG_WIDTH = 1;PM_WIDTH = 1;
	}

	ConfigValue =
			((PG_WIDTH&0x3)<<11)
			|((SrioLBERTStruct->PG_MODE&0xf)<<7)
			|((PM_WIDTH&0x3)<<4)
			|(SrioLBERTStruct->PM_MODE&0xf);

	*(BasePoint+1) = 0;//S0L0_PCS_LBERT_CFG
	*(BasePoint) = SrioLBERTStruct->Pattern;//PCS_LBERT_PAT_CFG
	DelaySRIO(1);
	*(BasePoint+1) = ConfigValue;//S0L0_PCS_LBERT_CFG

	/*if(SrioLBERTStruct->En){
		*(BasePoint+1) = *(BasePoint+1)  | (1<<13);
		DelaySRIO(1);
		*(BasePoint+1) = *(BasePoint+1) | (1<<6) | (1<<13);
		DelaySRIO(1);
		*(BasePoint+1) = *(BasePoint+1) | (1<<6) | (1<<13) | (1<<14);
	}*/
}

void LBERT_Config_InitialVer(struct SrioLBERTStruct *SrioLBERTStruct){
	unsigned int volatile * BasePoint;
			//base address is PAT_CFG
	unsigned int temp;
	unsigned int ConfigValue;
	unsigned int PG_WIDTH;
	unsigned int PM_WIDTH;

	BasePoint = (unsigned int volatile *)(0x0291E00C+SrioLBERTStruct->Instance*0x40000+SrioLBERTStruct->Lane*0x400);//指向LBERT_PAT_CFG寄存器

	if(SrioVersion == 1){
		switch(SrioLBERTStruct->Speed){
			case Speed1250:	PG_WIDTH = 1;PM_WIDTH = 1;break;
			case Speed2500:	PG_WIDTH = 1;PM_WIDTH = 1;break;
			case Speed3125:	PG_WIDTH = 1;PM_WIDTH = 1;break;
			case Speed5000:	PG_WIDTH = 3;PM_WIDTH = 3;break;
			case Speed6250:	PG_WIDTH = 3;PM_WIDTH = 3;break;
			default:		PG_WIDTH = 1;PM_WIDTH = 1;break;
		}
	}else if(SrioVersion == 2){
		PG_WIDTH = 1;PM_WIDTH = 1;
	}

	ConfigValue =
			((PG_WIDTH&0x3)<<11)
			|((SrioLBERTStruct->PG_MODE&0xf)<<7)
			|((PM_WIDTH&0x3)<<4)
			|(SrioLBERTStruct->PM_MODE&0xf);

	*(BasePoint+1) = 0;
	*(BasePoint) = SrioLBERTStruct->Pattern;
	DelaySRIO(1);
	*(BasePoint+1) = ConfigValue;

	if(SrioLBERTStruct->En){
		*(BasePoint+1) = *(BasePoint+1)  | (1<<13);
		DelaySRIO(1);
		*(BasePoint+1) = *(BasePoint+1) | (1<<6) | (1<<13);
		DelaySRIO(1);
		*(BasePoint+1) = *(BasePoint+1) | (1<<6) | (1<<13) | (1<<14);
	}
}

void LBERT_InsertErr(struct SrioLBERTStruct *SrioLBERTStruct){
	unsigned int volatile * BasePoint;
	BasePoint = (unsigned int volatile *)(0x0291E00C+SrioLBERTStruct->Instance*0x40000+SrioLBERTStruct->Lane*0x400);//指向LBERT_PAT_CFG寄存器
	*(BasePoint+1) |= 0x8000;
}

unsigned short LBERT_GetErr(struct SrioLBERTStruct *SrioLBERTStruct){
	unsigned int volatile * BasePoint;
	BasePoint = (unsigned int volatile *)(0x0291E00C+SrioLBERTStruct->Instance*0x40000+SrioLBERTStruct->Lane*0x400);//指向LBERT_PAT_CFG寄存器
	*(BasePoint+1) |= 0x4000;
	return *(BasePoint+2);
}

//return 0,link完成
//return 1,link超时
//return 2,width状态不对
unsigned int SrioManulConfig(unsigned int SrioInstance, struct SrioManulStruct SrioManulStruct)
{
	unsigned int ncheckRet;
	unsigned int LanModeBuf;
	unsigned int PreCursor[4];
	unsigned int Swing[4];
	unsigned int PostCursor[4];
	struct SrioPHYCtrlStruct * SrioPHYCtrlStruct;
	unsigned int Version;
	unsigned int Buf10;
	unsigned int i;
	unsigned int SynCount;
	struct SrioLanePCSStruct * LanePCSStruct0;
	struct SrioLanePCSStruct * LanePCSStruct1;
	struct SrioLanePCSStruct * LanePCSStruct2;
	struct SrioLanePCSStruct * LanePCSStruct3;
	struct SrioTxCtrl * SrioTxCtrl0;
	struct SrioTxCtrl * SrioTxCtrl1;
	struct SrioTxCtrl * SrioTxCtrl2;
	struct SrioTxCtrl * SrioTxCtrl3;
	struct SrioPCSModeStruct *SrioPCSModeStruct;
	unsigned int flag;

	volatile unsigned int *RAB_APB_CSR_Point = (volatile unsigned int *)((unsigned int)&RAB_APB_CSR + SrioInstance*0x40000);//对RAB_APB_CSR_Point赋值页选择寄存器指针，不使能页选项12bit为0
	volatile unsigned int *L0S0CSR_Point = (volatile unsigned int *)((unsigned int)&L0S0CSR + SrioInstance*0x40000);//00210 Lane0收发器的状态信息指针
	volatile unsigned int *L1S0CSR_Point = (volatile unsigned int *)((unsigned int)&L1S0CSR + SrioInstance*0x40000);//00230 Lane1收发器的状态信息
	volatile unsigned int *L2S0CSR_Point = (volatile unsigned int *)((unsigned int)&L2S0CSR + SrioInstance*0x40000);//00250 Lane2收发器的状态信息
	volatile unsigned int *L3S0CSR_Point = (volatile unsigned int *)((unsigned int)&L3S0CSR + SrioInstance*0x40000);//00270 Lane3收发器的状态信息

	Version = SrioVersion;
	i = ((unsigned int)&PBUS_ctrl_0 + SrioInstance*0x40000);//i=0290_0000
	SrioPHYCtrlStruct = (struct SrioPHYCtrlStruct *)((unsigned int)&PBUS_ctrl_0 + SrioInstance*0x40000);//SrioPHYCtrlStruct=0x0290_0000  SRIO0-PBUS
	LanePCSStruct0 =  (struct SrioLanePCSStruct *)((unsigned int)&S0L0_PCS_MISC_CFG_0 + SrioInstance*0x40000);//0x0291E000 PHY-lane0寄存器初始地址0C开始
	LanePCSStruct1 =  (struct SrioLanePCSStruct *)((unsigned int)&S0L1_PCS_MISC_CFG_0 + SrioInstance*0x40000);//0x0291E400 PHY-lane1寄存器初始地址0C开始
	LanePCSStruct2 =  (struct SrioLanePCSStruct *)((unsigned int)&S0L2_PCS_MISC_CFG_0 + SrioInstance*0x40000);//0x0291E800 PHY-lane2寄存器初始地址0C开始
	LanePCSStruct3 =  (struct SrioLanePCSStruct *)((unsigned int)&S0L3_PCS_MISC_CFG_0 + SrioInstance*0x40000);//0x0291EC00 PHY-lane3寄存器初始地址0C开始

	SrioTxCtrl0 = (struct SrioTxCtrl *)((unsigned int)&S0L0_TX_CFG_0 + SrioInstance*0x40000);//0x02916054??
	SrioTxCtrl1 = (struct SrioTxCtrl *)((unsigned int)&S0L1_TX_CFG_0 + SrioInstance*0x40000);//0x02916454??
	SrioTxCtrl2 = (struct SrioTxCtrl *)((unsigned int)&S0L2_TX_CFG_0 + SrioInstance*0x40000);//0x02916854??
	SrioTxCtrl3 = (struct SrioTxCtrl *)((unsigned int)&S0L3_TX_CFG_0 + SrioInstance*0x40000);//0x02916C54??
	if(SrioManulStruct.RefClkSrc & 0x1)
	{//内部时钟
		SrioPCSModeStruct = (struct SrioPCSModeStruct *)((unsigned int)&S0_PCS_PLL_PCIE1_125M_MODE_0 + SrioInstance*0x40000);//0x0291F0A8??
	}
	else
	{//外部时钟
		SrioPCSModeStruct = (struct SrioPCSModeStruct *)((unsigned int)&S0_PCS_PLL_PCIE1_MODE_0 + SrioInstance*0x40000);//0x0291F018?/
	}

	//设置PHY参数,在不同的DB参数下的衰减因子,pre post swing为幅度
	for(i=0;i<4;i++)
	{
		switch(SrioManulStruct.TXEq[i])
		{
			case TxEq_0		: PreCursor[i] = 0;Swing[i] = 0 ;PostCursor[i] = 0 ;break;
			case TxEq_1		: PreCursor[i] = 0;Swing[i] = 2 ;PostCursor[i] = 2 ;break;
			case TxEq_2		: PreCursor[i] = 0;Swing[i] = 4 ;PostCursor[i] = 4 ;break;
			case TxEq_2_5	: PreCursor[i] = 0;Swing[i] = 5 ;PostCursor[i] = 5 ;break;
			case TxEq_3		: PreCursor[i] = 0;Swing[i] = 6 ;PostCursor[i] = 6 ;break;
			case TxEq_3_5	: PreCursor[i] = 0;Swing[i] = 7 ;PostCursor[i] = 7 ;break;
			case TxEq_4_5	: PreCursor[i] = 0;Swing[i] = 8 ;PostCursor[i] = 8 ;break;
			case TxEq_5		: PreCursor[i] = 0;Swing[i] = 9 ;PostCursor[i] = 9 ;break;
			case TxEq_6		: PreCursor[i] = 0;Swing[i] = 10;PostCursor[i] = 10;break;
			case TxEq_7		: PreCursor[i] = 0;Swing[i] = 11;PostCursor[i] = 11;break;
			case TxEq_8		: PreCursor[i] = 0;Swing[i] = 12;PostCursor[i] = 12;break;
			case TxEq_9		: PreCursor[i] = 0;Swing[i] = 13;PostCursor[i] = 13;break;
			default			: PreCursor[i] = 0;Swing[i] = 0 ;PostCursor[i] = 0 ;break;
		 }
	}
	flag = 1;
	while(flag)
	{
		SrioClkClose(SrioInstance);//关闭

		if(Version==1)
		{
			if(SrioInstance)
			{//SRIO1
				if(SrioManulStruct.RefClkSrc & 0x1)
				{//内部时钟
					SRIO_SERDES_CFGPLL |= 0x01000000;//切换至内部时钟,默认
				}
				else
				{
					SRIO_SERDES_CFGPLL &= 0xfeffffff;//切换至外部时钟
				}
			}
			else
			{//SRIO0
				if(SrioManulStruct.RefClkSrc & 0x1)
				{//内部时钟
				  SRIO_SERDES_CFGPLL |= 0x00010000;//切换至内部时钟,默认
				}
				else
				{
				  SRIO_SERDES_CFGPLL &= 0xfffeffff;//切换至外部时钟
				}
			}//SrioVersion
		}
		else if(Version==2)
		{
			if(SrioInstance)
			{
				SRIO1_CLC_cfg_r_2 = 0xA7D20230;
				SRIO1_CLC_cfg_r_4 = 0xA7D40000 | (SrioManulStruct.RefClkSrc & 0x1);
				SRIO1_CLC_cfg_r_5 = 0xA7D50300;//srio 1 manual
			}
			else
			{
				SRIO0_CLC_cfg_r_2 = 0xA7D20230;//A7D2为wirte key 3配置发送为longrun 模式 11对应TT TM， 2配置对应RX 为longrun模式
				SRIO0_CLC_cfg_r_4 = 0xA7D40000 | (SrioManulStruct.RefClkSrc & 0x1);//A7D4为wirte key 0x02620370 配置外部时钟 0对应为外部时钟
				SRIO0_CLC_cfg_r_5 = 0xA7D50300;//A7D5为wirte key  0x02620374  3为选择CLC_RATE_MODE[2:0]为5G速率 ，x4 lane
				//交付物程序配置为0X0110 选择2.5gbps，选择2x模式 ，暂不明确此位域 不符产生的影响
			}
		}
		DelaySRIO(1000);
		SrioClkOpen(SrioInstance);
		if(Version==1)
		{
			SrioPHYCtrlStruct->PHYCtrl[9] = 0xb3f90001;//bit0,1,复位rab,高位写保护0xb3f90;bit4,1,复位phy,高位写保护0xb3f94
			switch(SrioManulStruct.LaneMode){//PHYCtrl[10] bit0,强制1x；bit1，强制2x；bit2，半速；bit3，1/4速；bit4，手工配置使能；bit7:5手工配置速率
				case lane4x:	SrioPHYCtrlStruct->PHYCtrl[11] = 0xb3fb3210;SrioPHYCtrlStruct->PHYCtrl[10] = 0xb3fa4010;Buf10 = 0xb3fa4010;LanModeBuf = 0xf;break;
				case lane2xl0l1:SrioPHYCtrlStruct->PHYCtrl[11] = 0xb3fb4410;SrioPHYCtrlStruct->PHYCtrl[10] = 0xb3fa4012;Buf10 = 0xb3fa4012;LanModeBuf = 0x3;break;
				case lane2xl2l3:SrioPHYCtrlStruct->PHYCtrl[11] = 0xb3fb1044;SrioPHYCtrlStruct->PHYCtrl[10] = 0xb3fa4012;Buf10 = 0xb3fa4012;LanModeBuf = 0xc;break;
				case lane1xl0:	SrioPHYCtrlStruct->PHYCtrl[11] = 0xb3fb4440;SrioPHYCtrlStruct->PHYCtrl[10] = 0xb3fa4011;Buf10 = 0xb3fa4011;LanModeBuf = 0x1;break;
				case lane1xl1:	SrioPHYCtrlStruct->PHYCtrl[11] = 0xb3fb4404;SrioPHYCtrlStruct->PHYCtrl[10] = 0xb3fa4011;Buf10 = 0xb3fa4011;LanModeBuf = 0x2;break;
				case lane1xl2:	SrioPHYCtrlStruct->PHYCtrl[11] = 0xb3fb4044;SrioPHYCtrlStruct->PHYCtrl[10] = 0xb3fa4011;Buf10 = 0xb3fa4011;LanModeBuf = 0x4;break;
				case lane1xl3:	SrioPHYCtrlStruct->PHYCtrl[11] = 0xb3fb0444;SrioPHYCtrlStruct->PHYCtrl[10] = 0xb3fa4011;Buf10 = 0xb3fa4011;LanModeBuf = 0x8;break;
				default:		SrioPHYCtrlStruct->PHYCtrl[11] = 0xb3fb3210;SrioPHYCtrlStruct->PHYCtrl[10] = 0xb3fa4010;Buf10 = 0xb3fa4010;LanModeBuf = 0xf;break;
			}

			switch(SrioManulStruct.WorkSpeed){
				case Speed1250:	SrioPHYCtrlStruct->PHYCtrl[10] = Buf10 | (0<<5) | 0xc;break;
				case Speed2500:	SrioPHYCtrlStruct->PHYCtrl[10] = Buf10 | (1<<5) | 0x4;break;
				case Speed3125:	SrioPHYCtrlStruct->PHYCtrl[10] = Buf10 | (2<<5) | 0x4;break;
				case Speed5000:	SrioPHYCtrlStruct->PHYCtrl[10] = Buf10 | (3<<5)      ;break;
				case Speed6250:	SrioPHYCtrlStruct->PHYCtrl[10] = Buf10 | (4<<5)      ;break;
				default:		SrioPHYCtrlStruct->PHYCtrl[10] = Buf10 | (2<<5) | 0x4;break;
			}

			SrioPHYCtrlStruct->PHYCtrl[7] = 0xffffffff;
			SrioPCSModeStruct->PLL_2_MODE_1 = 0x0816;//

			SrioPHYCtrlStruct->PHYCtrl[7] = 0xffffffff;
			switch(SrioManulStruct.WorkSpeed){
				case Speed1250:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0814;break;
				case Speed2500:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0814;break;
				case Speed3125:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0819;break;
				case Speed5000:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0814;break;
				case Speed6250:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0819;break;
				default:		SrioPCSModeStruct->PLL_2_MODE_1 = 0x0814;break;
			}

			i = 0;
			while(i!=0x000000ff){
				i = SrioPHYCtrlStruct->PHYCtrl[6];//查询SRIO0 PHY主锁相环是否成功
				i = i & 0x000000ff;
			}

			SrioPHYCtrlStruct->PHYCtrl[7] = 0xffffffff;
			SrioPCSModeStruct->LANE_2_MODE_0 = 0x3004;
			SrioPHYCtrlStruct->PHYCtrl[7] = 0xffffffff;
			switch(SrioManulStruct.WorkSpeed){
				case Speed1250:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x1a05;break;
				case Speed2500:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x1505;break;
				case Speed3125:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x1505;break;
				case Speed5000:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x100f;break;
				case Speed6250:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x100f;break;
				default:		SrioPCSModeStruct->LANE_2_MODE_0 = 0x3005;break;
			}

			i = 0;
			while(i!=0x000000ff){
				i = SrioPHYCtrlStruct->PHYCtrl[6];//检查状态
				i = i & 0x000000ff;
			}
			SrioPHYCtrlStruct->PHYCtrl[7] = 0xffffffff;
		}
		else if(Version==2)
		{
			switch(SrioManulStruct.LaneMode)
			{
			    //配置SRIO0-PBUS SrioPHYCtrlStruct对应地址0x0290_0000  PHYCtrl[1]对应PBUS_reg_1   PHYCtrl[2]对应PBUS_reg_2
			    //PHYCtrl[1] wk=b3f1; 3210对应x4 4410、1044对应x2 其他4种对应x1
			    //PHYCtrl[2] wk=b3f2; 0bit 对应x1使能， 1bit对应x2使能 bit0和bit1不能同时为1
				case lane4x:	SrioPHYCtrlStruct->PHYCtrl[1] = 0xb3f13210;SrioPHYCtrlStruct->PHYCtrl[2] = 0xb3f20000;LanModeBuf = 0xf;break;//PBUS_reg_1  4X MODE //PBUS_reg_2 DIS X1 X2
				case lane2xl0l1:SrioPHYCtrlStruct->PHYCtrl[1] = 0xb3f14410;SrioPHYCtrlStruct->PHYCtrl[2] = 0xb3f20002;LanModeBuf = 0x3;break;
				case lane2xl2l3:SrioPHYCtrlStruct->PHYCtrl[1] = 0xb3f11044;SrioPHYCtrlStruct->PHYCtrl[2] = 0xb3f20002;LanModeBuf = 0xc;break;
				case lane1xl0:	SrioPHYCtrlStruct->PHYCtrl[1] = 0xb3f14440;SrioPHYCtrlStruct->PHYCtrl[2] = 0xb3f20001;LanModeBuf = 0x1;break;
				case lane1xl1:	SrioPHYCtrlStruct->PHYCtrl[1] = 0xb3f14404;SrioPHYCtrlStruct->PHYCtrl[2] = 0xb3f20001;LanModeBuf = 0x2;break;
				case lane1xl2:	SrioPHYCtrlStruct->PHYCtrl[1] = 0xb3f14044;SrioPHYCtrlStruct->PHYCtrl[2] = 0xb3f20001;LanModeBuf = 0x4;break;
				case lane1xl3:	SrioPHYCtrlStruct->PHYCtrl[1] = 0xb3f10444;SrioPHYCtrlStruct->PHYCtrl[2] = 0xb3f20001;LanModeBuf = 0x8;break;
				default:		SrioPHYCtrlStruct->PHYCtrl[1] = 0xb3f13210;SrioPHYCtrlStruct->PHYCtrl[2] = 0xb3f20000;LanModeBuf = 0xf;break;
			}
			switch(SrioManulStruct.WorkSpeed)
			{
			    //PHYCtrl[3] wk=b3f4; 0bit 是否是能tx_clk_div2， 1bit是否使能tx_clk_div4 对tx的速率有影响
				case Speed1250:	SrioPHYCtrlStruct->PHYCtrl[3] = 0xb3f30002;break;//PBUS_reg_3 不使能 tx-div2    使能  tx-div4：div4？
				case Speed2500:	SrioPHYCtrlStruct->PHYCtrl[3] = 0xb3f30001;break;//PBUS_reg_3  使能    tx-div2   不使能tx-div4：div2？
				case Speed3125:	SrioPHYCtrlStruct->PHYCtrl[3] = 0xb3f30001;break;//PBUS_reg_3  使能    tx-div2   不使能tx-div4：div2？
				case Speed5000:	SrioPHYCtrlStruct->PHYCtrl[3] = 0xb3f30000;break;//PBUS_reg_3  不使能分频
				case Speed6250:	SrioPHYCtrlStruct->PHYCtrl[3] = 0xb3f30000;break;//PBUS_reg_3  不使能分频
				default:		SrioPHYCtrlStruct->PHYCtrl[3] = 0xb3f30000;break;
			}

			SrioPHYCtrlStruct->PHYCtrl[0] = 0xb3f00100;//解除serdes refclk的powerdown ：供电
			SrioPHYCtrlStruct->PHYCtrl[0] = 0xb3f00010;//解除serdes 的全局reset       ：退出复位
			while((SrioPHYCtrlStruct->PHYCtrl[8] & 0x000000ff));//PBUS_reg_8//检测rx  tx ack位域只可读，读出值只要不全置1就退出了循环？？？？是否应该全0再退出==0xff
			SrioPHYCtrlStruct->PHYCtrl[7] = 0xff;//写1清除 reg_6的bit[0]--bit[7]
			while((SrioPHYCtrlStruct->PHYCtrl[6] & 0x000000ff));//reg6 表示通道ack下降沿，reg6全是0时，退出循环

			//SrioPHYCtrlStruct->PHYCtrl[7] = 0xffffffff;//写1清除 reg_6的bit[0]--bit[7]
			SrioPCSModeStruct->PLL_2_MODE_1 = 0x080A;//PLL_DIV_PCIE设置为0x16 --refclk_frequency * mac_pcs_pll_div 参考时钟 125*0x16=2750M，
			                                         //后续仍会配置，此处配置为16无意义
			//SrioPHYCtrlStruct->PHYCtrl[7] = 0xffffffff;//写1清除 reg_6的bit[0]--bit[7]   此处不止何种用意
			switch(SrioManulStruct.WorkSpeed)
			{
				case Speed1250:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0814;break;//0x14--对应div=20  125*20*2（双沿）=5gbps   bps=pll/div
				case Speed2500:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0814;break;//0x14--对应div=20  125*20*2（双沿）=5gbps
				case Speed3125:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0819;break;//0x19--对应div=25  125*25*2（双沿）=6.25gbps
				case Speed5000:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0814;break;//0x14--对应div=20  125*20*2（双沿）=5gbps
				case Speed6250:	SrioPCSModeStruct->PLL_2_MODE_1 = 0x0819;break;//0x19--对应div=25  125*25*2（双沿）=6.25gbps
				default:		SrioPCSModeStruct->PLL_2_MODE_1 = 0x0814;break;
			}
			i = 0;
			while(i!=0x000000ff)//PHYCtrl[6]为全FF时才退出
			{
				i = SrioPHYCtrlStruct->PHYCtrl[6];//查询SRIO0 PHY主锁相环是否成功 PBUS_reg_6 继续查询PHYCtrl[6]是否收到ack 下降沿
				i = i & 0x000000ff;
			}

			SrioPHYCtrlStruct->PHYCtrl[7] = 0xffffffff;//再次清除REG6
			switch(SrioManulStruct.WorkSpeed)//配置rx极值带宽配置终端匹配电阻
			{   //配置LANE_PCIE2_MODE_0其中5为 配置tx rx 10bit数据通路 0采样率设置为满速率
				case Speed1250:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x1a05;break;//a-1010 1/4分频  5gbps/4=1.25gbps
				case Speed2500:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x1505;break;//5-0101 1/2分频  5gbps/2=2.5gbps
				case Speed3125:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x1505;break;//5-0101 1/2分配  6.25gbps/2=3.125gbps
				case Speed5000: SrioPCSModeStruct->LANE_2_MODE_0 = 0x2005;break;//0-full rate 5gbps
				case Speed6250:	SrioPCSModeStruct->LANE_2_MODE_0 = 0x7005;break;//full rate  6.25gbps
				default:		SrioPCSModeStruct->LANE_2_MODE_0 = 0x3005;break;
			}
			i = 0;
			while(i!=0x000000ff)//PHYCtrl[6]为全FF时才退出
			{
				i = SrioPHYCtrlStruct->PHYCtrl[6];//查询SRIO0 PHY主锁相环是否成功,两次查询PLL
				i = i & 0x000000ff;
			}
		}
	    //if(SrioManulStruct.WorkSpeed==Speed5000)
	    //{
			//Phy-Rx-Config
			if(SrioInstance0 == SrioInstance)
			{
				S0L0_RX_CFG_0 = 0xFD7;//0x8F17
				S0L1_RX_CFG_0 = 0xFD7;
				S0L2_RX_CFG_0 = 0xFD7;
				S0L3_RX_CFG_0 = 0xFD7;

				S0L0_RX_CFG_5 = 0x18;
				S0L1_RX_CFG_5 = 0x18;
				S0L2_RX_CFG_5 = 0x18;
				S0L3_RX_CFG_5 = 0x18;

				S0L0_RX_CTLE_CTRL= 0x5b7e;
				S0L1_RX_CTLE_CTRL= 0x5b7e;
				S0L2_RX_CTLE_CTRL= 0x5b7e;
				S0L3_RX_CTLE_CTRL= 0x5b7e;

				S0L0_RX_CFG_2= 0x185f;
				S0L1_RX_CFG_2= 0x185f;
				S0L2_RX_CFG_2= 0x185f;
				S0L3_RX_CFG_2= 0x185f;

				S0L0_RX_CFG_3=0x640;
				S0L1_RX_CFG_3=0x640;
				S0L2_RX_CFG_3=0x640;
				S0L3_RX_CFG_3=0x640;

			}
			else
			{
				S1L0_RX_CFG_0 = 0xFD7;
				S1L1_RX_CFG_0 = 0xFD7;
				S1L2_RX_CFG_0 = 0xFD7;
				S1L3_RX_CFG_0 = 0xFD7;

				S1L0_RX_CFG_5 = 0x18;
				S1L1_RX_CFG_5 = 0x18;
				S1L2_RX_CFG_5 = 0x18;
				S1L3_RX_CFG_5 = 0x18;

				S1L0_RX_CTLE_CTRL= 0x5b7e;
				S1L1_RX_CTLE_CTRL= 0x5b7e;
				S1L2_RX_CTLE_CTRL= 0x5b7e;
				S1L3_RX_CTLE_CTRL= 0x5b7e;

				S1L0_RX_CFG_2= 0x185f;
				S1L1_RX_CFG_2= 0x185f;
				S1L2_RX_CFG_2= 0x185f;
				S1L3_RX_CFG_2= 0x185f;

				S1L0_RX_CFG_3=0x640;
				S1L1_RX_CFG_3=0x640;
				S1L2_RX_CFG_3=0x640;
				S1L3_RX_CFG_3=0x640;
			}
	    //}
		//Lane config
		if(LanModeBuf&0x1)
		{
			LanePCSStruct0->CTLIFC_CTRL_0 = 0x2040;//使能tx-boost en 并使数据宽度为20bit//此处为何是20bit位宽? 10位宽度为01  0x20c0
			LanePCSStruct0->CTLIFC_CTRL_1 = 0x0001;//未使能rx pstate 使数据宽度为20bit  0x0003
			LanePCSStruct0->CTLIFC_CTRL_2 = 0x011e;//文档无描述
			LanePCSStruct0->CTLIFC_CTRL_2 = 0x811e;//文档无描述

			if(SrioManulStruct.TxPolarity&0x1)
			{
				LanePCSStruct0->MISC_CFG_1 |= 0x800;//TX反转
			}
			if(SrioManulStruct.RxPolarity&0x1)
			{
				LanePCSStruct0->MISC_CFG_1 |= 0x600;//RX反转 和RX反转使能
			}
		}

		if(LanModeBuf&0x2){
			LanePCSStruct1->CTLIFC_CTRL_0 = 0x2040;
			LanePCSStruct1->CTLIFC_CTRL_1 = 0x0001;//0x0003;
			LanePCSStruct1->CTLIFC_CTRL_2 = 0x011e;
			LanePCSStruct1->CTLIFC_CTRL_2 = 0x811e;

			if(SrioManulStruct.TxPolarity&0x2){
				LanePCSStruct1->MISC_CFG_1 |= 0x800;
			}
			if(SrioManulStruct.RxPolarity&0x2){
				LanePCSStruct1->MISC_CFG_1 |= 0x600;
			}
		}

		if(LanModeBuf&0x4){
			LanePCSStruct2->CTLIFC_CTRL_0 = 0x2040;
			LanePCSStruct2->CTLIFC_CTRL_1 = 0x0001;//0x0003;
			LanePCSStruct2->CTLIFC_CTRL_2 = 0x011e;
			LanePCSStruct2->CTLIFC_CTRL_2 = 0x811e;

			if(SrioManulStruct.TxPolarity&0x4){
				LanePCSStruct2->MISC_CFG_1 |= 0x800;
			}
			if(SrioManulStruct.RxPolarity&0x4){
				LanePCSStruct2->MISC_CFG_1 |= 0x600;
			}
		}

		if(LanModeBuf&0x8){
			LanePCSStruct3->CTLIFC_CTRL_0 = 0x2040;
			LanePCSStruct3->CTLIFC_CTRL_1 = 0x0001;//0x0003;
			LanePCSStruct3->CTLIFC_CTRL_2 = 0x011e;
			LanePCSStruct3->CTLIFC_CTRL_2 = 0x811e;

			if(SrioManulStruct.TxPolarity&0x8){
				LanePCSStruct3->MISC_CFG_1 |= 0x800;
			}
			if(SrioManulStruct.RxPolarity&0x8){
				LanePCSStruct3->MISC_CFG_1 |= 0x600;
			}
		}

		struct SrioLBERTStruct SrioLBERTStruct;
		//将所有lane配置成输出01测试码
		SrioLBERTStruct.En		= 1;
		SrioLBERTStruct.Speed	= SrioManulStruct.WorkSpeed;
		SrioLBERTStruct.PG_MODE	= MODE_Fixed_word;
		SrioLBERTStruct.PM_MODE	= MODE_Fixed_word;
		SrioLBERTStruct.Pattern	= 0x155;//测试码pattern

		if(LanModeBuf&0x1){
			SrioTxCtrl0->PREEMPH_0 = ((PostCursor[0] & 0x1f) << 4) | (PreCursor[0] & 0xf);//配置post 和pre加重方式
			SrioTxCtrl0->CFG_3 = 0x4070;//gain -max
			SrioTxCtrl0->CFG_0 = (Swing[0] & 0x1f) << 3;//配置幅度值
			SrioTxCtrl0->CFG_1 = 0x0b00;//打开各种配置使能
			SrioLBERTStruct.Instance= SrioInstance;
			SrioLBERTStruct.Lane	= 0;
			LBERT_Config(&SrioLBERTStruct);//控制输出数据，使对方rx能进行训练
		}

		if(LanModeBuf&0x2){
			SrioTxCtrl1->PREEMPH_0 = ((PostCursor[1] & 0x1f) << 4) | (PreCursor[1] & 0xf);
			SrioTxCtrl1->CFG_3 = 0x4070;
			SrioTxCtrl1->CFG_0 = (Swing[1] & 0x1f) << 3;
			SrioTxCtrl1->CFG_1 = 0x0b00;
			SrioLBERTStruct.Instance= SrioInstance;
			SrioLBERTStruct.Lane	= 1;
			LBERT_Config(&SrioLBERTStruct);
		}

		if(LanModeBuf&0x4){
			SrioTxCtrl2->PREEMPH_0 = ((PostCursor[2] & 0x1f) << 4) | (PreCursor[2] & 0xf);
			SrioTxCtrl2->CFG_3 = 0x4070;
			SrioTxCtrl2->CFG_0 = (Swing[2] & 0x1f) << 3;
			SrioTxCtrl2->CFG_1 = 0x0b00;
			SrioLBERTStruct.Instance= SrioInstance;
			SrioLBERTStruct.Lane	= 2;
			LBERT_Config(&SrioLBERTStruct);
		}

		if(LanModeBuf&0x8){
			SrioTxCtrl3->PREEMPH_0 = ((PostCursor[3] & 0x1f) << 4) | (PreCursor[3] & 0xf);
			SrioTxCtrl3->CFG_3 = 0x4070;
			SrioTxCtrl3->CFG_0 = (Swing[3] & 0x1f) << 3;
			SrioTxCtrl3->CFG_1 = 0x0b00;
			SrioLBERTStruct.Instance= SrioInstance;
			SrioLBERTStruct.Lane	= 3;
			LBERT_Config(&SrioLBERTStruct);
		}

		if(Version==1)
			SrioPHYCtrlStruct->PHYCtrl[9] = 0xb3f90000;//解除链路复位
		else if(Version==2)
			SrioPHYCtrlStruct->PHYCtrl[0] = 0xb3f00001;//解数字IP的复位//PBUS_reg_0

		if(SrioManulStruct.EnLoopBack){
			*RAB_APB_CSR_Point = 0x200000;//page 0x20
			if(SrioInstance){
				SRIO1_PnPCR |= 0x40000000;
			}else{
				PnPCR |= 0x40000000;//将输出缓冲中的数据全部排出，
			}
		}

		DelaySRIO(10);
		LanePCSStruct0->LBERT_CFG = LanePCSStruct0->LBERT_CFG  & (0xffffffbf) & (0xffffdfff);//误码PG_EN 和PM_EN 配置为0
		LanePCSStruct1->LBERT_CFG = LanePCSStruct1->LBERT_CFG  & (0xffffffbf) & (0xffffdfff);
		LanePCSStruct2->LBERT_CFG = LanePCSStruct2->LBERT_CFG  & (0xffffffbf) & (0xffffdfff);
		LanePCSStruct3->LBERT_CFG = LanePCSStruct3->LBERT_CFG  & (0xffffffbf) & (0xffffdfff);

		unsigned int CsrValue[4];
		unsigned int CountValue = 0;

		*RAB_APB_CSR_Point = 0;//APB页面设置为0
		i = 0;
		SynCount = 0;
		volatile unsigned int *PnESCSR_Point = (volatile unsigned int *)((unsigned int)&PnESCSR + SrioInstance*0x40000);
		switch(SrioManulStruct.LaneMode){//等待链路同步完成
			case lane4x:
				CsrValue[0] = *L0S0CSR_Point;//收发器的状态信息读取LmS0CSR
				CsrValue[2] = *L2S0CSR_Point;//收发器的状态信息读取LmS0CSR只读02 lane即可？
				//Receiver lane sync接收同步
				while( (! ((CsrValue[0] & 0x00200000) & (CsrValue[2] & 0x00200000))) & (SynCount < SrioManulStruct.SynTimeOut))//检测接收同步信号
				{//等待逻辑lane0、2同时syn完成
					i++;
					if(i==1000)
					{
						i = 0;
						SynCount++;
					}
					CsrValue[0] = *L0S0CSR_Point;
					CsrValue[2] = *L2S0CSR_Point;
					if((CsrValue[0] & 0x00200000) | (CsrValue[2] & 0x00200000))
					{
						CountValue++;
						if((CountValue>1000)&((CountValue%20)==0))
						{
							SrioCheckRx(SrioInstance,0,0);//对RX进行复位
							SrioCheckRx(SrioInstance,2,0);
						}
					}
				}
				//printf("PnESCSR_Point is aaaaaaaa 0x%x  0x%x\n",SynCount,i);
				break;
			case lane2xl0l1:
			case lane2xl2l3:
			case lane1xl0:
			case lane1xl1:
			case lane1xl2:
			case lane1xl3:
			//	CsrValue[0] = *L0S0CSR_Point;
			//	while( (! (CsrValue[0] & 0x00200000)) & (SynCount < SrioManulStruct.SynTimeOut)){//等待逻辑lane0syn完成
			//		i++;
			//		if(i==1000){
			//			i = 0;
			//			SynCount++;
			//		}
			//		CsrValue[0] = *L0S0CSR_Point;
			//	}
				//break;


			{
				CsrValue[0] = *L0S0CSR_Point;
				    SynCount = 0;
				    i = 0;
				    printf("Before sync: L0S0CSR=0x%08X, PnESCSR=0x%08X\n", CsrValue[0], *PnESCSR_Point);
				    while( (! (CsrValue[0] & 0x00200000)) && (SynCount < SrioManulStruct.SynTimeOut) ) {
				        i++;
				        if (i == 1000) {
				            i = 0;
				            SynCount++;
				            unsigned int pnes = *PnESCSR_Point;
#if 0
				            printf("L0S0CSR=0x%08X (RLS=%d,RLR=%d,DEN=%d), PnESCSR=0x%08X (PORT_OK=%d,ERR=0x%X), SynCount=%d\n",
				                   CsrValue[0],
				                   (CsrValue[0]>>18)&1,          // RLS
				                   (CsrValue[0]>>19)&1,          // RLR
				                   (CsrValue[0]>>20)&0xF,        // DEN 解码错误计数
				                   pnes,
				                   (pnes>>29)&1,                  // PORT_OK
				                   pnes & 0x3FFFFF,                // 错误位区域
				                   SynCount);
#endif
				            //UART_Print("no lane\n");
				        }
				        CsrValue[0] = *L0S0CSR_Point;
				    }
				    if (CsrValue[0] & 0x00200000) {
				        printf("Lane sync achieved.\n");
				    } else {
				        printf("Lane sync timeout!\n");
				        return 1;
				    }
				    break;
			}
			default:		break;
		}

		SrioPhyCheck(SrioInstance,SrioManulStruct.LaneMode);
//		DelaySRIO(1000);
		flag=Srio_read_PnESCSR(SrioInstance);
		//return SrioPhyCheck(SrioInstance,SrioManulStruct.LaneMode);
		if(0==SrioInstance)
		{
			u32RstCnt_Srio0++;
		}
		else if(1==SrioInstance)
		{
			u32RstCnt_Srio1++;
		}
	}

	ncheckRet = SrioPhyCheck(SrioInstance,SrioManulStruct.LaneMode);

	if (ncheckRet == 1)
	{
		UART_Print("phy timeout. . .\n");
	}
	else if(ncheckRet == 2)
	{
		UART_Print("phy width errot. . .\n");
	}

	return ncheckRet;
}


unsigned int SrioSentLinkReset(unsigned int SrioInstance){
	unsigned int volatile * PnLMREQCSRPoint;
	unsigned int volatile * RAB_APB_CSRPoint;

	if(SrioInstance){
		PnLMREQCSRPoint = &SRIO1_PnLMREQCSR;
		RAB_APB_CSRPoint = &SRIO1_RAB_APB_CSR;
	}else{
		PnLMREQCSRPoint = &PnLMREQCSR;
		RAB_APB_CSRPoint = &RAB_APB_CSR;
	}

	* RAB_APB_CSRPoint = 0;
	* PnLMREQCSRPoint = SrioChangeEnding(0x3);//连续发送4次Link Reset请求
	* PnLMREQCSRPoint = SrioChangeEnding(0x3);
	* PnLMREQCSRPoint = SrioChangeEnding(0x3);
	* PnLMREQCSRPoint = SrioChangeEnding(0x3);
	return 0;
}
