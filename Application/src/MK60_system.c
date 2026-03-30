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
 * CLOCK_SETUP=0: MCGOUT=100MHz, Core=50MHz, Bus=25MHz
 * CLOCK_SETUP=1: MCGOUT=150MHz, Core=75MHz, Bus=37.5MHz
 * CLOCK_SETUP=2: MCGOUT=180MHz, Core=90MHz, Bus=60MHz (默认)
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

    /* 读取 SIM_CLKDIV1 中的 OUTDIV 分频值
     * 注意: OUTDIV 字段的值是除数减 1，例如 OUTDIV1=0 表示除以 1，OUTDIV1=1 表示除以 2
     */
    outdiv1 = (uint8)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT);
    outdiv2 = (uint8)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT);
    outdiv3 = (uint8)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV3_MASK) >> SIM_CLKDIV1_OUTDIV3_SHIFT);
    outdiv4 = (uint8)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT);

    mcgout_clk_mhz = mcgout_clk / 1000000UL;

    /* OUTDIV=0 表示 1 分频，所以用 (value + 1) 作为除数 */
    core_clk_mhz = mcgout_clk / ((outdiv1 + 1) * 1000000UL);
    bus_clk_mhz  = mcgout_clk / ((outdiv2 + 1) * 1000000UL);
}
