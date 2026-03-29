/**
 * @file    main.c
 * @brief   LED 闪烁例程 - 使用 PIT 定时器控制 LED
 * @version 1.0
 *
 * 硬件连接:
 *   LED 连接到 PTA28，低电平点亮
 *
 * 功能说明:
 *   PIT_CH0 定时器每 500ms 触发一次中断
 *   中断服务程序中翻转 PTA28 电平，实现 LED 以 1Hz 频率闪烁
 */

#include "common.h"
#include "MK60_gpio.h"
#include "MK60_pit.h"

/**
 * @brief PIT0 中断服务程序
 *
 * 每 500ms 执行一次，翻转 LED 状态
 */
void PIT0_IRQHandler(void) {
    pit_clear_flag(PIT_CH0);   // 清除中断标志
    gpio_turn(A28);            // 翻转 LED 状态
}

/**
 * @brief 主函数
 */
int main(void) {
    // 更新系统时钟频率变量 (bus_clk_mhz 等)
    get_clk();

    // 初始化 PTA28 为 GPIO 输出，初始值为高 (LED 灭)
    gpio_init(A28, GPO, 1);

    // 初始化 PIT_CH0，定时 500ms (500000us)，使能中断
    pit_init_interrupt(PIT_CH0, 500000, 1);

    // 开启 PIT 定时器
    pit_start(PIT_CH0);

    // 主循环 - LED 翻转在中断服务程序中完成，主循环空闲
    while (1) {
        // PIT 中断自动处理 LED 闪烁，无需在主循环中做延时操作
    }
}
