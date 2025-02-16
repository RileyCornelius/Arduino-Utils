#include <Arduino.h>
#include <Logger2.h>

#if LOG_LEVEL >= LOG_LEVEL_ERROR

#if LOG_USE_TAG_SHORT
static const char *logLevelText[] = {"E", "W", "I", "D", "V"};
#else
static const char *logLevelText[] = {"EROR", "WARN", "INFO", "DBUG", "VERB"};
#endif

#if LOG_USE_TIME
static const char *formatTimeHMS()
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

static void iprintf(uint8_t loglevel, const char *color, const char *format, va_list args)
{
    char message[128];
    vsprintf(message, format, args);
#if LOG_USE_TIME
    const char *timeFormat = formatTimeHMS();
    LOG_OUTPUT.printf("%s[%s][%s] %s%s\n", color, logLevelText[loglevel], timeFormat, message, ARDUHAL_LOG_RESET_COLOR);
#else
    LOG_OUTPUT.printf("%s[%s] %s%s\n", color, logLevelText[loglevel], message, ARDUHAL_LOG_RESET_COLOR);
#endif
}

static void itagprintf(uint8_t loglevel, const char *color, const char *tag, const char *format, va_list args)
{
    char message[128];
    vsprintf(message, format, args);
#if LOG_USE_TIME
    const char *timeFormat = formatTimeHMS();
    LOG_OUTPUT.printf("%s[%s][%s][%s] %s%s\n", color, logLevelText[loglevel], timeFormat, tag, message, ARDUHAL_LOG_RESET_COLOR);
#else
    LOG_OUTPUT.printf("%s[%s][%s] %s%s\n", color, logLevelText[loglevel], tag, message, ARDUHAL_LOG_RESET_COLOR);
#endif
}

void _iformat(uint8_t level, const char *color, String format, ...)
{
    va_list args;
    va_start(args, format);
    iprintf(level, color, format.c_str(), args);
    va_end(args);
}

void _iformat(uint8_t level, const char *color, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    iprintf(level, color, format, args);
    va_end(args);
}

void _itagformat(uint8_t level, const char *color, const char *tag, String format, ...)
{
    va_list args;
    va_start(args, format);
    itagprintf(level, color, tag, format.c_str(), args);
    va_end(args);
}

void _itagformat(uint8_t level, const char *color, const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    itagprintf(level, color, tag, format, args);
    va_end(args);
}

#endif