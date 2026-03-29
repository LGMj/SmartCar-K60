/**
 * @file    MK60_pit.c
 * @brief   PIT 周期中断定时器驱动实现
 * @version 1.0
 *
 * PIT 模块时钟源为 bus_clk (默认 100MHz)
 * 定时时间计算公式: LDVAL = bus_clk_hz * us / 1000000 - 1
 * 计数器向下递减，递减到 0 时触发一次中断
 */

#include "MK60_pit.h"
#include "MK60D10.h"

#define PIT_CLK_HZ  (100000000UL)  /**< PIT 时钟频率: bus_clk = 100MHz */

/**
 * @brief 获取 bus_clk 时钟频率 (Hz)
 * @return 总线时钟频率
 */
static uint32 get_bus_clk_hz(void) {
    extern uint32 bus_clk_mhz;
    return bus_clk_mhz * 1000000UL;
}

/**
 * @brief PIT 模块使能
 *
 * PIT 模块默认处于禁用状态，调用任何 PIT 操作前需先调用此函数
 */
static void pit_enable(void) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
}

/**
 * @brief PIT 模块禁用
 */
static void pit_disable(void) {
    PIT->MCR |= PIT_MCR_MDIS_MASK;
}

/**
 * @brief PIT 初始化 (不带中断)
 * @param ch    PIT 通道 (PIT_CH0 ~ PIT_CH3)
 * @param us    定时时间，单位: 微秒 (us)
 */
void pit_init(PITn_e ch, uint32 us) {
    pit_enable();

    uint32 bus_hz = get_bus_clk_hz();
    uint32 ldval = bus_hz / 1000000UL * us - 1;

    PIT->CHANNEL[ch].LDVAL = ldval;
    PIT->CHANNEL[ch].TFLG  = PIT_TFLG_TIF_MASK;
    PIT->CHANNEL[ch].TCTRL = PIT_TCTRL_TEN_MASK;
}

/**
 * @brief PIT 初始化 (带中断)
 * @param ch    PIT 通道 (PIT_CH0 ~ PIT_CH3)
 * @param us    定时时间，单位: 微秒 (us)
 * @param irq   中断使能标志 (0=禁用中断, 1=使能中断)
 */
void pit_init_interrupt(PITn_e ch, uint32 us, uint8 irq) {
    pit_enable();

    uint32 bus_hz = get_bus_clk_hz();
    uint32 ldval = bus_hz / 1000000UL * us - 1;

    PIT->CHANNEL[ch].LDVAL = ldval;
    PIT->CHANNEL[ch].TFLG  = PIT_TFLG_TIF_MASK;

    if (irq) {
        // 使能 NVIC 中断 (PIT0_IRQn=68, PIT1=69, PIT2=70, PIT3=71)
        uint8_t irq_num = 68 + ch;
        NVIC->ICPR[irq_num >> 5] = (uint32_t)(1 << (irq_num & 0x1F));
        NVIC->ISER[irq_num >> 5] = (uint32_t)(1 << (irq_num & 0x1F));

        PIT->CHANNEL[ch].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    } else {
        PIT->CHANNEL[ch].TCTRL = PIT_TCTRL_TEN_MASK;
    }
}

/**
 * @brief 开启 PIT 定时器
 * @param ch    PIT 通道
 */
void pit_start(PITn_e ch) {
    PIT->CHANNEL[ch].TCTRL |= PIT_TCTRL_TEN_MASK;
}

/**
 * @brief 停止 PIT 定时器
 * @param ch    PIT 通道
 */
void pit_stop(PITn_e ch) {
    PIT->CHANNEL[ch].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

/**
 * @brief 清除 PIT 通道中断标志
 * @param ch    PIT 通道
 */
void pit_clear_flag(PITn_e ch) {
    PIT->CHANNEL[ch].TFLG = PIT_TFLG_TIF_MASK;
}
