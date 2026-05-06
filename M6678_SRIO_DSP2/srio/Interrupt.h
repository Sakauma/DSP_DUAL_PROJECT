
/*******************************************************************************
*文件名：Interrupt.h
*作    用： 用于中断设置的各种接口函数的声明和寄存器宏定义。
*版    本：Version 1.0  创建于2018.03.12
*******************************************************************************/
#ifndef INTERRUPT_H_
#define INTERRUPT_H_

extern void C6678_Int_GlobalEnable();
extern int  C6678_CoreInt_Init();
extern int  C6678_ChipInt_Init(char ChipIntNum);
extern void C6678_CoreInt_Set(int Evt_Num, int Int_Num);
extern void C6678_ChipInt_Set(int Numb,int Sys_Evt, int Host_Int);
extern void Interrupt_Service_Table(int ISR_Addr);

///////////////INTC/////////////////////////////////////////////////
#define EVTCLR0      0x01800040
#define EVTCLR1      0x01800044
#define EVTCLR2      0x01800048
#define EVTCLR3      0x0180004c
#define INTMUX_ADDR  0x01800100

///////////////CIC/////////////////////////////////////////////////
#define CIC0_BASE    0x02600000
#define CIC1_BASE    0x02604000
#define CIC2_BASE    0x02608000
#define CIC3_BASE    0x0260c000
#define STATUS_CLR_INDEX_REG_offset        0x24
#define GLOBAL_ENABLE_HINT_REG_offset      0x10
#define HINT_ENABLE_SET_INDEX_REG_offset   0x34
#define ENABLE_SET_INDEX_REG_offset        0x28
#define ENA_STATUS_REG0_offset             0x280
#define ENA_STATUS_REG1_offset             0x284
#define ENA_STATUS_REG2_offset             0x288
#define CH_MAP_REG0                        0x400

#endif /* INTERRUPT_H_ */
