#pragma once

/**--------------------------------------------------------------------------------------
 * Logger Defines
 *-------------------------------------------------------------------------------------*/

// Log levels defines
#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 4

// Min log level variable
#define LOG_LEVEL_MIN LOG_LEVEL_DEBUG

// Log level variable
#define LOG_LEVEL_MAX LOG_LEVEL_ERROR

// Time getter
#define LOG_TIME_GETTER() millis()

// Print time in message
#define LOG_PRINT_TIME 1

// Print log types full name or single letter
#define LOG_PRINT_LONG_NAME 1

// Log format defines
#if LOG_LEVEL_MIN > 0
#define LOG_DEBUG(tag, format, ...) iformat(LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)
#define LOG_INFO(tag, format, ...) iformat(LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)
#define LOG_WARN(tag, format, ...) iformat(LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__)
#define LOG_ERROR(tag, format, ...) iformat(LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG_F(tag, format, ...)
#define LOG_INFO_F(tag, format, ...)
#define LOG_WARN_F(tag, format, ...)
#define LOG_ERROR_F(tag, format, ...)
#endif

// Prototypes functions
void iformat(uint8_t level, const char *tag, const char *format, ...);
void iformat(uint8_t level, const char *tag, String format, ...);