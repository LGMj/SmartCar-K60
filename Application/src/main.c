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
    static uint32_t count = 0;
    static uint32_t tick = 0;
    count++;
    pit_clear_flag(PIT_CH0);   // 清除中断标志
    gpio_turn(A28);            // 翻转 LED 状态
    _pit_inc_tick(PIT_CH0);    // 更新中断计数器
    // LOG("[%ums] LED toggled", pit_get_tick(PIT_CH0) * 500);
    if (count % 2 == 0) {
        tick++;
        LOG("tick: %d", tick);
    }
}

/**
 * @brief 主函数
 */
int main(void) {
    // 更新系统时钟频率变量 (bus_clk_mhz 等)
    get_clk();

    // 初始化 SEGGER RTT（在 RAM 中建立控制块，供 J-Link RTT Viewer 读取）
    RTT_Init();

    LOG_INFO("========================================");
    LOG_INFO("  MK60 PIT LED Project - RTT Log");
    LOG_INFO("========================================");
    LOG_INFO("Core Clock: %d MHz", core_clk_mhz);
    LOG_INFO("Bus Clock:  %d MHz", bus_clk_mhz);
    LOG_INFO("MCGOUT:    %d MHz", mcgout_clk_mhz);
    LOG_INFO("PIT Timer: 500ms period, LED on PTA28");
    LOG_INFO("========================================");

    // 初始化 PTA28 为 GPIO 输出，初始值为高 (LED 灭)
    gpio_init(A28, GPO, 1);

    // 初始化 PIT_CH0，定时 500ms (500000us)，使能中断
    pit_init_interrupt(PIT_CH0, 500000, 1);

    // 开启 PIT 定时器
    pit_start(PIT_CH0);

    LOG_INFO("System initialized, starting main loop...");

    // 主循环 - LED 翻转在中断服务程序中完成，主循环空闲
    while (1) {
        // PIT 中断自动处理 LED 闪烁，无需在主循环中做延时操作
        // 可以在此处添加其他应用逻辑
    }
}
