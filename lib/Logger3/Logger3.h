#pragma once
#include <string.h>

/**--------------------------------------------------------------------------------------
 * Logger Setting Options
 *-------------------------------------------------------------------------------------*/

// LOG levels options
#define LOG_LEVEL_NONE -1
#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_VERBOSE 4

// Filter options
#define LOG_FILTER_INCLUDE 1
#define LOG_FILTER_EXCLUDE 0

/**--------------------------------------------------------------------------------------
 * Logger Settings
 *-------------------------------------------------------------------------------------*/

// Default LOG_LEVEL is set to LOG_LEVEL_DEBUG, if it's not defined in user code.
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

// Log output
#ifndef LOG_OUTPUT
#define LOG_OUTPUT Serial
#endif

// Filter
#ifndef LOG_FILTER
#define LOG_FILTER LOG_FILTER_EXCLUDE
#endif

#ifndef LOG_FILTER_LIST
#define LOG_FILTER_LIST ""
#endif

// Endline
#ifndef endl
#define endl "\r\n"
#endif

// Timestamp
#ifndef LOG_USE_TIME
#define LOG_USE_TIME 0
#endif

// Log Level Tag
#ifndef LOG_USE_TAG_SHORT
#define LOG_USE_TAG_SHORT 0
#endif

// Log colors
#ifndef LOG_USE_COLORS
#define LOG_USE_COLORS 0
#endif

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
#endif

/**--------------------------------------------------------------------------------------
 * Logger Private Functions and Macros
 *-------------------------------------------------------------------------------------*/

#if LOG_USE_TAG_SHORT
static const char *_logLevelText[] = {"E", "W", "I", "D", "V"};
#else
static const char *_logLevelText[] = {"EROR", "WARN", "INFO", "DBUG", "VERB"};
#endif

#if LOG_USE_TIME
static const char *_formatTimeHMS()
{
    static char timeFormat[18];
    unsigned long ms = millis();
    unsigned long seconds, minutes, hours, days;
    seconds = ms / 1000;
    minutes = seconds / 60;
    hours = minutes / 60;
    sprintf(timeFormat, "%02u:%02u:%02u:%03u", hours % 24, minutes % 60, seconds % 60, ms % 1000);
    return timeFormat;
}
#endif

static bool _logFilter(const char *tag)
{
    bool in_filter = strstr(LOG_FILTER_LIST, tag) != NULL;
    return LOG_FILTER ? in_filter : !in_filter; // INCLUDE or EXCLUDE
}

// Custom log print formats
#define _LOG_CUSTOM_TAG_FORMAT(loglevel, color, tag, format) color "[%s][%s] " format _LOG_RESET_COLOR endl, _logLevelText[loglevel], tag
#define _LOG_CUSTOM_FORMAT(loglevel, color, format) color "[%s] " format _LOG_RESET_COLOR endl, _logLevelText[loglevel]

#define _LOG_CUSTOM_TAG_TIME_FORMAT(loglevel, color, tag, format) color "[%s][%s][%s] " format _LOG_RESET_COLOR endl, _formatTimeHMS(), _logLevelText[loglevel], tag
#define _LOG_CUSTOM_TIME_FORMAT(loglevel, color, format) color "[%s][%s] " format _LOG_RESET_COLOR endl, _formatTimeHMS(), _logLevelText[loglevel]

/**--------------------------------------------------------------------------------------
 * Logger Public Macros
 *-------------------------------------------------------------------------------------*/

// Log format defines
#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#if LOG_USE_TIME
#define LOG_VERBOSE(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_TIME_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, tag, message), ##__VA_ARGS__);
#else
#define LOG_VERBOSE(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, tag, message), ##__VA_ARGS__);
#endif
#else
#define LOG_VERBOSE(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#if LOG_USE_TIME
#define LOG_DEBUG(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_TIME_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, tag, message), ##__VA_ARGS__);
#else
#define LOG_DEBUG(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, tag, message), ##__VA_ARGS__);

#endif
#else
#define LOG_DEBUG(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#if LOG_USE_TIME
#define LOG_INFO(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_TIME_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, tag, message), ##__VA_ARGS__);
#else
#define LOG_INFO(tag, message, ...)                                                                           \
    if (_logFilter(tag))                                                                                      \
    {                                                                                                         \
        LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, tag, message), ##__VA_ARGS__); \
    }
#endif
#else
#define LOG_INFO(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#if LOG_USE_TIME
#define LOG_WARNING(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_TIME_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, tag, message), ##__VA_ARGS__);
#else
#define LOG_WARNING(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, tag, message), ##__VA_ARGS__);
#endif
#else
#define LOG_WARNING(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#if LOG_USE_TIME
#define LOG_ERROR(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_TIME_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, tag, message), ##__VA_ARGS__);
#else
#define LOG_ERROR(tag, message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TAG_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, tag, message), ##__VA_ARGS__);
#endif
#else
#define LOG_ERROR(tag, message, ...)
#endif

// Without tag

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#if LOG_USE_TIME
#define LOG_V(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TIME_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, message), ##__VA_ARGS__);
#else
#define LOG_V(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_FORMAT(LOG_LEVEL_VERBOSE, _LOG_COLOR_V, message), ##__VA_ARGS__);
#endif
#else
#define LOG_V(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#if LOG_USE_TIME
#define LOG_D(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TIME_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, message), ##__VA_ARGS__);
#else
#define LOG_D(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_FORMAT(LOG_LEVEL_DEBUG, _LOG_COLOR_D, message), ##__VA_ARGS__);
#endif
#else
#define LOG_D(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#if LOG_USE_TIME
#define LOG_I(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TIME_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, message), ##__VA_ARGS__);
#else
#define LOG_I(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_FORMAT(LOG_LEVEL_INFO, _LOG_COLOR_I, message), ##__VA_ARGS__);
#endif
#else
#define LOG_I(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#if LOG_USE_TIME
#define LOG_W(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TIME_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, message), ##__VA_ARGS__);
#else
#define LOG_W(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_FORMAT(LOG_LEVEL_WARNING, _LOG_COLOR_W, message), ##__VA_ARGS__);
#endif
#else
#define LOG_W(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#if LOG_USE_TIME
#define LOG_E(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_TIME_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, message), ##__VA_ARGS__);
#else
#define LOG_E(message, ...) LOG_OUTPUT.printf(_LOG_CUSTOM_FORMAT(LOG_LEVEL_ERROR, _LOG_COLOR_E, message), ##__VA_ARGS__);
#endif
#else
#define LOG_E(message, ...)
#endif