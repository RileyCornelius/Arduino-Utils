#pragma once

/**--------------------------------------------------------------------------------------
 * Settings
 *-------------------------------------------------------------------------------------*/

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
#define LOG_USE_TIME 1
#endif

// Log Level Tag
#ifndef LOG_USE_TAG_SHORT
#define LOG_USE_TAG_SHORT 1
#endif

// Log colors
#ifndef LOG_USE_COLORS
#define LOG_USE_COLORS 0
#endif

// Log output
#ifndef LOG_OUTPUT
#define LOG_OUTPUT Serial
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
#define LOG_VERBOSE(tag, message, ...) _itagformat(LOG_LEVEL_VERBOSE, tag, message, ##__VA_ARGS__)
#else
#define LOG_VERBOSE(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(tag, message, ...) _itagformat(LOG_LEVEL_DEBUG, tag, message, ##__VA_ARGS__)
#else
#define LOG_DEBUG(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(tag, message, ...) _itagformat(LOG_LEVEL_INFO, tag, message, ##__VA_ARGS__)
#else
#define LOG_INFO(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_WARNING(tag, message, ...) _itagformat(LOG_LEVEL_WARNING, tag, message, ##__VA_ARGS__)
#else
#define LOG_WARNING(tag, message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(tag, message, ...) _itagformat(LOG_LEVEL_ERROR, tag, message, ##__VA_ARGS__)
#else
#define LOG_ERROR(tag, message, ...)
#endif

// Without tag

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOG_V(message, ...) _iformat(LOG_LEVEL_VERBOSE, message, ##__VA_ARGS__)
#else
#define LOG_V(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_D(message, ...) _iformat(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
#define LOG_D(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_I(message, ...) _iformat(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
#define LOG_I(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_W(message, ...) _iformat(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#else
#define LOG_W(message, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_E(message, ...) _iformat(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#else
#define LOG_E(message, ...)
#endif

// Prototypes functions
void _itagformat(uint8_t level, const char *tag, const char *format, ...);
void _itagformat(uint8_t level, const char *tag, String format, ...);
void _iformat(uint8_t level, const char *format, ...);
void _iformat(uint8_t level, String format, ...);