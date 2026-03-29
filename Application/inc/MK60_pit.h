/**
 * @file    MK60_pit.h
 * @brief   PIT 周期中断定时器驱动接口定义
 * @version 1.0
 */

#ifndef _MK60_pit_h
#define _MK60_pit_h

#include "common.h"

/**
 * @brief PIT 通道枚举
 */
typedef enum {
    PIT_CH0 = 0,  /**< PIT 通道 0 */
    PIT_CH1 = 1,  /**< PIT 通道 1 */
    PIT_CH2 = 2,  /**< PIT 通道 2 */
    PIT_CH3 = 3   /**< PIT 通道 3 */
} PITn_e;

/**
 * @brief PIT 模块初始化 (不带中断)
 * @param ch    PIT 通道 (PIT_CH0 ~ PIT_CH3)
 * @param us    定时时间，单位: 微秒 (us)
 *
 * @note  PIT 时钟源为 bus_clk，默认 100MHz
 *        定时时间范围: 1us ~ 42949672us (约 42.9 秒)
 */
void pit_init(PITn_e ch, uint32 us);

/**
 * @brief PIT 模块初始化 (带中断)
 * @param ch    PIT 通道 (PIT_CH0 ~ PIT_CH3)
 * @param us    定时时间，单位: 微秒 (us)
 * @param irq   中断使能标志 (0=禁用中断, 1=使能中断)
 *
 * @note  PIT 时钟源为 bus_clk，默认 100MHz
 *        如果使能中断，用户需要在中断服务程序中清除标志位
 */
void pit_init_interrupt(PITn_e ch, uint32 us, uint8 irq);

/**
 * @brief 开启 PIT 定时器
 * @param ch    PIT 通道
 */
void pit_start(PITn_e ch);

/**
 * @brief 停止 PIT 定时器
 * @param ch    PIT 通道
 */
void pit_stop(PITn_e ch);

/**
 * @brief 清除 PIT 通道中断标志
 * @param ch    PIT 通道
 */
void pit_clear_flag(PITn_e ch);

/**
 * @brief 获取 PIT 通道当前计数值的宏
 * @param ch    PIT 通道
 * @return      当前计数值 (CVAL 寄存器的值)
 *
 * @note  计数器向下递减，返回值越小说明剩余时间越短
 */
#define PIT_CVAL(ch)  (PIT->CHANNEL[ch].CVAL)

/**
 * @brief 获取 PIT 通道溢出标志的宏
 * @param ch    PIT 通道
 * @return      溢出标志 (0=无溢出, 1=已溢出)
 */
#define PIT_TFLG(ch)  (PIT->CHANNEL[ch].TFLG & PIT_TFLG_TIF_MASK)

/**
 * @brief PIT 中断服务函数声明 (在 main.c 或其他文件中实现)
 */
void PIT0_IRQHandler(void);
void PIT1_IRQHandler(void);
void PIT2_IRQHandler(void);
void PIT3_IRQHandler(void);

#endif /* _MK60_pit_h */
