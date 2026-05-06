/*******************************************************************************
 * 完整功能版 - 门铃接收(轮询) + 数据收发 + 按键控制
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "boot.h"
#include "M6678_SRIO.h"
#include "TestP.h"
#include "GPIO.h"
#include "DDR_regdefine.h"
#include "UART.h"

/* ==================== 配置 ==================== */
#define TEST_SPEED          Speed5000
//#define TEST_LANE           lane1xl0
#define TEST_LANE           lane2xl0l1

#define LOCAL_SELF_ID       0x61
#define REMOTE_DEVICE_ID    0x80
#define REMOTE_ADDR_SWRITE  0x0C002000
#define DSP_NAME            "DSP1"

/* 实际有效数据长度（字节），程序内部会自动对齐到8的倍数并补0发送 */
#define SEND_LEN            94

#pragma pack(4)
typedef struct srio_down_data_head_st
{
    unsigned short  nSrcID;
    unsigned short  nDstID;
    unsigned short  nRole;
    unsigned short  nSendLen;
} SRIO_DownData_Head_st;
#pragma pack()

/* 全局标志与缓冲区 */
volatile unsigned int g_wdma_done = 0;
volatile unsigned int g_doorbell_count = 0;
volatile int send_enabled = 0;          /* 发送使能：'s'启动，'q'停止 */

/* 打印与统计控制 */
volatile int g_print_enable = 1;        /* 0 = 静默（错误除外），1 = 允许普通日志 */
volatile int g_verbose = 1;             /* 0 = 摘要模式（仅计数），1 = 完整日志 */
static unsigned int doorbell_total_rx = 0;   /* 门铃收包总计数（摘要模式使用） */
static unsigned int doorbell_rx_info0 = 0;   /* info=0 计数 */
static unsigned int doorbell_rx_info1 = 0;   /* info=1 计数 */

#pragma DATA_SECTION(g_txBuf, ".msmc")
static unsigned char g_txBuf[512] __attribute__((aligned(4)));
#pragma DATA_SECTION(g_uartBuf, ".msmc")
static char g_uartBuf[256] __attribute__((aligned(4)));

/* ==================== 函数声明 ==================== */
void Core0_UART_Print(const char *str);
void UART_PrintHex(unsigned char *data, int len);
int UART_GetChar(void);
void ProcessSerialCommand(void);
void SrioFullReinit(void);
void PrintDoorbellRegs(void);
void PollDoorbellFifo(void);
void ClearDoorbellFifo(void);
int SendToRemote_SWrite(unsigned short dstID, unsigned int remoteAddr,
                        unsigned char *data, unsigned short dataLen);
void PrintRawData(unsigned int rx_addr);
void PrintMemory256(unsigned int addr);
unsigned short TransByteWord(unsigned short nData);

/* 必须保留！解决 vecs.obj 链接错误！空实现即可 */
interrupt void SRIO0Int(void){}
interrupt void SRIO1Int(void){}

static unsigned int rev_counter = 0;
static unsigned char fill_byte = 0;     /* 填充中间区域的字节，每包递增 */

/* ==================== 受控打印内联函数 ==================== */
/* 普通日志打印，受 g_print_enable 控制 */
static inline void print_str(const char *s) {
    if (g_print_enable) UART_Print(s);
}
static inline void print_core(const char *s) {
    if (g_print_enable) {
        UART_Print("[Core0] ");
        UART_Print(s);
    }
}
/* 错误打印，不受 g_print_enable 控制 */
static inline void error_core(const char *s) {
    UART_Print("[Core0] ");
    UART_Print(s);
}

/* ==================== 辅助函数实现 ==================== */
unsigned short TransByteWord(unsigned short nData)
{
    return ((nData & 0x00FF) << 8) | ((nData & 0xFF00) >> 8);
}

void Core0_UART_Print(const char *str)
{
    UART_Print("[Core0] ");
    UART_Print(str);
}

void UART_PrintHex(unsigned char *data, int len)
{
    int i;
    char hex_str[4];
    print_core("");
    for (i = 0; i < len; i++) {
        unsigned char val = data[i];
        unsigned char hi = (val >> 4) & 0x0F;
        unsigned char lo = val & 0x0F;
        hex_str[0] = (hi < 10) ? (hi + '0') : (hi - 10 + 'A');
        hex_str[1] = (lo < 10) ? (lo + '0') : (lo - 10 + 'A');
        hex_str[2] = ' ';
        hex_str[3] = '\0';
        print_str(hex_str);
        if ((i + 1) % 16 == 0) {
            print_str("\n");
            print_core("");
        }
    }
    print_str("\n");
}

int UART_GetChar(void)
{
    if ((StatusReg & 0xfc000000) >= 0x04000000) {
        return (int)DataReg;
    }
    return -1;
}

void ProcessSerialCommand(void)
{
    int ch = UART_GetChar();
    if (ch == -1) return;

    DataReg = (unsigned char)ch;    /* 回显 */

    if (ch == 'r' || ch == 'R') {
        SrioFullReinit();
        UART_Print("\nManual reset triggered.\n");
    }
    else if (ch == 's' || ch == 'S') {
        send_enabled = 1;
        UART_Print("\n[S] Sending started.\n");
    }
    else if (ch == 'q' || ch == 'Q') {
        send_enabled = 0;
        UART_Print("\n[Q] Sending stopped.\n");
    }
    else if (ch == 'd' || ch == 'D') {
        PrintDoorbellRegs();
    }
    else if (ch == 'p' || ch == 'P') {
        g_verbose = !g_verbose;
        if (g_verbose) {
            UART_Print("\n[P] Verbose mode: ON (full log for each doorbell)\n");
        } else {
            UART_Print("\n[P] Verbose mode: OFF (summary counters only)\n");
        }
    }
    else if (ch == 'n' || ch == 'N') {
        g_print_enable = !g_print_enable;
        if (g_print_enable) {
            UART_Print("\n[N] Print enabled.\n");
        } else {
            UART_Print("\n[N] Print disabled (errors still shown).\n");
        }
    }
    else if (ch == 'm' || ch == 'M') {
        /* 菜单始终打印 */
        UART_Print("\n========================================\n");
        UART_Print(" Command Menu:\n");
        UART_Print("  r/R : Reinit SRIO link\n");
        UART_Print("  s   : Start sending\n");
        UART_Print("  q   : Stop sending\n");
        UART_Print("  d   : Print doorbell registers\n");
        UART_Print("  p   : Toggle verbose/summary receive log\n");
        UART_Print("  n   : Toggle print enable/disable\n");
        UART_Print("  m   : Show this menu\n");
        UART_Print("========================================\n");
    }
}

/* ==================== 打印门铃寄存器 ==================== */
void PrintDoorbellRegs(void)
{
    unsigned int fifo_depth = (RAB_IB_DB_CSR >> 16) & 0x3F;

    print_str("\n");
    print_core("=== Doorbell Registers ===\n");
    sprintf(g_uartBuf, "RAB_CTRL:          0x%08X\n", RAB_CTRL);
    print_str(g_uartBuf);
    sprintf(g_uartBuf, "RAB_IB_DB_CSR:     0x%08X\n", RAB_IB_DB_CSR);
    print_str(g_uartBuf);
    sprintf(g_uartBuf, "RAB_IB_DB_INFO:    0x%08X\n", RAB_IB_DB_INFO);
    print_str(g_uartBuf);
    sprintf(g_uartBuf, "RAB_INTR_STAT_MISC:0x%08X\n", RAB_INTR_STAT_MISC);
    print_str(g_uartBuf);
    sprintf(g_uartBuf, "FIFO Depth:        %d\n", fifo_depth);
    print_str(g_uartBuf);
    print_core("==========================\n\n");
}

/* ==================== 清空门铃FIFO ==================== */
void ClearDoorbellFifo(void)
{
    unsigned int sigNum;
    unsigned int dummy;
    int i;

    sigNum = (RAB_IB_DB_CSR & 0x3F0000) >> 16;
    for (i = 0; i < sigNum; i++) {
        dummy = RAB_IB_DB_INFO;
    }
    RAB_INTR_STAT_MISC = 0xFFFFFFFF;
    g_doorbell_count = 0;
    /* 不清零分类计数，仅在上电或手动重置时可能需要清零，这里保持累计 */
}

/* ==================== 打印指定地址256字节 ==================== */
void PrintMemory256(unsigned int addr)
{
    volatile unsigned char *mem = (volatile unsigned char *)addr;
    sprintf(g_uartBuf, "Memory Dump at 0x%08X (256 bytes):\n", addr);
    print_str(g_uartBuf);
    UART_PrintHex((unsigned char *)mem, 256);
}

/* ==================== 轮询门铃FIFO（核心） ==================== */
void PollDoorbellFifo(void)
{
    unsigned int sigNum;
    unsigned int dbInfo;
    unsigned int src_id;
    unsigned int doorbell_info;
    int i;

    sigNum = (RAB_IB_DB_CSR & 0x3F0000) >> 16;
    if (sigNum == 0) {
        return;
    }

    for (i = 0; i < sigNum; i++) {
        dbInfo = RAB_IB_DB_INFO;
        g_doorbell_count++;
        doorbell_total_rx++;

        src_id = (dbInfo >> 16) & 0xFF;
        doorbell_info = ((dbInfo & 0xFF) << 8) | ((dbInfo >> 8) & 0xFF);

        /* 按 info 分类计数 */
        if (doorbell_info == 0) {
            doorbell_rx_info0++;
        } else if (doorbell_info == 1) {
            doorbell_rx_info1++;
        }

        if (g_print_enable) {
            if (g_verbose) {
                /* 详细模式：打印完整门铃信息 */
                sprintf(g_uartBuf, "\n[Core0] >>> DOORBELL RECEIVED #%d <<<\n", g_doorbell_count);
                print_str(g_uartBuf);
                sprintf(g_uartBuf, "  Raw Reg:  0x%08X\n", dbInfo);
                print_str(g_uartBuf);
                sprintf(g_uartBuf, "  Source ID: 0x%02X | Info: 0x%04X\n", src_id, doorbell_info);
                print_str(g_uartBuf);

                /* 根据 doorbell_info 打印特定内存区域 */
                if (doorbell_info == 0) {
                    print_core("Doorbell info=0: printing three memory areas...\n");
                    PrintMemory256(0x80000000);
                    PrintMemory256(0x80400000);
                    PrintMemory256(0x80800F00);
                } else if (doorbell_info == 1) {
                    print_core("Doorbell info=1: printing three memory areas...\n");
                    PrintMemory256(0x81000000);
                    PrintMemory256(0x81400000);
                    PrintMemory256(0x81800F00);
                }
            } else {
                /* 摘要模式：每 100 个门铃打印一次，附带 info 和分类计数 */
                if (doorbell_total_rx % 100 == 0) {
                    sprintf(g_uartBuf,
                        "[Core0] Doorbell received: total=%d, info0=%d, info1=%d\n",
                        doorbell_total_rx, doorbell_rx_info0, doorbell_rx_info1);
                    print_str(g_uartBuf);
                }
            }
        }
    }

    RAB_INTR_STAT_MISC = 0xFFFFFFFF;
}

/* ==================== SRIO 完整重新初始化 ==================== */
void SrioFullReinit(void)
{
    unsigned int ret;
    struct SrioManulStruct srio_cfg;
    struct SRIOinit srio_id;

    sprintf(g_uartBuf, "[Core0] Reinitializing %s SRIO...\n", DSP_NAME);
    print_str(g_uartBuf);

    SrioClkClose(SrioInstance0);
    DelaySRIO(1000);

    memset(&srio_cfg, 0, sizeof(srio_cfg));
    srio_cfg.WorkSpeed   = TEST_SPEED;
    srio_cfg.LaneMode    = TEST_LANE;
    srio_cfg.RefClkSrc   = 0;
    srio_cfg.TXEq[0]     = TxEq_0;
    srio_cfg.TXEq[1]     = TxEq_0;
    srio_cfg.TXEq[2]     = TxEq_0;
    srio_cfg.TXEq[3]     = TxEq_0;
    srio_cfg.SynTimeOut  = 0x10000;

    int link_attempts = 0;
    do {
        ret = SrioManulConfig(SrioInstance0, srio_cfg);
        if (ret == 0) break;
        link_attempts++;
    } while (link_attempts < 100);

    if (ret != 0) {
        error_core("Reinit: Link timeout!\n");
        return;
    }
    print_str("[Core0] Reinit: Link established.\n");

    srio_id.IDLen  = SrioIDLen8;
    srio_id.SelfID = LOCAL_SELF_ID;
    SrioCtl(SrioInstance0, &srio_id);
    sprintf(g_uartBuf, "[Core0] Reinit: Self ID 0x%x set.\n", LOCAL_SELF_ID);
    print_str(g_uartBuf);

    SrioMatchACKID(SrioInstance0, REMOTE_DEVICE_ID, 0);
    sprintf(g_uartBuf, "[Core0] Reinit: ACKID 0x%x matched.\n", REMOTE_DEVICE_ID);
    print_str(g_uartBuf);

    SrioRPIOMap(SrioInstance0);

    /* 硬件稳定延时 */
    DelaySRIO(1000);

    /* 使能门铃接收 */
    ClearDoorbellFifo();
    SrioEnRecvDB(SrioInstance0);
    print_str("[Core0] Reinit: Doorbell Rx enabled.\n");

    print_str("[Core0] Reinit completed.\n");
}

static int sendCnt = 0;
/* ==================== 发送函数 ==================== */
int SendToRemote_SWrite(unsigned short dstID, unsigned int remoteAddr,
                        unsigned char *data, unsigned short dataLen)
{
    int nSWcnt = 0;
    unsigned int totalLen;

    if (dataLen > 256) dataLen = 256;
    if (dataLen % 8 != 0) {
        error_core("Error: dataLen must be multiple of 8\n");
        return -1;
    }

    totalLen = dataLen;
    memcpy(g_txBuf, data, dataLen);
    asm(" mfence");

    struct SRIODMA dma;
    memset(&dma, 0, sizeof(dma));
    dma.DstID           = dstID;
    dma.SrcAdd          = (unsigned long long)g_txBuf;
    dma.DstAdd          = remoteAddr;
    dma.Priority        = 0;
    dma.CRF             = 0;
    dma.EnDescriptor    = 0;
    dma.FirstDescriptorAdd = 0;
    dma.DataLen         = totalLen / 4;
    dma.EnSendDB        = 0;

    g_wdma_done = 0;
    SrioWDMA(SrioInstance0, &dma);

    while (!g_wdma_done) {
        nSWcnt++;
        if (nSWcnt > 1000000) {
            g_wdma_done = 1;
            error_core("Send error.\n");
            return -1;
        }
        if (RAB_INTR_STAT_WDMA) {
            RAB_INTR_STAT_WDMA = RAB_INTR_STAT_WDMA;
            g_wdma_done = 1;
            break;
        }
    }
    sendCnt++;
    if(sendCnt % 1000 == 0)
    {
        sprintf(g_uartBuf, "[Core0]  Send OK %d.\n", sendCnt);
        print_str(g_uartBuf);
    }

    return 0;
}

void PrintRawData(unsigned int rx_addr)
{
    volatile unsigned char *raw = (volatile unsigned char *)rx_addr;
    print_str("\n");
    print_str("#########################################\n");
    sprintf(g_uartBuf, "#  Received Data[%d] (First 128 bytes):\n", rev_counter);
    print_str(g_uartBuf);
    print_str("#########################################\n");
    UART_PrintHex((unsigned char*)raw, 128);
    print_str("#########################################\n\n");
}

/* ==================== 主函数 ==================== */
int main(void)
{
    unsigned int ret;
    struct SrioManulStruct srio_cfg;
    struct SRIOinit srio_id;
    unsigned char test_data[256];
    static unsigned int send_counter = 0;
    static unsigned int send_pkgCnt = 0;
    unsigned int last_pattern = 0xEEEEEEEE;
    int aligned_len;
    int i;

    SysInit();
    PASSPLL();


    UART_Init(BaudRate_Value);
    /* 初始化阶段的打印直接输出，不受控制 */
    UART_Print("[Core0] \n");
    UART_Print("[Core0] =========================================\n");
    sprintf(g_uartBuf, "[Core0]   %s SRIO Full Demo (Doorbell Polling)\n", DSP_NAME);
    UART_Print(g_uartBuf);
    sprintf(g_uartBuf, "[Core0]   Self ID: 0x%x | Remote ID: 0x%x\n", LOCAL_SELF_ID, REMOTE_DEVICE_ID);
    UART_Print(g_uartBuf);
    UART_Print("[Core0]   Mode: 5Gbps 1X 8bit ID\n");
    UART_Print("[Core0]   Press 'r' to reset SRIO link\n");
    UART_Print("[Core0]   Press 's' to start sending, 'q' to stop\n");
    UART_Print("[Core0]   Press 'd' to print doorbell registers\n");
    UART_Print("[Core0]   Press 'p' to toggle verbose/summary log\n");
    UART_Print("[Core0]   Press 'n' to toggle print enable/disable\n");
    UART_Print("[Core0]   Press 'm' to show command menu\n");
    UART_Print("[Core0] =========================================\n");

    sprintf(g_uartBuf, "[Core0] %s System initialized.\n", DSP_NAME);
    UART_Print(g_uartBuf);

    DDR_entry(DDR_CLK_800MHZ, ROW_15, WIDTH_x16, NO_ECC_TYPE);

    SrioClkClose(SrioInstance0);
    UART_Print("[Core0] SRIO0 clock closed.\n");

    memset(&srio_cfg, 0, sizeof(srio_cfg));
    srio_cfg.WorkSpeed   = TEST_SPEED;
    srio_cfg.LaneMode    = TEST_LANE;
    srio_cfg.RefClkSrc   = 0;
    srio_cfg.TXEq[0]     = TxEq_0;
    srio_cfg.TXEq[1]     = TxEq_0;
    srio_cfg.TXEq[2]     = TxEq_0;
    srio_cfg.TXEq[3]     = TxEq_0;
    srio_cfg.SynTimeOut  = 0x10000;

    UART_Print("[Core0] Waiting for link...\n");
    int link_attempts = 0;
    do {
        ret = SrioManulConfig(SrioInstance0, srio_cfg);
        if (ret == 0) break;
        link_attempts++;
        sprintf(g_uartBuf, "[Core0] link times : %d \n", link_attempts);
        UART_Print(g_uartBuf);
    } while (link_attempts < 100);
    if (ret != 0) {
        UART_Print("[Core0] Initial link timeout!\n");   /* 致命错误，直接打印 */
        return -1;
    }
    UART_Print("[Core0] SRIO0 linked successfully.\n");

    srio_id.IDLen  = SrioIDLen8;
    srio_id.SelfID = LOCAL_SELF_ID;
    SrioCtl(SrioInstance0, &srio_id);
    UART_Print("[Core0] Self ID set.\n");

    UART_Print("[Core0] Matching ACKID...\n");
    SrioMatchACKID(SrioInstance0, REMOTE_DEVICE_ID, 0);
    UART_Print("[Core0] ACKID matched.\n");

    SrioRPIOMap(SrioInstance0);

    DelaySRIO(1000);
    ClearDoorbellFifo();
    SrioEnRecvDB(SrioInstance0);
    UART_Print("[Core0] Doorbell Rx enabled. Waiting for remote doorbell...\n");

    UART_Print("[Core0] Init done. Entering main loop.\n");

    while (1) {
        ProcessSerialCommand();

        /* 最高优先级：轮询门铃 */
        PollDoorbellFifo();

        /* 发送逻辑：仅在使能时运行 */
        if (send_enabled) {
            send_counter++;
            if (send_counter >= 40000) {
                send_counter = 0;

                aligned_len = ((SEND_LEN + 7) / 8) * 8;

                test_data[0] = 0xAA;
                for (i = 1; i < SEND_LEN - 1; i++) {
                    test_data[i] = fill_byte;
                }
                test_data[SEND_LEN - 1] = 0xFF;

                for (i = SEND_LEN; i < aligned_len; i++) {
                    test_data[i] = 0x00;
                }

                int result = SendToRemote_SWrite(REMOTE_DEVICE_ID,
                                                 REMOTE_ADDR_SWRITE,
                                                 test_data,
                                                 (unsigned short)aligned_len);
                if (result == 0) {
                    send_pkgCnt++;
                    fill_byte++;
                } else {
                    error_core("Send failed! Check link.\n");
                }
            }
        }
    }
    return 0;
}
