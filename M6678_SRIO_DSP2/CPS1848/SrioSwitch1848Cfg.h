/*
 * SrioSwitch1848Cfg.h
 *
 *  Created on: 2022年11月14日
 *      Author: Administrator
 */

#ifndef SRIOSWITCH1848CFG_H_
#define SRIOSWITCH1848CFG_H_

#include <stdint.h>


/*--------------------板卡硬件参数结构体声明-------------------------*/
typedef struct SRIOSWITCHCARDINFO
{
	unsigned char u8InfoInif;		//参数配置标志
	unsigned char u8UartisOpen;		//串口端口打开标志
	unsigned char u8SwitchI2CAddr;	//SRIO交换机I2C设备地址
	unsigned int u32BaudRate;		//DSP与FPGA串口通信波特率
}SrioSwitchCardInfo_st,*SrioSwitchCardInfo_pst;


/*--------------------端口速率枚举参数-------------------------*/
enum SrioSwitchSpeed
{
	Speed_1_25 = 0,
	Speed_2_5 = 1,
	Speed_3_125 = 2,
	Speed_5 = 3,
	Speed_6_25 = 4
};

#define SRIOSWITCH_CETC54_1848  0

extern int SrioSwitch_InterfaceInit(unsigned char u8SwitchI2CAddr, unsigned int unBaudRate);		//SRIO交换机串口配置模块-初始化
extern int SrioSwitch_ReadRegister(unsigned int *pu32RegVaule, unsigned int u32RegAddr);	//SRIO交换机串口配置模块-寄存器读取
extern int SrioSwitch_WriteRegister(unsigned int u32RegVaule, unsigned int u32RegAddr);   //SRIO交换机配置模块-寄存器赋值
extern int SrioSwitch_ShowRegister(unsigned int u32RegAddr);	//SRIO交换机配置模块-寄存器数值打印
extern int SrioSwitch_PortStatus(unsigned int unPort); //SRIO交换机端口状态
extern int SrioSwitch_NMS1800SerdesInit(); //SRIO交换机-NMS1848-Serdes初始化
extern int SrioSwitch_CETC54_1848Init(); //SRIO交换机-CETC54-1848-初始化

extern void SrioSwitch_Change_Speed(unsigned char speed);
extern int SrioSwitch_PRBSModeSet(unsigned int unPort);
extern void SrioSwitch_GetPRBSErrorCnt(unsigned int unPort);
extern void SrioSwitch_ShowLaneStatus(unsigned char uchLane);
extern void SrioSwitch_ShowPortStatus(unsigned char uchPort);

extern void SrioSwitch_SerdesSet_AMP(unsigned char u8Lane, unsigned char u8AmpCtl);
extern void SrioSwitch_SerdesSet_NEG(unsigned char u8Lane, unsigned char u8NegCtl);
extern void SrioSwitch_SerdesSet_Pos(unsigned char u8Lane, unsigned char u8PosCtl);
extern void SrioSwitch_Reset(unsigned char u8Cps1848Index);
extern void SrioSwitch_PortReset(unsigned int unPort);

extern void SrioSwitch_PortWidthOvrd(unsigned int unPort, unsigned char uchWidth);
extern void SrioSwitch_PortForward(unsigned int unPort);
extern void SrioSwitch_RoutTableSet(unsigned char u8SrcPort,unsigned char u8DestID,unsigned char u8DesPort);

extern void Delay(unsigned int cnt);

#endif /* SRIOSWITCH1848CFG_H_ */
