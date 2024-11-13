#pragma once

#include <Arduino.h>

// Print << operator for logging
// Example: LOG_DEBUG("TST", "var1" << var1 << ", var2=" << var2)
template <class T>
inline Print &operator<<(Print &obj, T arg)
{
    obj.print(arg);
    return obj;
}

#define endl "\r\n"

// Default to log to "Serial". But it can be defined in code to log to Serial2 for example.
#ifndef LOG_OUTPUT
#define LOG_OUTPUT Serial
#endif

// LOG levels order/priority. IF LOG_LEVEL_NONE is set, then nothing will be logged
#define LOG_LEVEL_NONE -1
#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_VERBOSE 4

// Default LOG_LEVEL is set to LOG_LEVEL_DEBUG, if it's not defined in user code.
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

// Filter
#define LOG_FILTER_INCLUDE 1
#define LOG_FILTER_EXCLUDE 0

#ifndef LOG_FILTER
#define LOG_FILTER LOG_FILTER_EXCLUDE
#endif

#ifndef LOG_FILTER_LIST
#define LOG_FILTER_LIST ""
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
#define ARDUHAL_LOG_COLOR_BLACK "30"
#define ARDUHAL_LOG_COLOR_RED "31"    // ERROR
#define ARDUHAL_LOG_COLOR_GREEN "32"  // INFO
#define ARDUHAL_LOG_COLOR_YELLOW "33" // WARNING
#define ARDUHAL_LOG_COLOR_BLUE "34"
#define ARDUHAL_LOG_COLOR_MAGENTA "35"
#define ARDUHAL_LOG_COLOR_CYAN "36" // DEBUG
#define ARDUHAL_LOG_COLOR_GRAY "37" // VERBOSE
#define ARDUHAL_LOG_COLOR_WHITE "38"

#define ARDUHAL_LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define ARDUHAL_LOG_BOLD(COLOR) "\033[1;" COLOR "m"
#define ARDUHAL_LOG_RESET_COLOR "\033[0m"

#define ARDUHAL_LOG_COLOR_E ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_RED)
#define ARDUHAL_LOG_COLOR_W ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_YELLOW)
#define ARDUHAL_LOG_COLOR_I ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_GREEN)
#define ARDUHAL_LOG_COLOR_D ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_CYAN)
#define ARDUHAL_LOG_COLOR_V ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_GRAY)
#define ARDUHAL_LOG_COLOR_PRINT(letter) log_printf(ARDUHAL_LOG_COLOR_##letter)
#define ARDUHAL_LOG_COLOR_PRINT_END log_printf(ARDUHAL_LOG_RESET_COLOR)
#else
#define ARDUHAL_LOG_COLOR_E ""
#define ARDUHAL_LOG_COLOR_W ""
#define ARDUHAL_LOG_COLOR_I ""
#define ARDUHAL_LOG_COLOR_D ""
#define ARDUHAL_LOG_COLOR_V ""
#define ARDUHAL_LOG_RESET_COLOR ""
#define ARDUHAL_LOG_COLOR_PRINT(letter)
#define ARDUHAL_LOG_COLOR_PRINT_END
#endif

// Log functions

#if LOG_LEVEL > LOG_LEVEL_NONE

#if LOG_USE_TAG_SHORT
static const char *_log_levels_text[] = {"[E]", "[W]", "[I]", "[D]", "[V]"};
#else
static const char *_log_levels_text[] = {"[EROR]", "[WARN]", "[INFO]", "[DBUG]", "[VERB]"};
#endif

static char _log_time_format_buff[18];

static void _log_format_time_hms()
{
#if LOG_USE_TIME
    long ms = millis();
    long seconds, minutes, hours, days;
    seconds = ms / 1000;
    minutes = seconds / 60;
    hours = minutes / 60;
    sprintf(_log_time_format_buff, "[%02u:%02u:%02u:%03u] ", hours % 24, minutes % 60, seconds % 60, ms % 1000);
#endif
}

static bool _log_filter(const char *tag)
{
    bool in_filter = strstr(LOG_FILTER_LIST, tag) != NULL;
    return LOG_FILTER ? in_filter : !in_filter; // INCLUDE or EXCLUDE
}

#define LOG_WRITE(letter, loglevel, message)                                                                                                              \
    _log_format_time_hms();                                                                                                                               \
    LOG_OUTPUT << ARDUHAL_LOG_COLOR_##letter << _log_time_format_buff << _log_levels_text[loglevel] << " " << message << ARDUHAL_LOG_RESET_COLOR << endl; \
    LOG_OUTPUT.flush();

#define LOG_WRITE_TAG(letter, loglevel, tag, message)                                                                                                                         \
    if (_log_filter(tag))                                                                                                                                                     \
    {                                                                                                                                                                         \
        _log_format_time_hms();                                                                                                                                               \
        LOG_OUTPUT << ARDUHAL_LOG_COLOR_##letter << _log_time_format_buff << _log_levels_text[loglevel] << " [" << tag << "] " << message << ARDUHAL_LOG_RESET_COLOR << endl; \
        LOG_OUTPUT.flush();                                                                                                                                                   \
    }

#endif // if LOG_LEVEL > LOG_LEVEL_NONE

// Define logging macro functions per LOG_LEVEL

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_VERBOSE(tag, message) LOG_WRITE_TAG(V, LOG_LEVEL_VERBOSE, tag, message)
#else
#define LOG_VERBOSE(tag, message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(tag, message) LOG_WRITE_TAG(D, LOG_LEVEL_DEBUG, tag, message)
#else
#define LOG_DEBUG(tag, message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(tag, message) LOG_WRITE_TAG(I, LOG_LEVEL_INFO, tag, message)
#else
#define LOG_INFO(tag, message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_WARNING(tag, message) LOG_WRITE_TAG(W, LOG_LEVEL_WARNING, tag, message)
#else
#define LOG_WARNING(tag, message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(tag, message) LOG_WRITE_TAG(E, LOG_LEVEL_ERROR, tag, message)
#else
#define LOG_ERROR(tag, message)
#endif

// Without tag

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_V(message) LOG_WRITE(V, LOG_LEVEL_VERBOSE, message)
#else
#define LOG_V(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_D(message) LOG_WRITE(D, LOG_LEVEL_DEBUG, message)
#else
#define LOG_D(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_I(message) LOG_WRITE(I, LOG_LEVEL_INFO, message)
#else
#define LOG_D(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_W(message) LOG_WRITE(W, LOG_LEVEL_WARNING, message)
#else
#define LOG_W(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_E(message) LOG_WRITE(E, LOG_LEVEL_ERROR, message)
#else
#define LOG_E(message)
#endif
