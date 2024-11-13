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
#define LOG_LEVEL_NOTICE 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

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
#ifndef LOG_COLORS
#define LOG_COLORS 1
#endif

// Log functions

#if LOG_LEVEL > LOG_LEVEL_NONE

#if LOG_USE_TAG_SHORT
static const char *__log_levels_text[] = {"[E]", "[W]", "[N]", "[I]", "[D]"};
#else
static const char *__log_levels_text[] = {"[EROR]", "[WARN]", "[NOTI]", "[INFO]", "[DBUG]"};
#endif

static char __log_time_format_buff[18];

void format_time_hms()
{
#if LOG_USE_TIME
    long ms = millis();
    long seconds, minutes, hours, days;
    seconds = ms / 1000;
    minutes = seconds / 60;
    hours = minutes / 60;
    sprintf(__log_time_format_buff, "[%02u:%02u:%02u:%03u]", hours % 24, minutes % 60, seconds % 60, ms % 1000);
#endif
}

bool can_log(const char *tag)
{
    bool in_filter = strstr(LOG_FILTER_LIST, tag) != NULL;
    return LOG_FILTER ? in_filter : !in_filter; // INCLUDE or EXCLUDE
}

#define LOG_WRITE(loglevel, message)                                                                      \
    format_time_hms();                                                                                    \
    LOG_OUTPUT << __log_time_format_buff << " " << __log_levels_text[loglevel] << " " << message << endl; \
    LOG_OUTPUT.flush();

#define LOG_WRITE_TAG(loglevel, tag, message)                                                                                 \
    if (can_log(tag))                                                                                                         \
    {                                                                                                                         \
        format_time_hms();                                                                                                    \
        LOG_OUTPUT << __log_time_format_buff << " " << __log_levels_text[loglevel] << " [" << tag << "] " << message << endl; \
        LOG_OUTPUT.flush();                                                                                                   \
    }

#endif // if LOG_LEVEL > LOG_LEVEL_NONE

// Define logging macro functions per LOG_LEVEL

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(tag, message) LOG_WRITE_TAG(LOG_LEVEL_DEBUG, tag, message)
#else
#define LOG_DEBUG(tag, message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(tag, message) LOG_WRITE_TAG(LOG_LEVEL_INFO, tag, message)
#else
#define LOG_INFO(tag, message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_NOTICE
#define LOG_NOTICE(tag, message) LOG_WRITE_TAG(LOG_LEVEL_NOTICE, tag, message)
#else
#define LOG_NOTICE(tag, message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_WARNING(tag, message) LOG_WRITE_TAG(LOG_LEVEL_WARNING, tag, message)
#else
#define LOG_WARNING(tag, message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(tag, message) LOG_WRITE_TAG(LOG_LEVEL_ERROR, tag, message)
#else
#define LOG_ERROR(tag, message)
#endif

// Without tag

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_D(message) LOG_WRITE(LOG_LEVEL_DEBUG, message)
#else
#define LOG_D(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_I(message) LOG_WRITE(LOG_LEVEL_INFO, message)
#else
#define LOG_D(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_NOTICE
#define LOG_N(message) LOG_WRITE(LOG_LEVEL_NOTICE, message)
#else
#define LOG_N(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_W(message) LOG_WRITE(LOG_LEVEL_WARNING, message)
#else
#define LOG_W(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_E(message) LOG_WRITE(LOG_LEVEL_ERROR, message)
#else
#define LOG_E(message)
#endif
