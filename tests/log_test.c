#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

// unsigned int get_time_ms() { return 114514; }

// /* config */
// #define LOG_OUTPUT_MAX_LEN 128

// #define LOG_TIMESTAMP_EN 1
// #define LOG_FUNCTION_EN 0
// #define LOG_FILE_LINE_EN 0

// #define LOG_ASSERT_EN 1
// #define LOG_ERROR_EN 1
// #define LOG_WARINING_EN 1
// #define LOG_INFO_EN 1
// #define LOG_DEBUG_EN 1

// #define __log_output(log_str, len)                   \
//     do                                               \
//     {                                                \
//         printf("%.*s", (len), (uint8_t *)(log_str)); \
//     } while (0)

// #ifndef LOG_TAG
// #define LOG_TAG "log"
// #endif

// #ifndef LOG_OUTPUT_LEVEL
// #define LOG_OUTPUT_LEVEL 4
// #endif

// #ifndef LOG_TIMESTAMP_EN
// #define LOG_TIMESTAMP_EN 0
// #endif

// #ifndef LOG_FUNCTION_EN
// #define LOG_FUNCTION_EN 0
// #endif

// /**
//  * print level:
//  **/

// #define LOG_ASSERT 1
// #define LOG_ERROR 2
// #define LOG_WARINING 3
// #define LOG_INFO 4
// #define LOG_DEBUG 5

// extern char *LOG_LEVEL_TAGS[];

// #define __log_with_level(level, ...)                                                                                                         \
//     do                                                                                                                                       \
//     {                                                                                                                                        \
//         char log_str[LOG_OUTPUT_MAX_LEN];                                                                                                    \
//         int len = 0;                                                                                                                         \
//         if ((level <= LOG_OUTPUT_LEVEL) && (level <= get_global_log_level()))                                                                \
//         {                                                                                                                                    \
//             len += snprintf(&log_str[len], LOG_OUTPUT_MAX_LEN - len, "\r\n");                                                                \
//             if (LOG_TIMESTAMP_EN)                                                                                                            \
//             {                                                                                                                                \
//                 len += snprintf(&log_str[len], LOG_OUTPUT_MAX_LEN - len, "[%d.%03d]", (int)get_time_ms() / 1000, (int)get_time_ms() % 1000); \
//             }                                                                                                                                \
//             len += snprintf(&log_str[len], LOG_OUTPUT_MAX_LEN - len, "[%s][%s]", LOG_TAG, LOG_LEVEL_TAGS[level]);                            \
//             if (LOG_FUNCTION_EN)                                                                                                             \
//             {                                                                                                                                \
//                 len += snprintf(&log_str[len], LOG_OUTPUT_MAX_LEN - len, "[%s]", __FUNCTION__);                                              \
//             }                                                                                                                                \
//             if ((LOG_FILE_LINE_EN) && (level <= LOG_ERROR))                                                                                  \
//             {                                                                                                                                \
//                 len += snprintf(&log_str[len], LOG_OUTPUT_MAX_LEN - len, "(%s,%d)", __FILE__, __LINE__);                                     \
//             }                                                                                                                                \
//             len += log_printf_to_buffer(&log_str[len], LOG_OUTPUT_MAX_LEN - len, __VA_ARGS__);                                               \
//             __log_output((uint8_t *)log_str, len);                                                                                           \
//         }                                                                                                                                    \
//     } while (0)

// #if defined(LOG_ASSERT_EN) && (LOG_ASSERT_EN == 1)
// #define log_assert(...) __log_with_level(LOG_ASSERT, __VA_ARGS__)
// #else
// #define log_assert(...)
// #endif

// #if defined(LOG_ERROR_EN) && (LOG_ERROR_EN == 1)
// #define log_e(...) __log_with_level(LOG_ERROR, __VA_ARGS__)
// #else
// #define log_e(...)
// #endif

// #if defined(LOG_WARINING_EN) && (LOG_WARINING_EN == 1)
// #define log_w(...) __log_with_level(LOG_WARINING, __VA_ARGS__)
// #else
// #define log_w(...)
// #endif

// #if defined(LOG_INFO_EN) && (LOG_INFO_EN == 1)
// #define log_i(...) __log_with_level(LOG_INFO, __VA_ARGS__)
// #else
// #define log_i(...)
// #endif

// #if defined(LOG_DEBUG_EN) && (LOG_DEBUG_EN == 1)
// #define log_d(...) __log_with_level(LOG_DEBUG, __VA_ARGS__)
// #else
// #define log_d(...)
// #endif

// void set_global_log_level(uint8_t level);
// uint8_t get_global_log_level(void);

// void log_printf(char *fmt, ...);
// int log_printf_to_buffer(char *buff, int size, char *fmt, ...);

// char *LOG_LEVEL_TAGS[6] = {"NULL", "ASSERT", "ERROR", "WARNING", "INFO", "DEBUG"};

// uint8_t log_level = 5;

// uint8_t get_global_log_level(void)
// {
//     return log_level;
// }

// void set_global_log_level(uint8_t level)
// {
//     log_level = level;
// }

// void log_printf(char *fmt, ...)
// {
//     char log_str[LOG_OUTPUT_MAX_LEN];
//     int len = 0;
//     va_list arg;
//     va_start(arg, fmt);
//     len += vsnprintf(log_str, LOG_OUTPUT_MAX_LEN - len, fmt, arg);
//     va_end(arg);
//     __log_output((uint8_t *)log_str, len);
// }

// int log_printf_to_buffer(char *buff, int size, char *fmt, ...)
// {
//     int len = 0;
//     va_list arg;
//     va_start(arg, fmt);
//     len += vsnprintf(buff, size, fmt, arg);
//     va_end(arg);
//     return len;
// }

#include "log.h"

int main()
{
    log_i("ii %d\n", 1);
    log_w("ww %d\n", 2);
    log_e("ee %d\n", 3);
    return 0;
}