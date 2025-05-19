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
#define LOG_TIME_MICROS 1
#define LOG_TIME_MILLIS 2
#define LOG_TIME_HHMMSSMS 3
#define LOG_TIME_HHHHMMSSMS 4

#define LOG_COLORS_DISABLE 0
#define LOG_COLORS_ENABLE 1

#define LOG_FILENAME_DISABLE 0
#define LOG_FILENAME_ENABLE 1

#define LOG_FILTER_DISABLE -1
#define LOG_FILTER_EXCLUDE 0
#define LOG_FILTER_INCLUDE 1

#define LOG_PRINT_TYPE_PRINTF 0
#define LOG_PRINT_TYPE_STD_FORMAT 1
#define LOG_PRINT_TYPE_FMT_FORMAT 2

/**--------------------------------------------------------------------------------------
 * Logger Default Settings
 *-------------------------------------------------------------------------------------*/

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG // If LOG_LEVEL is not defined by the user, it defaults to LOG_LEVEL_DEBUG
#endif

#ifndef LOG_TIME
#define LOG_TIME LOG_TIME_DISABLE
#endif

#ifndef LOG_LEVEL_TEXT_FORMAT
#define LOG_LEVEL_TEXT_FORMAT LOG_LEVEL_TEXT_FORMAT_SHORT
#endif

#ifndef LOG_COLORS
#define LOG_COLORS LOG_COLORS_DISABLE
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

#ifndef LOG_PRINT_TYPE
#define LOG_PRINT_TYPE LOG_PRINT_TYPE_PRINTF
#endif

#ifndef LOG_OUTPUT
#define LOG_OUTPUT Serial
#endif

#define LOG_EOL "\r\n"

/**--------------------------------------------------------------------------------------
 * Logger Settings Check
 *-------------------------------------------------------------------------------------*/

static_assert(LOG_LEVEL >= LOG_LEVEL_DISABLE && LOG_LEVEL <= LOG_LEVEL_VERBOSE, "LOG_LEVEL must be between LOG_LEVEL_DISABLE and LOG_LEVEL_VERBOSE");
static_assert(LOG_LEVEL_TEXT_FORMAT >= LOG_LEVEL_TEXT_FORMAT_LETTER && LOG_LEVEL_TEXT_FORMAT <= LOG_LEVEL_TEXT_FORMAT_FULL, "LOG_LEVEL_TEXT_FORMAT must be either LOG_LEVEL_TEXT_FORMAT_LETTER, LOG_LEVEL_TEXT_FORMAT_SHORT or LOG_LEVEL_TEXT_FORMAT_LONG");
static_assert(LOG_TIME >= LOG_TIME_DISABLE && LOG_TIME <= LOG_TIME_HHHHMMSSMS, "LOG_TIME must be between LOG_TIME_DISABLE and LOG_TIME_HHHHMMSSMS");
static_assert(LOG_FILTER >= LOG_FILTER_DISABLE && LOG_FILTER <= LOG_FILTER_INCLUDE, "LOG_FILTER must be between LOG_FILTER_DISABLE and LOG_FILTER_INCLUDE");
static_assert(LOG_PRINT_TYPE >= LOG_PRINT_TYPE_PRINTF || LOG_PRINT_TYPE <= LOG_PRINT_TYPE_FMT_FORMAT, "LOG_PRINT_TYPE must be either LOG_PRINT_TYPE_PRINTF, LOG_PRINT_TYPE_STD_FORMAT or LOG_PRINT_TYPE_FMT_FORMAT");
static_assert(LOG_FILENAME == LOG_FILENAME_DISABLE || LOG_FILENAME == LOG_FILENAME_ENABLE, "LOG_FILENAME must be either LOG_FILENAME_DISABLE or LOG_FILENAME_ENABLE");
static_assert(LOG_COLORS == LOG_COLORS_DISABLE || LOG_COLORS == LOG_COLORS_ENABLE, "LOG_COLORS must be either LOG_COLORS_DISABLE or LOG_COLORS_ENABLE");

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

#if LOG_FILENAME == LOG_FILENAME_ENABLE
    const char *filepathToName(const char *path);
#endif
}

/**--------------------------------------------------------------------------------------
 * Logger Private Macros
 *-------------------------------------------------------------------------------------*/

// Colors

#if LOG_COLORS == LOG_COLORS_ENABLE
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
#endif // LOG_COLORS == LOG_COLORS_ENABLE

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

#if LOG_PRINT_TYPE == LOG_PRINT_TYPE_STD_FORMAT || LOG_PRINT_TYPE == LOG_PRINT_TYPE_FMT_FORMAT
#define __LOG_TAG_FORMAT(loglevel, color, tag, format) color loglevel "[{}] " format _LOG_RESET_COLOR LOG_EOL, tag
#define __LOG_TAG_TIME_FORMAT(loglevel, color, tag, format) color "[{}]" loglevel "[{}] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), tag
#define __LOG_TAG_FILE_FORMAT(loglevel, color, tag, format) color loglevel "[{}][{}:{}] " format _LOG_RESET_COLOR LOG_EOL, tag, _logger::filepathToName(__FILE__), __LINE__
#define __LOG_TAG_TIME_FILE_FORMAT(loglevel, color, tag, format) color "[{}]" loglevel "[{}][{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), tag, _logger::filepathToName(__FILE__), __LINE__

#define __LOG_FORMAT(loglevel, color, format) color loglevel " " format _LOG_RESET_COLOR LOG_EOL
#define __LOG_TIME_FORMAT(loglevel, color, format) color "[{}]" loglevel " " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime()
#define __LOG_FILE_FORMAT(loglevel, color, format) color loglevel "[{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _logger::filepathToName(__FILE__), __LINE__
#define __LOG_TIME_FILE_FORMAT(loglevel, color, format) color "[{}]" loglevel "[{}:{}] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), _logger::filepathToName(__FILE__), __LINE__
#else
#define __LOG_TAG_FORMAT(loglevel, color, tag, format) color loglevel "[%s] " format _LOG_RESET_COLOR LOG_EOL, tag
#define __LOG_TAG_TIME_FORMAT(loglevel, color, tag, format) color "[%s]" loglevel "[%s] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), tag
#define __LOG_TAG_FILE_FORMAT(loglevel, color, tag, format) color loglevel "[%s][%s:%d] " format _LOG_RESET_COLOR LOG_EOL, tag, _logger::filepathToName(__FILE__), __LINE__
#define __LOG_TAG_TIME_FILE_FORMAT(loglevel, color, tag, format) color "[%s]" loglevel "[%s][%s:%d] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), tag, _logger::filepathToName(__FILE__), __LINE__

#define __LOG_FORMAT(loglevel, color, format) color loglevel " " format _LOG_RESET_COLOR LOG_EOL
#define __LOG_TIME_FORMAT(loglevel, color, format) color "[%s]" loglevel " " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime()
#define __LOG_FILE_FORMAT(loglevel, color, format) color loglevel "[%s:%d] " format _LOG_RESET_COLOR LOG_EOL, _logger::filepathToName(__FILE__), __LINE__
#define __LOG_TIME_FILE_FORMAT(loglevel, color, format) color "[%s]" loglevel "[%s:%d] " format _LOG_RESET_COLOR LOG_EOL, _logger::formatTime(), _logger::filepathToName(__FILE__), __LINE__
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

// Print macros

#define LOG_BEGIN(baud) LOG_OUTPUT.begin(baud)
#define LOG_PRINT(msg) LOG_OUTPUT.print(msg)
#define LOG_PRINTLN(msg) LOG_OUTPUT.println(msg)

#if LOG_PRINT_TYPE == LOG_PRINT_TYPE_STD_FORMAT && __cplusplus >= 202002L
#include <format> // Use the C++20 format library
#define LOG_PRINTF(msg, ...) LOG_OUTPUT.print(std::format(msg, ##__VA_ARGS__).c_str())
#elif LOG_PRINT_TYPE == LOG_PRINT_TYPE_FMT_FORMAT
#define FMT_HEADER_ONLY // Use the header only because compiling the source with the Arduino build system is difficult. Remove src folder from the fmtlib to fix compile error.
#pragma push_macro("F") // Backup conflicting macros
#pragma push_macro("B1")
#undef B1 // Disable conflicting macros
#undef F
#include <fmt/core.h>  // Include the library
#pragma pop_macro("F") // Restore conflicting macros
#pragma pop_macro("B1")
#define LOG_PRINTF(msg, ...) LOG_OUTPUT.print(fmt::format(msg, ##__VA_ARGS__).c_str())
#else
#define LOG_PRINTF(msg, ...) _logger::printf(msg, ##__VA_ARGS__)
#endif

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