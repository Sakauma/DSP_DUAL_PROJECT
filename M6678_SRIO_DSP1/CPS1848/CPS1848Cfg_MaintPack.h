/*******************************************************************************
 *  File Name                       :   CPS1848Cfg_MaintPack.h
 *  Copyright                       :   CCYH,All Rights Reserved.
 *  Module Name                     :   SrioSwitch CPS1848 Maintenance Packet Routing
 * 
 *  CPU                             :   FT-M6678
 *  RTOS                            :   None
 * 
 *  Create Data                     :   2022/11/21
 *  Author/Corporation              :   Zhujian/Application Development Department
 * 
 *  Abstract Description            :   SRIO交换机CPS-1848 维护包配置
 * --------------------------Revision History--------------------------
 * No   Version     Data       Revised By       Item        Description
 *  
 ********************************************************************************/

/*******************************************************************************
 * Multi-Include-Prevent Section
 ********************************************************************************/
#ifndef __CPS1848CFG_MAINTPACK_H
#define __CPS1848CFG_MAINTPACK_H

/*******************************************************************************
 * Debug Switch Section
 ********************************************************************************/
#define __I2C		1	//本文件中SRIO交换机接口类型: 1:使用I2C接口访问交换机 / 0:使用SRIO接口 维护包访问交换机(前提:DSP SRIO与交换机链接正常)

/*******************************************************************************
 * Include File Section
 ********************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Macro Define Section
 ********************************************************************************/
#define FT6678SLT_TESTCHIPCNT                       10      	//被测芯片数量
#define FT6678SLT_MAINTESTCHIPCNT                   1       	//陪测芯片数量
#define SWITCH_APIO_BASEADDR						0x02980000	//SRIO APIO维护包映射地址区域值(DSP存储空间)
#define SWITCH_PORTERROR_CHECKCNT					6			//交换机端口状态正确检测次数
#define SWITCH_TRANSMISSIONLINK_ERRORCHECKCNT		3			//SRIO传输链路-端口状态错误检测次数


#define CPS1848_PORTLINKREG_STATUS			(uint32_t)(0x00020202)
#define CPS1848_PORTLINKREG_PORTOK			(uint32_t)(0x00000002)
/*******************************************************************************
 * Struct Define Section
 ********************************************************************************/

/*--------------------78SLT V2 DSP SRIO端口序号-------------------------*/
enum SLT_DSPSRIO
{
	ENUM_DSP_SRIO0 = 0,
	ENUM_DSP_SRIO1 = 1
};

/*--------------------78SLT V2 SRIO端口参数-------------------------*/
typedef struct FT6678SLT_DSPSRIOPORTMAPINFO
{
	uint8_t u8InitFlag; //参数初始化标志
    uint8_t u8Srio0SelfID; //DSP SRIO0端口ID
    uint8_t u8Srio1SelfID; //DSP SRIO1端口ID
	int8_t n8Srio0Port;	//SRIO0在交换机上连接的端口号
	int8_t n8Srio1Port;	//SRIO1在交换机上连接的端口号
}DspSrioPortMapInfo_st,*DspSwitchPortMap_pst;


/*******************************************************************************
 * Protorype Declare Section
 ********************************************************************************/
extern int CPS1848Cfg_VariableInit();
extern int CPS1848Cfg_DeviceConfiguration();
extern int CPS1848cfg_DspMaintAPIOInit(uint32_t u32SrioInstance, uint32_t u32WindowNub, uint32_t u32AXIBase);
extern int CPS1848cfg_SetSelfDSPIndex(uint8_t u8DspIndex);
extern int CPS1848cfg_PortEnableCtl(uint8_t u8Hop, uint8_t u8SwitchPort, uint8_t u8Enable);

extern int CPS1848cfg_WriteRegister(uint8_t u8Hop, unsigned int u32RegVaule, unsigned int u32RegAddr);
unsigned int CPS1848cfg_GetPortStatus(uint8_t u8Port,uint8_t u8Hop);
extern DspSwitchPortMap_pst CPS1848Cfg_GetDspSrioPortMapInfo(uint8_t u8DspIndex);
extern int CPS1848cfg_PortStatusErrorCheck(uint8_t u8DspIndex, uint8_t u8SRIOIndex);

#endif
