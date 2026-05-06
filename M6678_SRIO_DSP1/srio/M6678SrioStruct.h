struct RAB_APIO{
	unsigned int Srio_CTRL;
	unsigned int Srio_STAT;
};

struct RAB_RIO_PIO{
	unsigned int Srio_CTRL;
	unsigned int Srio_STAT;
};

struct RAB_ASLV{
	unsigned int Srio_STAT_CMD;
	unsigned int Srio_STAT_ADDR;
};

struct RAB_APIO_AMAP{
	unsigned int Srio_CTRL;
	unsigned int Srio_SIZE;
	unsigned int Srio_ABAR;
	unsigned int Srio_RBAR;
};

struct RAB_OB_DB{
	unsigned int Srio_CSR;
	unsigned int Srio_INFO;
};

struct RAB_OB_DME{
	unsigned int Srio_CTRL;
	unsigned int Srio_ADDR;
	unsigned int Srio_STAT;
	unsigned int Srio_DESC;
};

struct RAB_IB_DME{
	unsigned int Srio_CTRL;
	unsigned int Srio_ADDR;
	unsigned int Srio_STAT;
	unsigned int Srio_DESC;
};

struct FixedRABReg{
	unsigned int Srio_RAB_VER;
	unsigned int Srio_RAB_CAPA;
	unsigned int Srio_RAB_CTRL;
	unsigned int Srio_RAB_STAT;
	unsigned int Srio_RAB_AXI_TIMEOUT;
	unsigned int Srio_RAB_DME_TIMEOUT;
	unsigned int Srio_RAB_RST_CTRL;
	unsigned int Srio_RAB_COOP_LOCK;
	unsigned int Srio_RAB_STAT_ESEL;
	unsigned int Srio_RAB_STAT_STAT;
	unsigned int Srio_RAB_IB_PW_CSR;
	unsigned int Srio_RAB_IB_PW_Data;
	unsigned int Srio_RAB_APB_CSR;
	unsigned int Srio_RAB_ARB_TIMEOUT;
	unsigned int Srio_RAB_DESC_RDY_TIMEOUT;
	unsigned int Srio_Recv0;
	unsigned int Srio_RAB_INTR_ENAB_GNRL;
	unsigned int Srio_RAB_INTR_ENAB_APIO;
	unsigned int Srio_RAB_INTR_ENAB_RPIO;
	unsigned int Srio_RAB_INTR_ENAB_WDMA;
	unsigned int Srio_RAB_INTR_ENAB_RDMA;
	unsigned int Srio_RAB_INTR_ENAB_IDME;
	unsigned int Srio_RAB_INTR_ENAB_ODME;
	unsigned int Srio_RAB_INTR_ENAB_MISC;
	unsigned int Srio_RAB_INTR_STAT_GNRL;
	unsigned int Srio_RAB_INTR_STAT_APIO;
	unsigned int Srio_RAB_INTR_STAT_RPIO;
	unsigned int Srio_RAB_INTR_STAT_WDMA;
	unsigned int Srio_RAB_INTR_STAT_RDMA;
	unsigned int Srio_RAB_INTR_STAT_IDME;
	unsigned int Srio_RAB_INTR_STAT_ODME;
	unsigned int Srio_RAB_INTR_STAT_MISC;
	struct RAB_RIO_PIO RAB_RIO_PIO[7];
	unsigned int Srio_Recv1[18];
	unsigned int Srio_RAB_RIO_AMAP_LUT[16];
	unsigned int Srio_RAB_RIO_AMAP_IDSL;
	unsigned int Srio_RAB_RIO_AMAP_BYPS;
	unsigned int Srio_Recv2[14];
	struct RAB_APIO RAB_APIO[8];
	struct RAB_ASLV RAB_ASLV[4];
	unsigned int Srio_RAB_AMST_STAT[8];
	struct RAB_APIO_AMAP RAB_APIO_AMAP[32];
	struct RAB_OB_DB RAB_OB_DB[14];
	unsigned int Srio_Recv3[2];
	unsigned int Srio_RAB_OB_IDB_CSR;
	unsigned int Srio_RAB_OB_IDB_INFO;
	unsigned int Srio_RAB_IB_DB_CSR;
	unsigned int Srio_RAB_IB_DB_INFO;
	unsigned int Srio_RAB_IB_DB_CHK[16];
	unsigned int Srio_Recv4[14];
	struct RAB_OB_DME RAB_OB_DME[16];
	struct RAB_IB_DME RAB_IB_DME[32];
};

struct NfixedRABReg{
	unsigned int Srio_Reg[512];
};

struct M6678SrioRABStruct{
	struct FixedRABReg FixedRABReg;
	struct NfixedRABReg NfixedRABReg;
};

struct M6678SrioRABStruct * M6678Srio0RABStruct = (struct M6678SrioRABStruct *)0x02920000;
struct M6678SrioRABStruct * M6678Srio1RABStruct = (struct M6678SrioRABStruct *)0x02960000;
