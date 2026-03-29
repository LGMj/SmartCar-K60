/**
 * @file    MK60_system.c
 * @brief   系统时钟初始化和时钟频率变量实现
 * @version 1.0
 *
 * 提供 mcgout_clk_mhz、core_clk_mhz、bus_clk_mhz 三个全局变量
 * 以及 get_clk() 函数，供其他驱动模块获取时钟频率
 */

#include "common.h"
#include "MK60D10.h"

/**
 * @brief 系统时钟频率变量 (MHz)
 *
 * 由 SystemInit() 根据 CLOCK_SETUP 配置自动设置
 * CLOCK_SETUP=1 (PEE 模式): 100MHz
 * CLOCK_SETUP=0 (FEI 模式): 约 42MHz
 * CLOCK_SETUP=2 (BLPE 模式): 8MHz
 */
uint32 mcgout_clk_mhz;
uint32 core_clk_mhz;
uint32 bus_clk_mhz;

/**
 * @brief 更新系统时钟频率变量
 *
 * 根据 MCG 模块和 SIM 分频器的当前配置，
 * 重新计算并更新 mcgout_clk_mhz、core_clk_mhz、bus_clk_mhz
 */
void get_clk(void) {
    uint32 mcgout_clk;
    uint8  outdiv1, outdiv2, outdiv3, outdiv4;

    mcgout_clk = SystemCoreClock;

    outdiv1 = (uint8)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT);
    outdiv2 = (uint8)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT);
    outdiv3 = (uint8)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV3_MASK) >> SIM_CLKDIV1_OUTDIV3_SHIFT);
    outdiv4 = (uint8)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT);

    mcgout_clk_mhz = mcgout_clk / 1000000UL;

    if (outdiv1 == 0) outdiv1 = 1;
    if (outdiv2 == 0) outdiv2 = 1;
    if (outdiv3 == 0) outdiv3 = 1;
    if (outdiv4 == 0) outdiv4 = 1;

    core_clk_mhz = mcgout_clk / (outdiv1 * 1000000UL);
    bus_clk_mhz  = mcgout_clk / (outdiv2 * 1000000UL);
}
