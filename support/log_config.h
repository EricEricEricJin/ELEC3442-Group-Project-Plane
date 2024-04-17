/**
 * This program is obtained from DJI Robomaster RoboRTS-Firmware
 * URL: https://github.com/RoboMaster/RoboRTS-Firmware/tree/icra2021/components/log
 */

#ifndef __LOG_CONFIG_H__
#define __LOG_CONFIG_H__

#include "board.h"
/* config */
#define LOG_OUTPUT_MAX_LEN 128

#define LOG_TIMESTAMP_EN 1
#define LOG_FUNCTION_EN 1
#define LOG_FILE_LINE_EN 1

#define LOG_ASSERT_EN 1
#define LOG_ERROR_EN 1
#define LOG_WARINING_EN 1
#define LOG_INFO_EN 1
#define LOG_DEBUG_EN 1

#define __log_output(log_str, len)                   \
    do                                               \
    {                                                \
        printf("%.*s", (len), (uint8_t *)(log_str)); \
    } while (0)

#endif // __LOG_CONFIG_H__