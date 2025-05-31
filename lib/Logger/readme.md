# Arduino Logger Library

A flexible and configurable logging library for Arduino/PlatformIO projects with support for multiple log levels, timestamps, colors, filtering, and modern formatting options.

## Features

- **Multiple Log Levels**: Error, Warning, Info, Debug, Verbose
- **Modern Formatting**: Supports std::format (C++20), fmtlib, and custom format libraries
- **Timestamp Support**: Microseconds, milliseconds, or formatted time (HH:MM:SS:MS)
- **Color Output**: ANSI color codes for different log levels
- **Tag Filtering**: Include/exclude specific tags from logging
- **File Information**: Optional filename and line number in logs
- **Configurable Buffer Size**: Adjustable static buffer for performance
- **Assertions**: Debug assertions with detailed error messages
- **Printf Fallback**: Traditional printf support for compatibility

## Quick Start

### Basic Usage

```cpp
#include <log.h>

void setup() {
    LOG_BEGIN(115200);
    
    LOG_I("System initialized");
    LOG_W("This is a warning");
    LOG_E("This is an error");
}

void loop() {
    LOG_D("Loop iteration: {}", millis());
    delay(1000);
}
```

### Tagged Logging

```cpp
LOG_INFO("SENSOR", "Temperature: %.2f°C", temperature);
LOG_WARN("WIFI", "Connection unstable, retrying...");
LOG_ERROR("SD_CARD", "Failed to write file: %s", filename);
```

## Configuration Options

Configure the logger by defining macros before including `logger.h`:

`log.h` provides a flexible configuration system to tailor the logging behavior to your needs. Below are the available configuration options:

### Log Levels

```cpp
#define LOG_LEVEL LOG_LEVEL_DEBUG
```

Available levels (in order of verbosity):
- `LOG_LEVEL_DISABLE` - No logging
- `LOG_LEVEL_ERROR` - Errors only
- `LOG_LEVEL_WARNING` - Warnings and errors
- `LOG_LEVEL_INFO` - Info, warnings, and errors
- `LOG_LEVEL_DEBUG` - Debug and above
- `LOG_LEVEL_VERBOSE` - All messages

### Timestamp Configuration

```cpp
#define LOG_TIME LOG_TIME_MILLIS
```

Options:
- `LOG_TIME_DISABLE` - No timestamps
- `LOG_TIME_MICROS` - Microseconds since boot
- `LOG_TIME_MILLIS` - Milliseconds since boot
- `LOG_TIME_HHMMSSMS` - Format: HH:MM:SS:MS (24-hour)
- `LOG_TIME_HHHHMMSSMS` - Format: HHHH:MM:SS:MS (unlimited hours)

### Level Text Format

```cpp
#define LOG_LEVEL_TEXT_FORMAT LOG_LEVEL_TEXT_FORMAT_SHORT
```

Options:
- `LOG_LEVEL_TEXT_FORMAT_LETTER` - Single letter: [E], [W], [I], [D], [V]
- `LOG_LEVEL_TEXT_FORMAT_SHORT` - Short: [EROR], [WARN], [INFO], [DBUG], [VERB]
- `LOG_LEVEL_TEXT_FORMAT_FULL` - Full: [ERROR], [WARNING], [INFO], [DEBUG], [VERBOSE]

### Colors

```cpp
#define LOG_COLOR LOG_COLOR_ENABLE
```

Enables ANSI color codes:
- Red for errors
- Yellow for warnings
- Green for info
- Cyan for debug
- Bright white for verbose

### File Information

```cpp
#define LOG_FILENAME LOG_FILENAME_ENABLE
```

Includes filename and line number in log output.

### Filtering

```cpp
#define LOG_FILTER LOG_FILTER_INCLUDE
#define LOG_FILTER_LIST {"SENSOR", "WIFI"}
```

Options:
- `LOG_FILTER_DISABLE` - No filtering
- `LOG_FILTER_EXCLUDE` - Exclude specified tags
- `LOG_FILTER_INCLUDE` - Include only specified tags

### Print Type (Formatting Engine)

The logger supports multiple formatting engines, with modern libraries recommended for better type safety, performance, and features:

```cpp
#define LOG_PRINT_TYPE LOG_PRINT_TYPE_FMT_FORMAT
```

**Recommended Options (in order of preference):**
- `LOG_PRINT_TYPE_FMT_FORMAT` - **Recommended**: fmtlib formatting with excellent performance and features
- `LOG_PRINT_TYPE_STD_FORMAT` - C++20 std::format (requires C++20 compiler support)
- `LOG_PRINT_TYPE_CUSTOM_FORMAT` - Custom lightweight format library

**Fallback Option:**
- `LOG_PRINT_TYPE_PRINTF` - Traditional printf formatting (compatibility fallback)

**Why modern formatting is preferred:**
- **Type Safety**: Compile-time format string validation
- **Performance**: Often faster than printf with better memory management
- **Features**: Support for custom types, better error handling
- **Modern C++**: Leverages modern C++ features and standards

### Buffer Size

```cpp
#define LOG_STATIC_BUFFER_SIZE 128
```

Sets the static buffer size for log messages.

### Output Stream

```cpp
#define LOG_OUTPUT Serial
```

Specify the output stream (Serial, Serial1, etc.).

## Complete Configuration Example

```cpp
#define LOG_LEVEL LOG_LEVEL_DEBUG
#define LOG_TIME LOG_TIME_MILLIS
#define LOG_COLOR LOG_COLOR_ENABLE
#define LOG_FILENAME LOG_FILENAME_ENABLE
#define LOG_FILTER LOG_FILTER_INCLUDE
#define LOG_FILTER_LIST {"MAIN", "SENSOR"}
#define LOG_STATIC_BUFFER_SIZE 128
#define LOG_PRINT_TYPE LOG_PRINT_TYPE_FMT_FORMAT  // Use modern formatting

#include <logger.h>
```

## Logging Macros

### Without Tags
- `LOG_E(message, ...)` - Error level
- `LOG_W(message, ...)` - Warning level
- `LOG_I(message, ...)` - Info level
- `LOG_D(message, ...)` - Debug level
- `LOG_V(message, ...)` - Verbose level

### With Tags
- `LOG_ERROR(tag, message, ...)`
- `LOG_WARN(tag, message, ...)`
- `LOG_INFO(tag, message, ...)`
- `LOG_DEBUG(tag, message, ...)`
- `LOG_VERB(tag, message, ...)`

### Utility Macros
- `LOG_BEGIN(baud)` - Initialize serial communication
- `LOG_PRINT(msg)` - Print without newline
- `LOG_PRINTLN(msg)` - Print with newline
- `ASSERT(condition, msg)` - Debug assertion (disabled in release builds)

## Output Examples

With timestamps and filename:

```
[00:12:34:567][INFO][MAIN] System started
[00:12:34:568][WARN][WIFI] Connecting to network...
[00:12:35:123][EROR][SD][main.cpp:42] Failed to mount SD card
```

## Performance Considerations

- **Modern formatting libraries** (fmtlib, std::format) typically offer better performance than printf
- Use static buffers to avoid dynamic allocation
- Higher log levels include all lower levels
- Filtering is evaluated at compile time when possible
- Colors add minimal overhead

## Dependencies

**Modern Formatting (Recommended):**
- **fmtlib**: For `LOG_PRINT_TYPE_FMT_FORMAT` - Fast, type-safe formatting
- **C++20**: For `LOG_PRINT_TYPE_STD_FORMAT` - Standard library formatting
- **Custom Format Library**: For `LOG_PRINT_TYPE_CUSTOM_FORMAT` - Lightweight alternative

**Fallback:**
- **Standard**: Arduino core libraries for `LOG_PRINT_TYPE_PRINTF` (compatibility mode)

## Migration from Printf

If migrating from printf-style logging, modern format libraries use `{}` placeholders instead of `%` specifiers:

```cpp
// Old printf style (still supported as fallback)
LOG_I("Temperature: %.2f°C, Humidity: %d%%", temp, humidity);

// Modern format style (recommended)
LOG_I("Temperature: {:.2f}°C, Humidity: {}%", temp, humidity);
```

## Format Library Options

### **fmt::format**: Highly recommended for performance and features

Install via PlatformIO:

```bash
lib_deps = https://github.com/RileyCornelius/fmt-arduino.git
```

### **std::format**: C++20 standard library formatting (requires C++20 support)

The following platforms support C++20:
- ESP32 with 3.1.0 and later
- STM32
- Rapsberry Pi Pico

Ensure your `platformio.ini` is configured for C++20:
```bash
build_unflags = -std=gnu++11 -std=gnu++14 -std=gnu++17 
build_flags = -std=gnu++20
```

PlatformIO by default uses ESP32 2.0.17 which does not support C++20. You can upgrade to the latest version using this configuration in your `platformio.ini`:
```bash
[env:stable]
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
```
### **afmt::format**: Custom lightweight format lib

Similar to fmtlib but with a smaller footprint, suitable for resource-constrained environments but with fewer features.

Copy Format from `lib/Format/` into your project.

