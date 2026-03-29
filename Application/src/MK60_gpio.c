/**
 * @file    MK60_gpio.c
 * @brief   GPIO 驱动实现
 * @version 1.0
 */

#include "MK60_gpio.h"
#include "MK60_port.h"

static PORT_Type *const PORTX[] = PORT_BASES;
static GPIO_Type *const GPIOX[] = GPIO_BASES;

/**
 * @brief 初始化 GPIO
 * @param ptx_n  引脚编号
 * @param ddr    方向 (GPI=输入, GPO=输出)
 * @param dat    初始输出值 (输出模式时有效)
 */
void gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat) {
    uint8 ptx = PTX(ptx_n);  // 获取端口号 (0-4)
    uint8 ptn = PTn(ptx_n);  // 获取引脚号 (0-31)

    // 使能端口时钟
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << ptx);

    // 清除之前的复用功能，设置复用功能为 GPIO (ALT1)
    PORTX[ptx]->PCR[ptn] &= ~(uint32)PORT_PCR_MUX_MASK;
    PORTX[ptx]->PCR[ptn] |= PORT_PCR_MUX(1);

    // 配置引脚方向，输出模式下同时设置初始电平
    if (ddr == GPO) {
        GPIOX[ptx]->PDDR |= (uint32)(1 << ptn);
        if (dat) {
            GPIOX[ptx]->PDOR |= (uint32)(1 << ptn);
        } else {
            GPIOX[ptx]->PDOR &= ~(uint32)(1 << ptn);
        }
    } else {
        GPIOX[ptx]->PDDR &= ~(uint32)(1 << ptn);
        PORTX[ptx]->PCR[ptn] |= PORT_PCR_PS_MASK;  // 选择上拉
        PORTX[ptx]->PCR[ptn] |= PORT_PCR_PE_MASK;  // 使能上下拉
    }
}

/**
 * @brief 设置 GPIO 方向
 * @param ptx_n  引脚编号
 * @param ddr    方向 (GPI=输入, GPO=输出)
 */
void gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr) {
    uint8 ptx = PTX(ptx_n);  // 获取端口号 (0-4)
    uint8 ptn = PTn(ptx_n);  // 获取引脚号 (0-31)

    if (ddr == GPO) {
        GPIOX[ptx]->PDDR |= (uint32)(1 << ptn);
    } else {
        GPIOX[ptx]->PDDR &= ~(uint32)(1 << ptn);
    }
}

/**
 * @brief 读取 GPIO 电平
 * @param ptx_n  引脚编号
 * @return       电平值 (0=低, 1=高)
 */
uint8 gpio_get(PTX_n ptx_n) {
    uint8 ptx = PTX(ptx_n);  // 获取端口号 (0-4)
    uint8 ptn = PTn(ptx_n);  // 获取引脚号 (0-31)

    return ((GPIOX[ptx]->PDIR >> ptn) & 0x1);
}

/**
 * @brief 设置 GPIO 电平
 * @param ptx_n  引脚编号
 * @param dat    电平值 (0=低, 1=高)
 */
void gpio_set(PTX_n ptx_n, uint8 dat) {
    uint8 ptx = PTX(ptx_n);  // 获取端口号 (0-4)
    uint8 ptn = PTn(ptx_n);  // 获取引脚号 (0-31)

    if (dat) {
        GPIOX[ptx]->PDOR |= (uint32)(1 << ptn);
    } else {
        GPIOX[ptx]->PDOR &= ~(uint32)(1 << ptn);
    }
}

/**
 * @brief 翻转 GPIO 电平
 * @param ptx_n  引脚编号
 */
void gpio_turn(PTX_n ptx_n) {
    uint8 ptx = PTX(ptx_n);  // 获取端口号 (0-4)
    uint8 ptn = PTn(ptx_n);  // 获取引脚号 (0-31)

    GPIOX[ptx]->PTOR = (uint32)(1 << ptn);
}
