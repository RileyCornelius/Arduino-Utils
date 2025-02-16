#include <Arduino.h>
#include <Logger3.h>

#if LOG_PRINT_TIME
#define I_VERBOSE_NAME "VERB"
#define I_DEBUG_NAME "DBUG"
#define I_INFO_NAME "INFO"
#define I_WARN_NAME "WARN"
#define I_ERROR_NAME "EROR"
#else
#define I_VERBOSE_NAME "V"
#define I_DEBUG_NAME "D"
#define I_INFO_NAME "I"
#define I_WARN_NAME "W"
#define I_ERROR_NAME "E"
#endif

#if LOG_PRINT_TIME
static const char *format_time_hms()
{
    static char time_format[18];
    unsigned long ms = millis();
    unsigned long seconds, minutes, hours, days;
    seconds = ms / 1000;
    minutes = seconds / 60;
    hours = minutes / 60;
    sprintf(time_format, "%02u:%02u:%02u:%03u", hours % 24, minutes % 60, seconds % 60, ms % 1000);
    return time_format;
}
#endif

static void iprintf(const char *loglevel, const char *format, va_list args)
{
    char message[128];
    vsprintf(message, format, args);
#if LOG_PRINT_TIME
    const char *time_format = format_time_hms();
    LOG_OUTPUT.printf("[%s] [%s] %s\n", loglevel, time_format, message);
#else
    LOG_OUTPUT.printf("[%s] %s\n", loglevel, message);
#endif
}

static void itagprintf(const char *loglevel, const char *tag, const char *format, va_list args)
{
    char message[128];
    vsprintf(message, format, args);
#if LOG_PRINT_TIME
    const char *time_format = format_time_hms();
    LOG_OUTPUT.printf("[%s] [%s] [%s] %s\n", loglevel, time_format, tag, message);
#else
    LOG_OUTPUT.printf("[%s] [%s] %s\n", loglevel, tag, message);
#endif
}

static void itaglogf(uint8_t level, const char *tag, const char *format, va_list args)
{
    switch (level)
    {
    case LOG_LEVEL_ERROR:
        itagprintf(I_ERROR_NAME, tag, format, args);
        break;
    case LOG_LEVEL_WARNING:
        itagprintf(I_WARN_NAME, tag, format, args);
        break;
    case LOG_LEVEL_INFO:
        itagprintf(I_INFO_NAME, tag, format, args);
        break;
    case LOG_LEVEL_DEBUG:
        itagprintf(I_DEBUG_NAME, tag, format, args);
        break;
    case LOG_LEVEL_VERBOSE:
        itagprintf(I_VERBOSE_NAME, tag, format, args);
        break;
    }
}

static void ilogf(uint8_t level, const char *format, va_list args)
{
    switch (level)
    {
    case LOG_LEVEL_ERROR:
        iprintf(I_ERROR_NAME, format, args);
        break;
    case LOG_LEVEL_WARNING:
        iprintf(I_WARN_NAME, format, args);
        break;
    case LOG_LEVEL_INFO:
        iprintf(I_INFO_NAME, format, args);
        break;
    case LOG_LEVEL_DEBUG:
        iprintf(I_DEBUG_NAME, format, args);
        break;
    case LOG_LEVEL_VERBOSE:
        iprintf(I_VERBOSE_NAME, format, args);
        break;
    }
}

void _iformat(uint8_t level, String format, ...)
{
    va_list args;
    va_start(args, format);
    ilogf(level, format.c_str(), args);
    va_end(args);
}

void _iformat(uint8_t level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    ilogf(level, format, args);
    va_end(args);
}

void _itagformat(uint8_t level, const char *tag, String format, ...)
{
    va_list args;
    va_start(args, format);
    itaglogf(level, tag, format.c_str(), args);
    va_end(args);
}

void _itagformat(uint8_t level, const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    itaglogf(level, tag, format, args);
    va_end(args);
}