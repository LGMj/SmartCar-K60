/**
 * @file    MK60_gpio.h
 * @brief   GPIO 操作接口定义
 * @version 1.0
 */

#ifndef _MK60_gpio_h
#define _MK60_gpio_h

#include "common.h"

/**
 * @brief 位带操作宏定义已移至 common.h
 * PAout(n), PAin(n), PBout(n) 等宏已在 common.h 中定义
 * 使用方法: PAout(28) = 1;  // 设置 PTA28 为高电平
 */

/**
 * @brief GPIO 函数声明
 * @param ptx_n  引脚编号 (PTX_n 枚举类型, 如 A28)
 * @param ddr    方向 (GPI=输入, GPO=输出)
 * @param dat    初始输出值 (0=低, 1=高, 仅在输出模式时有效)
 */
void    gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat);  /**< 初始化 GPIO */
void    gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr);               /**< 设置 GPIO 方向 */
uint8   gpio_get(PTX_n ptx_n);                            /**< 读取 GPIO 电平 */
void    gpio_set(PTX_n ptx_n, uint8 dat);                 /**< 设置 GPIO 电平 */
void    gpio_turn(PTX_n ptx_n);                           /**< 翻转 GPIO 电平 */

#endif /* _MK60_gpio_h */
