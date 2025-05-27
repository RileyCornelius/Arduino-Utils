# Arduino Format Library (afmt)

A lightweight, Arduino-friendly implementation of C++ formatting functionality inspired by fmtlib. This library provides type-safe string formatting with minimal memory footprint and runtime overhead, specifically designed for embedded systems.

## Features

### Core Capabilities
- **Zero-copy string views** - Efficient string handling without allocation
- **Small Buffer Optimization (SBO)** - Minimizes heap allocations for small strings
- **Arduino String integration** - Native support for Arduino String class
- **C++11 compatible** - Works with older compilers common in embedded development
- **Minimal STL dependency** - Reduced binary size and memory usage

### Unified Buffer System
The library uses a unified `buffer` class that operates in three modes:

1. **Adaptive Mode (SBO)** - Uses small buffer optimization for strings up to 64 characters (configurable), automatically grows on the heap when needed.
2. **Adaptive Mode (Heap)** - Automatically transitions to heap allocation for larger strings.
3. **External Mode** - Uses pre-allocated memory only, no dynamic allocation, and reports truncation if content doesn't fit.

- **`string_view`** - Non-owning string reference for efficient string handling

## Configuration

The library can be configured using preprocessor macros:

### Buffer Configuration
```cpp
// Default small buffer optimization size (default: 64)
#define AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE 128

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

// Basic formatting with adaptive buffer (default mode)
afmt::buffer buf;
afmt::format_to(buf, "Hello, {}!", "world");
Serial.println(buf.data()); // "Hello, world!"

// Arduino String formatting
String result = afmt::aformat("Value: {}", 42);
Serial.println(result); // "Value: 42"

// Direct Serial output
afmt::println("Temperature: {:.1f}°C", 23.7); // "Temperature: 23.7°C"

// Fixed buffer formatting (external mode - no allocation)
char buffer[50];
auto result = afmt::format_to(buffer, "Count: {}", 123);
if (!result.truncated) {
    Serial.println(buffer); // "Count: 123"
}

// Manual external buffer mode
char external_mem[100];
afmt::buffer ext_buf(external_mem, sizeof(external_mem));
afmt::format_to(ext_buf, "Sensor {}: {:.2f}", id, value);
if (!ext_buf.is_truncated()) {
    Serial.println(ext_buf.data());
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
- **Note**: Numeric alignment (`=`) is implemented as `align::numeric` internally but uses `0` padding syntax

### Sign Options
- `+` - Always show sign for numbers
- `-` - Show sign only for negative numbers (default)
- ` ` (space) - Use space for positive numbers, minus for negative

### Other Options
- `#` - Alternate form (adds prefixes like `0x` for hex, `0b` for binary, `0` for octal)
- `0` - Zero padding (equivalent to fill='0' with numeric alignment)
- `width` - Minimum field width
- `.precision` - Precision for floating-point numbers

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
- `f`/`F` - Fixed-point notation (default, precision defaults to 2)
- `e`/`E` - Scientific notation

```cpp
afmt::println("Pi: {:.3f}", 3.14159);        // "Pi: 3.142"
afmt::println("Default: {}", 3.14159);       // "Default: 3.14" (precision 2)
afmt::println("Scientific: {:.2e}", 1234.5); // "Scientific: 1.23e+03"
afmt::println("Scientific: {:.2E}", 1234.5); // "Scientific: 1.23E+03"
```

### String Types (`const char*`, `string_view`)
- `s` - String format (default)

```cpp
afmt::println("Name: {:>10}", "Alice");       // "Name:      Alice"
afmt::println("Trim: {:.5}", "Hello World"); // **Note**: Precision for strings not implemented
```

### Character Type (`char`)
- Default: Character output
- `d` - Decimal value of character

```cpp
afmt::println("Char: {}", 'A');             // "Char: A"
afmt::println("ASCII: {:d}", 'A');          // **Note**: Not implemented - outputs 'A'
```

### Boolean Type (`bool`)
- Default: `true`/`false`
- `d`: `1`/`0`

```cpp
afmt::println("Bool: {}", true);         // "Bool: true"
afmt::println("Numeric: {:d}", false);   // "Numeric: 0"
```

### Pointer Types
- Default: Pointer format (hexadecimal with `0x` prefix)

```cpp
int value = 42;
afmt::println("Pointer: {}", &value);     // "Pointer: 0x7fff5fbff6ac"
afmt::println("Null: {}", nullptr);      // "Null: nullptr"
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
afmt::println("Bin prefix:  {:#b}", 42);      // "Bin prefix:  0b101010"
afmt::println("Octal:       {:o}", 42);       // "Octal:       52"
afmt::println("Oct prefix:  {:#o}", 42);      // "Oct prefix:  052"
afmt::println("Zero pad:    {:05d}", 42);     // "Zero pad:    00042"
afmt::println("Sign space:  {: d}", 42);      // "Sign space:   42"
afmt::println("Sign plus:   {:+d}", 42);      // "Sign plus:   +42"
```

### Floating-Point Formatting
```cpp
double pi = 3.14159265359;
afmt::println("Default:     {}", pi);         // "Default:     3.14"
afmt::println("Precision:   {:.2f}", pi);     // "Precision:   3.14"
afmt::println("High Prec:   {:.5f}", pi);     // "High Prec:   3.14159"
afmt::println("Scientific:  {:.3e}", pi);     // "Scientific:  3.142e+00"
afmt::println("Scientific:  {:.3E}", pi);     // "Scientific:  3.142E+00"
afmt::println("Zero pad:    {:08.2f}", pi);   // "Zero pad:    00003.14"
afmt::println("Plus sign:   {:+.2f}", pi);    // "Plus sign:   +3.14"

// Large numbers with scientific notation
afmt::println("Large:       {:.1e}", 1.0e20);    // "Large:       1.0e+20"
afmt::println("Small:       {:.1e}", 1.0e-20);   // "Small:       1.0e-20"

// Special values
afmt::println("Infinity:    {:.2e}", 1.0/0.0);   // "Infinity:    inf"
afmt::println("NaN:         {:.2e}", 0.0/0.0);   // "NaN:         nan"
```

## Buffer Modes

### Adaptive Mode (Default)
When created with the default constructor, `buffer` operates in adaptive mode:
- Uses small buffer optimization (SBO) for strings up to 64 characters (configurable)
- Automatically grows on the heap when needed
- Manages memory automatically

```cpp
afmt::buffer buf;  // Adaptive mode (starts with SBO)
afmt::format_to(buf, "This will use SBO or heap as needed: {}", value);
Serial.println(buf.is_sbo() ? "Using SBO" : "Using heap");
```

### External Mode
When created with a char array, `buffer` operates in external mode:
- Uses pre-allocated memory only
- No dynamic allocation
- Reports truncation if content doesn't fit

```cpp
char memory[100];
afmt::buffer buf(memory, sizeof(memory));  // External mode
afmt::format_to(buf, "Fixed memory: {}", value);
if (buf.is_truncated()) {
    Serial.println("Warning: output was truncated");
}
```

### Buffer State Checking
```cpp
afmt::buffer buf;
// ... format operations ...

if (buf.is_sbo()) {
    Serial.println("Using small buffer optimization");
} else if (buf.is_heap()) {
    Serial.println("Using heap allocation");
} else if (buf.is_external()) {
    Serial.println("Using external memory");
}
```

## Performance Considerations

### Memory Efficiency
- Adaptive mode uses SBO for strings up to 64 characters (configurable)
- External mode provides zero-allocation formatting
- `string_view` provides zero-copy string handling

### Binary Size
- Unified buffer reduces template instantiation compared to separate buffer types
- Single code path for both buffer modes
- Minimal template instantiation compared to full fmtlib

### Example: Memory-Conscious Usage
```cpp
// Zero allocation approach using external mode
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
afmt::buffer reusable(reusable_buffer, sizeof(reusable_buffer));
reusable.clear();  // Reset for reuse
afmt::format_to(reusable, "Count: {}", counter++);
display.showText(reusable.data());
```

## API Reference

### Core Functions
```cpp
// Format to unified buffer
template<typename... Args>
void format_to(buffer& buf, string_view fmt, const Args&... args);

// Format to fixed-size array (uses external mode internally)
template<size_t N, typename... Args>
format_to_result format_to(char (&out)[N], string_view fmt, const Args&... args);

// Format to char* with size limit (uses external mode internally)
template<typename... Args>
format_to_result format_to_n(char* out, size_t n, string_view fmt, const Args&... args);

// Get formatted size without formatting
template<typename... Args>
size_t formatted_size(string_view fmt, const Args&... args);

// Standard library integration (uses adaptive mode internally)
template<typename... Args>
std::string format(string_view fmt, const Args&... args);
```

### Buffer Class
```cpp
class buffer {
public:
    // Constructors
    buffer();                              // Adaptive mode (starts with SBO)
    buffer(char* data, size_t capacity);   // External mode
    template<size_t N>
    buffer(char (&arr)[N]);               // External mode with array
    
    // Accessors
    char* data();
    const char* data() const;
    size_t size() const;
    size_t capacity() const;
    bool is_truncated() const;
    
    // Mode checking
    bool is_sbo() const;      // Small buffer optimization
    bool is_heap() const;     // Heap allocated
    bool is_external() const; // External memory
    
    // Operations
    void clear();
    void push_back(const char& value);
    void append(const char* begin, const char* end);
    
    // Only works in adaptive mode
    void reserve(size_t new_capacity);
};

struct format_to_result {
    char* out;
    bool truncated;
    
    operator char*() const; // Implicit conversion to char*
};
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
- `buffer.is_truncated()` indicates if buffer operations were truncated
- Invalid format specifiers are handled gracefully (often output as-is)
- No exceptions are thrown (embedded-friendly)

## Limitations

### Intentionally Omitted Features
- Localization support
- Named arguments
- Compile-time format string validation
- Custom formatters for user types
- Wide character support
- Date/time formatting

### Current Implementation Limitations
- General format specifier (`g`/`G`) not implemented for floating-point
- Hexadecimal floating-point format (`a`/`A`) not implemented
- Character type specifier (`c`) for non-char types not implemented
- Precision for string truncation not implemented
- Limited precision handling for very large numbers (beyond double limits)
- Debug specifier (`?`) not implemented

### Type Support
The library supports the following argument types:
- Integer types: `int`, `unsigned`, `long`, `unsigned long`, `long long`, `unsigned long long`
- Floating-point: `float`, `double`
- Character: `char`
- Boolean: `bool`
- String: `const char*`, `string_view`
- Pointer: Any pointer type (formatted as hexadecimal)

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

## Testing

The library includes comprehensive unit tests covering:
- Basic string formatting
- Integer formatting (decimal, hex, octal, binary)
- Floating-point formatting (fixed, scientific)
- Alignment and padding
- Zero padding with signs and prefixes
- Scientific notation edge cases (infinity, NaN, large/small numbers)
- Buffer truncation handling
- Arduino String integration
- Performance benchmarking

Run tests with: `pio test -e your_target_environment`
