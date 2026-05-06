#include "C6x.h"
#include "stdio.h"
#include "InitPLL.h"
#include "M6678_SRIO.h"
#include "M6678_INTC.h"
#include "InitPLL.h"
#include "M6678SrioStruct.h"
#include "SysInit.h"
#include "string.h"
#include "UART.h"
#include "UARTPrint.h"
#include "Chip_Initial.h"

struct SRIOinit SRIO0init;
struct SRIOinit SRIO1init;
struct SrioManulStruct Srio0ManulStruct;
struct SrioManulStruct Srio1ManulStruct;
struct SRIODMA SRIODMA;
struct SrioDescriptorStruct SrioDescriptorStruct;
struct SRIODoorBellStruct SRIODoorBellStruct;
struct SrioLBERTStruct SrioLBERTStruct;

unsigned int i,tscl,tsch,disccount;
volatile unsigned long long Time0,Time1,Time2,Time3;
unsigned int Version;

volatile unsigned int Srio0WDMAFlag=0;
volatile unsigned int Srio0RDMAFlag=0;
volatile unsigned int Srio1WDMAFlag=0;
volatile unsigned int Srio1RDMAFlag=0;
unsigned int SRIO0IntCnt=0;
unsigned int SRIO1IntCnt=0;
volatile unsigned int SRIO0MISC=0;
volatile unsigned int SRIO1MISC=0;
unsigned int SRIO0DBInfoBuffer[32];
unsigned int SRIO1DBInfoBuffer[32];
unsigned int SRIO0DBNub=0;
unsigned int SRIO1DBNub=0;

unsigned long long GetCountTime(){
	tscl = TSCL;
	tsch = TSCH;
	return _itoll(tsch,tscl);
}
void Delay(unsigned int cnt){
	int i,j;
	for(i=0;i<cnt;i++)
	{
		for(j=0;j<2000;j++)
			asm(" nop ");
	}
}

#define TestLen (0x100000/4-1)	//word	(1M-4)B
//#define TestLen (0x2000/4)		//word	1KB
//#define TestLen (0x400/4)		//word	1KB
//#define TestLen (0x4000/4)	//word	16KB
//#define TestLen (512/4-1)	//127word
//#define TestLen (256/4)	//64word
//#define TestLen (256/4-1)	//63word
//#define TestLen (128/4-1)	//31word
//#define TestLen (64/4-1)	//15word
//#define TestLen (32/4-1)	//7word
//#define TestLen (16/4-1)	//3word
//#define TestLen (8/4-1)	//1word	4B
double TestLenDouble = TestLen;

//Test space
//#pragma DATA_SECTION(SrioDescriptorReg, ".MSMC3");
//#pragma DATA_ALIGN(SrioDescriptorReg,8);
//struct SrioDescriptorReg SrioDescriptorReg[6];
//#pragma DATA_SECTION(SrioDescriptorRegR, ".MSMC3");
//#pragma DATA_ALIGN(SrioDescriptorRegR,8);
//struct SrioDescriptorReg SrioDescriptorRegR[6];
//#pragma DATA_SECTION(TestData, ".MSMC0");
//#pragma DATA_SECTION(Dst0, ".MSMC1");
//#pragma DATA_SECTION(Dst1, ".MSMC2");
//unsigned int Dst0[TestLen];
//unsigned int Dst1[TestLen];
//unsigned int TestData[]={
//0x45506ecf, 0xfef4bf1e, 0xb855defc, 0x8d27ec68, 0xcd9ecca3, 0xd429d7e7, 0x696c8f7d, 0xb713f50d,
//0xc4857e2c, 0x01b5c9be, 0xb44f11be, 0x7ea43aaf, 0x3183a276, 0x82c9d936, 0xf4853ee4, 0x1c5efc08,
//0x408fb3ba, 0x96238b4f, 0xa9d14ef4, 0x627f8ed9, 0x136df953, 0xfd362199, 0xfdf8de10, 0xcf69f62e,
//0x8692233e, 0x420a884e, 0x1f63f612, 0x5158cae5, 0x31641f36, 0x3258455e, 0xb9851a11, 0x1d9e6506,
//0x50e187b2, 0x67aeaa4e, 0xa7e1c059, 0xf0befb2e, 0x7d7f9de8, 0xdc17e99c, 0x746a10fa, 0x434c4988,
//0x6558542d, 0xe3cc7d45, 0x72e6052b, 0x049e4de5, 0x5ebabe0a, 0x8d64cc90, 0x52b300e5, 0x372e3c10,
//0x8c110c31, 0xe6de613d, 0x8543518f, 0x598a6850, 0x8bb77245, 0x3dd037c4, 0xf53fddea, 0xea4f0a20,
//0xcad7fecf, 0xd15adbbd, 0x814666df, 0x0d163882, 0x1548c30d, 0x1118d646, 0x7e3a479e, 0xf05f43e0,
//0x59f0d279, 0xa7cbd041, 0x536f3494, 0x0c1930e3, 0x9055fffa, 0xecb3cf54, 0x37181f54, 0xf18a4714,
//0x1b4defdf, 0x7d37ad89, 0xe5e466ea, 0x55b80cc9, 0xaf445323, 0xb23812ac, 0x9c74c960, 0xec372592,
//0x43b77d2c, 0x0232d7ad, 0xe771d3a8, 0x9e57217d, 0xb5227f6f, 0x01a92804, 0xff38d0d1, 0xf6896fb3,
//0x37791806, 0x5658426f, 0x120ea179, 0xd2dc3de0, 0x0f86ca96, 0x6706bd7c, 0x1943960c, 0x6c327ee5,
//0x5246c8e7, 0x38e0e6db, 0xcedd6c89, 0xb935f67a, 0xb884c8bd, 0x7b08f31f, 0xa2477300, 0xdbadaccb,
//0xe10ad834, 0x77075a3f, 0x4cf313bc, 0xc5431276, 0xa8af0e9e, 0x048cd7d3, 0x0c576489, 0x81ce488c,
//0x5d755a2c, 0x29a9f5a2, 0x13f13a54, 0xfababaea, 0xcdac8d67, 0x6e829cda, 0x67af0806, 0x8a1e430c,
//0x2f5c9969, 0x034ec2fa, 0x26639b99, 0xc772dda5, 0x820ee814, 0x59d3e69d, 0xf1dbecdb, 0xf41498e0,
//0x1ec6fd86, 0x56637793, 0xd2915287, 0xda023bd7, 0x93d9a01f, 0x71347f27, 0xbf92bf2e, 0x29195a82,
//0xa3b1fa2a, 0x10d0a011, 0x204c2569, 0xb7963f62, 0x1e2be162, 0xfc1d0a49, 0x0808f65e, 0x0bdebdeb,
//0x69f0bdc3, 0xe78a20ad, 0x4cfe7603, 0x6cc2600d, 0x59579fe2, 0x77096e67, 0xe8735a99, 0xd2f3beaf,
//0x1bd7628c, 0xe761f483, 0x0031b989, 0xff12b95a, 0xfbb34ec7, 0x2a8c9376, 0xd04453cc, 0x55429750,
//0xbcf9f450, 0x5ded2ad7, 0x1d0ef835, 0xaa24bee3, 0x31beed50, 0x6e99f608, 0x4190ca22, 0x9086d7a2,
//0xdc726270, 0x8dfefcf2, 0xe264e2ff, 0xac20294a, 0x2bc86021, 0xfcee87ba, 0x4334ff88, 0xfefa48a4,
//0x4a2e16db, 0x6e237409, 0x5184cc71, 0x021aabb7, 0xc50792f1, 0x59783f08, 0xbfe809c7, 0x3df7ca30,
//0xfe062e75, 0xb0865acf, 0x250c2213, 0x2192f326, 0xa4d27ab3, 0xaca08209, 0x75da48b2, 0x222a6dc6,
//0xe8557e03, 0x2eb78f74, 0x35fd23d9, 0x613e12be, 0x8fe886e5, 0x417bb8c6, 0x11851635, 0x3f30ef5b,
//0xeb8abe83, 0x16a1d88d, 0x42308867, 0xb774be44, 0x8a3194c3, 0x2baded8e, 0x9db7d077, 0x62af7298,
//0xd9439a1a, 0xed0c5c9c, 0x0120c9c6, 0x21657c67, 0x06b5ca93, 0x1ea3139b, 0x688518cb, 0xa2f7489e,
//0xb6ba7385, 0xb88cdbc2, 0xff6db1e1, 0x8a886842, 0xafed831c, 0xa3ce9436, 0xf32e60e3, 0x395b4376,
//0xcb6657a1, 0xe8bb225e, 0x252380b1, 0xd29aae5b, 0xabc53e59, 0x92928197, 0xd38fbfd7, 0x36e8e254,
//0xcf33add1, 0x4eee3a85, 0xb7419001, 0x662cc6bf, 0xfbf5b9cb, 0x8c1cc995, 0xef4fa28a, 0x0ef06161,
//0xb429d250, 0x6d97a54a, 0xd8448e98, 0x5d03e2fa, 0xa66b9232, 0x0a49f117, 0x2f34acce, 0x0adc2bcc,
//0xa909af4f, 0xe9935211, 0xf9f1b22e, 0xc0d72a86, 0x0daf0dd7, 0x60822621, 0x616d80e4, 0x5eb47d06
//};
