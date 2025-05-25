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

// Include Arduino.h for String class
#if AFMT_HAS_ARDUINO
#include <Arduino.h>
#endif

#include <type_traits>
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

// =================== Config =====================
#if AFMT_HAS_ARDUINO
#ifndef AFMT_SERIAL_OUTPUT
#define AFMT_SERIAL_OUTPUT Serial
#endif
#endif

// Default SBO size for the buffer
#ifndef AFMT_DEFAULT_ADAPTIVE_SBO_SIZE
#define AFMT_DEFAULT_ADAPTIVE_SBO_SIZE 64
#endif
static_assert(AFMT_DEFAULT_ADAPTIVE_SBO_SIZE > 0, "AFMT_DEFAULT_SBO_SIZE must be greater than 0");

// ================= Arduino FMT ==================

AFMT_BEGIN_NAMESPACE

// =============== Type Definitions ===============

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

// Helper function for AFMT_CONSTEXPR string length calculation
AFMT_CONSTEXPR size_t string_length(const char *s)
{
    return s == nullptr ? 0 : *s == '\0' ? 0
                                         : 1 + string_length(s + 1);
}

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

// =============== Type Traits for Buffer Constraints ===============

// Forward declarations for type checking
class external_buffer;
class adaptive_buffer;

// Add format_to_result struct definition
struct format_to_result
{
    char *out;
    bool truncated;

    // AFMT_CONSTEXPR operator char*() const {
    //     // Error reporting like fmtlib's report_error("output is truncated")
    //     // is omitted for lightweightness. The truncated flag should be checked by the caller.
    //     return out;
    // }
};

// Type trait to check if a type is a valid buffer type
template <typename T>
struct is_valid_buffer : std::false_type
{
};

// Specialization for external_buffer
template <>
struct is_valid_buffer<external_buffer> : std::true_type
{
};

// Specialization for adaptive_buffer
template <>
struct is_valid_buffer<adaptive_buffer> : std::true_type
{
};

// Helper macro for buffer type constraints
#define AFMT_BUFFER_CONSTRAINT(buffer_type) \
    typename std::enable_if<is_valid_buffer<buffer_type>::value, int>::type = 0

// =============== Buffer Classes ===============

// External buffer - uses external pre-allocated char array
class external_buffer
{
private:
    char *data_;
    size_t size_;
    size_t capacity_;
    bool truncated_;

public:
    AFMT_CONSTEXPR external_buffer(char *data, size_t capacity)
        : data_(data), size_(0), capacity_(capacity), truncated_(false) {}

    char *data() { return data_; }
    AFMT_CONSTEXPR const char *data() const { return data_; }
    AFMT_CONSTEXPR size_t size() const { return size_; }
    AFMT_CONSTEXPR size_t capacity() const { return capacity_; }
    AFMT_CONSTEXPR bool is_truncated() const { return truncated_; }

    void clear()
    {
        size_ = 0;
        truncated_ = false;
    }

    void push_back(const char &value)
    {
        if (size_ < capacity_)
        {
            data_[size_++] = value;
        }
        else
        {
            truncated_ = true; // Mark truncation
        }
    }

    void append(const char *begin, const char *end)
    {
        size_t count_to_append = static_cast<size_t>(end - begin);
        if (count_to_append == 0)
            return;

        size_t available_space = capacity_ - size_;
        size_t num_to_copy = count_to_append;

        if (count_to_append > available_space)
        {
            num_to_copy = available_space;
            truncated_ = true;
        }

        for (size_t i = 0; i < num_to_copy; ++i)
        {
            data_[size_ + i] = begin[i];
        }
        size_ += num_to_copy;
    }

    char &operator[](size_t pos) { return data_[pos]; }
    const char &operator[](size_t pos) const { return data_[pos]; }
};

// Adaptive buffer - small buffer optimization with dynamic growth
class adaptive_buffer
{
private:
    char storage_[AFMT_DEFAULT_ADAPTIVE_SBO_SIZE];
    char *heap_data_;
    size_t size_;
    size_t capacity_;

    AFMT_CONSTEXPR bool is_sbo() const
    {
        return heap_data_ == nullptr;
    }

public:
    AFMT_CONSTEXPR adaptive_buffer() : heap_data_(nullptr), size_(0), capacity_(AFMT_DEFAULT_ADAPTIVE_SBO_SIZE) {}

    ~adaptive_buffer()
    {
        if (!is_sbo())
        {
            delete[] heap_data_;
        }
    }

    // Disable copy
    adaptive_buffer(const adaptive_buffer &) = delete;
    adaptive_buffer &operator=(const adaptive_buffer &) = delete;

    // Move constructor
    adaptive_buffer(adaptive_buffer &&other) noexcept
        : heap_data_(other.heap_data_), size_(other.size_), capacity_(other.capacity_)
    {
        if (other.is_sbo())
        {
            // Copy SBO data
            for (size_t i = 0; i < size_; ++i)
            {
                storage_[i] = other.storage_[i];
            }
            heap_data_ = nullptr;
        }

        // Reset other to valid SBO state
        other.heap_data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = AFMT_DEFAULT_ADAPTIVE_SBO_SIZE;
    }

    // Move assignment
    adaptive_buffer &operator=(adaptive_buffer &&other) noexcept
    {
        if (this != &other)
        {
            // Clean up current state
            if (!is_sbo())
            {
                delete[] heap_data_;
            }

            // Move from other
            heap_data_ = other.heap_data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            if (other.is_sbo())
            {
                // Copy SBO data
                for (size_t i = 0; i < size_; ++i)
                {
                    storage_[i] = other.storage_[i];
                }
                heap_data_ = nullptr;
            }

            // Reset other to valid SBO state
            other.heap_data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = AFMT_DEFAULT_ADAPTIVE_SBO_SIZE;
        }
        return *this;
    }

    char *data() { return is_sbo() ? storage_ : heap_data_; }
    AFMT_CONSTEXPR const char *data() const { return is_sbo() ? storage_ : heap_data_; }
    AFMT_CONSTEXPR size_t size() const { return size_; }
    AFMT_CONSTEXPR size_t capacity() const { return capacity_; }

    void clear() { size_ = 0; }

    void reserve(size_t new_capacity)
    {
        if (new_capacity <= capacity_)
            return;

        if (is_sbo())
        {
            // Transition from SBO to heap
            char *new_data = new char[new_capacity];
            for (size_t i = 0; i < size_; ++i)
            {
                new_data[i] = storage_[i];
            }
            heap_data_ = new_data;
            capacity_ = new_capacity;
        }
        else
        {
            // Grow on heap
            char *new_data = new char[new_capacity];
            for (size_t i = 0; i < size_; ++i)
            {
                new_data[i] = heap_data_[i];
            }
            delete[] heap_data_;
            heap_data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    void push_back(const char &value)
    {
        if (size_ == capacity_)
        {
            // Need to grow
            size_t new_cap = capacity_ * 2;
            if (new_cap <= capacity_) // Handle overflow
                new_cap = capacity_ + 1;
            reserve(new_cap);
        }

        if (is_sbo())
        {
            storage_[size_++] = value;
        }
        else
        {
            heap_data_[size_++] = value;
        }
    }

    void append(const char *begin, const char *end)
    {
        size_t count = static_cast<size_t>(end - begin);
        if (count == 0)
            return;

        if (size_ + count > capacity_)
        {
            size_t new_cap = capacity_;
            while (new_cap < size_ + count)
            {
                new_cap = new_cap * 2;
                if (new_cap <= capacity_) // Handle overflow
                {
                    new_cap = size_ + count;
                    break;
                }
            }
            reserve(new_cap);
        }

        char *target = is_sbo() ? storage_ : heap_data_;
        for (size_t i = 0; i < count; ++i)
        {
            target[size_ + i] = begin[i];
        }
        size_ += count;
    }

    char &operator[](size_t pos)
    {
        return is_sbo() ? storage_[pos] : heap_data_[pos];
    }

    const char &operator[](size_t pos) const
    {
        return is_sbo() ? storage_[pos] : heap_data_[pos];
    }
};

// Default buffer type alias for convenience - now just an alias
using buffer = adaptive_buffer;

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

    AFMT_CONSTEXPR format_specs()
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
    AFMT_CONSTEXPR explicit parse_context(string_view fmt, int next_arg_id = 0)
        : fmt_(fmt), next_arg_id_(next_arg_id) {}

    AFMT_CONSTEXPR const char *begin() const { return fmt_.data(); }
    AFMT_CONSTEXPR const char *end() const { return fmt_.data() + fmt_.size(); }

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
AFMT_CONSTEXPR inline align parse_align(char c)
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
    format_specs &specs, parse_context &ctx)
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

    // Parse zero padding and width
    if (c == '0')
    {
        // Zero padding detected - set fill to '0' and alignment to numeric
        specs.fill = '0';
        specs.set_align(align::numeric);
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
            specs.type = presentation_type::dec;
            specs.upper = false;
            ++begin;
            break;
        case 'x':
            specs.type = presentation_type::hex;
            specs.upper = false;
            ++begin;
            break;
        case 'X':
            specs.type = presentation_type::hex;
            specs.upper = true;
            ++begin;
            break;
        case 'o':
            specs.type = presentation_type::oct;
            specs.upper = false;
            ++begin;
            break;
        case 'b':
            specs.type = presentation_type::bin;
            specs.upper = false;
            ++begin;
            break;
        case 'B':
            specs.type = presentation_type::bin;
            specs.upper = true;
            ++begin;
            break;
        case 'e':
            specs.type = presentation_type::exp;
            specs.upper = false;
            ++begin;
            break;
        case 'E':
            specs.type = presentation_type::exp;
            specs.upper = true;
            ++begin;
            break;
        case 'f':
            specs.type = presentation_type::fixed;
            specs.upper = false;
            ++begin;
            break;
        case 'F':
            specs.type = presentation_type::fixed;
            specs.upper = true;
            ++begin;
            break;
        case 'g':
            specs.type = presentation_type::general;
            specs.upper = false;
            ++begin;
            break;
        case 'G':
            specs.type = presentation_type::general;
            specs.upper = true;
            ++begin;
            break;
        case 'c':
            specs.type = presentation_type::chr;
            specs.upper = false;
            ++begin;
            break;
        case 's':
            specs.type = presentation_type::string;
            specs.upper = false;
            ++begin;
            break;
        case 'p':
            specs.type = presentation_type::pointer;
            specs.upper = false;
            ++begin;
            break;
        case '?':
            specs.type = presentation_type::debug;
            specs.upper = false;
            ++begin;
            break;
        }
    }

    return begin;
}

// =============== Format Arguments ===============

// Helper struct for variant-like visitation
struct monostate
{
    AFMT_CONSTEXPR monostate() {}
};

// Type-erased formatting argument value - forward declaration
class format_arg_value;

// A collection of formatting arguments
class format_args
{
private:
    const format_arg_value *args_;
    int count_;

public:
    AFMT_CONSTEXPR format_args() : args_(nullptr), count_(0) {}
    AFMT_CONSTEXPR format_args(const format_arg_value *args, int count)
        : args_(args), count_(count) {}

    AFMT_CONSTEXPR format_arg_value get(int id) const;
    AFMT_CONSTEXPR int max_size() const { return count_; }
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

    AFMT_CONSTEXPR format_arg_value() : type_(type::none_type) {}

    AFMT_CONSTEXPR format_arg_value(int value) : type_(type::int_type) { value_.int_value = value; }
    AFMT_CONSTEXPR format_arg_value(unsigned value) : type_(type::uint_type) { value_.uint_value = value; }
    AFMT_CONSTEXPR format_arg_value(long long value) : type_(type::long_long_type) { value_.long_long_value = value; }
    AFMT_CONSTEXPR format_arg_value(unsigned long long value) : type_(type::ulong_long_type) { value_.ulong_long_value = value; }
    AFMT_CONSTEXPR format_arg_value(bool value) : type_(type::bool_type) { value_.bool_value = value; }
    AFMT_CONSTEXPR format_arg_value(char value) : type_(type::char_type) { value_.char_value = value; }
    AFMT_CONSTEXPR format_arg_value(float value) : type_(type::float_type) { value_.float_value = value; }
    AFMT_CONSTEXPR format_arg_value(double value) : type_(type::double_type) { value_.double_value = value; }
    AFMT_CONSTEXPR format_arg_value(const char *value) : type_(type::cstring_type) { value_.string_value = value; }
    AFMT_CONSTEXPR format_arg_value(string_view value) : type_(type::string_type)
    {
        value_.sv_data.data = value.data();
        value_.sv_data.size = value.size();
    }
    AFMT_CONSTEXPR format_arg_value(const void *value) : type_(type::pointer_type) { value_.pointer_value = value; }

    // Modified to handle any pointer type with is_pointer trait
    template <typename T,
              typename = typename std::enable_if<std::is_pointer<T>::value &&
                                                 !std::is_same<T, const char *>::value>::type>
    AFMT_CONSTEXPR format_arg_value(T value) : type_(type::pointer_type)
    {
        value_.pointer_value = static_cast<const void *>(value);
    }

    AFMT_CONSTEXPR type get_type() const { return type_; }

    // Modified to avoid using 'auto' return type
    template <typename Visitor>
    AFMT_CONSTEXPR typename std::result_of<Visitor(monostate)>::type visit(Visitor &&vis) const
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

        AFMT_CONSTEXPR value() : int_value(0) {}
    } value_;
};

// Implementation of format_args::get
AFMT_CONSTEXPR inline format_arg_value format_args::get(int id) const
{
    if (id < count_)
        return args_[id];
    return format_arg_value();
}

// =============== Conversion Functions ===============

// Convert integer to string - constrained to valid buffer types
template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void to_string(T value, buffer_type &out, format_specs specs)
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

// Convert float to string - constrained to valid buffer types
template <typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
inline void to_string(double value, buffer_type &out, format_specs specs)
{
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
        int precision = specs.precision >= 0 ? specs.precision : 6;
        if (precision > 0 && (specs.type == presentation_type::fixed ||
                              specs.type == presentation_type::exp ||
                              specs.type == presentation_type::general))
        {
            out.push_back('.');
            for (int i = 0; i < precision; ++i)
            {
                out.push_back('0');
            }
        }
        // Add exponent for scientific notation
        if (specs.type == presentation_type::exp)
        {
            out.push_back(specs.upper ? 'E' : 'e');
            out.push_back('+');
            out.push_back('0');
            out.push_back('0');
        }
        return;
    }

    // Handle infinity
    if (value == value + 1.0 && value > 0) // Positive infinity
    {
        if (specs.upper)
        {
            out.push_back('I');
            out.push_back('N');
            out.push_back('F');
        }
        else
        {
            out.push_back('i');
            out.push_back('n');
            out.push_back('f');
        }
        return;
    }
    if (value == value + 1.0 && value < 0) // Negative infinity
    {
        out.push_back('-');
        if (specs.upper)
        {
            out.push_back('I');
            out.push_back('N');
            out.push_back('F');
        }
        else
        {
            out.push_back('i');
            out.push_back('n');
            out.push_back('f');
        }
        return;
    }

    // Handle sign
    bool negative = value < 0;
    if (negative)
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

    int precision = specs.precision >= 0 ? specs.precision : 6;

    // Determine format type
    presentation_type format_type = specs.type;
    if (format_type == presentation_type::none)
    {
        format_type = presentation_type::general; // Default to general format
    }

    // Handle general format - choose between fixed and scientific
    if (format_type == presentation_type::general)
    {
        // Calculate exponent to decide format
        int exponent = 0;
        double temp_value = value;
        if (temp_value >= 1.0)
        {
            while (temp_value >= 10.0)
            {
                temp_value /= 10.0;
                exponent++;
            }
        }
        else if (temp_value > 0.0)
        {
            while (temp_value < 1.0)
            {
                temp_value *= 10.0;
                exponent--;
            }
        }

        // Use scientific notation if exponent is outside [-4, precision)
        if (exponent < -4 || exponent >= precision)
        {
            format_type = presentation_type::exp;
        }
        else
        {
            format_type = presentation_type::fixed;
            // Adjust precision for general format
            precision = precision - exponent - 1;
            if (precision < 0)
                precision = 0;
        }
    }

    if (format_type == presentation_type::exp)
    {
        // Scientific notation
        int exponent = 0;

        // Normalize the number to [1.0, 10.0)
        if (value >= 1.0)
        {
            while (value >= 10.0)
            {
                value /= 10.0;
                exponent++;
            }
        }
        else if (value > 0.0)
        {
            while (value < 1.0)
            {
                value *= 10.0;
                exponent--;
            }
        }

        // Apply rounding based on precision
        double scale = 1.0;
        for (int i = 0; i < precision; ++i)
        {
            scale *= 10.0;
        }
        value = (value * scale + 0.5) / scale;

        // Handle case where rounding pushes us to 10.0
        if (value >= 10.0)
        {
            value /= 10.0;
            exponent++;
        }

        // Format the mantissa
        unsigned long long int_part = static_cast<unsigned long long>(value);
        double frac_part = value - int_part;

        // Output integer part
        out.push_back('0' + int_part);

        // Output fractional part
        if (precision > 0)
        {
            out.push_back('.');
            for (int i = 0; i < precision; ++i)
            {
                frac_part *= 10;
                int digit = static_cast<int>(frac_part);
                out.push_back('0' + digit);
                frac_part -= digit;
            }
        }

        // Output exponent
        out.push_back(specs.upper ? 'E' : 'e');
        if (exponent >= 0)
        {
            out.push_back('+');
        }
        else
        {
            out.push_back('-');
            exponent = -exponent;
        }

        // Format exponent with at least 2 digits
        if (exponent >= 100)
        {
            out.push_back('0' + (exponent / 100));
            exponent %= 100;
        }
        out.push_back('0' + (exponent / 10));
        out.push_back('0' + (exponent % 10));
    }
    else
    {
        // Fixed-point notation (default case from original implementation)
        // Apply rounding to the entire number based on precision
        double scale = 1.0;
        for (int i = 0; i < precision; ++i)
        {
            scale *= 10.0;
        }
        value = (value * scale + 0.5) / scale;

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

            // Convert fractional part to digits
            for (int i = 0; i < precision; ++i)
            {
                frac_part *= 10;
                int digit = static_cast<int>(frac_part);
                out.push_back('0' + digit);
                frac_part -= digit;
            }
        }
    }
}

// =============== Formatting Functions ===============

// Helper for padding - constrained to valid buffer types
template <typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
inline void pad(buffer_type &out, int width, char fill, align alignment, int content_width)
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

template <typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
inline void pad_after(buffer_type &out, int width, char fill, align alignment, int content_width)
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

// Format a single argument with proper padding - constrained to valid buffer types
template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value(const T &value, buffer_type &out, format_specs specs)
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

    // Zero padding should only apply to numeric types (not strings, chars, etc.)
    // For non-numeric types, convert zero padding to space padding
    if (specs.fill == '0' && specs.alignment == align::numeric)
    {
        if (!std::is_arithmetic<T>::value || std::is_same<T, char>::value)
        {
            // Not a numeric type that should use zero padding - convert to space padding
            specs.fill = ' ';
            specs.alignment = align::right; // Use right alignment instead of numeric
        }
    }

    // Special handling for zero padding with numeric alignment
    if (specs.width > 0 && specs.alignment == align::numeric && specs.fill == '0')
    {
        // For numeric zero padding, we need to handle signs and prefixes specially
        adaptive_buffer temp;
        format_specs temp_specs = specs;
        temp_specs.width = 0;               // Disable width for the measurement pass
        temp_specs.alignment = align::none; // Disable alignment for measurement

        format_value(value, temp, temp_specs);
        int content_width = static_cast<int>(temp.size());

        if (specs.width > content_width)
        {
            // We need to insert zeros between sign/prefix and digits
            const char *temp_data = temp.data();
            int sign_prefix_len = 0;

            // Count sign characters
            if (temp_data[0] == '-' || temp_data[0] == '+' || temp_data[0] == ' ')
            {
                out.push_back(temp_data[0]);
                sign_prefix_len++;
            }

            // Count prefix characters (like 0x, 0b)
            while (sign_prefix_len < content_width &&
                   (temp_data[sign_prefix_len] == '0' && sign_prefix_len + 1 < content_width &&
                    (temp_data[sign_prefix_len + 1] == 'x' || temp_data[sign_prefix_len + 1] == 'X' ||
                     temp_data[sign_prefix_len + 1] == 'b' || temp_data[sign_prefix_len + 1] == 'B')))
            {
                out.push_back(temp_data[sign_prefix_len]);
                out.push_back(temp_data[sign_prefix_len + 1]);
                sign_prefix_len += 2;
                break;
            }

            // Add zero padding
            int zeros_needed = specs.width - content_width;
            for (int i = 0; i < zeros_needed; ++i)
            {
                out.push_back('0');
            }

            // Add remaining digits
            for (int i = sign_prefix_len; i < content_width; ++i)
            {
                out.push_back(temp_data[i]);
            }
        }
        else
        {
            // No padding needed, just copy content
            out.append(temp.data(), temp.data() + temp.size());
        }
        return;
    }

    // Apply padding if needed
    if (specs.width > 0)
    {
        // We need to format into a temporary buffer to know the content width
        adaptive_buffer temp;
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

// Type dispatch helpers for format_value - constrained to valid buffer types
template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 1>)
{
    // Integral type
    to_string(value, out, specs);
}

template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 2>)
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

template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 3>)
{
    // Char type
    out.push_back(value);
}

template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 4>)
{
    // Float type
    to_string(static_cast<double>(value), out, specs);
}

template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 5>)
{
    // Double type
    to_string(value, out, specs);
}

template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 6>)
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

template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 7>)
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

template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 8>)
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

template <typename T, typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
void format_value_dispatch(const T &value, buffer_type &out, format_specs specs, std::integral_constant<int, 0>)
{
    // Fallback for unsupported types
    out.push_back('?');
}

// Visitor for format_arg_value - constrained to valid buffer types
template <typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
struct format_visitor
{
    buffer_type &out;
    format_specs specs;

    AFMT_CONSTEXPR format_visitor(buffer_type &out_, const format_specs &specs_)
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

// Parse a replacement field starting with '{' - constrained to valid buffer types
template <typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
inline const char *parse_replacement_field(const char *begin, const char *end,
                                           parse_context &ctx, format_args args,
                                           buffer_type &out)
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
        arg.visit(format_visitor<buffer_type>(out, specs));
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
        begin = parse_format_specs(begin, end, specs, ctx);
    }

    if (begin == end || *begin != '}')
    {
        // Error: missing closing '}'
        return begin;
    }

    // Format the argument
    auto arg = args.get(arg_id);
    arg.visit(format_visitor<buffer_type>(out, specs));

    return begin + 1;
}

// Parse a format string and write output to the buffer - constrained to valid buffer types
template <typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
inline void vformat_to(buffer_type &out, string_view fmt, format_args args)
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

    // Automatically null-terminate for C-style string compatibility
    out.push_back('\0');
}

// =============== Argument Storage Helper ===============

// format_arg_store_n holds N format_arg_value objects.
// This allows make_format_args to return a structure containing the arguments
// by value, ensuring their lifetime for the duration of the formatting call.
template <size_t N_args>
class format_arg_store_n
{
public:
    format_arg_value arg_buffer_[N_args > 0 ? N_args : 1];
    size_t count_;

    template <typename... ArgsPack>
    AFMT_CONSTEXPR format_arg_store_n(const ArgsPack &...actual_args)
        : arg_buffer_{format_arg_value(actual_args)...}, count_(N_args) {}

    // Implicit conversion to format_args, providing a view into the stored arguments.
    AFMT_CONSTEXPR operator format_args() const
    {
        return format_args(arg_buffer_, count_);
    }

    // Explicit conversion to format_args, providing a view into the stored arguments.
    AFMT_CONSTEXPR format_args get_args() const
    {
        return format_args(arg_buffer_, count_);
    }
};

// Helper to create format_arg_store_n from variadic template arguments.
template <typename... Args>
AFMT_CONSTEXPR inline format_arg_store_n<sizeof...(Args)> make_format_args(const Args &...args)
{
    return format_arg_store_n<sizeof...(Args)>(args...);
}

// =============== Public API Functions ===============

// Core formatting function vformat_to (remains as is for generic buffers)
// template <typename buffer_type, AFMT_BUFFER_CONSTRAINT(buffer_type)>
inline format_to_result vformat_to_n(char *out, size_t n, string_view fmt, format_args args)
{
    if (n == 0)
    {
        return {out, true}; // Truncated: no space for anything.
    }

    external_buffer temp_buffer(out, n);
    vformat_to(temp_buffer, fmt, args); // Calls the existing generic vformat_to taking a buffer reference

    bool was_truncated = temp_buffer.is_truncated();
    char *end_ptr;

    if (was_truncated)
    {
        // Content + null did not fit or content itself was too long.
        // Ensure out[n - 1] is '\0'.
        out[n - 1] = '\0';
        end_ptr = out + n - 1; // Points to the enforced null terminator.
    }
    else
    {
        // Content + null fit. temp_buffer.size() includes the null.
        // The null terminator is at out[temp_buffer.size() - 1].
        if (temp_buffer.size() > 0)
        {                                           // If something was written (at least the null terminator)
            end_ptr = out + temp_buffer.size() - 1; // Points to the written null terminator.
        }
        else
        { // Buffer capacity > 0, but nothing written (e.g. empty format string, resulting in only \0)
            // vformat_to should have written a \0, making size 1.
            // This case is defensive. If size is 0, ensure null at start.
            out[0] = '\0';
            end_ptr = out;
        }
    }
    return {end_ptr, was_truncated};
}

// Format to any valid buffer type - constrained to valid buffer types
template <typename buffer_type, typename... Args, AFMT_BUFFER_CONSTRAINT(buffer_type)>
inline void format_to(buffer_type &buf, string_view fmt, const Args &...args)
{
    vformat_to(buf, fmt, make_format_args(args...));
}

// Format to a fixed-size array using external_buffer
template <size_t N, typename... Args>
inline format_to_result format_to(char (&out)[N], string_view fmt, const Args &...args)
{
    return vformat_to_n(out, N, fmt, make_format_args(args...));
}

// Format to a buffer with size limitation using external buffer
template <typename... Args>
inline format_to_result format_to_n(char *out, size_t n, string_view fmt, const Args &...args)
{
    return vformat_to_n(out, n, fmt, make_format_args(args...));
}

// Get the size that would be required for formatting
template <typename... Args>
inline size_t formatted_size(string_view fmt, const Args &...args)
{
    adaptive_buffer buf;
    vformat_to(buf, fmt, make_format_args(args...));
    return buf.size();
}

inline std::string vformat(string_view fmt, format_args args)
{
    adaptive_buffer buf;
    vformat_to(buf, fmt, args);
    return std::string(buf.data(), buf.size());
}

template <typename... Args>
inline std::string format(string_view fmt, const Args &...args)
{
    return vformat(fmt, make_format_args(args...));
}

#if AFMT_HAS_ARDUINO
// Format to an Arduino String
inline String avformat(string_view fmt, format_args args)
{
    adaptive_buffer buf;
    vformat_to(buf, fmt, args);
    return String(buf.data(), buf.size());
}

template <typename... Args>
inline String aformat(string_view fmt, const Args &...args)
{
    return avformat(fmt, make_format_args(args...));
}

template <typename... Args>
inline void print(string_view fmt, const Args &...args)
{
    AFMT_SERIAL_OUTPUT.print(avformat(fmt, make_format_args(args...)));
}

template <typename... Args>
inline void println(string_view fmt, const Args &...args)
{
    AFMT_SERIAL_OUTPUT.println(avformat(fmt, make_format_args(args...)));
}
#endif

AFMT_END_NAMESPACE

#endif // ARDUINO_FMT_H_
