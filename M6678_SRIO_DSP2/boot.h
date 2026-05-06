/*
 * boot.h
 *
 *  Created on: 2022-12-9
 *      Author: jack
 */

#ifndef BOOT_H_
#define BOOT_H_
/*******************
 * ProgramSeclectCode说明
 * bit28-bit31:测试程序选择
 * bit8-bit27:保留
 * bit0-bit7:测试项选择
 */
#pragma pack(1)
struct BootControl
{
	unsigned short PacketHead;//0xed90
	unsigned short PacketLen;//0x0010
	unsigned short DataType;//0x0010
	unsigned char Res0;//0xFF
	unsigned int ProgramSeclectCode;
	unsigned int MacAddr;
	unsigned char Res1;//0xFF
	unsigned short PacketEnd;//0x7ef5
};
struct MoveResult
{
	unsigned short PacketHead;//0xed90
	unsigned short PacketLen;//0x000A
	unsigned short DataType;//0x0030
	unsigned char Result;
	unsigned char Res;//0xFF
	unsigned short PacketEnd;//0x7ef5
};
struct TestResult
{
	unsigned short PacketHead;//0x90EB
	unsigned short PacketLen;//0x0011
	unsigned short DataType;//0x0020
	unsigned char Res1;//0xFF
	unsigned char Res2;//0xFF
	unsigned char Res[4];
	unsigned int  TestResult;
	unsigned char Res3;//0xFF
	unsigned short PacketEnd;//0x7ef5
};
#pragma pack()
#define TestItemAddr	(*(volatile unsigned int*)(0x10800000))
#endif /* BOOT_H_ */
