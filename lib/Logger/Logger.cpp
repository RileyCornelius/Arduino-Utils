#include "log.h"

namespace _logger
{
#if LOG_LEVEL > LOG_LEVEL_DISABLE && LOG_PRINT_TYPE == LOG_PRINT_TYPE_PRINTF
    void vprintf(const char *format, va_list arg)
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

    void printf(const char *format, ...)
    {
        va_list arg;
        va_start(arg, format);
        vprintf(format, arg);
        va_end(arg);
    }
#endif // LOG_LEVEL > LOG_LEVEL_DISABLE

#if LOG_TIME != LOG_TIME_DISABLE
    const char *formatTime()
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

#if LOG_FILTER != LOG_FILTER_DISABLE
    bool logFilter(const char *tag)
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
#endif // LOG_LOG_FILTER != LOG_FILTER_DISABLE

#if LOG_FILENAME == LOG_FILENAME_ENABLE
    const char *filepathToName(const char *path)
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

    void assertion(bool condition, const char *file, int line, const char *func, const char *expr, const char *message)
    {
        if (!condition)
        {
            return;
        }

        char buff[256];
#if LOG_COLORS == LOG_COLORS_ENABLE
        snprintf(buff, sizeof(buff), _LOG_COLOR_E "[ASSERT] %s:%d - %s(): (%s) => %s" _LOG_RESET_COLOR, file, line, func, expr, message);
#else
        snprintf(buff, sizeof(buff), "[ASSERT] %s:%d - %s(): (%s) => %s", file, line, func, expr, message);
#endif

        LOG_PRINTLN();
        LOG_PRINTLN(buff);
        LOG_PRINTLN();

        while (true)
        {
            delay(1000);
        };
    }
} // namespace _logger