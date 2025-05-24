//
// Arduino-friendly fmtlib-lite Implementation
//
// A lightweight implementation of the C++ formatting library (fmtlib) suitable for embedded system
//

#ifndef ARDUINO_FMT_H_
#define ARDUINO_FMT_H_

// Define C++11/14/17 compatibility macros
#define AFMT_HAS_CXX_11 (__cplusplus >= 201103L)
#define AFMT_HAS_CXX_14 (__cplusplus >= 201402L)
#define AFMT_HAS_CXX_17 (__cplusplus >= 201703L)
#define AFMT_HAS_ARDUINO (defined(ARDUINO) && ARDUINO >= 100)

#include <stddef.h>
#include <stdint.h>
#include <type_traits> // For std::is_integral, std::is_floating_point, std::is_same

// Include Arduino.h for String class
#if AFMT_HAS_ARDUINO
#include <Arduino.h>
#endif

#include <string>

// Define macros for compiler detection
#if defined(__GNUC__)
#define AFMT_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#else
#define AFMT_GCC_VERSION 0
#endif

// Detect C++14 relaxed constexpr.
#ifdef AFMT_USE_CONSTEXPR
#elif AFMT_GCC_VERSION >= 702 && AFMT_HAS_CXX_14
#define AFMT_USE_CONSTEXPR 1
#else
#define AFMT_USE_CONSTEXPR 0
#endif
#if AFMT_USE_CONSTEXPR
#define AFMT_CONSTEXPR constexpr
#else
#define AFMT_CONSTEXPR
#endif

// Define namespace macros
#ifndef AFMT_BEGIN_NAMESPACE
#define AFMT_BEGIN_NAMESPACE \
    namespace afmt           \
    {
#define AFMT_END_NAMESPACE }
#endif

AFMT_BEGIN_NAMESPACE

// Helper struct for variant-like visitation
struct monostate
{
    AFMT_CONSTEXPR monostate() {}
};

// =============== Type Definitions ===============

// Helper function for AFMT_CONSTEXPR string length calculation
AFMT_CONSTEXPR size_t string_length(const char *s)
{
    return s == nullptr ? 0 : *s == '\0' ? 0
                                         : 1 + string_length(s + 1);
}

// Type identification for formatting
enum class type
{
    none_type,
    // Integer types
    int_type,
    uint_type,
    long_long_type,
    ulong_long_type,
    bool_type,
    char_type,
    last_integer_type = char_type,
    // Floating-point types
    float_type,
    double_type,
    last_numeric_type = double_type,
    cstring_type,
    string_type,
    pointer_type
};

// Format specifier types
enum class presentation_type : unsigned char
{
    // Common specifiers
    none = 0,
    debug = 1,  // '?'
    string = 2, // 's' (string, bool)

    // Integral, bool and character specifiers
    dec = 3, // 'd'
    hex,     // 'x' or 'X'
    oct,     // 'o'
    bin,     // 'b' or 'B'
    chr,     // 'c'

    // String and pointer specifiers:
    pointer = 3, // 'p'

    // Floating-point specifiers
    exp = 1, // 'e' or 'E'
    fixed,   // 'f' or 'F'
    general, // 'g' or 'G'
    hexfloat // 'a' or 'A'
};

// Format specification options
enum class align
{
    none,
    left,
    right,
    center,
    numeric
};
enum class sign
{
    none,
    minus,
    plus,
    space
};

// =============== Core Container Classes ===============

// string_view - non-owning string reference
class string_view
{
private:
    const char *data_;
    size_t size_;

public:
    // Constructors
    AFMT_CONSTEXPR string_view() : data_(nullptr), size_(0) {}
    AFMT_CONSTEXPR string_view(const char *s, size_t count) : data_(s), size_(count) {}
    AFMT_CONSTEXPR string_view(const char *s) : data_(s), size_(string_length(s)) {}
    template <size_t N>
    AFMT_CONSTEXPR string_view(const char (&s)[N]) : data_(s), size_(N - 1) {}

    // Basic accessors
    AFMT_CONSTEXPR const char *data() const { return data_; }
    AFMT_CONSTEXPR size_t size() const { return size_; }
    AFMT_CONSTEXPR bool empty() const { return size_ == 0; }

    // Substring extraction - fixed for C++11 compatibility with pure expression
    AFMT_CONSTEXPR string_view substr(size_t pos, size_t count = ~static_cast<size_t>(0)) const
    {
        return string_view(
            data_ + (pos > size_ ? size_ : pos),
            (count > size_ - (pos > size_ ? size_ : pos) ? size_ - (pos > size_ ? size_ : pos) : count));
    }

    // Comparison operators - fixed for C++11 compatibility with pure expression
    AFMT_CONSTEXPR bool operator==(const string_view &other) const
    {
        return size_ == other.size_ ? equal_strings(data_, other.data_, size_) : false;
    }

    // Helper for equality comparison
    static AFMT_CONSTEXPR bool equal_strings(const char *s1, const char *s2, size_t n)
    {
        return n == 0 ? true : *s1 != *s2 ? false
                                          : equal_strings(s1 + 1, s2 + 1, n - 1);
    }

    AFMT_CONSTEXPR bool operator!=(const string_view &other) const
    {
        return !(*this == other);
    }

    // Element access
    AFMT_CONSTEXPR char operator[](size_t pos) const
    {
        return data_[pos];
    }
};

// buffer - dynamic memory buffer
template <typename T>
class buffer
{
private:
    T *data_;
    size_t size_;
    size_t capacity_;

public:
    buffer() : data_(nullptr), size_(0), capacity_(0) {}
    buffer(size_t initial_capacity) : size_(0), capacity_(initial_capacity)
    {
        data_ = initial_capacity > 0 ? new T[initial_capacity] : nullptr;
    }

    ~buffer()
    {
        delete[] data_;
    }

    // Disable copy
    buffer(const buffer &) = delete;
    buffer &operator=(const buffer &) = delete;

    // Move
    buffer(buffer &&other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = other.capacity_ = 0;
    }

    buffer &operator=(buffer &&other) noexcept
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    // Accessors
    T *data() { return data_; }
    const T *data() const { return data_; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

    // Operations
    void clear() { size_ = 0; }
    void resize(size_t new_size)
    {
        if (new_size > capacity_)
        {
            reserve(new_size);
        }
        size_ = new_size;
    }

    void reserve(size_t new_capacity)
    {
        if (new_capacity > capacity_)
        {
            size_t target_capacity = capacity_ == 0 ? 16 : capacity_ * 2;
            if (target_capacity < new_capacity)
                target_capacity = new_capacity;

            T *new_data = new T[target_capacity];
            for (size_t i = 0; i < size_; ++i)
            {
                new_data[i] = data_[i];
            }

            delete[] data_;
            data_ = new_data;
            capacity_ = target_capacity;
        }
    }

    void push_back(T value)
    {
        if (size_ == capacity_)
        {
            reserve(size_ + 1);
        }
        data_[size_++] = value;
    }

    void append(const T *begin, const T *end)
    {
        size_t count = end - begin;
        size_t new_size = size_ + count;
        if (new_size > capacity_)
        {
            reserve(new_size);
        }
        for (size_t i = 0; i < count; ++i)
        {
            data_[size_ + i] = begin[i];
        }
        size_ = new_size;
    }

    T &operator[](size_t pos) { return data_[pos]; }
    const T &operator[](size_t pos) const { return data_[pos]; }
};

// Basic iterator for output operations - moved before context to fix declaration order
class appender
{
private:
    buffer<char> *buffer_;

public:
    explicit appender(buffer<char> &buf) : buffer_(&buf) {}

    appender &operator=(char c)
    {
        buffer_->push_back(c);
        return *this;
    }

    appender &operator*() { return *this; }
    appender &operator++() { return *this; }
    appender &operator++(int) { return *this; }
};

// =============== Format Specifications ===============

// Basic format specifications
struct format_specs
{
    presentation_type type;
    align alignment;
    sign sign_option;
    bool alt;
    bool upper;
    char fill;
    int width;
    int precision;

    format_specs()
        : type(presentation_type::none),
          alignment(align::none),
          sign_option(sign::none),
          alt(false),
          upper(false),
          fill(' '),
          width(0),
          precision(-1) {}

    void set_align(align a) { alignment = a; }
    void set_sign(sign s) { sign_option = s; }
    void set_alt() { alt = true; }
    void set_upper() { upper = true; }
    void set_type(presentation_type t) { type = t; }
};

// =============== Parsing Context ===============

class parse_context
{
private:
    string_view fmt_;
    int next_arg_id_;

public:
    explicit parse_context(string_view fmt, int next_arg_id = 0)
        : fmt_(fmt), next_arg_id_(next_arg_id) {}

    const char *begin() const { return fmt_.data(); }
    const char *end() const { return fmt_.data() + fmt_.size(); }

    void advance_to(const char *it)
    {
        fmt_ = string_view(it, static_cast<size_t>(fmt_.data() + fmt_.size() - it));
    }

    int next_arg_id()
    {
        if (next_arg_id_ < 0)
        {
            // Error: cannot switch from manual to automatic indexing
            return 0;
        }
        return next_arg_id_++;
    }

    void check_arg_id(int id)
    {
        if (next_arg_id_ > 0)
        {
            // Error: cannot switch from automatic to manual indexing
            return;
        }
        next_arg_id_ = -1;
    }
};

// =============== Formatting Context ===============

class context
{
private:
    appender out_;

public:
    using iterator = appender;

    explicit context(appender out) : out_(out) {}

    appender out() const { return out_; }
    void advance_to(appender) {} // No-op
};

// =============== Parsing Functions ===============

// Parse a non-negative integer
inline int parse_nonnegative_int(const char *&begin, const char *end, int error_value = -1)
{
    if (begin == end || *begin < '0' || *begin > '9')
        return error_value;

    unsigned value = 0;
    const char *p = begin;

    do
    {
        unsigned prev = value;
        value = value * 10 + static_cast<unsigned>(*p - '0');
        if (value < prev)
        {
            // Overflow
            return error_value;
        }
        ++p;
    } while (p != end && *p >= '0' && *p <= '9');

    begin = p;
    return static_cast<int>(value);
}

// Parse alignment specifier
inline align parse_align(char c)
{
    switch (c)
    {
    case '<':
        return align::left;
    case '>':
        return align::right;
    case '^':
        return align::center;
    default:
        return align::none;
    }
}

// Parse format specifications
inline const char *parse_format_specs(
    const char *begin, const char *end,
    format_specs &specs, parse_context &ctx, type arg_type)
{
    if (begin == end)
        return begin;

    char c = *begin;

    // Parse fill character and alignment
    if (begin + 1 != end && (*(begin + 1) == '<' || *(begin + 1) == '>' || *(begin + 1) == '^'))
    {
        // If the character after the current one is an alignment character,
        // then the current character is a fill character
        specs.fill = c;
        ++begin;    // Move past the fill character
        c = *begin; // Update c to the alignment character
    }

    // Parse alignment
    if (c == '<' || c == '>' || c == '^')
    {
        specs.set_align(parse_align(c));
        ++begin;
        if (begin == end)
            return begin;
        c = *begin;
    }

    // Parse sign
    if (c == '+' || c == '-' || c == ' ')
    {
        if (c == '+')
            specs.set_sign(sign::plus);
        else if (c == ' ')
            specs.set_sign(sign::space);
        ++begin;
        if (begin == end)
            return begin;
        c = *begin;
    }

    // Parse alternate form
    if (c == '#')
    {
        specs.set_alt();
        ++begin;
        if (begin == end)
            return begin;
        c = *begin;
    }

    // Parse width
    if (c >= '0' && c <= '9')
    {
        specs.width = parse_nonnegative_int(begin, end);
        if (begin == end)
            return begin;
        c = *begin;
    }

    // Parse precision
    if (c == '.')
    {
        ++begin;
        if (begin == end)
            return begin;
        c = *begin;
        if (c >= '0' && c <= '9')
        {
            specs.precision = parse_nonnegative_int(begin, end);
        }
        else
        {
            specs.precision = 0;
        }
        if (begin == end)
            return begin;
        c = *begin;
    }

    // Parse presentation type
    if (c != '}')
    {
        switch (c)
        {
        case 'd':
            specs.set_type(presentation_type::dec);
            break;
        case 'x':
            specs.set_type(presentation_type::hex);
            break;
        case 'X':
            specs.set_type(presentation_type::hex);
            specs.set_upper();
            break;
        case 'o':
            specs.set_type(presentation_type::oct);
            break;
        case 'b':
            specs.set_type(presentation_type::bin);
            break;
        case 'B':
            specs.set_type(presentation_type::bin);
            specs.set_upper();
            break;
        case 'e':
            specs.set_type(presentation_type::exp);
            break;
        case 'E':
            specs.set_type(presentation_type::exp);
            specs.set_upper();
            break;
        case 'f':
            specs.set_type(presentation_type::fixed);
            break;
        case 'F':
            specs.set_type(presentation_type::fixed);
            specs.set_upper();
            break;
        case 'g':
            specs.set_type(presentation_type::general);
            break;
        case 'G':
            specs.set_type(presentation_type::general);
            specs.set_upper();
            break;
        case 'c':
            specs.set_type(presentation_type::chr);
            break;
        case 's':
            specs.set_type(presentation_type::string);
            break;
        case 'p':
            specs.set_type(presentation_type::pointer);
            break;
        case '?':
            specs.set_type(presentation_type::debug);
            break;
        }
        ++begin;
    }

    return begin;
}

// =============== Format Arguments ===============

// Type-erased formatting argument value - forward declaration
class format_arg_value;

// A collection of formatting arguments
class format_args
{
private:
    const format_arg_value *args_;
    int count_;

public:
    format_args() : args_(nullptr), count_(0) {}
    format_args(const format_arg_value *args, int count)
        : args_(args), count_(count) {}

    format_arg_value get(int id) const;
    int max_size() const { return count_; }
};

// Type-erased formatting argument value
class format_arg_value
{
public:
    enum class type
    {
        none_type,
        int_type,
        uint_type,
        long_long_type,
        ulong_long_type,
        bool_type,
        char_type,
        float_type,
        double_type,
        cstring_type,
        string_type,
        pointer_type
    };

    format_arg_value() : type_(type::none_type) {}

    format_arg_value(int value) : type_(type::int_type) { value_.int_value = value; }
    format_arg_value(unsigned value) : type_(type::uint_type) { value_.uint_value = value; }
    format_arg_value(long long value) : type_(type::long_long_type) { value_.long_long_value = value; }
    format_arg_value(unsigned long long value) : type_(type::ulong_long_type) { value_.ulong_long_value = value; }
    format_arg_value(bool value) : type_(type::bool_type) { value_.bool_value = value; }
    format_arg_value(char value) : type_(type::char_type) { value_.char_value = value; }
    format_arg_value(float value) : type_(type::float_type) { value_.float_value = value; }
    format_arg_value(double value) : type_(type::double_type) { value_.double_value = value; }
    format_arg_value(const char *value) : type_(type::cstring_type) { value_.string_value = value; }
    format_arg_value(string_view value) : type_(type::string_type)
    {
        value_.sv_data.data = value.data();
        value_.sv_data.size = value.size();
    }
    format_arg_value(const void *value) : type_(type::pointer_type) { value_.pointer_value = value; }

    // Modified to handle any pointer type with is_pointer trait
    template <typename T,
              typename = typename std::enable_if<std::is_pointer<T>::value &&
                                                 !std::is_same<T, const char *>::value>::type>
    format_arg_value(T value) : type_(type::pointer_type)
    {
        value_.pointer_value = static_cast<const void *>(value);
    }

    type get_type() const { return type_; }

    // Modified to avoid using 'auto' return type
    template <typename Visitor>
    typename std::result_of<Visitor(monostate)>::type visit(Visitor &&vis) const
    {
        switch (type_)
        {
        case type::none_type:
            return vis(monostate{});
        case type::int_type:
            return vis(value_.int_value);
        case type::uint_type:
            return vis(value_.uint_value);
        case type::long_long_type:
            return vis(value_.long_long_value);
        case type::ulong_long_type:
            return vis(value_.ulong_long_value);
        case type::bool_type:
            return vis(value_.bool_value);
        case type::char_type:
            return vis(value_.char_value);
        case type::float_type:
            return vis(value_.float_value);
        case type::double_type:
            return vis(value_.double_value);
        case type::cstring_type:
            return vis(value_.string_value);
        case type::string_type:
            return vis(string_view(value_.sv_data.data,
                                   value_.sv_data.size));
        case type::pointer_type:
            return vis(value_.pointer_value);
        }
        return vis(monostate{});
    }

private:
    // String view data storage
    struct string_view_data
    {
        const char *data;
        size_t size;
    };

    type type_;
    union value
    {
        int int_value;
        unsigned uint_value;
        long long long_long_value;
        unsigned long long ulong_long_value;
        bool bool_value;
        char char_value;
        float float_value;
        double double_value;
        const char *string_value;
        string_view_data sv_data; // Renamed to avoid naming conflict
        const void *pointer_value;

        value() : int_value(0) {}
    } value_;
};

// Implementation of format_args::get
inline format_arg_value format_args::get(int id) const
{
    if (id < count_)
        return args_[id];
    return format_arg_value();
}

// =============== Conversion Functions ===============

// Convert integer to string
template <typename T>
void to_string(T value, buffer<char> &out, format_specs specs)
{
    // Handle special case of 0
    if (value == 0)
    {
        out.push_back('0');
        return;
    }

    // Determine base based on type
    int base = 10;
    if (specs.type == presentation_type::hex)
        base = 16;
    else if (specs.type == presentation_type::oct)
        base = 8;
    else if (specs.type == presentation_type::bin)
        base = 2;

    // Handle negative numbers for signed types
    bool negative = false;
    if (std::is_signed<T>::value && value < 0)
    {
        negative = true;
        value = -value;
    }

    // Convert to string (in reverse)
    char buffer[64]; // Large enough for 64-bit integers in any base
    int pos = 0;

    while (value > 0)
    {
        int digit = value % base;
        if (digit < 10)
        {
            buffer[pos++] = '0' + digit;
        }
        else
        {
            buffer[pos++] = (specs.upper ? 'A' : 'a') + (digit - 10);
        }
        value /= base;
    }

    // Add sign if needed
    if (negative)
    {
        out.push_back('-');
    }
    else if (specs.sign_option == sign::plus)
    {
        out.push_back('+');
    }
    else if (specs.sign_option == sign::space)
    {
        out.push_back(' ');
    }

    // Add prefix if alt format is requested
    if (specs.alt)
    {
        if (specs.type == presentation_type::hex)
        {
            out.push_back('0');
            out.push_back(specs.upper ? 'X' : 'x');
        }
        else if (specs.type == presentation_type::bin)
        {
            out.push_back('0');
            out.push_back(specs.upper ? 'B' : 'b');
        }
        else if (specs.type == presentation_type::oct)
        {
            out.push_back('0');
        }
    }

    // Output the digits in correct order
    for (int i = pos - 1; i >= 0; --i)
    {
        out.push_back(buffer[i]);
    }
}

// Convert float to string (simplified)
inline void to_string(double value, buffer<char> &out, format_specs specs)
{
    // Simple implementation that doesn't handle all format options
    char buffer[32];
    int precision = specs.precision >= 0 ? specs.precision : 6;

    // Handle special cases
    if (value != value)
    { // NaN
        out.push_back('n');
        out.push_back('a');
        out.push_back('n');
        return;
    }

    if (value == 0.0)
    {
        out.push_back('0');
        if (precision > 0)
        {
            out.push_back('.');
            for (int i = 0; i < precision; ++i)
            {
                out.push_back('0');
            }
        }
        return;
    }

    // Handle sign
    if (value < 0)
    {
        out.push_back('-');
        value = -value;
    }
    else if (specs.sign_option == sign::plus)
    {
        out.push_back('+');
    }
    else if (specs.sign_option == sign::space)
    {
        out.push_back(' ');
    }

    // Extract integer part
    unsigned long long int_part = static_cast<unsigned long long>(value);
    double frac_part = value - int_part;

    // Convert integer part
    if (int_part == 0)
    {
        out.push_back('0');
    }
    else
    {
        to_string(int_part, out, format_specs());
    }

    // Add decimal point and fractional part
    if (precision > 0)
    {
        out.push_back('.');

        // Multiply by 10^precision and round
        for (int i = 0; i < precision; ++i)
        {
            frac_part *= 10;
        }
        frac_part = static_cast<double>(static_cast<unsigned long long>(frac_part + 0.5));

        // Print fractional digits
        for (int i = 0; i < precision; ++i)
        {
            int digit = static_cast<int>(frac_part) % 10;
            frac_part /= 10;
            buffer[precision - i - 1] = '0' + digit;
        }

        for (int i = 0; i < precision; ++i)
        {
            out.push_back(buffer[i]);
        }
    }
}

// =============== Formatting Functions ===============

// Helper for padding
inline void pad(buffer<char> &out, int width, char fill, align alignment, int content_width)
{
    if (width <= content_width)
        return;

    int padding = width - content_width;

    if (alignment == align::right || alignment == align::numeric)
    {
        for (int i = 0; i < padding; ++i)
        {
            out.push_back(fill);
        }
    }
    else if (alignment == align::center)
    {
        int left_padding = padding / 2;
        for (int i = 0; i < left_padding; ++i)
        {
            out.push_back(fill);
        }

        // Right padding will be added after the content
        padding -= left_padding;
    }

    // For align::left, no padding is needed here
}

inline void pad_after(buffer<char> &out, int width, char fill, align alignment, int content_width)
{
    if (width <= content_width)
        return;

    int padding = width - content_width;

    if (alignment == align::left)
    {
        for (int i = 0; i < padding; ++i)
        {
            out.push_back(fill);
        }
    }
    else if (alignment == align::center)
    {
        int right_padding = padding / 2 + padding % 2;
        for (int i = 0; i < right_padding; ++i)
        {
            out.push_back(fill);
        }
    }

    // For align::right, no padding is needed here
}

// Format a single argument with proper padding
template <typename T>
void format_value(const T &value, buffer<char> &out, format_specs specs)
{
    // Set default alignment based on type if not explicitly specified
    if (specs.alignment == align::none)
    {
        // Use template traits instead of if constexpr
        if (std::is_arithmetic<T>::value && !std::is_same<T, char>::value)
        {
            specs.alignment = align::right; // Numbers are right-aligned by default
        }
        else
        {
            specs.alignment = align::left; // Other types are left-aligned by default
        }
    }

    // Apply padding if needed
    if (specs.width > 0)
    {
        // We need to format into a temporary buffer to know the content width
        buffer<char> temp;
        format_specs temp_specs = specs;
        temp_specs.width = 0; // Disable width for the measurement pass to prevent recursion

        format_value(value, temp, temp_specs);
        int content_width = static_cast<int>(temp.size());

        // Apply padding
        pad(out, specs.width, specs.fill, specs.alignment, content_width);

        // Copy the formatted content
        out.append(temp.data(), temp.data() + temp.size());

        // Apply padding after if needed
        pad_after(out, specs.width, specs.fill, specs.alignment, content_width);

        return;
    }

    // Format based on type - replace if constexpr with type dispatch
    // Split the template specialization using tag dispatch
    format_value_dispatch(value, out, specs, typename std::conditional < std::is_integral<T>::value && !std::is_same<T, bool>::value && !std::is_same<T, char>::value,
                          std::integral_constant<int, 1>,
                          typename std::conditional<
                              std::is_same<T, bool>::value,
                              std::integral_constant<int, 2>,
                              typename std::conditional<
                                  std::is_same<T, char>::value,
                                  std::integral_constant<int, 3>,
                                  typename std::conditional<
                                      std::is_same<T, float>::value,
                                      std::integral_constant<int, 4>,
                                      typename std::conditional<
                                          std::is_same<T, double>::value,
                                          std::integral_constant<int, 5>,
                                          typename std::conditional<
                                              std::is_same<T, const char *>::value,
                                              std::integral_constant<int, 6>,
                                              typename std::conditional<
                                                  std::is_same<T, string_view>::value,
                                                  std::integral_constant<int, 7>,
                                                  typename std::conditional<
                                                      std::is_same<T, const void *>::value,
                                                      std::integral_constant<int, 8>,
                                                      std::integral_constant<int, 0>>::type>::type>::type>::type>::type>::type>::type > ::type());
}

// Type dispatch helpers for format_value
template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 1>)
{
    // Integral type
    to_string(value, out, specs);
}

template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 2>)
{
    // Bool type
    if (specs.type == presentation_type::none || specs.type == presentation_type::string)
    {
        const char *str = value ? "true" : "false";
        while (*str)
            out.push_back(*str++);
    }
    else
    {
        to_string(static_cast<int>(value), out, specs);
    }
}

template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 3>)
{
    // Char type
    out.push_back(value);
}

template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 4>)
{
    // Float type
    to_string(static_cast<double>(value), out, specs);
}

template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 5>)
{
    // Double type
    to_string(value, out, specs);
}

template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 6>)
{
    // const char* type
    if (value)
    {
        const char *str = value;
        while (*str)
        {
            out.push_back(*str++);
        }
    }
    else
    {
        out.push_back('(');
        out.push_back('n');
        out.push_back('u');
        out.push_back('l');
        out.push_back('l');
        out.push_back(')');
    }
}

template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 7>)
{
    // string_view type
    if (value.data())
    {
        for (size_t i = 0; i < value.size(); ++i)
        {
            out.push_back(value.data()[i]);
        }
    }
}

template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 8>)
{
    // const void* type
    if (value)
    {
        out.push_back('0');
        out.push_back('x');
        uintptr_t ptr_val = reinterpret_cast<uintptr_t>(value);

        // Create temporary specs for hex format
        format_specs ptr_specs;
        ptr_specs.set_type(presentation_type::hex);
        to_string(ptr_val, out, ptr_specs);
    }
    else
    {
        out.push_back('n');
        out.push_back('u');
        out.push_back('l');
        out.push_back('l');
        out.push_back('p');
        out.push_back('t');
        out.push_back('r');
    }
}

template <typename T>
void format_value_dispatch(const T &value, buffer<char> &out, format_specs specs, std::integral_constant<int, 0>)
{
    // Fallback for unsupported types
    out.push_back('?');
}

// Visitor for format_arg_value
struct format_visitor
{
    buffer<char> &out;
    format_specs specs;

    format_visitor(buffer<char> &out_, const format_specs &specs_)
        : out(out_), specs(specs_) {}

    template <typename T>
    void operator()(const T &value)
    {
        format_value(value, out, specs);
    }

    void operator()(monostate)
    {
        // Nothing to format
    }
};

// =============== Format String Parsing ===============

// Parse a replacement field starting with '{'
inline const char *parse_replacement_field(const char *begin, const char *end,
                                           parse_context &ctx, format_args args,
                                           buffer<char> &out)
{
    if (begin == end)
        return begin;

    ++begin; // Skip '{'
    if (begin == end)
        return begin;

    if (*begin == '}')
    {
        // Empty replacement field '{}'
        auto arg = args.get(ctx.next_arg_id());
        format_specs specs;
        arg.visit(format_visitor(out, specs));
        return begin + 1;
    }

    if (*begin == '{')
    {
        // Escaped '{{' - just output a single '{'
        out.push_back('{');
        return begin + 1;
    }

    // Parse argument ID
    int arg_id = 0;
    if (*begin >= '0' && *begin <= '9')
    {
        arg_id = parse_nonnegative_int(begin, end);
        ctx.check_arg_id(arg_id);
    }
    else
    {
        arg_id = ctx.next_arg_id();
    }

    if (begin == end)
        return begin;

    // Check for format specifiers
    format_specs specs;
    if (*begin == ':')
    {
        ++begin;
        begin = parse_format_specs(begin, end, specs, ctx, type::none_type);
    }

    if (begin == end || *begin != '}')
    {
        // Error: missing closing '}'
        return begin;
    }

    // Format the argument
    auto arg = args.get(arg_id);
    arg.visit(format_visitor(out, specs));

    return begin + 1;
}

// Parse a format string and write output to the buffer
inline void vformat_to(buffer<char> &out, string_view fmt, format_args args)
{
    parse_context ctx(fmt);
    auto begin = fmt.data();
    auto end = begin + fmt.size();

    while (begin != end)
    {
        auto c = *begin++;

        if (c == '{')
        {
            --begin; // Put back the '{'
            begin = parse_replacement_field(begin, end, ctx, args, out);
        }
        else if (c == '}')
        {
            if (begin != end && *begin == '}')
            {
                // Escaped '}}' - just output a single '}'
                out.push_back('}');
                ++begin;
            }
            else
            {
                // Error: unmatched '}'
                out.push_back('}'); // Output as-is for now
            }
        }
        else
        {
            out.push_back(c);
        }
    }
}

// =============== Public API Functions ===============

// Format to a buffer
template <typename... Args>
void format_to(buffer<char> &buf, string_view fmt, const Args &...args)
{
    // Handle the case of no arguments gracefully to avoid zero-size array
    AFMT_CONSTEXPR size_t arg_count = sizeof...(args);
    format_arg_value values[arg_count > 0 ? arg_count : 1] = {format_arg_value(args)...};
    format_args fargs(values, arg_count);
    vformat_to(buf, fmt, fargs);
}

// Format to a fixed-size array
template <size_t N, typename... Args>
void format_to(char (&out)[N], string_view fmt, const Args &...args)
{
    buffer<char> buf;
    format_to(buf, fmt, args...);

    // Copy to the fixed array with null termination
    size_t size = buf.size() < N - 1 ? buf.size() : N - 1;
    for (size_t i = 0; i < size; ++i)
    {
        out[i] = buf.data()[i];
    }
    out[size] = '\0';
}

// Format to a buffer with size limitation
template <typename... Args>
void format_to_n(char *out, size_t n, string_view fmt, const Args &...args)
{
    if (n == 0)
        return; // No space to write anything

    buffer<char> buf;
    format_to(buf, fmt, args...);

    // Copy to the output buffer
    size_t size = buf.size() < n - 1 ? buf.size() : n - 1;
    for (size_t i = 0; i < size; ++i)
    {
        out[i] = buf.data()[i];
    }
    out[size] = '\0'; // Always null-terminate
}

// Get the size that would be required for formatting
template <typename... Args>
size_t formatted_size(string_view fmt, const Args &...args)
{
    buffer<char> buf;
    format_to(buf, fmt, args...);
    return buf.size();
}

#if AFMT_HAS_CXX_11
inline std::string vformat(string_view fmt, format_args args)
{
    buffer<char> buf;
    vformat_to(buf, fmt, args);

    // Create std::string directly from buffer data
    return std::string(buf.data(), buf.size());
}

template <typename... Args>
std::string format(string_view fmt, const Args &...args)
{
    // Handle the case of no arguments gracefully to avoid zero-size array
    AFMT_CONSTEXPR size_t arg_count = sizeof...(args);
    format_arg_value values[arg_count > 0 ? arg_count : 1] = {format_arg_value(args)...};
    format_args fargs(values, arg_count);
    return vformat(fmt, fargs);
}
#endif

#if AFMT_HAS_ARDUINO
// Format to an Arduino String
inline String avformat(string_view fmt, format_args args)
{
    buffer<char> buf;
    vformat_to(buf, fmt, args);

    // Create String directly from buffer data
    return String(buf.data(), buf.size());
}

template <typename... Args>
String aformat(string_view fmt, const Args &...args)
{
    // Handle the case of no arguments gracefully to avoid zero-size array
    AFMT_CONSTEXPR size_t arg_count = sizeof...(args);
    format_arg_value values[arg_count > 0 ? arg_count : 1] = {format_arg_value(args)...};
    format_args fargs(values, arg_count);
    return avformat(fmt, fargs);
}

template <typename... Args>
void print(string_view fmt, const Args &...args)
{
    AFMT_CONSTEXPR size_t arg_count = sizeof...(args);
    format_arg_value values[arg_count > 0 ? arg_count : 1] = {format_arg_value(args)...};
    format_args fargs(values, arg_count);

    Serial.print(avformat(fmt, fargs));
}

template <typename... Args>
void println(string_view fmt, const Args &...args)
{
    AFMT_CONSTEXPR size_t arg_count = sizeof...(args);
    format_arg_value values[arg_count > 0 ? arg_count : 1] = {format_arg_value(args)...};
    format_args fargs(values, arg_count);

    Serial.println(avformat(fmt, fargs));
}
#endif

AFMT_END_NAMESPACE

#endif // ARDUINO_FMT_H_
