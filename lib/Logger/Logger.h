#pragma once

#include <Arduino.h>

/**--------------------------------------------------------------------------------------
 * Logger Options
 *-------------------------------------------------------------------------------------*/

#define LOG_LEVEL_DISABLE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_VERBOSE 5

#define LOG_FILTER_DISABLE 0
#define LOG_FILTER_EXCLUDE 1
#define LOG_FILTER_INCLUDE 2

#define LOG_TIME_DISABLE 0
#define LOG_TIME_MICROS 1
#define LOG_TIME_MILLIS 2
#define LOG_TIME_HHMMSSMS 3
#define LOG_TIME_HHHHMMSSMS 4

#define LOG_COLORS_DISABLE 0
#define LOG_COLORS_ENABLE 1

#define LOG_FILENAME_DISABLE 0
#define LOG_FILENAME_ENABLE 1

#define LOG_TAG_SHORT 0
#define LOG_TAG_LONG 1

#define LOG_TYPE_PRINTF 0
#define LOG_TYPE_STD_FORMAT 1

/**--------------------------------------------------------------------------------------
 * Logger Default Settings
 *-------------------------------------------------------------------------------------*/

// If LOG_LEVEL is not defined by the user, it defaults to LOG_LEVEL_DEBUG
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#ifndef LOG_OUTPUT
#define LOG_OUTPUT Serial
#endif

#ifndef LOG_FILTER
#define LOG_FILTER LOG_FILTER_DISABLE
#endif

#ifndef LOG_FILTER_LIST
#define LOG_FILTER_LIST {""}
#endif

#ifndef LOG_TIME
#define LOG_TIME LOG_TIME_DISABLE
#endif

#ifndef LOG_TAG_TYPE
#define LOG_TAG_TYPE LOG_TAG_SHORT
#endif

#ifndef LOG_COLORS
#define LOG_COLORS LOG_COLORS_DISABLE
#endif

#ifndef LOG_FILENAME
#define LOG_FILENAME LOG_FILENAME_DISABLE
#endif

#ifndef LOG_PRINT_TYPE
#define LOG_PRINT_TYPE LOG_TYPE_PRINTF
#endif

/**--------------------------------------------------------------------------------------
 * Logger Assertions
 *-------------------------------------------------------------------------------------*/

#if __cplusplus >= 201103L
static_assert(LOG_LEVEL >= LOG_LEVEL_DISABLE && LOG_LEVEL <= LOG_LEVEL_VERBOSE, "LOG_LEVEL must be between LOG_LEVEL_DISABLE and LOG_LEVEL_VERBOSE");
static_assert(LOG_FILTER >= LOG_FILTER_DISABLE && LOG_FILTER <= LOG_FILTER_INCLUDE, "LOG_FILTER must be between LOG_FILTER_DISABLE and LOG_FILTER_INCLUDE");
static_assert(LOG_TIME >= LOG_TIME_DISABLE && LOG_TIME <= LOG_TIME_HHHHMMSSMS, "LOG_TIME must be between LOG_TIME_DISABLE and LOG_TIME_HHHHMMSSMS");
static_assert(LOG_TAG_TYPE == LOG_TAG_TYPE || LOG_TAG_TYPE == LOG_TAG_LONG, "LOG_TAG_SHORT must be either LOG_TAG_SHORT or LOG_TAG_LONG");
static_assert(LOG_COLORS == LOG_COLORS_DISABLE || LOG_COLORS == LOG_COLORS_ENABLE, "LOG_COLORS must be either LOG_COLORS_DISABLE or LOG_COLORS_ENABLE");
static_assert(LOG_PRINT_TYPE == LOG_TYPE_PRINTF || LOG_PRINT_TYPE == LOG_TYPE_STD_FORMAT, "LOG_PRINT_TYPE must be either LOG_TYPE_PRINTF or LOG_TYPE_STD_FORMAT");
#endif

/**--------------------------------------------------------------------------------------
 * Logger Private Functions and Macros
 *-------------------------------------------------------------------------------------*/

#define LOG_EOL "\r\n"

#if LOG_COLORS == LOG_COLORS_ENABLE
#define _LOG_COLOR_BLACK "30"
#define _LOG_COLOR_RED "31"    // ERROR
#define _LOG_COLOR_GREEN "32"  // INFO
#define _LOG_COLOR_YELLOW "33" // WARNING
#define _LOG_COLOR_BLUE "34"
#define _LOG_COLOR_MAGENTA "35"
#define _LOG_COLOR_CYAN "36" // DEBUG
#define _LOG_COLOR_GRAY "37" // VERBOSE
#define _LOG_COLOR_WHITE "38"
#define _LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define _LOG_BOLD(COLOR) "\033[1;" COLOR "m"
#define _LOG_RESET_COLOR "\033[0m"
#define _LOG_COLOR_E _LOG_COLOR(_LOG_COLOR_RED)
#define _LOG_COLOR_W _LOG_COLOR(_LOG_COLOR_YELLOW)
#define _LOG_COLOR_I _LOG_COLOR(_LOG_COLOR_GREEN)
#define _LOG_COLOR_D _LOG_COLOR(_LOG_COLOR_CYAN)
#define _LOG_COLOR_V _LOG_COLOR(_LOG_COLOR_GRAY)
#else
#define _LOG_COLOR_E ""
#define _LOG_COLOR_W ""
#define _LOG_COLOR_I ""
#define _LOG_COLOR_D ""
#define _LOG_COLOR_V ""
#define _LOG_RESET_COLOR ""
#endif // LOG_COLORS == LOG_COLORS_ENABLE

#if LOG_TAG == LOG_TAG_SHORT
static const char *_logLevelText[] = {"E", "W", "I", "D", "V"};
#else
static const char *_logLevelText[] = {"EROR", "WARN", "INFO", "DBUG", "VERB"};
#endif // LOG_TAG == LOG_TAG_SHORT

#if LOG_LEVEL > LOG_LEVEL_DISABLE
static void _vprintf(const char *format, va_list arg)
{
    char buf[64];
    char *temp = buf;
    va_list copy;
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(buf), format, copy);
    va_end(copy);
    if (len < 0)
    {
        va_end(arg);
        return;
    }
    if (len >= (int)sizeof(buf))
    {
        temp = (char *)malloc(len + 1);
        if (temp == NULL)
        {
            va_end(arg);
            return;
        }
        len = vsnprintf(temp, len + 1, format, arg);
    }
    va_end(arg);
    LOG_OUTPUT.print(temp);
    if (temp != buf)
    {
        free(temp);
    }
}

static void _printf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    _vprintf(format, arg);
    va_end(arg);
}
#endif // LOG_LEVEL > LOG_LEVEL_DISABLE

#if LOG_TIME != LOG_TIME_DISABLE
static const char *_formatTime()
{
#if LOG_TIME == LOG_TIME_MICROS
    static char timeFormat[12];
    sprintf(timeFormat, "%11lu", micros());
#elif LOG_TIME == LOG_TIME_MILLIS
    static char timeFormat[9];
    sprintf(timeFormat, "%8lu", millis());
#elif (LOG_TIME == LOG_TIME_HHMMSSMS) || (LOG_TIME == LOG_TIME_HHHHMMSSMS)
    unsigned long ms = millis();
    unsigned long seconds = ms / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
#if LOG_TIME == LOG_TIME_HHMMSSMS
    static char timeFormat[13];
    sprintf(timeFormat, "%02lu:%02lu:%02lu:%03lu", hours % 24, minutes % 60, seconds % 60, ms % 1000);
#elif LOG_TIME == LOG_TIME_HHHHMMSSMS
    static char timeFormat[15];
    sprintf(timeFormat, "%04lu:%02lu:%02lu:%03lu", hours, minutes % 60, seconds % 60, ms % 1000);
#endif // LOG_TIME == LOG_TIME_HHMMSSMS

#else
    static char timeFormat[1] = {'\0'}; // Default to an empty string as a safeguard.
#endif // LOG_TIME == LOG_TIME_MICROS

    return timeFormat;
}
#endif // LOG_TIME != LOG_TIME_DISABLE

static bool _logFilter(const char *tag)
{
    static const char *filterList[] = LOG_FILTER_LIST;
    static const int length = sizeof(filterList) / sizeof(filterList[0]);

    bool inFilter = false;
    for (int i = 0; i < length; i++)
    {
        if (strcmp(filterList[i], tag) == 0)
        {
            inFilter = true;
            break;
        }
    }
    return LOG_FILTER ? inFilter : !inFilter; // INCLUDE or EXCLUDE
}

#if LOG_USE_LOG_FILTER != LOG_FILTER_DISABLE
#define _IF_LOG_FILTER_BEGIN \
    if (_logFilter(tag))     \
    {
#define _IF_LOG_FILTER_END }
#else
#define _IF_LOG_FILTER_BEGIN
#define _IF_LOG_FILTER_END
#endif // LOG_USE_LOG_FILTER

#if LOG_FILENAME == LOG_FILENAME_ENABLE
const char *pathToName(const char *path)
{
    size_t i = 0;
    size_t pos = 0;
    char *p = (char *)path;
    while (*p)
    {
        i++;
        if (*p == '/' || *p == '\\')
        {
            pos = i;
        }
        if (*p == '.')
        {
            *p = '\0';
            break;
        }
        p++;
    }
    return path + pos;
}
#endif // LOG_FILENAME == LOG_FILENAME_ENABLE

#if LOG_PRINT_TYPE == LOG_TYPE_PRINTF
#define LOG_PRINT(format, ...) _printf(format, ##__VA_ARGS__)
#define LOG_PRINTLN(format, ...) _printf(format LOG_EOL, ##__VA_ARGS__)

#define __LOG_TAG_FORMAT(loglevel, color, tag, format) color "[%s][%s] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel - 1], tag
#define __LOG_TAG_TIME_FORMAT(loglevel, color, tag, format) color "[%s][%s][%s] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel - 1], tag
#define __LOG_TAG_FILE_FORMAT(loglevel, color, tag, format) color "[%s][%s][%s:%s] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel - 1], tag, pathToName(__FILE__), __LINE__
#define __LOG_TAG_TIME_FILE_FORMAT(loglevel, color, tag, format) color "[%s][%s][%s][%s:%s] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel - 1], tag, pathToName(__FILE__), __LINE__

#define __LOG_FORMAT(loglevel, color, format) color "[%s] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel - 1]
#define __LOG_TIME_FORMAT(loglevel, color, format) color "[%s][%s] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel - 1]
#define __LOG_FILE_FORMAT(loglevel, color, format) color "[%s][%s][%s:%s] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel - 1], pathToName(__FILE__), __LINE__
#define __LOG_TIME_FILE_FORMAT(loglevel, color, format) color "[%s][%s][%s:%s] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel - 1], pathToName(__FILE__), __LINE__

#elif (LOG_PRINT_TYPE == LOG_TYPE_STD_FORMAT)
static_assert(__cplusplus >= 202002L, "LOG_TYPE_STD_FORMAT requires C++20 or later");

#include <format>
#define LOG_PRINT(msg, ...) LOG_OUTPUT.print(std::format(msg, ##__VA_ARGS__).c_str());
#define LOG_PRINTLN(msg, ...) LOG_OUTPUT.print(std::format(msg LOG_EOL, ##__VA_ARGS__).c_str());

#define __LOG_TAG_FORMAT(loglevel, color, tag, format) color "[{}][{}] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel - 1], tag
#define __LOG_TAG_TIME_FORMAT(loglevel, color, tag, format) color "[{}][{}][{}] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel - 1], tag
#define __LOG_TAG_FILE_FORMAT(loglevel, color, tag, format) color "[{}][{}][{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel - 1], tag, pathToName(__FILE__), __LINE__
#define __LOG_TAG_TIME_FILE_FORMAT(loglevel, color, tag, format) color "[{}][{}][{}][{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel - 1], tag, pathToName(__FILE__), __LINE__

#define __LOG_FORMAT(loglevel, color, format) color "[{}] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel - 1]
#define __LOG_TIME_FORMAT(loglevel, color, format) color "[{}][{}] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel - 1]
#define __LOG_FILE_FORMAT(loglevel, color, format) color "[{}][{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel - 1], pathToName(__FILE__), __LINE__
#define __LOG_TIME_FILE_FORMAT(loglevel, color, format) color "[{}][{}][{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel - 1], pathToName(__FILE__), __LINE__

#endif // LOG_PRINT_TYPE == LOG_TYPE_PRINTF

#if LOG_TIME != LOG_TIME_DISABLE && LOG_FILENAME != LOG_FILENAME_DISABLE
#define _LOG_FORMAT(level, color, msg) __LOG_TIME_FILE_FORMAT(level, color, msg)
#elif LOG_TIME != LOG_TIME_DISABLE && LOG_FILENAME == LOG_FILENAME_DISABLE
#define _LOG_FORMAT(level, color, msg) __LOG_TIME_FORMAT(level, color, msg)
#elif LOG_TIME == LOG_TIME_DISABLE && LOG_FILENAME != LOG_FILENAME_DISABLE
#define _LOG_FORMAT(level, color, msg) __LOG_FILE_FORMAT(level, color, msg)
#else
#define _LOG_FORMAT(level, color, msg) __LOG_FORMAT(level, color, msg)
#endif

#if LOG_TIME != LOG_TIME_DISABLE && LOG_FILENAME != LOG_FILENAME_DISABLE
#define _LOG_TAG_FORMAT(level, color, tag, msg) __LOG_TAG_TIME_FILE_FORMAT(level, color, tag, msg)
#elif LOG_TIME != LOG_TIME_DISABLE && LOG_FILENAME == LOG_FILENAME_DISABLE
#define _LOG_TAG_FORMAT(level, color, tag, msg) __LOG_TAG_TIME_FORMAT(level, color, tag, msg)
#elif LOG_TIME == LOG_TIME_DISABLE && LOG_FILENAME != LOG_FILENAME_DISABLE
#define _LOG_TAG_FORMAT(level, color, tag, msg) __LOG_TAG_FILE_FORMAT(level, color, tag, msg)
#else
#define _LOG_TAG_FORMAT(level, color, tag, msg) __LOG_TAG_FORMAT(level, color, tag, msg)
#endif

/**--------------------------------------------------------------------------------------
 * Logger Public Macros
 *-------------------------------------------------------------------------------------*/

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_VERBOSE(tag, message, ...)                                                        \
    _IF_LOG_FILTER_BEGIN                                                                      \
    LOG_PRINT(_LOG_TAG_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_VERBOSE(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(tag, message, ...)                                                        \
    _IF_LOG_FILTER_BEGIN                                                                    \
    LOG_PRINT(_LOG_TAG_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_DEBUG(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(tag, message, ...)                                                        \
    _IF_LOG_FILTER_BEGIN                                                                   \
    LOG_PRINT(_LOG_TAG_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_INFO(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_WARNING(tag, message, ...)                                                        \
    _IF_LOG_FILTER_BEGIN                                                                      \
    LOG_PRINT(_LOG_TAG_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_WARNING(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(tag, message, ...)                                                        \
    _IF_LOG_FILTER_BEGIN                                                                    \
    LOG_PRINT(_LOG_TAG_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_ERROR(tag, message, ...)
#endif

// Without tag

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_V(message, ...) LOG_PRINT(_LOG_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, message), ##__VA_ARGS__);
#else
#define LOG_V(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_D(message, ...) LOG_PRINT(_LOG_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, message), ##__VA_ARGS__);
#else
#define LOG_D(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_I(message, ...) LOG_PRINT(_LOG_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, message), ##__VA_ARGS__);
#else
#define LOG_I(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_W(message, ...) LOG_PRINT(_LOG_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, message), ##__VA_ARGS__);
#else
#define LOG_W(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_E(message, ...) LOG_PRINT(_LOG_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, message), ##__VA_ARGS__);
#else
#define LOG_E(message, ...)
#endif