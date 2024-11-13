#pragma once

#include <Arduino.h>

/**--------------------------------------------------------------------------------------
 * Settings
 *-------------------------------------------------------------------------------------*/

enum LogLevelEnum
{
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
};

// Log level variable
#define LOG_LEVEL LOG_LEVEL_INFO

// Time getter
#define LOG_TICK_MS_CALLBACK millis()

// Print callback
#define LOG_PRINTER Serial

// Enable logger
#define USE_LOG 1

// Print time in message
#define LOG_USE_TIMESTAMP 0

// Print file and line number
#define LOG_USE_FILE_LINE 1

// Print file name
#define LOG_USE_FILE_NAME 1

// Print function name
#define LOG_USE_FUNCTION_NAME 1

// Format time in message
#define LOG_FORMAT_TIMESTAMP 0

// Print log types full name or single letter
#define LOG_FORMAT_LONG_NAME 0

/**--------------------------------------------------------------------------------------
 * Logger Defines
 *-------------------------------------------------------------------------------------*/

// #define LOG_LEVEL_NONE 0
// #define LOG_LEVEL_DEBUG 1
// #define LOG_LEVEL_INFO 2
// #define LOG_LEVEL_WARN 3
// #define LOG_LEVEL_ERROR 4

#define LOG_LEVEL_LOCAL_CHECK(level, tag, format, ...)  \
    do                                                  \
    {                                                   \
        if (LOG_LOCAL_LEVEL <= level)                   \
            iformat(level, tag, format, ##__VA_ARGS__); \
    } while (0)

// Log format defines
#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(tag, format, ...) LOG_LEVEL_LOCAL_CHECK(LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(tag, format, ...) LOG_LEVEL_LOCAL_CHECK(LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)
#else
#define LOG_INFO(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN(tag, format, ...) LOG_LEVEL_LOCAL_CHECK(LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__)
#else
#define LOG_WARN(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(tag, format, ...) LOG_LEVEL_LOCAL_CHECK(LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)
#else
#define LOG_ERROR(tag, format, ...)
#endif

typedef int8_t LogLevel;
typedef int (*LogPrintCallback)(const char *, ...);

// public
void logLevelSet(const char *tag, LogLevel level);
void logRegisterPrintCb(LogPrintCallback);

// private helper
uint32_t logTimestamp();

// Prototypes functions
void iformat(uint8_t level, const char *tag, const char *format, ...);
void iformat(uint8_t level, const char *tag, String format, ...);

//

//

#define LOG_FILTER "vs,fgd,gds"

static bool should_log_line(const char *svc)
{
    bool svc_found_in_filter = (strstr(LOG_FILTER, svc) != NULL);
    return svc_found_in_filter;
}

// #if LOG_LEVEL >= LOG_LEVEL_DEBUG
// #ifdef LOG_FILTER
// #define LOG_DEBUG(svc, content) LOG_RAW_FILTER_LINE(LOG_LEVEL_DEBUG, svc, content)
// #else
// #define LOG_DEBUG(svc, content) LOG_RAW_LINE(LOG_LEVEL_DEBUG, svc, content)
// #endif
// #else
// #define LOG_DEBUG(svc, content) LOG_RAW_DO_NOTHING(LOG_LEVEL_DEBUG, svc, content)
// #endif

// #if CONFIG_LOG_COLORS
// #define LOG_COLOR_BLACK "30"
// #define LOG_COLOR_RED "31"
// #define LOG_COLOR_GREEN "32"
// #define LOG_COLOR_BROWN "33"
// #define LOG_COLOR_BLUE "34"
// #define LOG_COLOR_PURPLE "35"
// #define LOG_COLOR_CYAN "36"
// #define LOG_COLOR(COLOR) "\033[0;" COLOR "m"
// #define LOG_BOLD(COLOR) "\033[1;" COLOR "m"
// #define LOG_RESET_COLOR "\033[0m"
// #define LOG_COLOR_E LOG_COLOR(LOG_COLOR_RED)
// #define LOG_COLOR_W LOG_COLOR(LOG_COLOR_BROWN)
// #define LOG_COLOR_I LOG_COLOR(LOG_COLOR_GREEN)
// #define LOG_COLOR_D
// #define LOG_COLOR_V
// #else // CONFIG_LOG_COLORS
// #define LOG_COLOR_E
// #define LOG_COLOR_W
// #define LOG_COLOR_I
// #define LOG_COLOR_D
// #define LOG_COLOR_V
// #define LOG_RESET_COLOR
// #endif // CONFIG_LOG_COLORS

// #define LOG_FORMAT(letter, format) LOG_COLOR_##letter #letter " (%u) %s: " format LOG_RESET_COLOR "\n"
// #define LOG_SYSTEM_TIME_FORMAT(letter, format) LOG_COLOR_##letter #letter " (%s) %s: " format LOG_RESET_COLOR "\n"
// std::cout << _CLR_GREY << "[" << timestamp << "][" << _CLR_RST << level << _CLR_GREY << "] — [" << __FILENAME__ << ":" << __LINE__ << " " << __func__ << "()] — " << _CLR_RST << msg << std::endl;