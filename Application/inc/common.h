/**
 * @file    common.h
 * @brief   基础类型定义和枚举声明
 * @version 1.0
 */

#ifndef _common_h
#define _common_h

#include <stdint.h>

#define NVIC_PriorityGroup_0         ((uint32_t)0x7)
#define NVIC_PriorityGroup_1         ((uint32_t)0x6)
#define NVIC_PriorityGroup_2         ((uint32_t)0x5)
#define NVIC_PriorityGroup_3         ((uint32_t)0x4)
#define NVIC_PriorityGroup_4         ((uint32_t)0x3)

/**
 * @brief 标准数据类型定义
 */
typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned long int   uint32;
typedef unsigned long long  uint64;

typedef char                int8;
typedef short int           int16;
typedef long  int           int32;
typedef long  long          int64;

typedef volatile int8       vint8;
typedef volatile int16      vint16;
typedef volatile int32      vint32;
typedef volatile int64      vint64;

typedef volatile uint8      vuuint8;
typedef volatile uint16     vuuint16;
typedef volatile uint32     vuuint32;
typedef volatile uint64     vuuint64;

/**
 * @brief GPIO 方向枚举
 */
typedef enum {
    GPI = 0,   /**< 输入模式 */
    GPO = 1    /**< 输出模式 */
} GPIO_MOD;

/**
 * @brief 端口引脚枚举
 */
typedef enum {
    A0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,  A8,  A9,  A10, A11, A12, A13, A14, A15,
    A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31,

    B0,  B1,  B2,  B3,  B4,  B5,  B6,  B7,  B8,  B9,  B10, B11, B12, B13, B14, B15,
    B16, B17, B18, B19, B20, B21, B22, B23, B24, B25, B26, B27, B28, B29, B30, B31,

    C0,  C1,  C2,  C3,  C4,  C5,  C6,  C7,  C8,  C9,  C10, C11, C12, C13, C14, C15,
    C16, C17, C18, C19, C20, C21, C22, C23, C24, C25, C26, C27, C28, C29, C30, C31,

    D0,  D1,  D2,  D3,  D4,  D5,  D6,  D7,  D8,  D9,  D10, D11, D12, D13, D14, D15,
    D16, D17, D18, D19, D20, D21, D22, D23, D24, D25, D26, D27, D28, D29, D30, D31,

    E0,  E1,  E2,  E3,  E4,  E5,  E6,  E7,  E8,  E9,  E10, E11, E12, E13, E14, E15,
    E16, E17, E18, E19, E20, E21, E22, E23, E24, E25, E26, E27, E28, E29, E30, E31,
} PTX_n;

#define PTX(ptx_n)  (ptx_n >> 5)   /**< 获取端口号 (0-4, 对应 A-E) */
#define PTn(ptx_n)  (ptx_n & 0x1f) /**< 获取引脚号 (0-31) */

/**
 * @brief 时钟频率变量声明
 */
extern uint32 mcgout_clk_mhz;
extern uint32 core_clk_mhz;
extern uint32 bus_clk_mhz;
void get_clk(void);

/**
 * @brief 包含设备头文件 (BITBAND_REG 等宏已在此文件中定义)
 */
#include "MK60D10.h"

/**
 * @brief 包含 SEGGER RTT 日志接口（见 SEGGER_RTT_Conf.h、ThirdParty/SEGGER_RTT）
 */
#include "rtt_log.h"

/**
 * @brief 位带操作便捷宏 - 基于 SDK 中的 BITBAND_REG
 * 使用方法: PAout(28) = 1;  // 设置 PTA28 为高电平
 */
#define PAout(n)   BITBAND_REG(PTA->PDOR, n)  /**< PTA 端口位输出 */
#define PAin(n)    BITBAND_REG(PTA->PDIR, n)  /**< PTA 端口位输入 */

#define PBout(n)   BITBAND_REG(PTB->PDOR, n)  /**< PTB 端口位输出 */
#define PBin(n)    BITBAND_REG(PTB->PDIR, n)  /**< PTB 端口位输入 */

#define PCout(n)   BITBAND_REG(PTC->PDOR, n)  /**< PTC 端口位输出 */
#define PCin(n)    BITBAND_REG(PTC->PDIR, n)  /**< PTC 端口位输入 */

#define PDout(n)   BITBAND_REG(PTD->PDOR, n)  /**< PTD 端口位输出 */
#define PDin(n)    BITBAND_REG(PTD->PDIR, n)  /**< PTD 端口位输入 */

#define PEout(n)   BITBAND_REG(PTE->PDOR, n)  /**< PTE 端口位输出 */
#define PEin(n)    BITBAND_REG(PTE->PDIR, n)  /**< PTE 端口位输入 */

#endif /* _common_h */
