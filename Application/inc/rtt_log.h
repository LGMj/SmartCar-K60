/**
 * @file    rtt_log.h
 * @brief   基于 SEGGER RTT 的日志宏（需链接 SEGGER_RTT.c / SEGGER_RTT_printf.c）
 */

#ifndef RTT_LOG_H
#define RTT_LOG_H

#include "SEGGER_RTT.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void RTT_LogInit(void) {
    (void)SEGGER_RTT_Init();
}

/** 在 main 最早处调用一次，建立 RTT 控制块 */
#define RTT_Init()              RTT_LogInit()

#define LOG(fmt, ...)           SEGGER_RTT_printf(0, "[LOG]  " fmt "\r\n", ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)      SEGGER_RTT_printf(0, "[INFO] " fmt "\r\n", ##__VA_ARGS__)
#define LOG_ERR(fmt, ...)       SEGGER_RTT_printf(0, "[ERR]  " fmt "\r\n", ##__VA_ARGS__)
#define LOG_HEX(label, data, len) do { \
    SEGGER_RTT_printf(0, "[HEX]  %s: ", label); \
    for (int _i = 0; _i < (len); _i++) { \
        SEGGER_RTT_printf(0, "%02X ", ((const unsigned char*)(data))[_i]); \
    } \
    SEGGER_RTT_printf(0, "\r\n"); \
} while (0)

#ifdef __cplusplus
}
#endif

#endif /* RTT_LOG_H */
