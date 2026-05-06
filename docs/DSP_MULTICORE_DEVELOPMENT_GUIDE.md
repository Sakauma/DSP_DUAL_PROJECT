# DSP 多核图像算法开发指南

## 1. 定位与边界

本指南只覆盖 DSP 侧算法开发。SRIO 包格式、doorbell 语义、图像帧地址编排、FPGA 初筛结果结构体等接口细节由硬件部门定义并维护，DSP 算法侧不再自拟数据包结构。

DSP 算法模块只依赖硬件接口层提供的抽象输入：

- 当前帧图像缓冲区地址。
- 前一帧或历史帧缓冲区地址。
- 帧号或帧计数。
- FPGA 初筛候选目标列表。
- 当前 ping-pong buffer index。
- 帧完成/数据可用标志。

Core0 可以调用或封装现有 SRIO API 完成数据接收和状态同步，但 Core1-Core7 不直接操作 SRIO DMA 寄存器，也不参与 SRIO 包解析。

## 2. 已确认工程与文档约束

当前仓库包含两个 CCS 工程：`M6678_SRIO_DSP1/` 和 `M6678_SRIO_DSP2/`。两者基本一致，主要身份差异在 `main.c`：

- DSP1: `LOCAL_SELF_ID 0x61`，`DSP_NAME "DSP1"`。
- DSP2: `LOCAL_SELF_ID 0x62`，`DSP_NAME "DSP2"`。

`docs/SRIO_API说明文档.docx` 给出的 SRIO 约束：

- SRIO 线路状态：`5G 1x 8bit 8通道`。
- 当前 DSP 内存交互基于 MSMC 区域。
- 当前支持双 DSP 间由 Core0 处理核进行 SRIO 交互，也支持单 DSP 对外 SRIO 交互。
- `SrioCtl()` 配置的 `BDIDCSR` 是整个 SRIO 硬件共享的 SelfID 寄存器，后写会覆盖前写。
- `SrioWDMA()` 固定操作 WDMA 通道0，没有多通道调度和互斥保护，多核同时调用会导致寄存器配置冲突。
- `SRIODMA.DataLen` 的单位为 32-bit word，不是 byte。
- `SrioRPIOMap()` 使用 SRIO 地址 bit[31:28] 选择 LUT，并配置入站地址到 DSP 内部地址的 1:1 映射。

`docs/EB4110预处理图像输出设计报告.docx` 给出的图像输入约束：

- 图像分辨率为 `2049 x 2048`，每像素 `16 bit`，单色模式。
- 第 0 行为参数行，第 1-2048 行为红外图像数据。
- 参数行中包含图像帧计数低 16 bit 和高 16 bit。
- 预处理1到 DSP1/V7_1：源 ID `0x51`，目的 ID `0x61`。
- 预处理2到 DSP2/V7_2：源 ID `0x52`，目的 ID `0x62`。
- 图像按 ping-pong 地址输出，文档给出的两个偏移为 `0x8000_0000` 和 `0x8100_0000`。
- 每两帧之间插入 doorbell，`info=0` 与 `info=1` 分别对应两个 ping-pong 帧完成事件。

`docs/预处理SRIO接口转视频处理接模块口和仿真工程简介.docx` 当前需要密码，暂未纳入本指南依据。

## 3. 算法总体架构

### 3.1 Core0 主核职责

Core0 负责控制面、任务调度和全局跟踪，不承担重型像素计算：

- 从硬件接口层获取当前帧地址、帧号、ping-pong buffer index 和 FPGA 候选列表。
- 校验帧号连续性，维护当前帧、前一帧和历史帧索引。
- 将有效图像区域切分为 tile，并为 Core1-Core7 分配任务。
- 收集从核输出的确认目标、虚警目标、漏检候选和错检候选。
- 合并 tile 边界重复目标，将局部坐标转换为整帧坐标。
- 执行卡尔曼滤波、匈牙利匹配和轨迹生命周期管理。
- 输出最终目标验证结果和轨迹状态。

Core0 可以统一封装 SRIO API，但必须保证同一时刻只有一个上下文操作 `SrioWDMA()` / `SrioRDMA()`，避免 WDMA 通道0冲突。

### 3.2 Core1-Core7 从核职责

从核只处理 tile 内算法：

- 读取 Core0 分配的当前 tile、前一帧 tile、历史 DoG 摘要和候选目标子集。
- 执行帧差、DoG、局部阈值、连通域和候选框验证。
- 对 FPGA 候选输出 `confirmed`、`false_alarm`、`wrong_detection`。
- 对强帧差/强 DoG 但未被 FPGA 候选覆盖的区域输出 `miss_candidate`。
- 将结果写回共享结果队列并设置完成标志。

从核不得直接调用 SRIO DMA API，不修改 SelfID，不配置 ACKID，不操作 doorbell。

## 4. 算法输入与内部数据模型

硬件部门定义外部接口；DSP 算法内部只保留与算法计算相关的归一化模型。接口适配层负责把硬件字段转换为以下内部结构，字段名称可按实际代码调整。

```c
typedef struct {
    unsigned int frame_id;
    unsigned int pingpong_index;
    unsigned short width;
    unsigned short height;
    unsigned short valid_y0;
    unsigned short valid_height;
    unsigned int curr_image_addr;
    unsigned int prev_image_addr;
    unsigned int fpga_candidate_count;
    const void *fpga_candidate_list;
} AlgoFrameView;
```

```c
typedef struct {
    unsigned short tile_id;
    unsigned short core_id;
    unsigned short x0;
    unsigned short y0;
    unsigned short width;
    unsigned short height;
    unsigned int curr_addr;
    unsigned int prev_addr;
    const void *candidate_begin;
    unsigned short candidate_count;
} AlgoTileTask;
```

```c
typedef struct {
    short cx;
    short cy;
    unsigned short w;
    unsigned short h;
    unsigned int diff_energy;
    unsigned int dog_energy;
    unsigned int area;
    unsigned short flags;
} AlgoDetectionResult;
```

注意：这些是 DSP 算法内部视图，不是 SRIO 包格式，也不要求硬件部门按此传输。

## 5. 图像预处理与切片策略

EB4110 输出图像第 0 行为参数行，算法默认只处理第 1-2048 行红外图像数据。

建议切片策略：

- 输入有效图像区域：`2048 x 2048`。
- 纵向按 Core1-Core7 划分 7 个 tile，或按更小 tile 建队列动态分配。
- 每个 tile 上下各保留 1-2 行 halo，用于 DoG/形态滤波和连通域边界处理。
- FPGA 候选框按中心点或外接框与 tile 的交叠分发到从核。
- 边界目标由 Core0 汇总阶段按 IoU 和中心距离去重。

历史帧建议采用环形缓冲：

- `prev_frame`：上一帧完整图像。
- `dog_hist[k]`：最近若干帧的 DoG 摘要，不保存完整 DoG 图像时至少保存目标局部峰值、能量、面积和质心。
- `track_hist`：轨迹历史状态，由 Core0 维护。

## 6. 从核目标验证算法

### 6.1 帧差法

每个 tile 内执行：

1. `diff = abs(curr - prev)`。
2. 对 `diff` 做阈值分割，得到运动 mask。
3. 对 mask 做小尺度形态滤波，去掉孤立点和单行噪声。
4. 做连通域标记，统计面积、外接框、质心和差分能量。
5. 将连通域与 FPGA 候选框做 IoU、中心距离和能量一致性检查。

第一版建议使用 8/16-bit 定点运算，避免浮点和动态内存。连通域表使用固定上限，溢出时保留面积/能量最大的前 N 个区域。

### 6.2 DoG 特征

DoG 用于发现漏检、错检和弱目标：

1. 小尺度高斯或近似平滑得到 `Gs`。
2. 大尺度高斯或近似平滑得到 `Gb`。
3. `DoG = Gs - Gb`。
4. 对 DoG 响应做阈值和局部峰值检测。
5. 输出局部峰值、响应面积、DoG 能量和质心。

DSP 第一版不直接移植 OpenCV。建议使用 separable 3x3/5x5 定点近似核，先保证稳定性，再逐步优化。

### 6.3 FPGA 候选验证

对每个 FPGA 候选框统计：

- 框内帧差总能量。
- 框内 DoG 峰值和 DoG 总能量。
- 运动 mask 覆盖率。
- 目标面积、宽高比和边界位置。
- 与上一帧轨迹预测位置的距离。

判定输出：

- `confirmed`：帧差或 DoG 支持，且形态合理。
- `false_alarm`：FPGA 有候选，但帧差、DoG、历史轨迹均不支持。
- `wrong_detection`：响应存在，但位置、尺度或能量与预测明显不一致。
- `miss_candidate`：帧差/DoG 强响应未被 FPGA 候选覆盖。

## 7. Core0 跟踪融合

### 7.1 卡尔曼状态向量

为适应目标从中心到边缘的形态变化，状态向量加入面积和能量：

```text
x = [cx, cy, vx, vy, area, d_area, energy, d_energy]
z = [cx, cy, area, energy]
```

含义：

- `cx, cy`：目标中心。
- `vx, vy`：中心速度。
- `area`：目标面积或候选框面积。
- `d_area`：面积变化率。
- `energy`：灰度、帧差或 DoG 综合能量。
- `d_energy`：能量变化率。

第一版使用线性卡尔曼，固定 `dt = 1 frame`。如果边缘形变明显非线性，再评估 EKF。

### 7.2 匈牙利匹配代价

匹配代价建议由以下项组成：

```text
cost =
  w_pos    * position_distance
+ w_area   * normalized_area_error
+ w_energy * normalized_energy_error
+ w_iou    * (1 - IoU)
+ w_alg    * algorithm_penalty
```

推荐初值：

- `w_pos = 0.45`
- `w_area = 0.20`
- `w_energy = 0.20`
- `w_iou = 0.10`
- `w_alg = 0.05`

`algorithm_penalty` 用于区分目标来源：FPGA 确认目标、DSP 补检目标、低可信目标。补检目标允许参与匹配，但新建轨迹需要更严格确认。

### 7.3 轨迹生命周期

建议状态：

- `tentative`：新目标，连续命中不足 3 帧。
- `confirmed`：稳定目标。
- `lost`：短时未匹配，保留预测。
- `dead`：超过最大丢失帧数，删除。

建议默认参数：

- 连续命中 3 帧确认。
- 普通区域允许丢失 5 帧。
- 边缘区域允许丢失 8 帧。
- 若面积和能量仍符合预测，可延长保留。

## 8. 开发阶段

### 阶段 1：接口适配检查

目标：只确认算法能正确消费硬件接口层提供的数据。

- 读取帧号、ping-pong index、当前帧地址和候选目标数量。
- 跳过参数行，只处理 2048 行红外图像。
- 校验 `0x8000_0000` / `0x8100_0000` 两个 buffer 是否按预期切换。
- 确认 DSP1/DSP2 的目的 ID 与硬件配置一致。

验收：

- 连续输入 1000 帧，帧号连续，buffer index 切换正确。
- 算法入口不关心 SRIO 包字段，只接收已转换的内部视图。

### 阶段 2：单核算法原型

目标：先在一个核心上验证帧差、DoG 和候选验证。

- 实现固定阈值帧差。
- 实现定点 DoG 近似。
- 实现连通域统计。
- 对 FPGA 候选输出确认、虚警和补检结果。

验收：

- 固定回放数据下结果可重复。
- 能解释每个虚警/漏检判定依据。

### 阶段 3：多核 tile 并行

目标：Core0 分发任务，Core1-Core7 并行处理。

- 建立 MSMC 任务表和结果队列。
- 每个从核只处理自己的 tile。
- 明确 cache writeback/invalidate。
- Core0 汇总并去重边界目标。

验收：

- 7 个从核均可独立完成 tile。
- 同一帧所有 tile 完成后，Core0 再进入跟踪融合。

### 阶段 4：跟踪融合

目标：完成 Kalman + Hungarian。

- 使用固定最大轨迹数和候选数。
- 禁止运行期 `malloc`。
- 面积和能量加入状态预测与匹配代价。
- 支持短时漏检保持轨迹。

验收：

- 目标从中心移动到边缘时，轨迹 ID 不因面积/能量变化频繁中断。
- FPGA 漏检短时出现时，DSP 补检可维持轨迹。

### 阶段 5：性能优化

目标：满足实时帧率。

- 优先优化帧差、DoG 卷积、阈值、连通域。
- 数据按 8/16 字节对齐。
- 使用定点、查表和行缓存。
- 热点参考 TI IMGLIB 的 C66x 优化思路。

## 9. 学习路线

1. 阅读 `docs/SRIO_API说明文档.docx`，明确 SRIO API 能力和限制，尤其是 WDMA 通道0限制。
2. 阅读 `docs/EB4110预处理图像输出设计报告.docx`，明确图像尺寸、参数行、帧计数、ping-pong 地址和 DSP1/DSP2 ID。
3. 阅读当前工程 `main.c`、`M6678.cmd`、`srio/M6678_SRIO.c`，理解现有初始化和内存布局。
4. 在 PC 端用 OpenCV 验证帧差、DoG、连通域、候选验证策略。
5. 将算法改写为 C66x 友好的 C：静态内存、定点优先、固定数组上限。
6. 实现多核 tile 并行和 cache 一致性。
7. 实现 Kalman + Hungarian，并加入面积/能量状态。
8. 用回放数据覆盖虚警、漏检、错检、边缘形变和目标交叉场景。

## 10. 推荐资料与开源仓库

- [TI OpenMP-DSP 文档](https://downloads.ti.com/mctools/esd/docs/openmp-dsp/intro.html)：用于理解 C667x 多核并行可选路线。
- [TI C64x+/C66x IMGLIB](https://www.ti.com/tool/SPRC264)：参考 C66x 图像处理热点优化。
- [OpenCV](https://github.com/opencv/opencv)：PC 端算法原型参考，不直接移植到 DSP 工程。
- [SORT](https://github.com/abewley/sort)：学习 Kalman + 数据关联流程；GPL-3.0 代码不要直接拷入固件。
- [Smorodov/Multitarget-tracker](https://github.com/Smorodov/Multitarget-tracker)：参考工程化多目标跟踪管线，Apache-2.0。
- [mcximing/hungarian-algorithm-cpp](https://github.com/mcximing/hungarian-algorithm-cpp)：BSD-2-Clause，可参考后重写为 C 静态数组版本。
- [TinyEKF](https://github.com/simondlevy/TinyEKF)：学习轻量滤波器实现方式。
- [Difference of Gaussian 教程](https://grp-bio-it.embl-community.io/image-analysis-training-resources/filter_difference_of_gaussian/index.html)：理解 DoG 原理和参数影响。

## 11. 落地原则

- DSP 算法不定义 SRIO 包格式，不修改硬件接口字段。
- SRIO、SelfID、ACKID、WDMA/RDMA 和 doorbell 由 Core0 或硬件接口层统一处理。
- 从核只处理算法任务，不直接访问 SRIO DMA。
- 所有跨核结构使用固定上限，禁止运行期动态分配。
- 所有算法结果必须带 `frame_id`，避免跨帧混读。
- 先在 `M6678_SRIO_DSP1` 验证，再同步 `M6678_SRIO_DSP2`，保留 DSP ID/name 差异。
- 新增代码建议拆分为算法模块、调度模块、跟踪模块和 cache 操作模块，不手工编辑 `Debug/` 生成文件。
