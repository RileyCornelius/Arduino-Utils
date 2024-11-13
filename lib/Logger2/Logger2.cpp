#include <Arduino.h>
#include "Logger2.h"

#include <map>
#include <vector>
#include <chrono>

#if LOG_PRINT_LONG_NAME
#define I_DEBUG_NAME "DEBUG"
#define I_INFO_NAME "INFO"
#define I_WARN_NAME "WARN"
#define I_ERROR_NAME "ERROR"
#else
#define I_DEBUG_NAME "D"
#define I_INFO_NAME "I"
#define I_WARN_NAME "W"
#define I_ERROR_NAME "E"
#endif

// LogPrintCallback logPrintCb = log_printf;
// std::vector<std::map<const char *, LogLevel>> logTagLevels;

// int logPrint(const char *format, ...)
// {
//     va_list args;
//     va_start(args, format);
//     int ret = Serial.printf(format, args);
//     va_end(args);
//     return ret;
// }

// void logLevelSet(const char *tag, LogLevel level)
// {
//     for (auto &tagLevel : logTagLevels)
//     {
//         if (tagLevel.find(tag) != tagLevel.end())
//         {
//             tagLevel[tag] = level;
//             return;
//         }
//     }

//     std::map<const char *, LogLevel> tagLevel;
//     tagLevel[tag] = level;
//     logTagLevels.push_back(tagLevel);
// }

// void logRegisterPrintCb(LogPrintCallback cb)
// {
//     logPrintCb = cb;
// }

// void logAdd(const char *tag, LogLevel level, const char *message)
// {
//     if (level > LOG_LEVEL)
//     {
//         return;
//     }

//         for (auto &tagLevel : logTagLevels)
//     {
//         if (tagLevel.find(tag) == tagLevel.end())
//         {
//             continue;
//         }

//         if (tagLevel[tag] > level)
//         {
//             return;
//         }

//         logPrintCb(message);
//         return;
//     }

//     va_list args;
//     va_start(args, format);
// }

// private helper
// LogPrintCallback custom_print_cb = LOG_PRINT_CALLBACK;
LogPrintCallback custom_print_cb = log_printf;

uint32_t logTimestamp();

static void iprintf(const char *loglevel, const char *tag, const char *format, va_list args)
{
    char message[64];
    vsprintf(message, format, args);
#if LOG_PRINT_TIME

    Serial.printf("[%s] (%s) %s %s\n", loglevel, LOG_TIME_GETTER, tag, message);
#else
    Serial.printf("[%s] %s %s\n", loglevel, tag, message);
#endif
}

static void ilogf(uint8_t level, const char *tag, const char *format, va_list args)
{
    if (LOG_LEVEL >= level)
    {
        switch (level)
        {
        case LOG_LEVEL_ERROR:
            iprintf(I_ERROR_NAME, tag, format, args);
            break;
        case LOG_LEVEL_WARN:
            iprintf(I_WARN_NAME, tag, format, args);
            break;
        case LOG_LEVEL_INFO:
            iprintf(I_INFO_NAME, tag, format, args);
            break;
        default:
            iprintf(I_DEBUG_NAME, tag, format, args);
            break;
        }
    }
}

void iformat(uint8_t level, const char *tag, String format, ...)
{
    va_list args;
    va_start(args, format);
    ilogf(level, tag, format.c_str(), args);
    va_end(args);
}

void iformat(uint8_t level, const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    ilogf(level, tag, format, args);
    va_end(args);
}

// static const char *log_levels_text[] = {"E", "W", "N", "I", "D"};
// #else
// static const char *log_levels_text[] = {"EROR", "WARN", "NOTI", "INFO", "DBUG"};