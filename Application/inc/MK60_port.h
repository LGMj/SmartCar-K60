/**
 * @file    MK60_port.h
 * @brief   PORT 配置定义
 * @version 1.0
 */

#ifndef _MK60_port_h
#define _MK60_port_h

#include "common.h"

/**
 * @brief PORT 配置枚举
 */
typedef enum {
    // 中断方式和 DMA 请求方式，两者只能选其中一种（可以不选）
    // 中断方式选择
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   /**< 低电平触发 */
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   /**< 上升沿触发 */
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   /**< 下降沿触发 */
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   /**< 跳变沿触发 */
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   /**< 高电平触发 */

    // DMA 请求选择
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   /**< DMA 上升沿触发 */
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   /**< DMA 下降沿触发 */
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   /**< DMA 跳变沿触发 */

    // 驱动能力配置
    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    /**< 高驱动能力 */
    ODO          = 0x01 << PORT_PCR_ODE_SHIFT,    /**< 漏极输出 */
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    /**< 带无源滤波器 */
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    /**< 输出慢变化率 */

    // 上下拉配置
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     /**< 下拉电阻 */
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     /**< 上拉电阻 */

    // 功能复用选择
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,   /**< 引脚复用功能 0 */
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    /**< 引脚复用功能 1 (GPIO) */
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,    /**< 引脚复用功能 2 */
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,    /**< 引脚复用功能 3 */
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,    /**< 引脚复用功能 4 */
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,    /**< 引脚复用功能 5 */
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,    /**< 引脚复用功能 6 */
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,    /**< 引脚复用功能 7 */
} port_cfg;

/**
 * @brief PORT 清除中断标志宏
 */
#define PORTA_FLAG_CLR(ptx_n)   (PORTA->ISFR = (uint32)(1 << PTn(ptx_n)))
#define PORTB_FLAG_CLR(ptx_n)   (PORTB->ISFR = (uint32)(1 << PTn(ptx_n)))
#define PORTC_FLAG_CLR(ptx_n)   (PORTC->ISFR = (uint32)(1 << PTn(ptx_n)))
#define PORTD_FLAG_CLR(ptx_n)   (PORTD->ISFR = (uint32)(1 << PTn(ptx_n)))
#define PORTE_FLAG_CLR(ptx_n)   (PORTE->ISFR = (uint32)(1 << PTn(ptx_n)))

/**
 * @brief PORT 函数声明
 */
void port_init(PTX_n ptx_n, uint32 cfg);      /**< 初始化 PORT */
void port_init_NoAlt(PTX_n ptx_n, uint32 cfg); /**< 初始化 PORT (保留原 MUX) */

#endif /* _MK60_port_h */
