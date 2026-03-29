/**
 * @file    main.c
 * @brief   LED 闪烁例程 - 使用 PTA28 引脚控制 LED
 * @version 1.0
 *
 * 硬件连接:
 *   LED 连接到 PTA28，低电平点亮
 *
 * 功能说明:
 *   初始化系统时钟和 GPIO 后，LED 以 1Hz 频率闪烁
 */

#include "common.h"
#include "MK60_gpio.h"

/**
 * @brief 简单延时函数
 * @param count 延时循环次数
 */
static void delay(volatile uint32 count) {
    while (count--) {
        __asm__("nop");  // 空操作，执行一条汇编指令消耗约 12.5ns @ 80MHz
    }
}

/**
 * @brief 主函数
 */
int main(void) {
    // 初始化系统时钟 (由 system_MK60D10.c 中的 SystemInit() 自动调用)
    // 默认配置: 外部 8MHz 晶振 -> PLL -> 100MHz 内核时钟

    // 初始化 PTA28 为 GPIO 输出，初始值为高 (LED 灭)
    gpio_init(A28, GPO, 1);

    // 主循环: LED 闪烁
    while (1) {
        // LED 点亮 (A28 输出低电平)
        gpio_set(A28, 0);

        // 延时约 500ms
        delay(5000000);

        // LED 熄灭 (A28 输出高电平)
        gpio_set(A28, 1);

        // 延时约 500ms
        delay(5000000);
    }
}
