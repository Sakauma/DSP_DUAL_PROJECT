/*******************************************************************************
 *  File Name                       :   CPS1848Cfg_MaintPack.c
 *  Copyright                       :   CCYH,All Rights Reserved.
 *  Module Name                     :   SrioSwitch CPS1848 Maintenance Packet Routing
 * 
 *  CPU                             :   FT-M6678
 *  RTOS                            :   None
 * 
 *  Create Data                     :   2021/02/22
 *  Author/Corporation              :   Zhujian/Application Development Department
 * 
 *  Abstract Description            :   SRIO交换机CPS-1848 维护包配置
 * --------------------------Revision History--------------------------
 * No   Version     Date       Revised By       Item        Description
 *  
 ********************************************************************************/

/*******************************************************************************
 * Debug Switch Section
 ********************************************************************************/



/*******************************************************************************
 * Include File Section
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include "CPS1848Cfg_MaintPack.h"
#include "M6678_SRIO.h"
#include "UARTPrint.h"

#if __I2C
#include "SrioSwitch1848Cfg.h"
#endif
/*******************************************************************************
 * Macro Define Section
 ********************************************************************************/
#define readb(a)			(*(volatile uint8_t *)(a))
#define readw(a)			(*(volatile uint16_t *)(a))
#define readl(a)			(*(volatile uint32_t *)(a))

#define writeb(v,a)			(*(volatile uint8_t *)(a) = (v))
#define writew(v,a)			(*(volatile uint16_t *)(a) = (v))
#define writel(v,a)			(*(volatile uint32_t *)(a) = (v))

//32位数据四个字节大小端转换
#define REVERSE(a) (((a) & 0xff) << 24 | ((a) & 0xff00) << 8 | ((a) & 0xff0000) >> 8 | ((a) & 0xff000000) >> 24)

#define SRIO_MAINTENANCE_ID	0xF0	            //SRIO维护包ID
#define SRIO_SWITCHMAINTENANCE_PORT	0x0B        //SRIO维护包-交换机间转发端口-Port11
#define SRIO_FPAG_PORT	0x00					//FPGA-PORT0




/*******************************************************************************
 * Struct Define Section
 ********************************************************************************/


/*******************************************************************************
 * Protorype Declare Section
 ********************************************************************************/


/*******************************************************************************
 * Global Variable Declare Section
 ********************************************************************************/


/*******************************************************************************
 * File Static Variable Define Section
 ********************************************************************************/
uint8_t g_u8DspIndex = 11; //运行CPS1848-维护包配置模块的DSP序号
uint32_t g_u32DspSrioInstance = 0;   //需要通过SRIO维护包对1848进行配置的DSP SRIO端口号
struct SRIOAPIOStruct g_stSRIOAPIOStruct;   //SRIO APIO端口设置参数(维护包窗口)
DspSrioPortMapInfo_st g_astDspSrioPortMapInfo[FT6678SLT_TESTCHIPCNT+FT6678SLT_MAINTESTCHIPCNT+1]; //FT6678 SLTV2 SRIO端口映射参数

/*******************************************************************************
 * Function Define Section
 ********************************************************************************/

/*******************************************************************************
 *      Function Name           :   CPS1848Cfg_APIOWindowChange
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module SRIO-APIO Windows Change
 *      Param                   :   
 *      long long RIOAddr -- CPS1848交换机-窗口映射基地址 (注意:最小单位0x400)
 *      unsigned int unHopCount -- SRIO维护包-跳数
 * 
 *      Return Code             :   None
 *      Global Variable         :   
 *      g_astDspSrioPortMapInfo - FT6678 SLTV2 SRIO端口映射参数
 *    
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 * 
 *******************************************************************************/
static void CPS1848Cfg_APIOWindowChange(long long RIOAddr, unsigned int unHopCount)
{
    g_stSRIOAPIOStruct.RIOBase = RIOAddr;   //CPS1848交换机窗口映射起始地址
    g_stSRIOAPIOStruct.hopcount = unHopCount; //SRIO维护包跳数

    //SRIO APIO参数设置
	SrioCreatAPIOWindow(g_u32DspSrioInstance,&g_stSRIOAPIOStruct);
}

/*******************************************************************************
 *      Function Name           :   CPS1848Cfg_PortRoutConfiguration
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module Port Rout Configuration
 *      Param                   :   
 *      luint8_t u8SrcPort -- 源端口号
 *      uint8_t u8DestID -- 目的ID
 *      uint8_t u8DestPort -- 目的端口
 *      uint8_t u8HopCount -- 维护包跳数
 * 
 *      Return Code             :   None
 *      Global Variable         :   
 *      g_astDspSrioPortMapInfo - FT6678 SLTV2 SRIO端口映射参数
 *      g_stSRIOAPIOStruct - SRIO APIO端口设置参数(维护包窗口)
 * 
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 * 
 *******************************************************************************/
static int CPS1848Cfg_PortRoutConfiguration(uint8_t u8SrcPort,uint8_t u8DestID, uint8_t u8DestPort, uint8_t u8HopCount)
{
#if __I2C
    SrioSwitch_RoutTableSet(u8SrcPort,u8DestID,u8DestPort);
#else
    uint32_t u32RegAddr,u32RegVaule;
    //Register Offset: 0x010070 - RTE_PORT_SEL
	CPS1848Cfg_APIOWindowChange(0x10070, u8HopCount); 
    u32RegAddr = g_stSRIOAPIOStruct.AXIBase + 0x70;
    u32RegVaule = REVERSE(u8SrcPort + 0x1);
    writel(u32RegVaule, u32RegAddr);
    Delay(1);

    //Register Offset: 0x000070 - RTE_DESTID_CSR
    CPS1848Cfg_APIOWindowChange(0x00, u8HopCount);
    u32RegAddr = g_stSRIOAPIOStruct.AXIBase + 0x70;
    u32RegVaule = REVERSE(u8DestID);
    writel(u32RegVaule, u32RegAddr);
    Delay(1);

    //Register Offset: 0x000074 - RTE_PORT_CSR
    u32RegAddr = g_stSRIOAPIOStruct.AXIBase + 0x74;
    u32RegVaule = REVERSE(u8DestPort);
    writel(u32RegVaule, u32RegAddr);
#endif
    return 0;
}


/*******************************************************************************
 *      Function Name           :   CPS1848Cfg_DeviceConfiguration
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module Variable Init
 *      Param                   :   None
 *      Return Code             :   
 *      0  - Success
 *      -1 - Error
 *      Global Variable         :   
 *      g_astDspSrioPortMapInfo - FT6678 SLTV2 SRIO端口映射参数
 *    
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 * 
 *******************************************************************************/
int CPS1848Cfg_VariableInit()
{
	//DSP SRIO 维护包 APIO参数初始化
    //------------SRIO维护包参数配置-Start
	g_stSRIOAPIOStruct.WindowNub = 0;				//使用的窗口号，每个部件最多可使用16个窗口（0-15）
	g_stSRIOAPIOStruct.DstID = SRIO_MAINTENANCE_ID;	//目的ID -- APIO-维护包ID值
	g_stSRIOAPIOStruct.WindowTYPE = 0;				//窗口类型
	g_stSRIOAPIOStruct.WindowSize = 0x400;			//窗口大小
	g_stSRIOAPIOStruct.AXIBase = SWITCH_APIO_BASEADDR;		//DSP内axi基址
	g_stSRIOAPIOStruct.RIOBase = 0;		//对端器件RIO基址
	g_stSRIOAPIOStruct.Priority = 0;	//优先级
	g_stSRIOAPIOStruct.hopcount = 0;	//跳数
	g_stSRIOAPIOStruct.CRF = 0;			//CRF值，默认为0
    //------------SRIO维护包参数配置-End

    return 0;
}

/*******************************************************************************
 *      Function Name           :   CPS1848cfg_DspMaintAPIOInit
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module Configuration - DSP MaintPack APIO Winodws Init
 *      Param                   :
 *      uint32_t u32SrioInstance -- DSP SRIO端口序号(SrioInstance0-SRIO0 / SrioInstance1-SRIO1)
 *      uint32_t u32WindowNub -- DSP SRIO APRIO窗口序号
 *      uint32_t u32AXIBase -- DSP SRIO AXI总线基地址
 *      Return Code             :   
 *      0  - Success
 *      -1 - Error
 *      Global Variable         :   
 *      g_stSRIOAPIOStruct - DSP SRIO APIO全局配置参数
 *    
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 * 
 *******************************************************************************/
int CPS1848cfg_DspMaintAPIOInit(uint32_t u32SrioInstance, uint32_t u32WindowNub, uint32_t u32AXIBase)
{
    if(u32SrioInstance < 2)
     g_u32DspSrioInstance = u32SrioInstance;
    if(u32WindowNub < 16)
	    g_stSRIOAPIOStruct.WindowNub = u32WindowNub;	//使用的窗口号，每个部件最多可使用16个窗口（0-15）

	g_stSRIOAPIOStruct.AXIBase = u32AXIBase;		//DSP内axi基址

    //DSP SRIO APIO窗口设置
    CPS1848Cfg_APIOWindowChange(0x00,0);

    return 0;
}

/*******************************************************************************
 *      Function Name           :   CPS1848cfg_SetSelfDSPIndex
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module Configuration - Set Module Slef Dsp Serial Num
 *      Param                   :
 *      uint8_t u8DspIndex - 运行当前模块的DSP序号
 *      Return Code             :
 *      Other  - Current Module Self Dsp Num
 *      -1 - Error
 *      Global Variable         :
 *      g_stSRIOAPIOStruct - DSP SRIO APIO全局配置参数
 *      g_u8DspIndex - Module Self Dsp Num
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
int CPS1848cfg_SetSelfDSPIndex(uint8_t u8DspIndex)
{
    if(u8DspIndex > FT6678SLT_TESTCHIPCNT + FT6678SLT_MAINTESTCHIPCNT)
        return -1;

    g_u8DspIndex = u8DspIndex;
    return g_u8DspIndex;
}


/*******************************************************************************
 *      Function Name           :   CPS1848cfg_PortEnableCtl
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module Configuration - 端口打开/关闭设置
 *      Param                   :
 *      uint8_t u8Hop - SRIO维护包跳数:用于访问级联的SRIO交换机
 *      uint8_t u8SwitchPort - SRIO端口序号
 *		uint8_t u8Enable - 端口控制标志 :1-开启/0-关闭
 *
 *      Return Code             :
 *      1 - Success
 *      0 - Fault
 *      -1 - Param Error
 *
 *      Global Variable         :
 *      g_stSRIOAPIOStruct - DSP SRIO APIO全局配置参数
 *      g_u8DspIndex - Module Self Dsp Num
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
int CPS1848cfg_PortEnableCtl(uint8_t u8Hop, uint8_t u8SwitchPort, uint8_t u8Enable)
{
	uint32_t u32RegValue = 0x00;
	uint32_t u32RegAddr = 0x15C + u8SwitchPort * 0x20;

	//1.获取寄存器Port {0..17} Control 1 CSR 当前数值
#if __I2C
	SrioSwitch_ReadRegister(&u32RegValue, u32RegAddr);
#else
	u32RegValue = CPS1848cfg_GetPortStatus(u8SwitchPort,u8Hop);
#endif
	//2.Bit8-PORT_DIS-参数设置
	if(u8Enable)
		u32RegValue &= ~(0x00800000);
	else
		u32RegValue |= 0x00800000;

	//3.数值写入寄存器
#if __I2C
	SrioSwitch_WriteRegister(u32RegValue, u32RegAddr);
#else
	CPS1848cfg_WriteRegister(u32RegValue, u32RegAddr, u8Hop);
#endif

	return 1;
}

/*******************************************************************************
 *      Function Name           :   CSP1848cfg_PortStatusReCheck
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module -- SRIO交换机端口状态寄存器数值获取-判定ERROR位置位状态-清空ERROR状态位
 *      Param                   :
 *      uint8_t u8Port - 交换机端口序号
 *      uint8_t u8SwitchIndex - 交换机序号
 *      Return Code             :
 *      1  -  交换机端口状态无错误
 *      0  -  交换机端口状态存在错误
 *      Global Variable         :
 *      g_stSRIOAPIOStruct - DSP SRIO APIO全局配置参数
 *      g_u8DspIndex - Module Self Dsp Num
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
static int CSP1848cfg_PortStatusReCheck(uint8_t u8Port, uint8_t u8SwitchIndex, uint32_t *pu32RegValue)
{
	uint32_t u32RegValue = 0x00;

#if __I2C
	u32RegValue = SrioSwitch_PortStatus(u8Port);
	//端口状态存在错误
	if((u32RegValue&CPS1848_PORTLINKREG_STATUS) != CPS1848_PORTLINKREG_PORTOK)
	{
		//将寄存器数值-原值写入
		SrioSwitch_WriteRegister(u32RegValue, 0x158+u8Port*0x20);
		//读取清空后的数值
		u32RegValue = SrioSwitch_PortStatus(u8Port);
		//寄存器数值输出
		*pu32RegValue = u32RegValue;
		//再次判定端口状态-错误
		if((u32RegValue & CPS1848_PORTLINKREG_STATUS) != CPS1848_PORTLINKREG_PORTOK)
			return 0;
	}
#else
	u32RegValue = CPS1848cfg_GetPortStatus(u8Port,u8SwitchIndex);
	//端口状态存在错误
	if((u32RegValue&CPS1848_PORTLINKREG_STATUS) != CPS1848_PORTLINKREG_PORTOK)
	{
		//将寄存器数值-原值写入
		CPS1848cfg_WriteRegister(u8SwitchIndex, u32RegValue, 0x158+u8Port*0x20);
		//读取清空后的数值
		u32RegValue = CPS1848cfg_GetPortStatus(u8Port,u8SwitchIndex);
		//寄存器数值输出
		*pu32RegValue = u32RegValue;
		//再次判定端口状态-错误
		if((u32RegValue & CPS1848_PORTLINKREG_STATUS) != CPS1848_PORTLINKREG_PORTOK)
			return 0;
	}

#endif
	//寄存器数值输出
	*pu32RegValue = u32RegValue;

	return 1;
}

/*******************************************************************************
 *      Function Name           :   CPS1848Cfg_GetDspSrioPortMapInfo
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module -- Get Dsp SrioPort Map Info
 *      Param                   :   
 *      uint8_t u8DspIndex [IN] - DSP序号
 *
 *      Return Code             :   
 *      DspSwitchPortMap_pst - DSP端口映射参数指针
 * 
 *      Global Variable         :   
 *      g_astDspSrioPortMapInfo - FT6678 SLTV2 SRIO端口映射参数
 *    
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 * 
 *******************************************************************************/
DspSwitchPortMap_pst CPS1848Cfg_GetDspSrioPortMapInfo(uint8_t u8DspIndex)
{
    if(u8DspIndex < 1 || u8DspIndex > FT6678SLT_TESTCHIPCNT + FT6678SLT_MAINTESTCHIPCNT)
        return 0;

    return g_astDspSrioPortMapInfo + u8DspIndex;
}

/*******************************************************************************
 *      Function Name           :   CPS1848Cfg_DeviceConfiguration
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module -- 1848-1/1848-2设备初始化
 *                                  1.Port0~Port11 端口包发送与接收使能
 *                                  2.路由表配置
 *      Param                   :   None
 *      Return Code             :   
 *      0  - Success
 *      -1 - Error 
 *      Global Variable         :   
 *      g_astDspSrioPortMapInfo - FT6678 SLTV2 SRIO端口映射参数
 *    
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 * 
 *******************************************************************************/
int CPS1848Cfg_DeviceConfiguration()
{
	uint32_t i = 0;
    uint32_t u32RegVaule = 0x00;
    char achTextBuff[64] = {0};

#if __I2C
    SrioSwitch_ReadRegister(&u32RegVaule,0x00000000);
    sprintf(achTextBuff,"CSP1848_ID:%08X\n",u32RegVaule);
#else
    //------------------- 1848-directly connected------------------------
    CPS1848Cfg_APIOWindowChange(0x00,0);

    //CSP1848-ID
    u32RegVaule = readl(g_stSRIOAPIOStruct.AXIBase);
    sprintf(achTextBuff,"CSP1848-%d(直连)_ID:%08X\n",g_astDspSrioPortMapInfo[g_u8DspIndex].u8SwitchIndex, REVERSE(u32RegVaule));
#endif
    printf("%s",achTextBuff);

    //Port0~Port10 端口参数配置
    for(i = 0; i<12; i++)
    {
        //端口发送|接收使能 -- Port {0..17} Control 1 CSR -- Addr:0x00015C + (0x20 * port_num)
#if __I2C
    	SrioSwitch_WriteRegister(0xD0600001,0x15c+0x20*i);
#else
        writel(0x010060d0,(g_stSRIOAPIOStruct.AXIBase + 0x15c+0x20*i));
#endif
        Delay(1);
    }

    //路由表配置-DSP1-Srio0:Port3_ID:0x10->FPGA_Port0
    CPS1848Cfg_PortRoutConfiguration(3, 0x10, SRIO_FPAG_PORT, 0);
    //FPGA_Port0_ID:0x10->DSP1_Srio0_Port3
    CPS1848Cfg_PortRoutConfiguration(SRIO_FPAG_PORT, 0x10, 3, 0);
    //路由表配置-DSP1-Srio1:Port7_ID:0x11->FPGA_Port0
    CPS1848Cfg_PortRoutConfiguration(7, 0x11, SRIO_FPAG_PORT, 0);
    //FPGA_Port0_ID:0x11->DSP1_Srio1_Port7
    CPS1848Cfg_PortRoutConfiguration(SRIO_FPAG_PORT, 0x11, 7, 0);

    return 0;
}

/*******************************************************************************
 *      Function Name           :   CPS1848cfg_WriteRegister
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module -- Register Vaule Write
 *      Param                   :
 		uint8_t unHop -- SRIO 维护包跳数
		unsigned char u8Cps1848Index - CPS1848-交换机序号
		unsigned int u32RegVaule - 寄存器数值
		unsigned int u32RegAddr - 寄存器地址
 *      Return Code             :
 *      1 - Success
 *      0 - Fault
 *
 *      Global Variable         :
 *      g_astDspSrioPortMapInfo - FT6678 SLTV2 SRIO端口映射参数
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
int CPS1848cfg_WriteRegister(uint8_t u8Hop, unsigned int u32RegVaule, unsigned int u32RegAddr)
{
	uint32_t u32RegValue = 0x00;

	//APIO窗口区域调整
	uint32_t u32Hop = u8Hop; //维护包-跳数:DSP SRIO直连的交换机-跳数数值为0 -- 当需要通过维护包访问交换机级联的其他交换机时-跳数数值为级联次数值-注意：交换机级联端口需要配置维护包ID的包路由
	uint32_t u32Offset = (u32RegAddr & 0x3FF);
	uint32_t u32Basic = (u32RegAddr & 0xFFFFFFC00);
	CPS1848Cfg_APIOWindowChange(u32Basic,u32Hop);

	//寄存器数值写入
	writel(u32RegValue,g_stSRIOAPIOStruct.AXIBase + u32Offset);
	return 1;
}

/*******************************************************************************
 *      Function Name           :   CPS1848cfg_GetPortStatus
 *      Create Date             :   2022/11/21
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module -- Get Port Status(Port {0..17} Control 1 CSR) Register Vaule
 *      Param                   :
        uint8_t u8Hop - SRIO维护包跳数
		uint8_t u8Port - CPS1848 SRIO端口号
 *      Return Code             :
 *      uint32_t -- PortStatusRegisterValue
 *      Global Variable         :
 *      g_astDspSrioPortMapInfo - FT6678 SLTV2 SRIO端口映射参数
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
unsigned int CPS1848cfg_GetPortStatus(uint8_t u8Hop, uint8_t u8Port)
{
	uint32_t u32RegAddress,u32RegVaule;
	uint8_t u8SwitchHop = u8Hop;	//维护包跳数

    CPS1848Cfg_APIOWindowChange(0x00,u8SwitchHop);
    Delay(1);
    //Port {0..17} Control 1 CSR -- Register Offset: 0x00015C + (0x20 * port_num)
    u32RegAddress = g_stSRIOAPIOStruct.AXIBase + 0x000158 + (0x20 * u8Port);
    u32RegVaule = readl(u32RegAddress);
    return REVERSE(u32RegVaule);
}

/*******************************************************************************
 *      Function Name           :   CPS1848cfg_PortStatusErrorCheck
 *      Create Date             :   2023/02/23
 *      Author/Corporation      :   ZhuJian
 *      Description             :   CPS1848CFG-Module SRIO端口状态-故障检测-在一点判定次数内端口状态需全对
 *      Param                   :
 *      uint8_t u8DspIndex - DSP序号
 *      uint8_t u8SRIOIndex - DSP SRIO端口序号(ENUM_DSP_SRIO0/ENUM_DSP_SRIO1)
 *      Return Code             :
 *      1  - Status-No Error
 *      0  - Status-Have Error
 *      -1 - Param Error
 *      Global Variable         :
 *      g_stSRIOAPIOStruct - DSP SRIO APIO全局配置参数
 *      g_u8DspIndex - Module Self Dsp Num
 *
 *      File Static Variable    :   None
 *      Function Static Variable:   None
 * -----------------------------------------------------------------------------
 *      Revision History
 *      No.             Data            Revised By          Item        Description
 *
 *******************************************************************************/
int CPS1848cfg_PortStatusErrorCheck(uint8_t u8DspIndex, uint8_t u8SRIOIndex)
{
	int i = 0;
	int n8SwitchPort = -1;
	uint32_t u32RegValue = 0x00;

	if(u8DspIndex == 1)
	{
		n8SwitchPort = (u8SRIOIndex == ENUM_DSP_SRIO0) ? 3 : 7;
	}

	if(n8SwitchPort < 0)
    	return -1;

    for(i = 0; i<SWITCH_PORTERROR_CHECKCNT; i++)
    {
    	//获取DSP序号-SRIO端口对应交换机端口的状态寄存器数值
#if __I2C
		u32RegValue = SrioSwitch_PortStatus(n8SwitchPort);
		//端口状态存在错误
		if((u32RegValue&CPS1848_PORTLINKREG_STATUS) != CPS1848_PORTLINKREG_PORTOK)
		{
			//将寄存器数值-原值写入
			SrioSwitch_WriteRegister(u32RegValue, 0x158+n8SwitchPort*0x20);
			//读取清空后的数值
			u32RegValue = SrioSwitch_PortStatus(n8SwitchPort);
			//再次判定端口状态-错误
			if((u32RegValue & CPS1848_PORTLINKREG_STATUS) != CPS1848_PORTLINKREG_PORTOK)
				break;
		}

#else
		u32RegValue = CPS1848cfg_GetPortStatus(0, n8SwitchPort);
		//端口状态存在错误
		if((u32RegValue&CPS1848_PORTLINKREG_STATUS) != CPS1848_PORTLINKREG_PORTOK)
		{
			//将寄存器数值-原值写入
			CPS1848cfg_WriteRegister(0, u32RegValue, 0x158+n8SwitchPort*0x20);
			//读取清空后的数值
			u32RegValue = CPS1848cfg_GetPortStatus(0, n8SwitchPort);
			//再次判定端口状态-错误
			if((u32RegValue & CPS1848_PORTLINKREG_STATUS) != CPS1848_PORTLINKREG_PORTOK)
				break;
		}
#endif
    }

    //SrioSwitch_ShowRegister(pstDspMapInfo->u8SwitchIndex-1, 0x15C + 0x20*n8SwitchPort);
    printf("DSP%d-SRIO%d LinkStatus:0x%08X\n",u8DspIndex,u8SRIOIndex,u32RegValue);

    return (SWITCH_PORTERROR_CHECKCNT ==  i);
}
