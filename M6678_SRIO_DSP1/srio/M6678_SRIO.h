/*
 * M6678_SRIO.h
 *
 *  Created on: 2017-1-17
 */

#ifndef M6678_SRIO_H_
#define M6678_SRIO_H_

#define SRIOPLLCTL0			*(unsigned int volatile *)(0x02620338)
#define SRIOPLLCTL1			*(unsigned int volatile *)(0x0262033C)
#define SRIOPLLCMD			*(unsigned int volatile *)(0x02310170)
#define SRIOPLLC_GMACII_MUX	*(unsigned int volatile *)(0x02310174)

#define PTCMD			*(unsigned int volatile *)(0x02350120)

#define PSC_MDCTL11		*(unsigned int volatile *)(0x02350a2c)//srio 0
#define PSC_MDCTL13		*(unsigned int volatile *)(0x02350a34)//srio 1

#define PSC_MDSTAT11	*(unsigned int volatile *)(0x0235082c)//srio 0
#define PSC_MDSTAT13	*(unsigned int volatile *)(0x02350834)//srio 1

#define S0L0_PCS_LBERT_CFG   	*(unsigned int volatile *)(0x0291E010)
#define S0L1_PCS_LBERT_CFG   	*(unsigned int volatile *)(0x0291E410)
#define S0L2_PCS_LBERT_CFG   	*(unsigned int volatile *)(0x0291E810)
#define S0L3_PCS_LBERT_CFG   	*(unsigned int volatile *)(0x0291EC10)

#define S1L0_PCS_LBERT_CFG   	*(unsigned int volatile *)(0x0295E010)
#define S1L1_PCS_LBERT_CFG   	*(unsigned int volatile *)(0x0295E410)
#define S1L2_PCS_LBERT_CFG   	*(unsigned int volatile *)(0x0295E810)
#define S1L3_PCS_LBERT_CFG   	*(unsigned int volatile *)(0x0295EC10)

#define PBUS_ctrl_0   	*(unsigned int volatile *)(0x02900000)
#define PBUS_ctrl_1   	*(unsigned int volatile *)(0x02900004)
#define PBUS_ctrl_2   	*(unsigned int volatile *)(0x02900008)
#define PBUS_ctrl_3   	*(unsigned int volatile *)(0x0290000c)
#define PBUS_ctrl_4   	*(unsigned int volatile *)(0x02900010)
#define PBUS_ctrl_5   	*(unsigned int volatile *)(0x02900014)
#define PBUS_ctrl_6   	*(unsigned int volatile *)(0x02900018)
#define PBUS_ctrl_7   	*(unsigned int volatile *)(0x0290001c)
#define PBUS_ctrl_8   	*(unsigned int volatile *)(0x02900020)
#define PBUS_ctrl_9   	*(unsigned int volatile *)(0x02900024)
#define PBUS_ctrl_10  	*(unsigned int volatile *)(0x02900028)
#define PBUS_ctrl_11  	*(unsigned int volatile *)(0x02920504)
#define PBUS_ctrl_hide	*(unsigned int volatile *)(0x02900030)


//GRIO bigendian page0
#define DIDCAR         	*(unsigned int volatile *)(0x02920800)
#define DICAR         	*(unsigned int volatile *)(0x02920804)
#define AIDCAR         	*(unsigned int volatile *)(0x02920808)
#define AICAR         	*(unsigned int volatile *)(0x0292080C)
#define PEFCAR         	*(unsigned int volatile *)(0x02920810)
#define BDIDCSR         *(unsigned int volatile *)(0x02920860)
#define PnLMREQCSR      *(unsigned int volatile *)(0x02920940)
#define PnLMRESPCSR     *(unsigned int volatile *)(0x02920944)
#define PnLASCSR	    *(unsigned int volatile *)(0x02920948)
#define PnESCSR      	*(unsigned int volatile *)(0x02920958)
#define PnCCSR      	*(unsigned int volatile *)(0x0292095c)
#define PnERTCSR     	*(unsigned int volatile *)(0x02920c6c)
#define LTLEDCSR     	*(unsigned int volatile *)(0x02920e08)
#define LTLEECSR     	*(unsigned int volatile *)(0x02920e0c)

//page 0x20
#define PnPCR      		*(unsigned int volatile *)(0x02920940)


//RIO
#define RAB_CTRL                      *(unsigned int volatile *)(0x02920008)
#define RAB_RST_CTRL				  *(unsigned int volatile *)(0x02920018)
#define RAB_APB_CSR                   *(unsigned int volatile *)(0x02920030)
#define RAB_INTR_ENAB_GNRL            *(unsigned int volatile *)(0x02920040)
#define RAB_INTR_ENAB_APIO            *(unsigned int volatile *)(0x02920044)
#define RAB_INTR_ENAB_RPIO            *(unsigned int volatile *)(0x02920048)
#define RAB_INTR_ENAB_WDMA            *(unsigned int volatile *)(0x0292004c)
#define RAB_INTR_ENAB_RDMA            *(unsigned int volatile *)(0x02920050)
#define RAB_INTR_ENAB_MISC            *(unsigned int volatile *)(0x0292005c)
#define RAB_INTR_STAT_GNRL            *(unsigned int volatile *)(0x02920060)
#define RAB_INTR_STAT_APIO            *(unsigned int volatile *)(0x02920064)
#define RAB_INTR_STAT_RPIO            *(unsigned int volatile *)(0x02920068)
#define RAB_INTR_STAT_WDMA            *(unsigned int volatile *)(0x0292006c)
#define RAB_INTR_STAT_RDMA            *(unsigned int volatile *)(0x02920070)
#define RAB_INTR_STAT_MISC            *(unsigned int volatile *)(0x0292007c)
#define RAB_RIO_PIO_CTRLn             *(unsigned int volatile *)(0x02920080)
#define RAB_RIO_AMAP_LUT0             *(unsigned int volatile *)(0x02920100)
#define RAB_RIO_AMAP_LUT1             *(unsigned int volatile *)(0x02920104)
#define RAB_RIO_AMAP_LUT2             *(unsigned int volatile *)(0x02920108)
#define RAB_RIO_AMAP_LUT3             *(unsigned int volatile *)(0x0292010c)
#define RAB_RIO_AMAP_LUT4             *(unsigned int volatile *)(0x02920110)
#define RAB_RIO_AMAP_LUT5             *(unsigned int volatile *)(0x02920114)
#define RAB_RIO_AMAP_LUT6             *(unsigned int volatile *)(0x02920118)
#define RAB_RIO_AMAP_LUT7             *(unsigned int volatile *)(0x0292011c)
#define RAB_RIO_AMAP_LUT8             *(unsigned int volatile *)(0x02920120)
#define RAB_RIO_AMAP_LUT9             *(unsigned int volatile *)(0x02920124)
#define RAB_RIO_AMAP_LUT10            *(unsigned int volatile *)(0x02920128)
#define RAB_RIO_AMAP_LUT11            *(unsigned int volatile *)(0x0292012c)
#define RAB_RIO_AMAP_LUT12            *(unsigned int volatile *)(0x02920130)
#define RAB_RIO_AMAP_LUT13            *(unsigned int volatile *)(0x02920134)
#define RAB_RIO_AMAP_LUT14            *(unsigned int volatile *)(0x02920138)
#define RAB_RIO_AMAP_LUT15            *(unsigned int volatile *)(0x0292013c)
#define RAB_RIO_AMAP_IDSL             *(unsigned int volatile *)(0x02920140)
#define RAB_RIO_AMAP_BYPS             *(unsigned int volatile *)(0x02920144)
#define RAB_APIO_N_CTRL               *(unsigned int volatile *)(0x02920180)
#define RAB_APIO_AMAP_0_CTRL          *(unsigned int volatile *)(0x02920200)
#define RAB_APIO_AMAP_0_SIZE          *(unsigned int volatile *)(0x02920204)
#define RAB_APIO_AMAP_0_ABAR          *(unsigned int volatile *)(0x02920208)
#define RAB_APIO_AMAP_0_RBAR          *(unsigned int volatile *)(0x0292020C)
#define RAB_APIO_AMAP_1_CTRL          *(unsigned int volatile *)(0x02920210)
#define RAB_APIO_AMAP_1_SIZE          *(unsigned int volatile *)(0x02920214)
#define RAB_APIO_AMAP_1_ABAR          *(unsigned int volatile *)(0x02920218)
#define RAB_APIO_AMAP_1_RBAR          *(unsigned int volatile *)(0x0292021C)
#define RAB_APIO_AMAP_2_CTRL          *(unsigned int volatile *)(0x02920220)
#define RAB_APIO_AMAP_2_SIZE          *(unsigned int volatile *)(0x02920224)
#define RAB_APIO_AMAP_2_ABAR          *(unsigned int volatile *)(0x02920228)
#define RAB_APIO_AMAP_2_RBAR          *(unsigned int volatile *)(0x0292022C)
#define RAB_APIO_AMAP_3_CTRL          *(unsigned int volatile *)(0x02920230)
#define RAB_APIO_AMAP_3_SIZE          *(unsigned int volatile *)(0x02920234)
#define RAB_APIO_AMAP_3_ABAR          *(unsigned int volatile *)(0x02920238)
#define RAB_APIO_AMAP_3_RBAR          *(unsigned int volatile *)(0x0292023C)
#define RAB_APIO_AMAP_4_CTRL          *(unsigned int volatile *)(0x02920240)
#define RAB_APIO_AMAP_4_SIZE          *(unsigned int volatile *)(0x02920244)
#define RAB_APIO_AMAP_4_ABAR          *(unsigned int volatile *)(0x02920248)
#define RAB_APIO_AMAP_4_RBAR          *(unsigned int volatile *)(0x0292024C)
#define RAB_APIO_AMAP_5_CTRL          *(unsigned int volatile *)(0x02920250)
#define RAB_APIO_AMAP_5_SIZE          *(unsigned int volatile *)(0x02920254)
#define RAB_APIO_AMAP_5_ABAR          *(unsigned int volatile *)(0x02920258)
#define RAB_APIO_AMAP_5_RBAR          *(unsigned int volatile *)(0x0292025C)
#define RAB_APIO_AMAP_6_CTRL          *(unsigned int volatile *)(0x02920260)
#define RAB_APIO_AMAP_6_SIZE          *(unsigned int volatile *)(0x02920264)
#define RAB_APIO_AMAP_6_ABAR          *(unsigned int volatile *)(0x02920268)
#define RAB_APIO_AMAP_6_RBAR          *(unsigned int volatile *)(0x0292026C)
#define RAB_APIO_AMAP_7_CTRL          *(unsigned int volatile *)(0x02920270)
#define RAB_APIO_AMAP_7_SIZE          *(unsigned int volatile *)(0x02920274)
#define RAB_APIO_AMAP_7_ABAR          *(unsigned int volatile *)(0x02920278)
#define RAB_APIO_AMAP_7_RBAR          *(unsigned int volatile *)(0x0292027C)
#define RAB_APIO_AMAP_8_CTRL          *(unsigned int volatile *)(0x02920280)
#define RAB_APIO_AMAP_8_SIZE          *(unsigned int volatile *)(0x02920284)
#define RAB_APIO_AMAP_8_ABAR          *(unsigned int volatile *)(0x02920288)
#define RAB_APIO_AMAP_8_RBAR          *(unsigned int volatile *)(0x0292028C)
#define RAB_APIO_AMAP_9_CTRL          *(unsigned int volatile *)(0x02920290)
#define RAB_APIO_AMAP_9_SIZE          *(unsigned int volatile *)(0x02920294)
#define RAB_APIO_AMAP_9_ABAR          *(unsigned int volatile *)(0x02920298)
#define RAB_APIO_AMAP_9_RBAR          *(unsigned int volatile *)(0x0292029C)
#define RAB_APIO_AMAP_10_CTRL         *(unsigned int volatile *)(0x029202A0)
#define RAB_APIO_AMAP_10_SIZE         *(unsigned int volatile *)(0x029202A4)
#define RAB_APIO_AMAP_10_ABAR         *(unsigned int volatile *)(0x029202A8)
#define RAB_APIO_AMAP_10_RBAR         *(unsigned int volatile *)(0x029202AC)
#define RAB_APIO_AMAP_11_CTRL         *(unsigned int volatile *)(0x029202B0)
#define RAB_APIO_AMAP_11_SIZE         *(unsigned int volatile *)(0x029202B4)
#define RAB_APIO_AMAP_11_ABAR         *(unsigned int volatile *)(0x029202B8)
#define RAB_APIO_AMAP_11_RBAR         *(unsigned int volatile *)(0x029202BC)
#define RAB_APIO_AMAP_12_CTRL         *(unsigned int volatile *)(0x029202C0)
#define RAB_APIO_AMAP_12_SIZE         *(unsigned int volatile *)(0x029202C4)
#define RAB_APIO_AMAP_12_ABAR         *(unsigned int volatile *)(0x029202C8)
#define RAB_APIO_AMAP_12_RBAR         *(unsigned int volatile *)(0x029202CC)
#define RAB_APIO_AMAP_13_CTRL         *(unsigned int volatile *)(0x029202D0)
#define RAB_APIO_AMAP_13_SIZE         *(unsigned int volatile *)(0x029202D4)
#define RAB_APIO_AMAP_13_ABAR         *(unsigned int volatile *)(0x029202D8)
#define RAB_APIO_AMAP_13_RBAR         *(unsigned int volatile *)(0x029202DC)
#define RAB_APIO_AMAP_14_CTRL         *(unsigned int volatile *)(0x029202E0)
#define RAB_APIO_AMAP_14_SIZE         *(unsigned int volatile *)(0x029202E4)
#define RAB_APIO_AMAP_14_ABAR         *(unsigned int volatile *)(0x029202E8)
#define RAB_APIO_AMAP_14_RBAR         *(unsigned int volatile *)(0x029202EC)
#define RAB_APIO_AMAP_15_CTRL         *(unsigned int volatile *)(0x029202F0)
#define RAB_APIO_AMAP_15_SIZE         *(unsigned int volatile *)(0x029202F4)
#define RAB_APIO_AMAP_15_ABAR         *(unsigned int volatile *)(0x029202F8)
#define RAB_APIO_AMAP_15_RBAR         *(unsigned int volatile *)(0x029202FC)
#define RAB_OB_DB_0_CSR               *(unsigned int volatile *)(0x02920400)
#define RAB_OB_DB_0_INFO              *(unsigned int volatile *)(0x02920404)
#define RAB_OB_DB_1_CSR               *(unsigned int volatile *)(0x02920408)
#define RAB_OB_DB_1_INFO              *(unsigned int volatile *)(0x0292040c)
#define RAB_OB_DB_2_CSR               *(unsigned int volatile *)(0x02920410)
#define RAB_OB_DB_2_INFO              *(unsigned int volatile *)(0x02920414)
#define RAB_OB_DB_3_CSR               *(unsigned int volatile *)(0x02920418)
#define RAB_OB_DB_3_INFO              *(unsigned int volatile *)(0x0292041c)
#define RAB_OB_DB_4_CSR               *(unsigned int volatile *)(0x02920420)
#define RAB_OB_DB_4_INFO              *(unsigned int volatile *)(0x02920424)
#define RAB_OB_DB_5_CSR               *(unsigned int volatile *)(0x02920428)
#define RAB_OB_DB_5_INFO              *(unsigned int volatile *)(0x0292042c)
#define RAB_OB_DB_6_CSR               *(unsigned int volatile *)(0x02920430)
#define RAB_OB_DB_6_INFO              *(unsigned int volatile *)(0x02920434)
#define RAB_OB_DB_7_CSR               *(unsigned int volatile *)(0x02920438)
#define RAB_OB_DB_7_INFO              *(unsigned int volatile *)(0x0292043c)
#define RAB_OB_DB_8_CSR               *(unsigned int volatile *)(0x02920440)
#define RAB_OB_DB_8_INFO              *(unsigned int volatile *)(0x02920444)
#define RAB_OB_DB_9_CSR               *(unsigned int volatile *)(0x02920448)
#define RAB_OB_DB_9_INFO              *(unsigned int volatile *)(0x0292044c)
#define RAB_OB_DB_10_CSR              *(unsigned int volatile *)(0x02920450)
#define RAB_OB_DB_10_INFO             *(unsigned int volatile *)(0x02920454)
#define RAB_OB_DB_11_CSR              *(unsigned int volatile *)(0x02920458)
#define RAB_OB_DB_11_INFO             *(unsigned int volatile *)(0x0292045c)
#define RAB_OB_DB_12_CSR              *(unsigned int volatile *)(0x02920460)
#define RAB_OB_DB_12_INFO             *(unsigned int volatile *)(0x02920464)
#define RAB_OB_DB_13_CSR              *(unsigned int volatile *)(0x02920468)
#define RAB_OB_DB_13_INFO             *(unsigned int volatile *)(0x0292046c)
#define RAB_OB_DB_14_CSR              *(unsigned int volatile *)(0x02920470)
#define RAB_OB_DB_14_INFO             *(unsigned int volatile *)(0x02920474)
#define RAB_OB_IDB_CSR                *(unsigned int volatile *)(0x02920478)
#define RAB_OB_IDB_INFO               *(unsigned int volatile *)(0x0292047c)
#define RAB_IB_DB_CSR                 *(unsigned int volatile *)(0x02920480)
#define RAB_IB_DB_INFO                *(unsigned int volatile *)(0x02920484)
#define RAB_IB_DB_0_CHK               *(unsigned int volatile *)(0x02920488)
#define RAB_IB_DB_1_CHK               *(unsigned int volatile *)(0x0292048c)
#define RAB_IB_DB_2_CHK               *(unsigned int volatile *)(0x02920490)
#define RAB_IB_DB_3_CHK               *(unsigned int volatile *)(0x02920494)
#define RAB_IB_DB_4_CHK               *(unsigned int volatile *)(0x02920498)
#define RAB_IB_DB_5_CHK               *(unsigned int volatile *)(0x0292049c)
#define RAB_IB_DB_6_CHK               *(unsigned int volatile *)(0x029204a0)
#define RAB_IB_DB_7_CHK               *(unsigned int volatile *)(0x029204a4)
#define RAB_IB_DB_8_CHK               *(unsigned int volatile *)(0x029204a8)
#define RAB_IB_DB_9_CHK               *(unsigned int volatile *)(0x029204ac)
#define RAB_IB_DB_10_CHK              *(unsigned int volatile *)(0x029204b0)
#define RAB_IB_DB_11_CHK              *(unsigned int volatile *)(0x029204b4)
#define RAB_IB_DB_12_CHK              *(unsigned int volatile *)(0x029204b8)
#define RAB_IB_DB_13_CHK              *(unsigned int volatile *)(0x029204bc)
#define RAB_IB_DB_14_CHK              *(unsigned int volatile *)(0x029204c0)
#define RAB_IB_DB_15_CHK              *(unsigned int volatile *)(0x029204c4)
#define RAB_WDMA_0_CTRL               *(unsigned int volatile *)(0x02920800)
#define RAB_WDMA_0_ADDR               *(unsigned int volatile *)(0x02920804)
#define RAB_WDMA_0_STAT               *(unsigned int volatile *)(0x02920808)
#define RAB_WDMA_0_ADDR_EXT           *(unsigned int volatile *)(0x0292080c)
#define RAB_WDMA_1_CTRL               *(unsigned int volatile *)(0x02920810)
#define RAB_WDMA_1_ADDR               *(unsigned int volatile *)(0x02920814)
#define RAB_WDMA_1_ADDR_EXT           *(unsigned int volatile *)(0x0292081c)
#define RAB_WDMA_2_CTRL               *(unsigned int volatile *)(0x02920820)
#define RAB_WDMA_2_ADDR               *(unsigned int volatile *)(0x02920824)
#define RAB_WDMA_2_ADDR_EXT           *(unsigned int volatile *)(0x0292082c)
#define RAB_WDMA_3_CTRL               *(unsigned int volatile *)(0x02920830)
#define RAB_WDMA_3_ADDR               *(unsigned int volatile *)(0x02920834)
#define RAB_WDMA_3_ADDR_EXT           *(unsigned int volatile *)(0x0292083c)
#define RAB_RDMA_0_CTRL               *(unsigned int volatile *)(0x02920880)
#define RAB_RDMA_0_ADDR               *(unsigned int volatile *)(0x02920884)
#define RAB_RDMA_0_ADDR_EXT           *(unsigned int volatile *)(0x0292088c)
#define RAB_DMA_IADDR_DESC_SEL        *(unsigned int volatile *)(0x02920900)
#define RAB_DMA_IADDR_DESC_CTRL       *(unsigned int volatile *)(0x02920904)
#define RAB_DMA_IADDR_DESC_SRC_ADDR   *(unsigned int volatile *)(0x02920908)
#define RAB_DMA_IADDR_DESC_DEST_ADDR  *(unsigned int volatile *)(0x0292090c)
#define RAB_DMA_IADDR_DESC_NEXT_ADDR  *(unsigned int volatile *)(0x02920910)
#define RAB_RIO0_AMAP_LUT0       *(unsigned int volatile *)(0x02920100)
#define RAB_RIO0_AMAP_LUT1       *(unsigned int volatile *)(0x02920104)
#define RAB_RIO0_AMAP_LUT2       *(unsigned int volatile *)(0x02920108)
#define RAB_RIO0_AMAP_LUT3       *(unsigned int volatile *)(0x0292010c)
#define RAB_RIO0_AMAP_LUT4       *(unsigned int volatile *)(0x02920110)
#define RAB_RIO0_AMAP_LUT5       *(unsigned int volatile *)(0x02920114)
#define RAB_RIO0_AMAP_LUT6       *(unsigned int volatile *)(0x02920118)
#define RAB_RIO0_AMAP_LUT7       *(unsigned int volatile *)(0x0292011c)
#define RAB_RIO0_AMAP_LUT8       *(unsigned int volatile *)(0x02920120)
#define RAB_RIO0_AMAP_LUT9       *(unsigned int volatile *)(0x02920124)
#define RAB_RIO0_AMAP_LUT10      *(unsigned int volatile *)(0x02920128)
#define RAB_RIO0_AMAP_LUT11      *(unsigned int volatile *)(0x0292012c)
#define RAB_RIO0_AMAP_LUT12      *(unsigned int volatile *)(0x02920130)
#define RAB_RIO0_AMAP_LUT13      *(unsigned int volatile *)(0x02920134)
#define RAB_RIO0_AMAP_LUT14      *(unsigned int volatile *)(0x02920138)
#define RAB_RIO0_AMAP_LUT15      *(unsigned int volatile *)(0x0292013c)
#define RAB_RIO0_AMAP_IDSL      *(unsigned int volatile *)(0x02920140)
#define RAB_RIO0_AMAP_BYPS      *(unsigned int volatile *)(0x02920144)

//srio 1
#define SRIO1_PBUS_ctrl_0    *(unsigned int volatile *)(0x02940000)
#define SRIO1_PBUS_ctrl_1    *(unsigned int volatile *)(0x02940004)
#define SRIO1_PBUS_ctrl_2    *(unsigned int volatile *)(0x02940008)
#define SRIO1_PBUS_ctrl_3    *(unsigned int volatile *)(0x0294000c)
#define SRIO1_PBUS_ctrl_4    *(unsigned int volatile *)(0x02940010)
#define SRIO1_PBUS_ctrl_5    *(unsigned int volatile *)(0x02940014)
#define SRIO1_PBUS_ctrl_6    *(unsigned int volatile *)(0x02940018)
#define SRIO1_PBUS_ctrl_7    *(unsigned int volatile *)(0x0294001c)
#define SRIO1_PBUS_ctrl_8    *(unsigned int volatile *)(0x02940020)
#define SRIO1_PBUS_ctrl_9    *(unsigned int volatile *)(0x02940024)
#define SRIO1_PBUS_ctrl_10   *(unsigned int volatile *)(0x02940028)
#define SRIO1_PBUS_ctrl_11   *(unsigned int volatile *)(0x02960504)
#define SRIO1_PBUS_ctrl_hide *(unsigned int volatile *)(0x02940030)

//GRIO bigendian page0
#define SRIO1_DIDCAR        *(unsigned int volatile *)(0x02960800)
#define SRIO1_DICAR         *(unsigned int volatile *)(0x02960804)
#define SRIO1_AIDCAR        *(unsigned int volatile *)(0x02960808)
#define SRIO1_AICAR         *(unsigned int volatile *)(0x0296080C)
#define SRIO1_PEFCAR        *(unsigned int volatile *)(0x02960810)
#define SRIO1_BDIDCSR       *(unsigned int volatile *)(0x02960860)
#define SRIO1_PnLMREQCSR    *(unsigned int volatile *)(0x02960940)
#define SRIO1_PnLMRESPCSR   *(unsigned int volatile *)(0x02960944)
#define SRIO1_PnLASCSR	    *(unsigned int volatile *)(0x02960948)
#define SRIO1_PnESCSR       *(unsigned int volatile *)(0x02960958)
#define SRIO1_PnCCSR		*(unsigned int volatile *)(0x0296095c)
#define SRIO1_PnERTCSR		*(unsigned int volatile *)(0x02960c6c)
#define SRIO1_LTLEDCSR     	*(unsigned int volatile *)(0x02960e08)
#define SRIO1_LTLEECSR     	*(unsigned int volatile *)(0x02960e0c)
//page 0x20
#define SRIO1_PnPCR      	*(unsigned int volatile *)(0x02960940)

#define SRIO1_RAB_CTRL                *(unsigned int volatile *)(0x02960008)
#define SRIO1_RAB_RST_CTRL			  *(unsigned int volatile *)(0x02960018)
#define SRIO1_RAB_APB_CSR             *(unsigned int volatile *)(0x02960030)
#define SRIO1_RAB_INTR_ENAB_GNRL      *(unsigned int volatile *)(0x02960040)
#define SRIO1_RAB_INTR_ENAB_APIO      *(unsigned int volatile *)(0x02960044)
#define SRIO1_RAB_INTR_ENAB_RPIO      *(unsigned int volatile *)(0x02960048)
#define SRIO1_RAB_INTR_ENAB_WDMA      *(unsigned int volatile *)(0x0296004c)
#define SRIO1_RAB_INTR_ENAB_RDMA      *(unsigned int volatile *)(0x02960050)
#define SRIO1_RAB_INTR_ENAB_MISC      *(unsigned int volatile *)(0x0296005c)
#define SRIO1_RAB_INTR_STAT_GNRL      *(unsigned int volatile *)(0x02960060)
#define SRIO1_RAB_INTR_STAT_APIO      *(unsigned int volatile *)(0x02960064)
#define SRIO1_RAB_INTR_STAT_RPIO      *(unsigned int volatile *)(0x02960068)
#define SRIO1_RAB_INTR_STAT_WDMA      *(unsigned int volatile *)(0x0296006c)
#define SRIO1_RAB_INTR_STAT_RDMA      *(unsigned int volatile *)(0x02960070)
#define SRIO1_RAB_INTR_STAT_MISC      *(unsigned int volatile *)(0x0296007c)
#define SRIO1_RAB_RIO_PIO_CTRLn       *(unsigned int volatile *)(0x02960080)
#define SRIO1_RAB_RIO_AMAP_LUT0       *(unsigned int volatile *)(0x02960100)
#define SRIO1_RAB_RIO_AMAP_LUT1       *(unsigned int volatile *)(0x02960104)
#define SRIO1_RAB_RIO_AMAP_LUT2       *(unsigned int volatile *)(0x02960108)
#define SRIO1_RAB_RIO_AMAP_LUT3       *(unsigned int volatile *)(0x0296010c)
#define SRIO1_RAB_RIO_AMAP_LUT4       *(unsigned int volatile *)(0x02960110)
#define SRIO1_RAB_RIO_AMAP_LUT5       *(unsigned int volatile *)(0x02960114)
#define SRIO1_RAB_RIO_AMAP_LUT6       *(unsigned int volatile *)(0x02960118)
#define SRIO1_RAB_RIO_AMAP_LUT7       *(unsigned int volatile *)(0x0296011c)
#define SRIO1_RAB_RIO_AMAP_LUT8       *(unsigned int volatile *)(0x02960120)
#define SRIO1_RAB_RIO_AMAP_LUT9       *(unsigned int volatile *)(0x02960124)
#define SRIO1_RAB_RIO_AMAP_LUT10      *(unsigned int volatile *)(0x02960128)
#define SRIO1_RAB_RIO_AMAP_LUT11      *(unsigned int volatile *)(0x0296012c)
#define SRIO1_RAB_RIO_AMAP_LUT12      *(unsigned int volatile *)(0x02960130)
#define SRIO1_RAB_RIO_AMAP_LUT13      *(unsigned int volatile *)(0x02960134)
#define SRIO1_RAB_RIO_AMAP_LUT14      *(unsigned int volatile *)(0x02960138)
#define SRIO1_RAB_RIO_AMAP_LUT15      *(unsigned int volatile *)(0x0296013c)
#define SRIO1_RAB_RIO_AMAP_IDSL       *(unsigned int volatile *)(0x02960140)
#define SRIO1_RAB_RIO_AMAP_BYPS       *(unsigned int volatile *)(0x02960144)
#define SRIO1_RAB_APIO_N_CTRL         *(unsigned int volatile *)(0x02960180)
#define SRIO1_RAB_APIO_AMAP_0_CTRL          *(unsigned int volatile *)(0x02960200)
#define SRIO1_RAB_APIO_AMAP_0_SIZE          *(unsigned int volatile *)(0x02960204)
#define SRIO1_RAB_APIO_AMAP_0_ABAR          *(unsigned int volatile *)(0x02960208)
#define SRIO1_RAB_APIO_AMAP_0_RBAR          *(unsigned int volatile *)(0x0296020C)
#define SRIO1_RAB_APIO_AMAP_1_CTRL          *(unsigned int volatile *)(0x02960210)
#define SRIO1_RAB_APIO_AMAP_1_SIZE          *(unsigned int volatile *)(0x02960214)
#define SRIO1_RAB_APIO_AMAP_1_ABAR          *(unsigned int volatile *)(0x02960218)
#define SRIO1_RAB_APIO_AMAP_1_RBAR          *(unsigned int volatile *)(0x0296021C)
#define SRIO1_RAB_APIO_AMAP_2_CTRL          *(unsigned int volatile *)(0x02960220)
#define SRIO1_RAB_APIO_AMAP_2_SIZE          *(unsigned int volatile *)(0x02960224)
#define SRIO1_RAB_APIO_AMAP_2_ABAR          *(unsigned int volatile *)(0x02960228)
#define SRIO1_RAB_APIO_AMAP_2_RBAR          *(unsigned int volatile *)(0x0296022C)
#define SRIO1_RAB_APIO_AMAP_3_CTRL          *(unsigned int volatile *)(0x02960230)
#define SRIO1_RAB_APIO_AMAP_3_SIZE          *(unsigned int volatile *)(0x02960234)
#define SRIO1_RAB_APIO_AMAP_3_ABAR          *(unsigned int volatile *)(0x02960238)
#define SRIO1_RAB_APIO_AMAP_3_RBAR          *(unsigned int volatile *)(0x0296023C)
#define SRIO1_RAB_APIO_AMAP_4_CTRL          *(unsigned int volatile *)(0x02960240)
#define SRIO1_RAB_APIO_AMAP_4_SIZE          *(unsigned int volatile *)(0x02960244)
#define SRIO1_RAB_APIO_AMAP_4_ABAR          *(unsigned int volatile *)(0x02960248)
#define SRIO1_RAB_APIO_AMAP_4_RBAR          *(unsigned int volatile *)(0x0296024C)
#define SRIO1_RAB_APIO_AMAP_5_CTRL          *(unsigned int volatile *)(0x02960250)
#define SRIO1_RAB_APIO_AMAP_5_SIZE          *(unsigned int volatile *)(0x02960254)
#define SRIO1_RAB_APIO_AMAP_5_ABAR          *(unsigned int volatile *)(0x02960258)
#define SRIO1_RAB_APIO_AMAP_5_RBAR          *(unsigned int volatile *)(0x0296025C)
#define SRIO1_RAB_APIO_AMAP_6_CTRL          *(unsigned int volatile *)(0x02960260)
#define SRIO1_RAB_APIO_AMAP_6_SIZE          *(unsigned int volatile *)(0x02960264)
#define SRIO1_RAB_APIO_AMAP_6_ABAR          *(unsigned int volatile *)(0x02960268)
#define SRIO1_RAB_APIO_AMAP_6_RBAR          *(unsigned int volatile *)(0x0296026C)
#define SRIO1_RAB_APIO_AMAP_7_CTRL          *(unsigned int volatile *)(0x02960270)
#define SRIO1_RAB_APIO_AMAP_7_SIZE          *(unsigned int volatile *)(0x02960274)
#define SRIO1_RAB_APIO_AMAP_7_ABAR          *(unsigned int volatile *)(0x02960278)
#define SRIO1_RAB_APIO_AMAP_7_RBAR          *(unsigned int volatile *)(0x0296027C)
#define SRIO1_RAB_APIO_AMAP_8_CTRL          *(unsigned int volatile *)(0x02960280)
#define SRIO1_RAB_APIO_AMAP_8_SIZE          *(unsigned int volatile *)(0x02960284)
#define SRIO1_RAB_APIO_AMAP_8_ABAR          *(unsigned int volatile *)(0x02960288)
#define SRIO1_RAB_APIO_AMAP_8_RBAR          *(unsigned int volatile *)(0x0296028C)
#define SRIO1_RAB_APIO_AMAP_9_CTRL          *(unsigned int volatile *)(0x02960290)
#define SRIO1_RAB_APIO_AMAP_9_SIZE          *(unsigned int volatile *)(0x02960294)
#define SRIO1_RAB_APIO_AMAP_9_ABAR          *(unsigned int volatile *)(0x02960298)
#define SRIO1_RAB_APIO_AMAP_9_RBAR          *(unsigned int volatile *)(0x0296029C)
#define SRIO1_RAB_APIO_AMAP_10_CTRL         *(unsigned int volatile *)(0x029602A0)
#define SRIO1_RAB_APIO_AMAP_10_SIZE         *(unsigned int volatile *)(0x029602A4)
#define SRIO1_RAB_APIO_AMAP_10_ABAR         *(unsigned int volatile *)(0x029602A8)
#define SRIO1_RAB_APIO_AMAP_10_RBAR         *(unsigned int volatile *)(0x029602AC)
#define SRIO1_RAB_APIO_AMAP_11_CTRL         *(unsigned int volatile *)(0x029602B0)
#define SRIO1_RAB_APIO_AMAP_11_SIZE         *(unsigned int volatile *)(0x029602B4)
#define SRIO1_RAB_APIO_AMAP_11_ABAR         *(unsigned int volatile *)(0x029602B8)
#define SRIO1_RAB_APIO_AMAP_11_RBAR         *(unsigned int volatile *)(0x029602BC)
#define SRIO1_RAB_APIO_AMAP_12_CTRL         *(unsigned int volatile *)(0x029602C0)
#define SRIO1_RAB_APIO_AMAP_12_SIZE         *(unsigned int volatile *)(0x029602C4)
#define SRIO1_RAB_APIO_AMAP_12_ABAR         *(unsigned int volatile *)(0x029602C8)
#define SRIO1_RAB_APIO_AMAP_12_RBAR         *(unsigned int volatile *)(0x029602CC)
#define SRIO1_RAB_APIO_AMAP_13_CTRL         *(unsigned int volatile *)(0x029602D0)
#define SRIO1_RAB_APIO_AMAP_13_SIZE         *(unsigned int volatile *)(0x029602D4)
#define SRIO1_RAB_APIO_AMAP_13_ABAR         *(unsigned int volatile *)(0x029602D8)
#define SRIO1_RAB_APIO_AMAP_13_RBAR         *(unsigned int volatile *)(0x029602DC)
#define SRIO1_RAB_APIO_AMAP_14_CTRL         *(unsigned int volatile *)(0x029602E0)
#define SRIO1_RAB_APIO_AMAP_14_SIZE         *(unsigned int volatile *)(0x029602E4)
#define SRIO1_RAB_APIO_AMAP_14_ABAR         *(unsigned int volatile *)(0x029602E8)
#define SRIO1_RAB_APIO_AMAP_14_RBAR         *(unsigned int volatile *)(0x029602EC)
#define SRIO1_RAB_APIO_AMAP_15_CTRL         *(unsigned int volatile *)(0x029602F0)
#define SRIO1_RAB_APIO_AMAP_15_SIZE         *(unsigned int volatile *)(0x029602F4)
#define SRIO1_RAB_APIO_AMAP_15_ABAR         *(unsigned int volatile *)(0x029602F8)
#define SRIO1_RAB_APIO_AMAP_15_RBAR         *(unsigned int volatile *)(0x029602FC)
#define SRIO1_RAB_OB_DB_0_CSR         *(unsigned int volatile *)(0x02960400)
#define SRIO1_RAB_OB_DB_0_INFO        *(unsigned int volatile *)(0x02960404)
#define SRIO1_RAB_OB_DB_1_CSR         *(unsigned int volatile *)(0x02960408)
#define SRIO1_RAB_OB_DB_1_INFO        *(unsigned int volatile *)(0x0296040c)
#define SRIO1_RAB_OB_DB_2_CSR         *(unsigned int volatile *)(0x02960410)
#define SRIO1_RAB_OB_DB_2_INFO        *(unsigned int volatile *)(0x02960414)
#define SRIO1_RAB_OB_DB_3_CSR         *(unsigned int volatile *)(0x02960418)
#define SRIO1_RAB_OB_DB_3_INFO        *(unsigned int volatile *)(0x0296041c)
#define SRIO1_RAB_OB_DB_4_CSR         *(unsigned int volatile *)(0x02960420)
#define SRIO1_RAB_OB_DB_4_INFO        *(unsigned int volatile *)(0x02960424)
#define SRIO1_RAB_OB_DB_5_CSR         *(unsigned int volatile *)(0x02960428)
#define SRIO1_RAB_OB_DB_5_INFO        *(unsigned int volatile *)(0x0296042c)
#define SRIO1_RAB_OB_DB_6_CSR         *(unsigned int volatile *)(0x02960430)
#define SRIO1_RAB_OB_DB_6_INFO        *(unsigned int volatile *)(0x02960434)
#define SRIO1_RAB_OB_DB_7_CSR         *(unsigned int volatile *)(0x02960438)
#define SRIO1_RAB_OB_DB_7_INFO        *(unsigned int volatile *)(0x0296043c)
#define SRIO1_RAB_OB_DB_8_CSR         *(unsigned int volatile *)(0x02960440)
#define SRIO1_RAB_OB_DB_8_INFO        *(unsigned int volatile *)(0x02960444)
#define SRIO1_RAB_OB_DB_9_CSR         *(unsigned int volatile *)(0x02960448)
#define SRIO1_RAB_OB_DB_9_INFO        *(unsigned int volatile *)(0x0296044c)
#define SRIO1_RAB_OB_DB_10_CSR        *(unsigned int volatile *)(0x02960450)
#define SRIO1_RAB_OB_DB_10_INFO       *(unsigned int volatile *)(0x02960454)
#define SRIO1_RAB_OB_DB_11_CSR        *(unsigned int volatile *)(0x02960458)
#define SRIO1_RAB_OB_DB_11_INFO       *(unsigned int volatile *)(0x0296045c)
#define SRIO1_RAB_OB_DB_12_CSR        *(unsigned int volatile *)(0x02960460)
#define SRIO1_RAB_OB_DB_12_INFO       *(unsigned int volatile *)(0x02960464)
#define SRIO1_RAB_OB_DB_13_CSR        *(unsigned int volatile *)(0x02960468)
#define SRIO1_RAB_OB_DB_13_INFO       *(unsigned int volatile *)(0x0296046c)
#define SRIO1_RAB_OB_DB_14_CSR        *(unsigned int volatile *)(0x02960470)
#define SRIO1_RAB_OB_DB_14_INFO       *(unsigned int volatile *)(0x02960474)
#define SRIO1_RAB_OB_IDB_CSR          *(unsigned int volatile *)(0x02960478)
#define SRIO1_RAB_OB_IDB_INFO         *(unsigned int volatile *)(0x0296047c)
#define SRIO1_RAB_IB_DB_CSR           *(unsigned int volatile *)(0x02960480)
#define SRIO1_RAB_IB_DB_INFO          *(unsigned int volatile *)(0x02960484)
#define SRIO1_RAB_IB_DB_0_CHK         *(unsigned int volatile *)(0x02960488)
#define SRIO1_RAB_IB_DB_1_CHK         *(unsigned int volatile *)(0x0296048c)
#define SRIO1_RAB_IB_DB_2_CHK         *(unsigned int volatile *)(0x02960490)
#define SRIO1_RAB_IB_DB_3_CHK         *(unsigned int volatile *)(0x02960494)
#define SRIO1_RAB_IB_DB_4_CHK         *(unsigned int volatile *)(0x02960498)
#define SRIO1_RAB_IB_DB_5_CHK         *(unsigned int volatile *)(0x0296049c)
#define SRIO1_RAB_IB_DB_6_CHK         *(unsigned int volatile *)(0x029604a0)
#define SRIO1_RAB_IB_DB_7_CHK         *(unsigned int volatile *)(0x029604a4)
#define SRIO1_RAB_IB_DB_8_CHK         *(unsigned int volatile *)(0x029604a8)
#define SRIO1_RAB_IB_DB_9_CHK         *(unsigned int volatile *)(0x029604ac)
#define SRIO1_RAB_IB_DB_10_CHK        *(unsigned int volatile *)(0x029604b0)
#define SRIO1_RAB_IB_DB_11_CHK        *(unsigned int volatile *)(0x029604b4)
#define SRIO1_RAB_IB_DB_12_CHK        *(unsigned int volatile *)(0x029604b8)
#define SRIO1_RAB_IB_DB_13_CHK        *(unsigned int volatile *)(0x029604bc)
#define SRIO1_RAB_IB_DB_14_CHK        *(unsigned int volatile *)(0x029604c0)
#define SRIO1_RAB_IB_DB_15_CHK        *(unsigned int volatile *)(0x029604c4)
#define SRIO1_RAB_WDMA_0_CTRL               *(unsigned int volatile *)(0x02960800)
#define SRIO1_RAB_WDMA_0_ADDR               *(unsigned int volatile *)(0x02960804)
#define SRIO1_RAB_WDMA_0_STAT               *(unsigned int volatile *)(0x02960808)
#define SRIO1_RAB_WDMA_0_ADDR_EXT           *(unsigned int volatile *)(0x0296080c)
#define SRIO1_RAB_WDMA_1_CTRL               *(unsigned int volatile *)(0x02960810)
#define SRIO1_RAB_WDMA_1_ADDR               *(unsigned int volatile *)(0x02960814)
#define SRIO1_RAB_WDMA_1_ADDR_EXT           *(unsigned int volatile *)(0x0296081c)
#define SRIO1_RAB_WDMA_2_CTRL               *(unsigned int volatile *)(0x02960820)
#define SRIO1_RAB_WDMA_2_ADDR               *(unsigned int volatile *)(0x02960824)
#define SRIO1_RAB_WDMA_2_ADDR_EXT           *(unsigned int volatile *)(0x0296082c)
#define SRIO1_RAB_WDMA_3_CTRL               *(unsigned int volatile *)(0x02960830)
#define SRIO1_RAB_WDMA_3_ADDR               *(unsigned int volatile *)(0x02960834)
#define SRIO1_RAB_WDMA_3_ADDR_EXT           *(unsigned int volatile *)(0x0296083c)
#define SRIO1_RAB_RDMA_0_CTRL               *(unsigned int volatile *)(0x02960880)
#define SRIO1_RAB_RDMA_0_ADDR               *(unsigned int volatile *)(0x02960884)
#define SRIO1_RAB_RDMA_0_ADDR_EXT           *(unsigned int volatile *)(0x0296088c)
#define SRIO1_RAB_DMA_IADDR_DESC_SEL        *(unsigned int volatile *)(0x02960900)
#define SRIO1_RAB_DMA_IADDR_DESC_CTRL       *(unsigned int volatile *)(0x02960904)
#define SRIO1_RAB_DMA_IADDR_DESC_SRC_ADDR   *(unsigned int volatile *)(0x02960908)
#define SRIO1_RAB_DMA_IADDR_DESC_DEST_ADDR  *(unsigned int volatile *)(0x0296090c)
#define SRIO1_RAB_DMA_IADDR_DESC_NEXT_ADDR  *(unsigned int volatile *)(0x02960910)
//------------------------rio to axi address look up table------------------------)
#define SRIO1_RAB_RIO0_AMAP_LUT0       *(unsigned int volatile *)(0x02960100)
#define SRIO1_RAB_RIO0_AMAP_LUT1       *(unsigned int volatile *)(0x02960104)
#define SRIO1_RAB_RIO0_AMAP_LUT2       *(unsigned int volatile *)(0x02960108)
#define SRIO1_RAB_RIO0_AMAP_LUT3       *(unsigned int volatile *)(0x0296010c)
#define SRIO1_RAB_RIO0_AMAP_LUT4       *(unsigned int volatile *)(0x02960110)
#define SRIO1_RAB_RIO0_AMAP_LUT5       *(unsigned int volatile *)(0x02960114)
#define SRIO1_RAB_RIO0_AMAP_LUT6       *(unsigned int volatile *)(0x02960118)
#define SRIO1_RAB_RIO0_AMAP_LUT7       *(unsigned int volatile *)(0x0296011c)
#define SRIO1_RAB_RIO0_AMAP_LUT8       *(unsigned int volatile *)(0x02960120)
#define SRIO1_RAB_RIO0_AMAP_LUT9       *(unsigned int volatile *)(0x02960124)
#define SRIO1_RAB_RIO0_AMAP_LUT10      *(unsigned int volatile *)(0x02960128)
#define SRIO1_RAB_RIO0_AMAP_LUT11      *(unsigned int volatile *)(0x0296012c)
#define SRIO1_RAB_RIO0_AMAP_LUT12      *(unsigned int volatile *)(0x02960130)
#define SRIO1_RAB_RIO0_AMAP_LUT13      *(unsigned int volatile *)(0x02960134)
#define SRIO1_RAB_RIO0_AMAP_LUT14      *(unsigned int volatile *)(0x02960138)
#define SRIO1_RAB_RIO0_AMAP_LUT15      *(unsigned int volatile *)(0x0296013c)
#define SRIO1_RAB_RIO0_AMAP_IDSL       *(unsigned int volatile *)(0x02960140)
#define SRIO1_RAB_RIO0_AMAP_BYPS       *(unsigned int volatile *)(0x02960144)

#define SRIO0_CLC_cfg_r_0					*(unsigned int volatile *)(0x02620360)
#define SRIO0_CLC_cfg_r_1					*(unsigned int volatile *)(0x02620364)
#define SRIO0_CLC_cfg_r_2					*(unsigned int volatile *)(0x02620368)
#define SRIO0_CLC_cfg_r_3					*(unsigned int volatile *)(0x0262036c)
#define SRIO0_CLC_cfg_r_4                   *(unsigned int volatile *)(0x02620370)
#define SRIO0_CLC_cfg_r_5                   *(unsigned int volatile *)(0x02620374)

#define SRIO1_CLC_cfg_r_0                   *(unsigned int volatile *)(0x026203a0)
#define SRIO1_CLC_cfg_r_1                   *(unsigned int volatile *)(0x026203a4)
#define SRIO1_CLC_cfg_r_2                   *(unsigned int volatile *)(0x026203a8)
#define SRIO1_CLC_cfg_r_3                   *(unsigned int volatile *)(0x026203ac)
#define SRIO1_CLC_cfg_r_4                   *(unsigned int volatile *)(0x026203b0)
#define SRIO1_CLC_cfg_r_5                   *(unsigned int volatile *)(0x026203b4)

#define L0S0CSR                   *(unsigned int volatile *)(0x02920a10)
#define L1S0CSR                   *(unsigned int volatile *)(0x02920a30)
#define L2S0CSR                   *(unsigned int volatile *)(0x02920a50)
#define L3S0CSR                   *(unsigned int volatile *)(0x02920a70)

#define SRIO1_L0S0CSR                   *(unsigned int volatile *)(0x02960a10)
#define SRIO1_L1S0CSR                   *(unsigned int volatile *)(0x02960a30)
#define SRIO1_L2S0CSR                   *(unsigned int volatile *)(0x02960a50)
#define SRIO1_L3S0CSR                   *(unsigned int volatile *)(0x02960a70)


#define SRIO_SERDES_CFGPLL *(unsigned int volatile *)(0x02620360)

#define Speed1250	0x000
#define Speed2500	0x100
#define Speed3125	0x200
#define Speed5000	0x300
#define Speed6250	0x400

#define lane4x		0x00
#define lane2xl0l1	0x10
#define lane2xl2l3	0x20
#define lane1xl0	0x30
#define lane1xl1	0x40
#define lane1xl2	0x50
#define lane1xl3	0x60

#define SrioStatusOk	0x2
#define SrioEnConfig	0x1

#define SrioKeyWord		0xb3f50000

#define SrioInstance0	0
#define SrioInstance1	1

#define SrioIDLen8	0
#define SrioIDLen16	1

#define MaintenanceModel	0
#define MemoryModel			1

#define MODE_lfsr31				0x1
#define MODE_lfsr23_1  			0x2
#define MODE_lfsr23_2			0x3
#define MODE_lfsr16   			0x4
#define MODE_lfsr15				0x5
#define MODE_lfsr11   			0x6
#define MODE_lfsr7     			0x7
#define MODE_Fixed_word 		0x8           //PAT0
#define MODE_DC_balanced_word   0x9
#define MODE_Fixed_pattern		0xA


#define TxEq_0							0      //tx Equalization 0	 dB
#define TxEq_1							1      //tx Equalization 1	 dB
#define TxEq_2							2      //tx Equalization 2	 dB
#define TxEq_2_5						3      //tx Equalization 2.5 dB
#define TxEq_3							4      //tx Equalization 3	 dB
#define TxEq_3_5						5      //tx Equalization 3.5 dB
#define TxEq_4_5						6      //tx Equalization 4.5 dB
#define TxEq_5							7      //tx Equalization 5	 dB
#define TxEq_6							8      //tx Equalization 6	 dB
#define TxEq_7							9      //tx Equalization 7	 dB
#define TxEq_8							10     //tx Equalization 8	 dB
#define TxEq_9							11     //tx Equalization 9	 dB

//变量声明
extern volatile unsigned int SrioVersion;

//参数结构体
struct SRIOinit{
	unsigned int IDLen;//ID长度，SrioIDLen16（1）:16位ID。	SrioIDLen8（0）:8位ID。
	unsigned int SelfID;//本port ID
};

struct SRIODMA{
	unsigned int DstID;//目的ID
	unsigned long long SrcAdd;//操作源地址
	unsigned long long DstAdd;//操作目的地址
	unsigned int Priority;//采用的优先级
	unsigned int CRF;//采用的优先级
	unsigned int DataLen;//数据长度，单位字（32bit），最大传输长度（1MB-4B）
	unsigned int EnSendDB;//1:使能DMA操作完成后产生发送门铃消息信号给发送门铃引擎
	unsigned int EnDescriptor;//本次传输是否使能描述符方式
	unsigned int FirstDescriptorAdd;//若使用描述符方式，本次传输描述符首地址，需四字节对齐
};

struct SRIODoorBellStruct{
	unsigned int Nub;//使用的门铃寄存器编号，0-14有效
	unsigned int DstID;//目的ID
	unsigned int Info;//门铃消息内容（低16位有效）
	unsigned int CRF;//发送的CRF值，默认填0，第0位有效
	unsigned int Priority;//发送的优先级，可选值0-2
	unsigned int Send;//1:手动发送门铃消息，0匹配到WDMA
};

struct SRIOAPIOStruct{
	unsigned int WindowNub;//使用的窗口号，每个部件最多可使用16个窗口（0-15）
	unsigned int DstID;//目的ID
	unsigned int WindowTYPE;//窗口类型，0，维护读写；1，NREAD NWRITE；2，NREAD NWRITE_R；3，NREAD SWRITE
	unsigned int WindowSize;//窗口大小,需大于0x400B
	unsigned int AXIBase;//DSP内axi基址，需1KB对齐
	long long RIOBase;//对端器件RIO基址，需1KB对齐
	unsigned int Priority;//优先级，0-2
	unsigned int CRF;//CRF，最低位有效，默认为0
	unsigned int hopcount;//当配置为维护操作时，表示跳数
};

struct SrioLBERTStruct{
	unsigned int En;
	unsigned int Instance;
	unsigned int Lane;
	unsigned int Speed;
	unsigned int PG_MODE;
	unsigned int PM_MODE;
	unsigned int Pattern;
};

//描述符相关定义
#define SrioDescriptorValid 			(0x1)
#define SrioDescriptorUnValid 			(0x0)

#define SrioDescriptorNextPointValid 	(0x2)//explicit specified
#define SrioDescriptorNextPointUnValid 	(0x0)//implicitly specified

#define PhyCheckTimeOut					1000

struct SrioDescriptorReg{
	unsigned int Ctrl;
	unsigned int SrcAdd;
	unsigned int DestAdd;
	unsigned int NextAdd;
};

struct SrioDescriptorStruct{
	unsigned char En;		//使能当前描述符
	unsigned int  NextAdd;	//下一个描述符地址,需使用全局地址且8字节对齐，0 表明是最后一个描述符
	unsigned int  Len;		//数据长度，单位字（32bit），最大传输长度（1MB-4B）
	unsigned long long  SrcAdd;	//源地址，需4字节对齐
	unsigned long long  DstAdd;	//目的地址，需4字节对齐
};

struct SrioManulStruct{
	unsigned int WorkSpeed;		//工作频率，可选Speed1250 Speed2500	 Speed3125	Speed5000 Speed6250，其中6.25G不推荐用户使用
	unsigned int LaneMode;		//lane工作模式，可选lane1xl0	lane1xl1 lane1xl2 lane1xl3 lane2xl0l1 lane2xl2l3 lane4x
	unsigned int RefClkSrc;		//1,内部时钟；0，外部时钟
	unsigned int TXEq[4];		//各个lane TX Equalization设置值,可选0 1 2 2.5 3 3.5 4.5 5 6 7 8 9
	unsigned int TxPolarity;	//bit0-3，表示lane0-3 tx是否PN极性反转
	unsigned int RxPolarity;	//bit0-3，表示lane0-3 rx是否PN极性反转
	unsigned int EnLoopBack;	//使能loopback
	unsigned int SynTimeOut;	//同步未完成的超时时间，默认值0x100000时，函数超时时间为223ms
};

struct SrioPHYCtrlStruct{
	volatile unsigned int PHYCtrl[12];
};

struct SrioLanePCSStruct{
	volatile unsigned int MISC_CFG_0   ;
	volatile unsigned int MISC_CFG_1   ;
	volatile unsigned int Recv0		   ;
	volatile unsigned int LBERT_PAT_CFG;
	volatile unsigned int LBERT_CFG    ;
	volatile unsigned int LBERT_ECNT   ;
	volatile unsigned int RESET_0      ;
	volatile unsigned int RESET_1      ;
	volatile unsigned int RESET_2      ;
	volatile unsigned int RESET_3      ;
	volatile unsigned int Recv1[2]	   ;
	volatile unsigned int CTLIFC_CTRL_0;
	volatile unsigned int CTLIFC_CTRL_1;
	volatile unsigned int CTLIFC_CTRL_2;
	volatile unsigned int Recv2[18]	   ;
	volatile unsigned int MACIFC_MON_0 ;
	volatile unsigned int Recv3		   ;
	volatile unsigned int MACIFC_MON_1 ;
};

struct SrioTxCtrl{
	volatile unsigned int CFG_0     ;
	volatile unsigned int CFG_1     ;
	volatile unsigned int CFG_2     ;
	volatile unsigned int CFG_3     ;
	volatile unsigned int PREEMPH_0 ;
};

struct SrioPCSModeStruct{
	volatile unsigned int PLL_1_MODE_0;
	volatile unsigned int PLL_1_MODE_1;
	volatile unsigned int LANE_1_MODE_0;
	volatile unsigned int LANE_1_MODE_1;
	volatile unsigned int PLL_2_MODE_0;
	volatile unsigned int PLL_2_MODE_1;
	volatile unsigned int LANE_2_MODE_0;
	volatile unsigned int LANE_2_MODE_1;
	volatile unsigned int PLL_3_MODE_0;
	volatile unsigned int PLL_3_MODE_1;
	volatile unsigned int LANE_3_MODE_0;
	volatile unsigned int LANE_3_MODE_1;
};

//全局变量声明
extern unsigned int u32RstCnt_Srio0;
extern unsigned int u32RstCnt_Srio1;
//函数声明
unsigned int SrioChangeEnding(unsigned int Input);
unsigned int SrioClkClose(unsigned int SrioInstance);
unsigned int SrioClkOpen(unsigned int SrioInstance);
unsigned int SrioCreatAPIOWindow(unsigned int Instance,struct SRIOAPIOStruct * SRIOAPIOStruct);
void SrioRPIOMap(unsigned int Instance);
unsigned int SrioPhyInit(unsigned int SrioInstance,unsigned int Speed,unsigned Lane);
unsigned int SrioPhyCheck(unsigned int SrioInstance,unsigned Lane);
unsigned int SrioCtl(unsigned int SrioInstance,struct SRIOinit * SRIOinit);
unsigned int SrioMatchACKID(unsigned int SrioInstance,unsigned int DstID,unsigned int RemotePort);
unsigned int SrioWDMA(unsigned int SrioInstance,struct SRIODMA * SRIODMA);
unsigned int SrioRDMA(unsigned int SrioInstance,struct SRIODMA * SRIODMA);
void SrioConfigSendDB(unsigned int SrioInstance,struct SRIODoorBellStruct * SRIODoorBellStruct);
void SrioEnRecvDB(unsigned int SrioInstance);
void SrioSetDescriptor(struct SrioDescriptorReg * SrioDescriptorReg,struct SrioDescriptorStruct SrioDescriptorStruct);
void LBERT_Config(struct SrioLBERTStruct *SrioLBERTStruct);
void LBERT_Config_InitialVer(struct SrioLBERTStruct *SrioLBERTStruct);
void LBERT_InsertErr(struct SrioLBERTStruct *SrioLBERTStruct);
unsigned short LBERT_GetErr(struct SrioLBERTStruct *SrioLBERTStruct);
unsigned int SrioSentLinkReset(unsigned int SrioInstance);
unsigned int SrioManulConfig(unsigned int SrioInstance, struct SrioManulStruct SrioManulStruct);
#endif /* M6678_SRIO_H_ */
