# MK60DN512xxx10 LED Blinky Project

基于 Segger Embedded Studio 的 MK60DN512xxx10 LED 闪烁例程。

## 硬件信息

- **芯片**: NXP MK60DN512VLL10 (Cortex-M4, 100MHz)
- **LED 连接**: PTA28 (低电平点亮)
- **调试器**: J-Link (SWD 接口)

## 工程结构

```
SES_Project/
├── MK60_Blinky.emProject          # SES 项目文件
├── Settings/                       # 配置文件
│   ├── K60_Flash.icf              # 链接脚本
│   ├── MK60DN512xxx10_MemoryMap.xml
│   ├── MK60D10_Registers.xml
│   └── K60_Debug.jlink            # J-Link 调试配置
├── CMSIS_5/                       # CMSIS 内核头文件
│   └── CMSIS/Core/Include/
├── Kinetis_K60/                   # Kinetis SDK 驱动
│   ├── Device/
│   │   ├── Include/               # 设备头文件
│   │   └── Source/                # system_MK60D10.c
│   └── Source/                     # 启动文件和中断向量表
├── Application/                    # 用户应用代码
│   ├── inc/
│   │   ├── common.h              # 基础类型定义
│   │   ├── MK60_gpio.h           # GPIO 操作接口
│   │   └── MK60_port.h           # PORT 配置
│   └── src/
│       ├── main.c                # 主函数 (LED 闪烁)
│       └── MK60_gpio.c           # GPIO 驱动实现
└── README.md
```

## SDK 来源

本工程使用的 CMSIS 和 Kinetis SDK 文件复制自:
`/Users/ligangmin/Documents/SEGGER Embedded Studio for ARM Projects/K60/`

## 编译

1. 使用 Segger Embedded Studio 打开 `MK60_Blinky.emProject`
2. 选择 **Build** -> **Build Solution** 或按 `F7`
3. 编译产物位于 `Output/Debug/Exe/` 目录

## 调试

1. 连接 J-Link 调试器
2. 在 SES 中选择 **Target** -> **Download and Debug** 或按 `F5`
3. 程序将下载到芯片并进入调试模式

## GPIO 使用说明

### 位带操作 (快速)

```c
PAout(28) = 1;   // PTA28 输出高电平
PAout(28) = 0;   // PTA28 输出低电平
uint8 val = PAin(28);  // 读取 PTA28 电平
```

### API 函数

```c
gpio_init(A28, GPO, 1);  // 初始化 PTA28 为输出，初始值高
gpio_set(A28, 0);        // PTA28 输出低 (LED 亮)
gpio_set(A28, 1);        // PTA28 输出高 (LED 灭)
gpio_turn(A28);          // PTA28 电平翻转
uint8 val = gpio_get(A28);  // 读取 PTA28 电平
```

## 系统时钟

默认配置为 PEE 模式:
- 外部晶振: 8MHz
- PLL 倍频: x25
- 内核时钟: 100MHz
- 总线时钟: 100MHz

## 版本历史

- v1.0: 初始版本，实现 LED 闪烁功能
