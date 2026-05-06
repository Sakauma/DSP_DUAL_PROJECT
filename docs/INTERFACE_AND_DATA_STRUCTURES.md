# 接口与数据结构说明

本文档由 `docs/` 下三份 Word 资料提取整合而成，作为后续开发和代码审查时的接口参考。远程仓库只同步本 Markdown，不同步 Word 原件。

## 1. 使用边界

- DSP 算法侧只消费硬件接口层提供的图像地址、帧号、候选目标、完成标志等字段。
- SRIO 包格式、doorbell 语义、FPGA 初筛结构体、视频接口时序由硬件部门定义。
- Core0 统一封装 SRIO 交互和硬件接口适配；Core1-Core7 只做算法任务，不直接调用 SRIO DMA。
- 任何从 Word 原件新增或修正的接口信息，都应同步更新本文档。

## 2. SRIO 当前状态与关键限制

来源：`SRIO_API说明文档.docx`

- SRIO 线路：`5G 1x 8bit 8通道`。
- DSP 信息交互基于 MSMC 内存区域。
- 当前支持双 DSP 间由 `core0` 处理核进行 SRIO 交互，也支持单 DSP 对外 SRIO 交互，并可串口输出。
- `SrioCtl()` 写入的 `BDIDCSR` 是整个 SRIO 硬件共享的 SelfID 寄存器，后写会覆盖前写。
- `SrioWDMA()` 固定操作 `RAB_WDMA_0_CTRL`，即 WDMA 通道0；无多通道调度、无互斥保护，多核同时调用会导致寄存器配置冲突。
- `SRIODMA.DataLen` 的单位是 32-bit word，不是 byte。
- 点对点非交换机场景下，`SrioMatchACKID()` 的 `RemotePort` 固定填 `0`。

## 3. SRIO 核心数据结构

### 3.1 `struct SRIOAPIOStruct`

用于 `SrioCreatAPIOWindow()`，配置 SRIO APIO 出站地址映射窗口。

```c
struct SRIOAPIOStruct {
    unsigned int WindowNub;      // 窗口号，0-15
    unsigned int DstID;          // 目的 ID
    unsigned int WindowTYPE;     // 窗口类型
    unsigned int WindowSize;     // 窗口大小
    unsigned long long RIOBase;  // SRIO 基地址
    unsigned long long AXIBase;  // AXI 基地址
    unsigned int Priority;       // 优先级
    unsigned int hopcount;       // 跳数
    unsigned int CRF;            // CRF 值
};
```

### 3.2 `struct SrioManulStruct`

用于 `SrioManulConfig()`，配置 SRIO 手动建链参数。

```c
struct SrioManulStruct {
    unsigned int WorkSpeed;   // 工作速率，如 Speed1250/Speed2500/Speed5000
    unsigned int LaneMode;    // 链路模式，如 lane4x/lane2xl0l1/lane1xl0
    unsigned int RefClkSrc;   // 参考时钟源
    unsigned int TXEq[4];     // 4 个通道均衡参数
    unsigned int SynTimeOut;  // 同步超时时间
    unsigned int EnLoopBack;  // 回环使能
    unsigned int TxPolarity;  // 发送极性反转
    unsigned int RxPolarity;  // 接收极性反转
};
```

### 3.3 `struct SRIOinit`

用于 `SrioCtl()`，配置 SRIO 端口设备 ID。

```c
struct SRIOinit {
    unsigned int IDLen;   // ID 长度，SrioIDLen8/SrioIDLen16
    unsigned int SelfID;  // 本端口 ID
};
```

### 3.4 `struct SRIODMA`

用于 `SrioWDMA()` / `SrioRDMA()`，配置 DMA 读写传输。

```c
struct SRIODMA {
    unsigned int DstID;                    // 目的 ID
    unsigned long long SrcAdd;             // 源地址
    unsigned long long DstAdd;             // 目的地址
    unsigned int Priority;                 // 优先级
    unsigned int CRF;                      // CRF 值
    unsigned int EnDescriptor;             // 描述符使能
    unsigned long long FirstDescriptorAdd; // 第一个描述符地址
    unsigned int DataLen;                  // 数据长度，单位：32-bit word
    unsigned int EnSendDB;                 // 发送完成后发 Doorbell 使能
};
```

## 4. SRIO 核心 API

| API | 功能 | 关键参数/返回 |
| --- | --- | --- |
| `DelaySRIO(unsigned int cnt)` | SRIO 初始化时序等待。 | `cnt` 越大延时越长；无返回值。 |
| `SrioChangeEnding(unsigned int Input)` | 32-bit 数据按字节大小端转换。 | 返回转换后的 32-bit 数据。 |
| `SrioCreatAPIOWindow(unsigned int Instance, struct SRIOAPIOStruct *cfg)` | 创建 SRIO APIO 出站窗口，实现本地地址到对端 SRIO 地址映射。 | `Instance` 为 `SrioInstance0/1`；返回 `0` 成功，`1` 窗口号非法。 |
| `SrioRPIOMap(unsigned int Instance)` | 配置 SRIO 入站地址映射。 | 使用 SRIO 地址 `bit[31:28]` 选择 LUT，配置 16 个 LUT，实现入站 1:1 映射。 |
| `SrioClkOpen(unsigned int SrioInstance)` | 打开 SRIO PSC 电源和时钟。 | 返回 `0`。 |
| `SrioClkClose(unsigned int SrioInstance)` | 关闭 SRIO PSC 电源和时钟。 | 返回 `0`。 |
| `SrioPhyInit(unsigned int SrioInstance, unsigned int Speed, unsigned Lane)` | 初始化 SRIO PHY 速率和链路宽度。 | `Speed` 可选 `Speed1250/2500/3125/5000/6250`；返回 `0`。 |
| `SrioManulConfig(unsigned int SrioInstance, struct SrioManulStruct cfg)` | SRIO 手动建链核心函数。 | 返回 `0` 建链成功，`1` 超时，`2` 链路宽度不匹配。 |
| `SrioCtl(unsigned int SrioInstance, struct SRIOinit *cfg)` | 配置 ID 长度和 SelfID。 | 注意 SelfID 寄存器全局共享。 |
| `SrioMatchACKID(unsigned int SrioInstance, unsigned int DstID, unsigned int RemotePort)` | 匹配 SRIO ACKID 流控。 | 返回对端 Inbound ACKID。 |
| `SrioWDMA(unsigned int SrioInstance, struct SRIODMA *dma)` | 使用 WDMA 通道0向对端发送数据。 | 返回 `0` 表示启动成功；多核不可并发调用。 |
| `SrioRDMA(unsigned int SrioInstance, struct SRIODMA *dma)` | 使用 RDMA 从对端读取数据。 | 返回 `0` 表示启动成功。 |
| `SrioConfigSendDB(unsigned int SrioInstance, struct SRIODoorBellStruct *db)` | 配置并发送 Doorbell。 | Doorbell info 低 16 位有效。 |
| `SrioEnRecvDB(unsigned int SrioInstance)` | 使能 Doorbell 接收。 | 无返回值。 |

## 5. EB4110 图像输出接口

来源：`EB4110预处理图像输出设计报告.docx`

### 5.1 图像配置寄存器

| 序号 | 寄存器名称 | 地址 | 默认值 | 用途 |
| --- | --- | --- | --- | --- |
| 1 | 视频输出 SID-DID 设置 | `0x8600_0000` | `32'h0051_0061` | 默认源 ID `0x51`，目的 ID `0x61`。 |
| 2 | 视频输入 `srio_x1` 选择 | `0x8600_0004` | `0` | `1` 表示选择 `srio_x1` 通道；默认选择 `srio_x2`。 |
| 3 | SRIO 视频输出使能 | `0x8600_0008` | `1` | 写 `1` 开启输出，写 `0` 关闭。 |
| 4 | SRIO 视频降频值 | `0x8600_000C` | `4` | `0` 不降频；`n` 表示输入 `4*n` 帧输出 1 帧。 |

### 5.2 源 ID 与目的 ID

- 预处理1到 DSP1/V7_1：源 ID `0x51`，目的 ID `0x61`。
- 预处理2到 DSP2/V7_2：源 ID `0x52`，目的 ID `0x62`。
- 预处理软件、DSP 软件和 V7 接收逻辑都必须保持 ID 配置一致。

### 5.3 图像布局

- 分辨率：`2049 x 2048`。
- 像素位宽：`16 bit`。
- 图像模式：单色，SRIO 中依次从 `bit15` 到 `bit0` 发送。
- 传输模式：SRIO `SWRITE`。
- 每行需要 `2048 * 2 / 256 = 16` 个 SWRITE 报文。
- 每帧需要 `2049 * 16 = 32784` 个 SWRITE 报文。
- 每帧图像 SRIO 报文基地址范围：`0x0000_0000 ~ 0x0080_0F00`。
- 像素基地址范围：`0x0000_0000 ~ 0x0080_0FFF`。

### 5.4 行地址描述

| 行号 | 内容 | SRIO 报文基地址 | 数据基地址 |
| --- | --- | --- | --- |
| 第 0 行 | 参数行 | `0x0000_0000 ~ 0x0000_0F00` | `0x0000_0000 ~ 0x0000_0FFF` |
| 第 1 行 | 红外图像数据 | `0x0000_1000 ~ 0x0000_1F00` | `0x0000_1000 ~ 0x0000_1FFF` |
| 第 2 行 | 红外图像数据 | `0x0000_2000 ~ 0x0000_2F00` | `0x0000_2000 ~ 0x0000_2FFF` |
| 第 x 行 | 红外图像数据 | `0x0000_x000 ~ 0x0000_xF00` | `0x0000_x000 ~ 0x0000_xFFF` |
| 第 2048 行 | 红外图像数据 | `0x0080_0000 ~ 0x0080_0F00` | `0x0080_0000 ~ 0x0080_0FFF` |

算法默认跳过第 0 行参数行，只处理第 1-2048 行红外图像。

### 5.5 参数行帧计数

- 地址 `0x0000_0002 ~ 0x0000_0003`：图像帧计数低 16 bit。
- 地址 `0x0000_0004 ~ 0x0000_0005`：图像帧计数高 16 bit。
- 参数行第一个 SRIO 报文中，地址为 `0x00000000` 的数据包含上述帧计数字段。

### 5.6 Ping-pong 输出流程

EB4110 为适配 DSP 乒乓处理，对图像帧分配两个偏移地址：

- Buffer 0：`0x8000_0000 ~ 0x8080_0F00`，帧完成后输出 `info=0` doorbell。
- Buffer 1：`0x8100_0000 ~ 0x8180_0F00`，帧完成后输出 `info=1` doorbell。

循环流程：

1. 准备输出第 `x` 帧。
2. 按 `0x8000_0000` 偏移输出第 `x+0` 帧。
3. 输出 `info=0` doorbell。
4. 按 `0x8100_0000` 偏移输出第 `x+1` 帧。
5. 输出 `info=1` doorbell。
6. `x = x + 1`，重复输出流程。

### 5.7 通讯速率

- 图像负载：`2049 * 2048 * 16 bit`。
- SWRITE 报文头：`32784 * 64 bit`。
- Doorbell：`64 bit`。
- 全速 50 Hz 时，负载数据通讯速率约 `3.3570816 Gbps`。
- 全部报文通讯速率约 `3.4619936 Gbps`。

## 6. 预处理 SRIO 转视频处理接口

来源：`预处理SRIO接口转视频处理接模块口和仿真工程简介.docx`，密码 `1`。

### 6.1 BRAM 资源设计

- 每帧图像大小：`2049 * 2048 * 2 = 0x800100` 字节。
- 通过 parameter 调整图像深度。
- 当 `A_RAM_DEPTH = 256 * 512` 时，BRAM 缓存行深度为 256 行，占用 256 个 BRAM。

### 6.2 主要文件

- `vbram_lutaxi4_to_axis.v`：视频处理接口文件。
- `SRIO_2_Video.v`：SRIO 和视频数据转换顶层模块。
- 仿真工程中，`vbram_lutaxi4_to_axis.v` 的读 BRAM 数据通路已通过硬件测试：从 BRAM 按 16-bit 读取数据，再拼接为 64-bit AXIS 发送给 SRIO。

### 6.3 BRAM 写状态接口

用于防止读出 BRAM 和写入 BRAM 重叠。

| 信号 | 含义 |
| --- | --- |
| `bram_line_cur_w` | 当前写入 BRAM 的行地址位置。 |
| `bram_line_cur_w_en` | 为 `1` 表示第 `bram_line_cur_w` 行成功写入。 |

### 6.4 BRAM 行号接口

用于标识视频 BRAM 中已存储的数据对应视频第几行。

| 信号 | 含义 |
| --- | --- |
| `bram_line_num_addr` | 每段 BRAM 地址对应的行号地址。 |
| `bram_line_num` | 每段 BRAM 地址对应的行号。`bram_line_num_addr * 0x0 ~ bram_line_num_addr * 0x800` 对应该行不同列地址。 |

### 6.5 BRAM 视频数据读取接口

| 信号 | 位宽 | 含义 |
| --- | --- | --- |
| `bram_addrb` | 16 bit | 视频 BRAM 地址。 |
| `bram_doutb` | 16 bit | 视频数据。 |

### 6.6 图像处理输出 AXIS 接口

图像处理后按行输出，使用 AXIS 格式。

```verilog
output wire [63:0] video_i_axis_tdata,
output wire [31:0] video_i_axis_tuser,
input  wire        video_i_axis_tready,
output wire        video_i_axis_tvalid,
output wire        video_i_axis_tlast
```

### 6.7 SRIO AXIS 接口

SRIO 视频输出接口为标准 AXIS 总线。

```verilog
m_srio_axis_aclk
m_srio_axis_rstn
m_srio_axis_tdata
m_srio_axis_tready
m_srio_axis_tvalid
m_srio_axis_tlast
```

### 6.8 DDR AXI4 读接口

LUT 读取接口为标准 AXI4 读 DDR 接口。

```verilog
output wire [3:0]  V_LUT_AXI_ARID,
output wire [31:0] V_LUT_AXI_ARADDR,
output wire [7:0]  V_LUT_AXI_ARLEN,
output wire [2:0]  V_LUT_AXI_ARSIZE,
output wire [1:0]  V_LUT_AXI_ARBURST,
output wire        V_LUT_AXI_ARLOCK,
output wire [3:0]  V_LUT_AXI_ARCACHE,
output wire [2:0]  V_LUT_AXI_ARPROT,
output wire [3:0]  V_LUT_AXI_ARQOS,
output wire        V_LUT_AXI_ARVALID,
input  wire        V_LUT_AXI_ARREADY,
input  wire [3:0]  V_LUT_AXI_RID,
input  wire [63:0] V_LUT_AXI_RDATA,
input  wire [1:0]  V_LUT_AXI_RRESP,
input  wire        V_LUT_AXI_RLAST,
input  wire        V_LUT_AXI_RVALID,
output wire        V_LUT_AXI_RREADY
```

### 6.9 仿真工程输入输出

当前仿真工程：`EB4110_FPGA_SRIO_2_BRAM`

- 输入：两帧图像 + 半个深度 BRAM 图像的 SRIO 视频接收数据，信号前缀为 `srio_r_axis**`。
- 输出：两帧图像的 SRIO 视频发送数据，信号前缀为 `SRIO_T_axi**`。

## 7. DSP 算法侧对接要求

- 算法只读取硬件接口层转换后的图像视图、帧号、候选列表和 ping-pong 状态。
- Core0 是唯一允许统一封装 SRIO API 的 DSP 侧上下文。
- 从核只处理 tile 算法，不直接访问 `SrioWDMA()`、`SrioRDMA()`、`SrioCtl()`、`SrioMatchACKID()` 或 Doorbell API。
- 算法处理有效图像区域时跳过第 0 行参数行，默认输入图像为 `2048 x 2048` 有效红外数据。
- 每个算法结果必须携带帧号，防止 ping-pong buffer 切换时跨帧混读。
- 若硬件部门修改 Word 原始接口说明，应同步更新本 Markdown，再更新算法开发指南。
