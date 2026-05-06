# DSP_DUAL_PROJECT

双 DSP 的 TI Code Composer Studio 工程，包含两个基本一致的 TMS320C6678 工程：

- `M6678_SRIO_DSP1/`
- `M6678_SRIO_DSP2/`

两个工程的共享源码应保持同步。当前主要身份差异在 `main.c`：

- DSP1: `LOCAL_SELF_ID 0x61`，`DSP_NAME "DSP1"`
- DSP2: `LOCAL_SELF_ID 0x62`，`DSP_NAME "DSP2"`

## 目录结构

每个工程包含以下主要模块：

- `main.c`、`boot.h`、`M6678.cmd`、`vecs.asm`：应用入口、启动配置、链接脚本和中断向量。
- `srio/`：SRIO 初始化、寄存器定义、UART 辅助函数和 PSC 支持。
- `ddr/`：DDR 初始化、训练、BIST、裕量测试和内存测试。
- `GPIO/`：板级 GPIO 支持。
- `CPS1848/`：SRIO 交换芯片配置。
- `targetConfigs/`：CCS 目标配置文件。

`Debug/` 是 CCS/gmake 生成目录，不纳入版本控制。

## 构建方式

推荐使用 TI Code Composer Studio 导入并构建：

1. 打开 CCS。
2. 选择 `File > Import`。
3. 选择 `Code Composer Studio > CCS Projects` 或 `Existing CCS Eclipse Projects`。
4. 将搜索目录指向本仓库根目录。
5. 勾选并导入 `M6678_SRIO_DSP1` 和 `M6678_SRIO_DSP2`。
6. 分别构建两个工程，生成对应的 `.out` 文件。

## 工具链要求

工程元数据中使用：

- Device: `TMS320C66XX.TMS320C6678`
- Endianness: little
- Output format: ELF
- C6000 code generation tools: `7.4.4`
- Runtime support library: `libc.a`

如果本机 CCS 或 MCSDK 安装路径不同，请在 CCS 工程属性中检查 include path、library path 和 code generation tools 配置。

## 维护注意事项

- 不提交 `Debug/`、`.obj`、`.pp`、`.out`、`.map`、`*_linkInfo.xml` 等中间产物。
- 修改共享模块时，同步检查 DSP1 和 DSP2 两份工程。
- 不要误改 DSP 专属身份宏，除非硬件 ID 分配发生变化。
