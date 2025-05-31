/**--------------------------------------------------------------------------------------
 ** Don't include this file. Include log.h instead.
 *-------------------------------------------------------------------------------------*/

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

#define LOG_LEVEL_TEXT_FORMAT_LETTER 0
#define LOG_LEVEL_TEXT_FORMAT_SHORT 1
#define LOG_LEVEL_TEXT_FORMAT_FULL 2

#define LOG_TIME_DISABLE 0
#define LOG_TIME_ENABLE 2 // Defined for consistency with other options. Same as LOG_TIME_MILLIS.
#define LOG_TIME_MICROS 1
#define LOG_TIME_MILLIS 2
#define LOG_TIME_HHMMSSMS 3
#define LOG_TIME_HHHHMMSSMS 4

#define LOG_COLOR_DISABLE 0
#define LOG_COLOR_ENABLE 1

#define LOG_FILENAME_DISABLE 0
#define LOG_FILENAME_ENABLE 1

#define LOG_FILTER_DISABLE -1
#define LOG_FILTER_ENABLE 0 // Defined for consistency with other options. Same as LOG_FILTER_EXCLUDE.
#define LOG_FILTER_EXCLUDE 0
#define LOG_FILTER_INCLUDE 1

#define LOG_PRINT_TYPE_PRINTF 0
#define LOG_PRINT_TYPE_CUSTOM_FORMAT 1
#define LOG_PRINT_TYPE_STD_FORMAT 2
#define LOG_PRINT_TYPE_FMT_FORMAT 3

/**--------------------------------------------------------------------------------------
 * Logger Default Settings
 *-------------------------------------------------------------------------------------*/

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG // Defaults LOG_LEVEL_DEBUG
#endif

#ifndef LOG_TIME
#define LOG_TIME LOG_TIME_DISABLE
#endif

#ifndef LOG_LEVEL_TEXT_FORMAT
#define LOG_LEVEL_TEXT_FORMAT LOG_LEVEL_TEXT_FORMAT_SHORT
#endif

#ifndef LOG_COLOR
#define LOG_COLOR LOG_COLOR_DISABLE
#endif

#ifndef LOG_FILENAME
#define LOG_FILENAME LOG_FILENAME_DISABLE
#endif

#ifndef LOG_FILTER
#define LOG_FILTER LOG_FILTER_DISABLE
#endif

#ifndef LOG_FILTER_LIST
#define LOG_FILTER_LIST {""}
#endif

#ifndef LOG_STATIC_BUFFER_SIZE
#define LOG_STATIC_BUFFER_SIZE 64
#endif

#ifndef LOG_PRINT_TYPE
#define LOG_PRINT_TYPE LOG_PRINT_TYPE_PRINTF
#endif

#ifndef LOG_OUTPUT
#define LOG_OUTPUT Serial
#endif

#ifndef LOG_EOL
#define LOG_EOL "\r\n"
#endif

/**--------------------------------------------------------------------------------------
 * Logger Settings Check
 *-------------------------------------------------------------------------------------*/

static_assert(LOG_LEVEL >= LOG_LEVEL_DISABLE && LOG_LEVEL <= LOG_LEVEL_VERBOSE, "LOG_LEVEL must be between LOG_LEVEL_DISABLE and LOG_LEVEL_VERBOSE");
static_assert(LOG_LEVEL_TEXT_FORMAT >= LOG_LEVEL_TEXT_FORMAT_LETTER && LOG_LEVEL_TEXT_FORMAT <= LOG_LEVEL_TEXT_FORMAT_FULL, "LOG_LEVEL_TEXT_FORMAT must be either LOG_LEVEL_TEXT_FORMAT_LETTER, LOG_LEVEL_TEXT_FORMAT_SHORT or LOG_LEVEL_TEXT_FORMAT_LONG");
static_assert(LOG_TIME >= LOG_TIME_DISABLE && LOG_TIME <= LOG_TIME_HHHHMMSSMS, "LOG_TIME must be between LOG_TIME_DISABLE and LOG_TIME_HHHHMMSSMS");
static_assert(LOG_FILTER >= LOG_FILTER_DISABLE && LOG_FILTER <= LOG_FILTER_INCLUDE, "LOG_FILTER must be between LOG_FILTER_DISABLE and LOG_FILTER_INCLUDE");
static_assert(LOG_PRINT_TYPE >= LOG_PRINT_TYPE_PRINTF || LOG_PRINT_TYPE <= LOG_PRINT_TYPE_FMT_FORMAT, "LOG_PRINT_TYPE must be either LOG_PRINT_TYPE_PRINTF, LOG_PRINT_TYPE_CUSTOM_FORMAT, LOG_PRINT_TYPE_STD_FORMAT or LOG_PRINT_TYPE_FMT_FORMAT");
static_assert(LOG_FILENAME == LOG_FILENAME_DISABLE || LOG_FILENAME == LOG_FILENAME_ENABLE, "LOG_FILENAME must be either LOG_FILENAME_DISABLE or LOG_FILENAME_ENABLE");
static_assert(LOG_COLOR == LOG_COLOR_DISABLE || LOG_COLOR == LOG_COLOR_ENABLE, "LOG_COLOR must be either LOG_COLOR_DISABLE or LOG_COLOR_ENABLE");
static_assert(LOG_STATIC_BUFFER_SIZE > 0, "LOG_STATIC_BUFFER_SIZE must be greater than 0");

/**--------------------------------------------------------------------------------------
 * Logger Private Functions
 *-------------------------------------------------------------------------------------*/

namespace _logger
{
#if LOG_LEVEL > LOG_LEVEL_DISABLE && LOG_PRINT_TYPE == LOG_PRINT_TYPE_PRINTF
    void vprintf(const char *format, va_list arg);
    void printf(const char *format, ...);
#endif

#if LOG_TIME != LOG_TIME_DISABLE
    const char *formatTime();
#endif

#if LOG_FILTER != LOG_FILTER_DISABLE
    bool logFilter(const char *tag);
#endif

    const char *filePathToName(const char *path);
    void assertion(bool flag, const char *file, int line, const char *func, const char *expr, const char *message = "");
}

/**--------------------------------------------------------------------------------------
 * Logger Private Macros
 *-------------------------------------------------------------------------------------*/

// Colors

#if LOG_COLOR == LOG_COLOR_ENABLE
#define _LOG_COLOR_BLACK "30"
#define _LOG_COLOR_BRIGHT_RED "91" // ERROR
#define _LOG_COLOR_RED "31"
#define _LOG_COLOR_GREEN "32"  // INFO
#define _LOG_COLOR_YELLOW "33" // WARNING
#define _LOG_COLOR_BLUE "34"
#define _LOG_COLOR_MAGENTA "35"
#define _LOG_COLOR_CYAN "36"         // DEBUG
#define _LOG_COLOR_BRIGHT_WHITE "37" // VERBOSE
#define _LOG_COLOR_WHITE "38"
#define _LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define _LOG_BOLD(COLOR) "\033[1;" COLOR "m"
#define _LOG_RESET_COLOR "\033[0m"
#define _LOG_COLOR_E _LOG_COLOR(_LOG_COLOR_BRIGHT_RED)
#define _LOG_COLOR_W _LOG_COLOR(_LOG_COLOR_YELLOW)
#define _LOG_COLOR_I _LOG_COLOR(_LOG_COLOR_GREEN)
#define _LOG_COLOR_D _LOG_COLOR(_LOG_COLOR_CYAN)
#define _LOG_COLOR_V _LOG_COLOR(_LOG_COLOR_BRIGHT_WHITE)
#else
#define _LOG_COLOR_E ""
#define _LOG_COLOR_W ""
#define _LOG_COLOR_I ""
#define _LOG_COLOR_D ""
#define _LOG_COLOR_V ""
#define _LOG_RESET_COLOR ""
#endif // LOG_COLOR == LOG_COLOR_ENABLE

// Filter

#if LOG_FILTER != LOG_FILTER_DISABLE
#define _IF_LOG_FILTER_BEGIN(tag) \
    if (_logger::logFilter(tag))  \
    {
#define _IF_LOG_FILTER_END }
#else
#define _IF_LOG_FILTER_BEGIN(tag)
#define _IF_LOG_FILTER_END
#endif // LOG_LOG_FILTER != LOG_FILTER_DISABLE

// Log level text

#if LOG_LEVEL_TEXT_FORMAT == LOG_LEVEL_TEXT_FORMAT_LETTER
#define _LOG_LEVEL_VERBOSE_TEXT "[V]"
#define _LOG_LEVEL_DEBUG_TEXT "[D]"
#define _LOG_LEVEL_INFO_TEXT "[I]"
#define _LOG_LEVEL_WARNING_TEXT "[W]"
#define _LOG_LEVEL_ERROR_TEXT "[E]"
#elif LOG_LEVEL_TEXT_FORMAT == LOG_LEVEL_TEXT_FORMAT_SHORT
#define _LOG_LEVEL_VERBOSE_TEXT "[VERB]"
#define _LOG_LEVEL_DEBUG_TEXT "[DBUG]"
#define _LOG_LEVEL_INFO_TEXT "[INFO]"
#define _LOG_LEVEL_WARNING_TEXT "[WARN]"
#define _LOG_LEVEL_ERROR_TEXT "[EROR]"
#else
#define _LOG_LEVEL_VERBOSE_TEXT "[VERBOSE]"
#define _LOG_LEVEL_DEBUG_TEXT "[DEBUG]"
#define _LOG_LEVEL_INFO_TEXT "[INFO]"
#define _LOG_LEVEL_WARNING_TEXT "[WARNING]"
#define _LOG_LEVEL_ERROR_TEXT "[ERROR]"
#endif // LOG_LEVEL_TEXT_FORMAT == LOG_LEVEL_TEXT_FORMAT_LETTER

// Preamble format

#if LOG_PRINT_TYPE == LOG_PRINT_TYPE_STD_FORMAT || LOG_PRINT_TYPE == LOG_PRINT_TYPE_FMT_FORMAT || LOG_PRINT_TYPE == LOG_PRINT_TYPE_CUSTOM_FORMAT
#define __LOG_TAG_FORMAT(loglevel, color, tag, format) color loglevel "[{}] " format _LOG_RESET_COLOR LOG_EOL, tag
#define __LOG_TAG_TIME_FORMAT(loglevel, color, tag, format) color "[{}]" loglevel "[{}] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), tag
#define __LOG_TAG_FILE_FORMAT(loglevel, color, tag, format) color loglevel "[{}][{}:{}] " format _LOG_RESET_COLOR LOG_EOL, tag, _logger::filePathToName(__FILE__), __LINE__
#define __LOG_TAG_TIME_FILE_FORMAT(loglevel, color, tag, format) color "[{}]" loglevel "[{}][{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), tag, _logger::filePathToName(__FILE__), __LINE__

#define __LOG_FORMAT(loglevel, color, format) color loglevel " " format _LOG_RESET_COLOR LOG_EOL
#define __LOG_TIME_FORMAT(loglevel, color, format) color "[{}]" loglevel " " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime()
#define __LOG_FILE_FORMAT(loglevel, color, format) color loglevel "[{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _logger::filePathToName(__FILE__), __LINE__
#define __LOG_TIME_FILE_FORMAT(loglevel, color, format) color "[{}]" loglevel "[{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), _logger::filePathToName(__FILE__), __LINE__
#else
#define __LOG_TAG_FORMAT(loglevel, color, tag, format) color loglevel "[%s] " format _LOG_RESET_COLOR LOG_EOL, tag
#define __LOG_TAG_TIME_FORMAT(loglevel, color, tag, format) color "[%s]" loglevel "[%s] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), tag
#define __LOG_TAG_FILE_FORMAT(loglevel, color, tag, format) color loglevel "[%s][%s:%d] " format _LOG_RESET_COLOR LOG_EOL, tag, _logger::filePathToName(__FILE__), __LINE__
#define __LOG_TAG_TIME_FILE_FORMAT(loglevel, color, tag, format) color "[%s]" loglevel "[%s][%s:%d] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), tag, _logger::filePathToName(__FILE__), __LINE__

#define __LOG_FORMAT(loglevel, color, format) color loglevel " " format _LOG_RESET_COLOR LOG_EOL
#define __LOG_TIME_FORMAT(loglevel, color, format) color "[%s]" loglevel " " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime()
#define __LOG_FILE_FORMAT(loglevel, color, format) color loglevel "[%s:%d] " format _LOG_RESET_COLOR LOG_EOL, _logger::filePathToName(__FILE__), __LINE__
#define __LOG_TIME_FILE_FORMAT(loglevel, color, format) color "[%s]" loglevel "[%s:%d] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), _logger::filePathToName(__FILE__), __LINE__
#endif // LOG_PRINT_TYPE == LOG_PRINT_TYPE_STD_FORMAT || LOG_PRINT_TYPE == LOG_PRINT_TYPE_FMT_FORMAT

// Format without tag

#if LOG_TIME != LOG_TIME_DISABLE && LOG_FILENAME != LOG_FILENAME_DISABLE
#define _LOG_FORMAT(level, color, msg) __LOG_TIME_FILE_FORMAT(level, color, msg)
#elif LOG_TIME != LOG_TIME_DISABLE && LOG_FILENAME == LOG_FILENAME_DISABLE
#define _LOG_FORMAT(level, color, msg) __LOG_TIME_FORMAT(level, color, msg)
#elif LOG_TIME == LOG_TIME_DISABLE && LOG_FILENAME != LOG_FILENAME_DISABLE
#define _LOG_FORMAT(level, color, msg) __LOG_FILE_FORMAT(level, color, msg)
#else
#define _LOG_FORMAT(level, color, msg) __LOG_FORMAT(level, color, msg)
#endif

// Format with tag

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

// Assert macros

#ifndef NDEBUG
#define ASSERT(condition, msg) _logger::assertion((condition), __FILE__, __LINE__, __func__, #condition, msg)
#else
#define ASSERT(condition, msg) ((void)0)
#endif

// Print macros

#if LOG_LEVEL != LOG_LEVEL_DISABLE
#define LOG_BEGIN(baud) LOG_OUTPUT.begin(baud)
#define LOG_PRINT(msg) LOG_OUTPUT.print(msg)
#define LOG_PRINTLN(msg) LOG_OUTPUT.println(msg)
#else
#define LOG_BEGIN(baud)
#define LOG_PRINT(msg)
#define LOG_PRINTLN(msg)
#endif

/**--------------------------------------------------------------------------------------
 * Logger Print Macros
 *-------------------------------------------------------------------------------------*/

// Use std::format (C++20)
#if LOG_PRINT_TYPE == LOG_PRINT_TYPE_STD_FORMAT
#include <format>
#define LOG_PRINTF(msg, ...)                                                                                       \
    do                                                                                                             \
    {                                                                                                              \
        char buffer[LOG_STATIC_BUFFER_SIZE];                                                                       \
        std::format_to_n_result result = std::format_to_n(buffer, LOG_STATIC_BUFFER_SIZE - 1, msg, ##__VA_ARGS__); \
        if (result.size < LOG_STATIC_BUFFER_SIZE)                                                                  \
        {                                                                                                          \
            buffer[result.size] = '\0';                                                                            \
            LOG_OUTPUT.print(buffer);                                                                              \
        }                                                                                                          \
        else                                                                                                       \
        {                                                                                                          \
            char larger_buffer[result.size + 1];                                                                   \
            std::format_to_n(larger_buffer, result.size, msg, ##__VA_ARGS__);                                      \
            larger_buffer[result.size] = '\0';                                                                     \
            LOG_OUTPUT.print(larger_buffer);                                                                       \
        }                                                                                                          \
    } while (0)

// Use fmtlib fmt::format
#elif LOG_PRINT_TYPE == LOG_PRINT_TYPE_FMT_FORMAT
#include <fmt.h>        // lib_deps = https://github.com/RileyCornelius/fmt-arduino.git
#include <fmt/ranges.h> // Include the ranges support for fmtlib
#define LOG_PRINTF(msg, ...)                                            \
    do                                                                  \
    {                                                                   \
        fmt::basic_memory_buffer<char, LOG_STATIC_BUFFER_SIZE> buffer;  \
        fmt::format_to(std::back_inserter(buffer), msg, ##__VA_ARGS__); \
        buffer.push_back('\0');                                         \
        LOG_OUTPUT.print(buffer.data());                                \
    } while (0)

// Use custom format lib afmt::format
#elif LOG_PRINT_TYPE == LOG_PRINT_TYPE_CUSTOM_FORMAT
#define AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE LOG_STATIC_BUFFER_SIZE
#include "format.h"
#define LOG_PRINTF(msg, ...)                         \
    do                                               \
    {                                                \
        afmt::buffer buf;                            \
        afmt::format_to(buf, format, ##__VA_ARGS__); \
        LOG_OUTPUT.print(buf.c_str());               \
    } while (0)

#else
#define LOG_PRINTF(msg, ...) _logger::printf(msg, ##__VA_ARGS__)
#endif

/**--------------------------------------------------------------------------------------
 * Logger Log Macros
 *-------------------------------------------------------------------------------------*/

// Log without tag

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_V(message, ...) LOG_PRINTF(_LOG_FORMAT(_LOG_LEVEL_VERBOSE_TEXT, _LOG_COLOR_V, message), ##__VA_ARGS__)
#else
#define LOG_V(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_D(message, ...) LOG_PRINTF(_LOG_FORMAT(_LOG_LEVEL_DEBUG_TEXT, _LOG_COLOR_D, message), ##__VA_ARGS__)
#else
#define LOG_D(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_I(message, ...) LOG_PRINTF(_LOG_FORMAT(_LOG_LEVEL_INFO_TEXT, _LOG_COLOR_I, message), ##__VA_ARGS__)
#else
#define LOG_I(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_W(message, ...) LOG_PRINTF(_LOG_FORMAT(_LOG_LEVEL_WARNING_TEXT, _LOG_COLOR_W, message), ##__VA_ARGS__)
#else
#define LOG_W(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_E(message, ...) LOG_PRINTF(_LOG_FORMAT(_LOG_LEVEL_ERROR_TEXT, _LOG_COLOR_E, message), ##__VA_ARGS__)
#else
#define LOG_E(message, ...)
#endif

// Log with tag

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_VERB(tag, message, ...)                                                                  \
    _IF_LOG_FILTER_BEGIN(tag)                                                                        \
    LOG_PRINTF(_LOG_TAG_FORMAT(_LOG_LEVEL_VERBOSE_TEXT, _LOG_COLOR_V, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_VERB(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(tag, message, ...)                                                               \
    _IF_LOG_FILTER_BEGIN(tag)                                                                      \
    LOG_PRINTF(_LOG_TAG_FORMAT(_LOG_LEVEL_DEBUG_TEXT, _LOG_COLOR_D, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_DEBUG(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(tag, message, ...)                                                               \
    _IF_LOG_FILTER_BEGIN(tag)                                                                     \
    LOG_PRINTF(_LOG_TAG_FORMAT(_LOG_LEVEL_INFO_TEXT, _LOG_COLOR_I, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_INFO(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_WARN(tag, message, ...)                                                                  \
    _IF_LOG_FILTER_BEGIN(tag)                                                                        \
    LOG_PRINTF(_LOG_TAG_FORMAT(_LOG_LEVEL_WARNING_TEXT, _LOG_COLOR_W, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_WARN(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(tag, message, ...)                                                               \
    _IF_LOG_FILTER_BEGIN(tag)                                                                      \
    LOG_PRINTF(_LOG_TAG_FORMAT(_LOG_LEVEL_ERROR_TEXT, _LOG_COLOR_E, tag, message), ##__VA_ARGS__); \
    _IF_LOG_FILTER_END
#else
#define LOG_ERROR(tag, message, ...)
#endif