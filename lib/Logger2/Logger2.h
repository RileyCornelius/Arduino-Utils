#pragma once

/**--------------------------------------------------------------------------------------
 * Settings
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
// #define LOG_FILTER_INCLUDE 1
// #define LOG_FILTER_EXCLUDE 0

// #ifndef LOG_FILTER
// #define LOG_FILTER LOG_FILTER_EXCLUDE
// #endif

// #ifndef LOG_FILTER_LIST
// #define LOG_FILTER_LIST ""
// #endif

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

/**--------------------------------------------------------------------------------------
 * Logger Defines
 *-------------------------------------------------------------------------------------*/

// LOG levels order/priority. IF LOG_LEVEL_NONE is set, then nothing will be logged
#define LOG_LEVEL_NONE -1
#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_VERBOSE 4
// Log format defines
#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_VERBOSE(tag, message, ...) _itagformat(LOG_LEVEL_VERBOSE, ARDUHAL_LOG_COLOR_V, tag, message, ##__VA_ARGS__)
#else
#define LOG_VERBOSE(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(tag, message, ...) _itagformat(LOG_LEVEL_DEBUG, ARDUHAL_LOG_COLOR_D, tag, message, ##__VA_ARGS__)
#else
#define LOG_DEBUG(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(tag, message, ...) _itagformat(LOG_LEVEL_INFO, ARDUHAL_LOG_COLOR_I, tag, message, ##__VA_ARGS__)
#else
#define LOG_INFO(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_WARNING(tag, message, ...) _itagformat(LOG_LEVEL_WARNING, ARDUHAL_LOG_COLOR_W, tag, message, ##__VA_ARGS__)
#else
#define LOG_WARNING(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(tag, message, ...) _itagformat(LOG_LEVEL_ERROR, ARDUHAL_LOG_COLOR_E, tag, message, ##__VA_ARGS__)
#else
#define LOG_ERROR(tag, message, ...)
#endif

// Without tag

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_V(message, ...) _iformat(LOG_LEVEL_VERBOSE, ARDUHAL_LOG_COLOR_V, message, ##__VA_ARGS__)
#else
#define LOG_V(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_D(message, ...) _iformat(LOG_LEVEL_DEBUG, ARDUHAL_LOG_COLOR_D, message, ##__VA_ARGS__)
#else
#define LOG_D(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_I(message, ...) _iformat(LOG_LEVEL_INFO, ARDUHAL_LOG_COLOR_I, message, ##__VA_ARGS__)
#else
#define LOG_I(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_W(message, ...) _iformat(LOG_LEVEL_WARNING, ARDUHAL_LOG_COLOR_W, message, ##__VA_ARGS__)
#else
#define LOG_W(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_E(message, ...) _iformat(LOG_LEVEL_ERROR, ARDUHAL_LOG_COLOR_E, message, ##__VA_ARGS__)
#else
#define LOG_E(message, ...)
#endif

// Prototypes functions
void _itagformat(uint8_t level, const char *color, const char *tag, const char *format, ...);
void _itagformat(uint8_t level, const char *color, const char *tag, String format, ...);
void _iformat(uint8_t level, const char *color, const char *format, ...);
void _iformat(uint8_t level, const char *color, String format, ...);
