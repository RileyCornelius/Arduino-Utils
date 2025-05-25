# Arduino Format Library (afmt)

A lightweight, Arduino-friendly implementation of C++ formatting functionality inspired by fmtlib. This library provides type-safe string formatting with minimal memory footprint and runtime overhead, specifically designed for embedded systems.

## Features

### Core Capabilities
- **Zero-copy string views** - Efficient string handling without allocation
- **Small Buffer Optimization (SBO)** - Minimizes heap allocations for small strings
- **Arduino String integration** - Native support for Arduino String class
- **C++11 compatible** - Works with older compilers common in embedded development
- **Minimal STL dependency** - Reduced binary size and memory usage

### Buffer Types
- **`adaptive_buffer`** - Dynamic buffer with small buffer optimization (default)
- **`external_buffer`** - Uses pre-allocated char arrays for zero-allocation formatting
- **`string_view`** - Non-owning string reference for efficient string handling

## Configuration

The library can be configured using preprocessor macros:

### Buffer Configuration
```cpp
// Default small buffer optimization size (default: 64)
#define AFMT_DEFAULT_ADAPTIVE_SBO_SIZE 128

// Arduino Serial output configuration (default: Serial)
#define AFMT_SERIAL_OUTPUT Serial1
```

### Compiler Features
```cpp
// Force constexpr usage (auto-detected by default)
#define AFMT_USE_CONSTEXPR 1

// Custom namespace configuration
#define AFMT_BEGIN_NAMESPACE namespace my_fmt {
#define AFMT_END_NAMESPACE }
```

## Basic Usage

```cpp
#include <format.h>

// Basic formatting
afmt::adaptive_buffer buf;
afmt::format_to(buf, "Hello, {}!", "world");
Serial.println(buf.data()); // "Hello, world!"

// Arduino String formatting
String result = afmt::aformat("Value: {}", 42);
Serial.println(result); // "Value: 42"

// Direct Serial output
afmt::println("Temperature: {:.1f}°C", 23.7); // "Temperature: 23.7°C"

// Fixed buffer formatting (no allocation)
char buffer[50];
auto result = afmt::format_to(buffer, "Count: {}", 123);
if (!result.truncated) {
    Serial.println(buffer); // "Count: 123"
}
```

## Format Specification Syntax

The format specification follows the pattern: `{[arg_id][:format_spec]}`

### Format Specifier Structure
```
{[argument_id][:[[fill]align][sign][#][0][width][.precision][type]]}
```

### Alignment Options
- `<` - Left align (default for strings)
- `>` - Right align (default for numbers)
- `^` - Center align
- `=` - Numeric alignment (sign-aware padding)

### Sign Options
- `+` - Always show sign for numbers
- `-` - Show sign only for negative numbers (default)
- ` ` (space) - Use space for positive numbers, minus for negative

### Other Options
- `#` - Alternate form (adds prefixes like `0x` for hex)
- `0` - Zero padding (equivalent to fill='0' with numeric alignment)
- `width` - Minimum field width
- `.precision` - Precision for floating-point or max chars for strings

## Type Specifiers

### Integer Types (`int`, `unsigned`, `long`, etc.)
- `d` - Decimal (default)
- `x` - Lowercase hexadecimal
- `X` - Uppercase hexadecimal  
- `o` - Octal
- `b` - Lowercase binary
- `B` - Uppercase binary

```cpp
afmt::println("Hex: {:#x}", 255);        // "Hex: 0xff"
afmt::println("Binary: {:08b}", 42);     // "Binary: 00101010"
afmt::println("Padded: {:05d}", 123);    // "Padded: 00123"
```

### Floating-Point Types (`float`, `double`)
- `f`/`F` - Fixed-point notation (default)
- `e`/`E` - Scientific notation
- `g`/`G` - General format (chooses shorter of `f` or `e`)

```cpp
afmt::println("Pi: {:.3f}", 3.14159);        // "Pi: 3.142"
afmt::println("Scientific: {:.2e}", 1234.5); // "Scientific: 1.23e+03"
afmt::println("Scientific: {:.2E}", 1234.5); // "Scientific: 1.23E+03"
afmt::println("General: {:.3g}", 0.0001);    // "General: 0.0001"
afmt::println("General: {:.3g}", 123456);    // "General: 1.23e+05"
afmt::println("General: {:.3G}", 123456);    // "General: 1.23E+05"
```

### String Types (`const char*`, `string_view`)
- `s` - String format (default)

```cpp
afmt::println("Name: {:>10s}", "Alice");  // "Name:      Alice"
afmt::println("Trim: {:.5s}", "Hello World"); // "Trim: Hello"
```

### Character Type (`char`)
- `c` - Character format (default)
- `d` - Decimal value of character

```cpp
afmt::println("Char: {:c}", 65);         // "Char: A"
afmt::println("ASCII: {:d}", 'A');       // "ASCII: 65"
```

### Boolean Type (`bool`)
- Default: `true`/`false`
- `d`: `1`/`0`

```cpp
afmt::println("Bool: {}", true);         // "Bool: true"
afmt::println("Numeric: {:d}", false);   // "Numeric: 0"
```

### Pointer Types
- `p` - Pointer format (default: hexadecimal with `0x` prefix)

```cpp
int value = 42;
afmt::println("Pointer: {:p}", &value);  // "Pointer: 0x7fff5fbff6ac"
```

## Formatting Examples

### Alignment and Padding
```cpp
afmt::println("Left:   '{:<10}'", "text");    // "Left:   'text      '"
afmt::println("Right:  '{:>10}'", "text");    // "Right:  '      text'"
afmt::println("Center: '{:^10}'", "text");    // "Center: '   text   '"
afmt::println("Fill:   '{:*^10}'", "text");   // "Fill:   '***text***'"
```

### Numeric Formatting
```cpp
afmt::println("Decimal:     {:d}", 42);       // "Decimal:     42"
afmt::println("Hex:         {:x}", 42);       // "Hex:         2a"
afmt::println("Hex upper:   {:X}", 42);       // "Hex upper:   2A"
afmt::println("Hex prefix:  {:#x}", 42);      // "Hex prefix:  0x2a"
afmt::println("Binary:      {:b}", 42);       // "Binary:      101010"
afmt::println("Octal:       {:o}", 42);       // "Octal:       52"
afmt::println("Zero pad:    {:05d}", 42);     // "Zero pad:    00042"
afmt::println("Sign space:  {: d}", 42);      // "Sign space:   42"
afmt::println("Sign plus:   {:+d}", 42);      // "Sign plus:   +42"
```

### Floating-Point Formatting
```cpp
double pi = 3.14159265359;
afmt::println("Default:     {}", pi);         // "Default:     3.141593"
afmt::println("Precision:   {:.2f}", pi);     // "Precision:   3.14"
afmt::println("Scientific:  {:.3e}", pi);     // "Scientific:  3.142e+00"
afmt::println("Scientific:  {:.3E}", pi);     // "Scientific:  3.142E+00"
afmt::println("General:     {:.4g}", pi);     // "General:     3.142"
afmt::println("General:     {:.4G}", pi);     // "General:     3.142"
afmt::println("Zero pad:    {:08.2f}", pi);   // "Zero pad:    00003.14"
afmt::println("Plus sign:   {:+.2f}", pi);    // "Plus sign:   +3.14"

// Large numbers with general format
afmt::println("Large g:     {:.3g}", 123456.0);  // "Large g:     1.23e+05"
afmt::println("Small g:     {:.3g}", 0.00123);   // "Small g:     0.00123"
```

## Performance Considerations

### Memory Efficiency
- Use `external_buffer` for zero-allocation formatting
- `adaptive_buffer` uses SBO for strings up to 64 characters (configurable)
- `string_view` provides zero-copy string handling

### Binary Size
- Minimal template instantiation compared to full fmtlib
- No locale support or advanced features
- Optimized for embedded systems

### Example: Memory-Conscious Usage
```cpp
// Zero allocation approach
char buffer[100];
auto result = afmt::format_to_n(buffer, sizeof(buffer), 
                                "Sensor {}: {:.2f}", id, value);

// Check for truncation
if (result.truncated) {
    Serial.println("Warning: output truncated");
} else {
    Serial.println(buffer);
}

// For repeated formatting, reuse buffers
static char reusable_buffer[50];
afmt::format_to(reusable_buffer, "Count: {}", counter++);
display.showText(reusable_buffer);
```

## API Reference

### Core Functions
```cpp
// Format to various buffer types
template<typename... Args>
void format_to(adaptive_buffer& buf, string_view fmt, const Args&... args);

template<size_t N, typename... Args>
format_to_result format_to(char (&out)[N], string_view fmt, const Args&... args);

template<typename... Args>
format_to_result format_to_n(char* out, size_t n, string_view fmt, const Args&... args);

// Get formatted size without formatting
template<typename... Args>
size_t formatted_size(string_view fmt, const Args&... args);

// Standard library integration
template<typename... Args>
std::string format(string_view fmt, const Args&... args);
```

### Arduino Integration
```cpp
// Arduino String formatting
template<typename... Args>
String aformat(string_view fmt, const Args&... args);

// Direct Serial output (uses AFMT_SERIAL_OUTPUT)
template<typename... Args>
void print(string_view fmt, const Args&... args);

template<typename... Args>
void println(string_view fmt, const Args&... args);
```

## Arduino-Specific Features

### Serial Output Configuration
The library uses `AFMT_SERIAL_OUTPUT` for direct serial output functions:

```cpp
// Default configuration (uses Serial)
afmt::print("Temperature: {:.1f}°C", temp);

// Custom Serial port configuration
#define AFMT_SERIAL_OUTPUT Serial1
#include <format.h>
// Now afmt::print() uses Serial1

// Alternative approach - specify Serial port explicitly
Serial2.print(afmt::aformat("Data: {}", value));
```

### Arduino String Integration
```cpp
// Arduino String formatting
String greeting = afmt::aformat("Hello, {}!", "Arduino");
Serial.println(greeting);

// Memory-efficient formatting with external buffer
char display_buffer[32];
auto result = afmt::format_to(display_buffer, "T:{:.1f}°C H:{}%", 
                              temperature, humidity);
if (!result.truncated) {
    lcd.print(display_buffer);
}

// Sensor data logging
String log_entry = afmt::aformat("[{}] Sensor {}: {:.2f}", 
                                 millis(), sensor_id, reading);
SD.write(log_entry.c_str());
```

## Error Handling

The library uses lightweight error indicators:
- `format_to_result.truncated` indicates if output was truncated
- Invalid format specifiers are handled gracefully
- No exceptions are thrown (embedded-friendly)

## Limitations

### Intentionally Omitted Features
- Localization support
- Named arguments
- Compile-time format string validation
- Custom formatters for user types
- Wide character support
- Date/time formatting

### Current Limitations
- Hexadecimal floating-point format (`a`/`A`) not implemented
- Limited precision handling for very large numbers
- No custom alignment characters beyond basic set

## Integration

Add to your `platformio.ini`:
```ini
lib_deps = 
    https://github.com/your-repo/Arduino-Utils.git
```

Include in your sketch:
```cpp
#include <format.h>
using namespace afmt; // Optional: for shorter syntax
```

## Compatibility

- **Arduino**: All boards supported by Arduino framework
- **Platform**: ESP32, ESP8266, AVR, STM32, etc.
- **Compiler**: GCC 4.8+, Clang 3.4+
- **C++ Standard**: C++11 or later
