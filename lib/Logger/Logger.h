#pragma once

#include <Arduino.h>

/**--------------------------------------------------------------------------------------
 * Logger Level Options
 *-------------------------------------------------------------------------------------*/

#define LOG_LEVEL_NONE -1
#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_VERBOSE 4

#define LOG_FILTER_INCLUDE 2
#define LOG_FILTER_EXCLUDE 1
#define LOG_FILTER_DISABLE 0

#define LOG_TIME_HHHHMMSSMS 4
#define LOG_TIME_HHMMSSMS 3
#define LOG_TIME_MILLIS 2
#define LOG_TIME_MICROS 1
#define LOG_TIME_DISABLE 0

/**--------------------------------------------------------------------------------------
 * Logger Settings
 *-------------------------------------------------------------------------------------*/

#if LOG_LEVEL != LOG_LEVEL_NONE

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

// #ifndef LOG_TIME != LOG_TIME_DISABLE
// #define LOG_TIME != LOG_TIME_DISABLE 0
// #endif

#ifndef LOG_USE_TAG_SHORT
#define LOG_USE_TAG_SHORT 0
#endif

#ifndef LOG_USE_COLORS
#define LOG_USE_COLORS 0
#endif

#ifndef LOG_EOL
#define LOG_EOL "\r\n"
#endif

/**--------------------------------------------------------------------------------------
 * Logger Private Functions and Macros
 *-------------------------------------------------------------------------------------*/

#if LOG_USE_COLORS
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
#endif // LOG_USE_COLORS

#if LOG_USE_TAG_SHORT
static const char *_logLevelText[] = {"E", "W", "I", "D", "V"};
#else
static const char *_logLevelText[] = {"EROR", "WARN", "INFO", "DBUG", "VERB"};
#endif // LOG_USE_TAG_SHORT

#if LOG_LEVEL != LOG_LEVEL_NONE
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
#endif // LOG_LEVEL != LOG_LEVEL_NONE

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

#if LOG_USE_LOG_FILTER != LOG_FILTER_DISABLE
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

#define _IF_LOG_FILTER if (_logFilter(tag))
#else
#define _IF_LOG_FILTER
#endif // LOG_USE_LOG_FILTER

#define _LOG_TAG_FORMAT(loglevel, color, tag, format) color "[%s][%s] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel], tag
#define _LOG_FORMAT(loglevel, color, format) color "[%s] " format _LOG_RESET_COLOR LOG_EOL, _logLevelText[loglevel]

#define _LOG_TAG_TIME_FORMAT(loglevel, color, tag, format) color "[%s][%s][%s] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel], tag
#define _LOG_TIME_FORMAT(loglevel, color, format) color "[%s][%s] " format _LOG_RESET_COLOR LOG_EOL, _formatTime(), _logLevelText[loglevel]

#endif // LOG_LEVEL != LOG_LEVEL_NONE

/**--------------------------------------------------------------------------------------
 * Logger Public Macros
 *-------------------------------------------------------------------------------------*/

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_VERBOSE(tag, message, ...) \
    _IF_LOG_FILTER                     \
    _printf(_LOG_TAG_TIME_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, tag, message), ##__VA_ARGS__);
#else
#define LOG_VERBOSE(tag, message, ...) \
    _IF_LOG_FILTER                     \
    _printf(_LOG_TAG_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, tag, message), ##__VA_ARGS__);
#endif // LOG_TIME != LOG_TIME_DISABLE
#else
#define LOG_VERBOSE(tag, message, ...)
#endif // LOG_LEVEL >= LOG_LEVEL_VERBOSE

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_DEBUG(tag, message, ...) \
    _IF_LOG_FILTER                   \
    _printf(_LOG_TAG_TIME_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, tag, message), ##__VA_ARGS__);
#else
#define LOG_DEBUG(tag, message, ...) \
    _IF_LOG_FILTER                   \
    _printf(_LOG_TAG_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, tag, message), ##__VA_ARGS__);
#endif
#else
#define LOG_DEBUG(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_INFO(tag, message, ...) \
    _IF_LOG_FILTER                  \
    _printf(_LOG_TAG_TIME_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, tag, message), ##__VA_ARGS__);
#else
#define LOG_INFO(tag, message, ...) \
    _IF_LOG_FILTER                  \
    _printf(_LOG_TAG_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, tag, message), ##__VA_ARGS__);
#endif
#else
#define LOG_INFO(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_WARNING(tag, message, ...) \
    _IF_LOG_FILTER                     \
    _printf(_LOG_TAG_TIME_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, tag, message), ##__VA_ARGS__);
#else
#define LOG_WARNING(tag, message, ...) \
    _IF_LOG_FILTER                     \
    _printf(_LOG_TAG_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, tag, message), ##__VA_ARGS__);
#endif
#else
#define LOG_WARNING(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_ERROR(tag, message, ...) \
    _IF_LOG_FILTER                   \
    _printf(_LOG_TAG_TIME_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, tag, message), ##__VA_ARGS__);
#else
#define LOG_ERROR(tag, message, ...) \
    _IF_LOG_FILTER                   \
    _printf(_LOG_TAG_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, tag, message), ##__VA_ARGS__);
#endif
#else
#define LOG_ERROR(tag, message, ...)
#endif

// Without tag

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_V(message, ...) _printf(_LOG_TIME_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, message), ##__VA_ARGS__);
#else
#define LOG_V(message, ...) _printf(_LOG_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, message), ##__VA_ARGS__);
#endif
#else
#define LOG_V(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_D(message, ...) _printf(_LOG_TIME_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, message), ##__VA_ARGS__);
#else
#define LOG_D(message, ...) _printf(_LOG_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, message), ##__VA_ARGS__);
#endif
#else
#define LOG_D(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_I(message, ...) _printf(_LOG_TIME_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, message), ##__VA_ARGS__);
#else
#define LOG_I(message, ...) _printf(_LOG_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, message), ##__VA_ARGS__);
#endif
#else
#define LOG_I(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_W(message, ...) _printf(_LOG_TIME_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, message), ##__VA_ARGS__);
#else
#define LOG_W(message, ...) _printf(_LOG_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, message), ##__VA_ARGS__);
#endif
#else
#define LOG_W(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#if LOG_TIME != LOG_TIME_DISABLE
#define LOG_E(message, ...) _printf(_LOG_TIME_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, message), ##__VA_ARGS__);
#else
#define LOG_E(message, ...) _printf(_LOG_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, message), ##__VA_ARGS__);
#endif
#else
#define LOG_E(message, ...)
#endif