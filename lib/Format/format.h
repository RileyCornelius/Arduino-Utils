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
#ifndef AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE
#define AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE 64
#endif
static_assert(AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE > 0, "AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE must be greater than 0");

// ================= Arduino FMT ==================

AFMT_BEGIN_NAMESPACE

// =============== Type Definitions ===============

// Format specifier types
enum class presentation_type
{
    // Common specifiers
    none = 0,
    string = 2, // 's'

    // Integral, bool and character specifiers
    dec = 3, // 'd'
    hex,     // 'x' or 'X'
    oct,     // 'o'
    bin,     // 'b' or 'B'
    chr,     // 'c'

    // String and pointer specifiers:
    pointer = 3, // 'p'

    // Floating-point specifiers
    exp = 1,   // 'e' or 'E' //
    fixed,     // 'f' or 'F'
    general    // 'g' or 'G'
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

// =============== Unified Buffer Class ===============

// 1. Small Buffer Optimization (SBO): Uses internal static storage for small strings
// 2. Heap allocation: Dynamically allocates memory for larger strings
// 3. External buffer: Uses pre-allocated external memory with fixed capacity
class buffer
{
private:
    enum class buffer_mode
    {
        internal_static,
        internal_heap,
        external_static
    };

    char storage_[AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE];
    char *data_; // Points to internal heap, internal or external buffer
    size_t size_;
    size_t capacity_;
    buffer_mode mode_;
    bool truncated_;

public:
    // Default constructor - adaptive mode with SBO
    buffer()
        : data_(storage_), size_(0), capacity_(AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE),
          mode_(buffer_mode::internal_static), truncated_(false) {}

    // External buffer constructor
    buffer(char *ext_data, size_t ext_capacity)
        : data_(ext_data), size_(0), capacity_(ext_capacity),
          mode_(buffer_mode::external_static), truncated_(false) {}

    // External buffer constructor for C-style arrays
    template <size_t N_arr>
    buffer(char (&arr)[N_arr])
        : data_(arr), size_(0), capacity_(N_arr),
          mode_(buffer_mode::external_static), truncated_(false) {}

    ~buffer()
    {
        if (mode_ == buffer_mode::internal_heap && data_ != nullptr)
        {
            delete[] data_;
        }
    }

    // Disable copy
    buffer(const buffer &) = delete;
    buffer &operator=(const buffer &) = delete;

    // Move constructor
    buffer(buffer &&other) noexcept
        : size_(other.size_), capacity_(other.capacity_),
          mode_(other.mode_), truncated_(other.truncated_)
    {
        if (mode_ == buffer_mode::internal_static)
        {
            for (size_t i = 0; i < size_; ++i)
            {
                storage_[i] = other.storage_[i]; // Copy SBO content from other
            }
            data_ = storage_; // Point to own storage
        }
        else if (mode_ == buffer_mode::internal_heap)
        {
            data_ = other.data_;
            other.data_ = nullptr; // Prevent other from deleting moved heap data
        }
        else // buffer_mode::external_static
        {
            data_ = other.data_;
        }

        // Reset other to a default SBO state
        other.data_ = other.storage_;
        other.size_ = 0;
        other.capacity_ = AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE;
        other.mode_ = buffer_mode::internal_static;
        other.truncated_ = false;
    }

    // Move assignment
    buffer &operator=(buffer &&other) noexcept
    {
        if (this != &other)
        {
            // Clean up current state
            if (mode_ == buffer_mode::internal_heap && data_ != nullptr)
            {
                delete[] data_;
            }

            // Move from other
            size_ = other.size_;
            capacity_ = other.capacity_;
            mode_ = other.mode_;
            truncated_ = other.truncated_;

            if (mode_ == buffer_mode::internal_static)
            {
                for (size_t i = 0; i < size_; ++i)
                {
                    storage_[i] = other.storage_[i];
                }
                data_ = storage_;
            }
            else if (mode_ == buffer_mode::internal_heap)
            {
                data_ = other.data_;
                other.data_ = nullptr;
            }
            else // buffer_mode::external_static
            {
                data_ = other.data_;
            }

            // Reset other
            other.data_ = other.storage_;
            other.size_ = 0;
            other.capacity_ = AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE;
            other.mode_ = buffer_mode::internal_static;
            other.truncated_ = false;
        }
        return *this;
    }

    AFMT_CONSTEXPR bool is_sbo() const { return mode_ == buffer_mode::internal_static; }
    AFMT_CONSTEXPR bool is_external() const { return mode_ == buffer_mode::external_static; }
    AFMT_CONSTEXPR bool is_heap() const { return mode_ == buffer_mode::internal_heap; }

    char *data() { return data_; }
    AFMT_CONSTEXPR const char *data() const { return data_; }
    AFMT_CONSTEXPR size_t size() const { return size_; }
    AFMT_CONSTEXPR size_t capacity() const { return capacity_; }
    AFMT_CONSTEXPR bool is_truncated() const { return truncated_; }

    char *c_str()
    {
        // null is all ready added
        if (buffer_mode::external_static == mode_)
        {
            return data_;
        }

        if (size_ >= capacity_)
        {
            reserve(size_ + 1);
        }

        // add null terminator
        data_[size_] = '\0';
        return data_;
    }

    void clear()
    {
        size_ = 0;
        truncated_ = false;
        // Note: Does not shrink capacity or change mode back to SBO from heap.
        // To release heap and go back to SBO, a dedicated shrink_to_fit or reset method would be needed.
    }

    void reserve(size_t new_capacity)
    {
        if (mode_ == buffer_mode::external_static || new_capacity <= capacity_)
        {
            return;
        }

        char *new_data_ptr = new char[new_capacity];
        if (new_data_ptr == nullptr) // Allocation failed
        {
            truncated_ = true; // Mark as truncated, cannot reserve
            return;
        }

        if (size_ > 0 && data_ != nullptr) // Only copy if there's existing data
        {
            for (size_t i = 0; i < size_; ++i)
            {
                new_data_ptr[i] = data_[i];
            }
        }

        if (mode_ == buffer_mode::internal_heap && data_ != nullptr)
        {
            delete[] data_;
        }
        // If mode_ was sbo, data_ pointed to storage_, no delete needed for old data_.

        data_ = new_data_ptr;
        mode_ = buffer_mode::internal_heap;
        capacity_ = new_capacity;
    }

    void push_back(const char &value)
    {
        if (size_ >= capacity_)
        {
            if (mode_ == buffer_mode::external_static)
            {
                truncated_ = true;
                return;
            }
            // Grow: If capacity is 0 (should not happen with SBO > 0), start with default SBO size. Otherwise, double.
            size_t new_cap = capacity_ == 0 ? AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE : capacity_ * 2;
            if (new_cap == 0 && AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE > 0)
                new_cap = AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE; // Ensure growth if SBO is defined
            else if (new_cap <= capacity_)
                new_cap = capacity_ + 1; // Handle overflow or non-growth case

            if (new_cap == 0)
            { // Still 0, cannot grow (e.g. AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE is 0 and initial capacity was 0)
                truncated_ = true;
                return;
            }
            reserve(new_cap);

            if (size_ >= capacity_) // Still no space (e.g., reserve failed or SBO_SIZE_PARAM was 0 and initial reserve was too small)
            {
                truncated_ = true;
                return;
            }
        }
        data_[size_++] = value;
    }

    void append(const char *begin, const char *end)
    {
        size_t count = static_cast<size_t>(end - begin);
        if (count == 0)
            return;

        if (size_ + count > capacity_)
        {
            if (mode_ == buffer_mode::external_static)
            {
                size_t available_space = capacity_ - size_;
                size_t num_to_copy = count > available_space ? available_space : count;

                if (data_ != nullptr)
                { // Ensure data_ is not null before writing
                    for (size_t i = 0; i < num_to_copy; ++i)
                    {
                        data_[size_ + i] = begin[i];
                    }
                }
                size_ += num_to_copy;

                if (count > available_space)
                {
                    truncated_ = true;
                }
                return;
            }

            size_t required_cap = size_ + count;
            size_t new_cap = capacity_ == 0 ? AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE : capacity_;
            if (new_cap == 0 && AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE > 0)
                new_cap = AFMT_DEFAULT_INTERNAL_SMALL_BUFFER_SIZE;
            else if (new_cap == 0)
                new_cap = 16; // Fallback if SBO is 0 and capacity is 0

            while (new_cap < required_cap)
            {
                new_cap *= 2;
                if (new_cap <= capacity_)
                {
                    new_cap = required_cap; // Grow to at least required
                    break;
                }
            }
            reserve(new_cap);

            if (size_ + count > capacity_) // Still not enough space
            {
                // Append what fits and truncate
                size_t available_space = capacity_ - size_;
                size_t num_to_copy = count > available_space ? available_space : count;
                if (data_ != nullptr)
                {
                    for (size_t i = 0; i < num_to_copy; ++i)
                    {
                        data_[size_ + i] = begin[i];
                    }
                }
                size_ += num_to_copy;
                truncated_ = true;
                return;
            }
        }

        if (data_ != nullptr)
        { // Ensure data_ is not null
            for (size_t i = 0; i < count; ++i)
            {
                data_[size_ + i] = begin[i];
            }
        }
        size_ += count;
    }

    char &operator[](size_t pos)
    {
        return data_[pos];
    }

    const char &operator[](size_t pos) const
    {
        return data_[pos];
    }
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
        specs.fill = c;
        ++begin;
        c = *begin;
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
        specs.set_sign(c == '+' ? sign::plus : c == '-' ? sign::minus
                                                        : sign::space);
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
        specs.alignment = align::numeric;
        specs.fill = '0';
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
        specs.precision = parse_nonnegative_int(begin, end);
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
        // case '?': // Removed debug specifier parsing
        //     specs.set_type(presentation_type::debug);
        //     break;
        default:
            // Unknown presentation type
            break;
        }
        ++begin;
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
    AFMT_CONSTEXPR format_arg_value(long value) : type_(type::long_long_type) { value_.long_long_value = value; }
    AFMT_CONSTEXPR format_arg_value(unsigned long value) : type_(type::ulong_long_type) { value_.ulong_long_value = value; }
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

// Convert integer to string
template <typename T>
void to_string(T value, buffer &out, format_specs specs)
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

// Convert float to string
inline void to_string(double value, buffer &out, format_specs specs)
{
    // Handle special cases
    if (value != value)
    {
        out.append(specs.upper ? "NAN" : "nan", specs.upper ? "NAN" + 3 : "nan" + 3);
        return;
    }

    if (value == 0.0)
    {
        out.push_back('0');
        
        // For general format, default precision is 6, but trailing zeros are removed
        if (specs.type == presentation_type::general || specs.type == presentation_type::none)
        {
            // For {:g}, 0.0 is just "0"
            return;
        }
        
        int precision = specs.precision >= 0 ? specs.precision : (specs.type == presentation_type::general ? 6 : 2);
        if (precision > 0 && (specs.type == presentation_type::fixed || specs.type == presentation_type::exp))
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

    // Check if value is infinite by comparing with known large finite values
    const double max_finite = 1.7976931348623157e+308;
    bool is_infinite = false;

    if (value > 0)
    {
        is_infinite = (value > max_finite) || (value == value * 2.0 && value != 0.0);
    }
    else if (value < 0)
    {
        is_infinite = (value < -max_finite) || (value == value * 2.0 && value != 0.0);
    }

    if (is_infinite)
    {
        if (value < 0)
        {
            out.push_back('-');
        }
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

    // Default type for float/double is general if 'none'
    if (specs.type == presentation_type::none)
    {
        // For default {} format, use a different approach than explicit {:g}
        // For small integers represented as doubles, show them as integers
        if (value == static_cast<double>(static_cast<long long>(value)) && value < 1e15)
        {
            format_specs int_specs;
            to_string(static_cast<long long>(value), out, int_specs);
            return;
        }
        
        // but doesn't show floating-point artifacts
        specs.type = presentation_type::general;
        
        if (specs.precision == -1)
        {
            // Use a precision that captures the meaningful digits without artifacts
            specs.precision = 6; // Default for general format
            
            // For values that are clearly meant to have decimal places, 
            if (value > 0.001 && value < 1000000.0)
            {
                // Check if this looks like it has meaningful decimal places
                double fractional_part = value - static_cast<long long>(value);
                if (fractional_part > 0.0001)
                {
                    // This has significant fractional content, use higher precision
                    specs.precision = 15; // Close to double precision
                }
            }
        }
    }

    // Handle general format
    if (specs.type == presentation_type::general)
    {
        int p = specs.precision >= 0 ? specs.precision : 6; // Default precision for explicit g is 6
        if (p == 0) p = 1; // Precision 0 for g means 1 significant digit

        // Calculate the exponent
        int exp_val = 0;
        if (value >= 1.0)
        {
            double temp = value;
            while (temp >= 10.0)
            {
                temp /= 10.0;
                exp_val++;
            }
        }
        else if (value > 0.0)
        {
            double temp = value;
            while (temp < 1.0)
            {
                temp *= 10.0;
                exp_val--;
            }
        }

        // Choose between fixed and scientific notation
        bool use_scientific = (exp_val < -4 || exp_val >= p);

        if (use_scientific)
        {
            // Use scientific notation with precision p-1
            format_specs sci_specs = specs;
            sci_specs.type = presentation_type::exp;
            sci_specs.precision = p - 1;
            to_string(value, out, sci_specs);
        }
        else
        {
            // Use fixed notation with precision p-1-exp_val
            format_specs fixed_specs = specs;
            fixed_specs.type = presentation_type::fixed;
            fixed_specs.precision = p - 1 - exp_val;
            if (fixed_specs.precision < 0) fixed_specs.precision = 0;
            
            // Format with fixed notation
            buffer temp;
            to_string(value, temp, fixed_specs);
            
            // Remove trailing zeros and decimal point if not needed
            size_t len = temp.size();
            const char* data = temp.data();
            
            // Find decimal point
            size_t decimal_pos = len;
            for (size_t i = 0; i < len; ++i)
            {
                if (data[i] == '.')
                {
                    decimal_pos = i;
                    break;
                }
            }
            
            if (decimal_pos < len)
            {
                // Remove trailing zeros after decimal point
                while (len > decimal_pos + 1 && data[len - 1] == '0')
                {
                    len--;
                }
                
                // Remove decimal point if no fractional part remains
                if (len == decimal_pos + 1)
                {
                    len--;
                }
            }
            
            // Copy the trimmed result
            out.append(data, data + len);
        }
        return;
    }

    // Handle exp and fixed formatting
    int precision = specs.precision >= 0 ? specs.precision : 2;

    if (specs.type == presentation_type::exp)
    {
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
            // Create temporary default specs for integer part
            format_specs int_specs;
            to_string(int_part, out, int_specs);
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

inline void pad_before(buffer &out, int width, char fill, align alignment, int content_width)
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

inline void pad_after(buffer &out, int width, char fill, align alignment, int content_width)
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

// Forward declaration for format_value_dispatch
template <typename T, int DispatchSelector>
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, DispatchSelector>);

// Format a single argument with proper padding
template <typename T>
void format_value(const T &value, buffer &out, format_specs specs)
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
        buffer temp;
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
        buffer temp;
        format_specs temp_specs = specs;
        temp_specs.width = 0; // Disable width for the measurement pass to prevent recursion

        format_value(value, temp, temp_specs);
        int content_width = static_cast<int>(temp.size());

        // Apply padding
        pad_before(out, specs.width, specs.fill, specs.alignment, content_width);

        // Copy the formatted content
        out.append(temp.data(), temp.data() + temp.size());

        // Apply padding after if needed
        pad_after(out, specs.width, specs.fill, specs.alignment, content_width);

        return;
    }

    // Format based on type - replace if constexpr with type dispatch
    // Split the template specialization using tag dispatch
    format_value_dispatch_impl(value, out, specs, typename std::conditional < std::is_integral<T>::value && !std::is_same<T, bool>::value && !std::is_same<T, char>::value,
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
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 1>) // Integral
{
    to_string(value, out, specs);
}

template <typename T>
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 2>) // Bool
{
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
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 3>) // Char
{
    out.push_back(value);
}

template <typename T>
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 4>) // Float
{
    to_string(static_cast<double>(value), out, specs);
}

template <typename T>
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 5>) // Double
{
    to_string(value, out, specs);
}

template <typename T>
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 6>) // C-string
{
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
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 7>) // string_view
{
    if (value.data())
    {
        for (size_t i = 0; i < value.size(); ++i)
        {
            out.push_back(value.data()[i]);
        }
    }
}

template <typename T>
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 8>) // Pointer
{
    if (value)
    {
        out.push_back('0');
        out.push_back('x');
        uint32_t ptr_val = reinterpret_cast<uint32_t>(value);

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
void format_value_dispatch_impl(const T &value, buffer &out, format_specs specs, std::integral_constant<int, 0>) // Fallback
{
    out.push_back('?');
}

// Visitor for format_arg_value
struct format_visitor_impl
{
    buffer &out;
    format_specs specs;

    AFMT_CONSTEXPR format_visitor_impl(buffer &out_, const format_specs &specs_)
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
                                           buffer &out)
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
        arg.visit(format_visitor_impl(out, specs));
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
    arg.visit(format_visitor_impl(out, specs));

    return begin + 1;
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

struct format_to_result
{
    char *out;
    bool truncated;

    AFMT_CONSTEXPR operator char *() const
    {
        return out;
    }
};

// ================ Core format Functions ================

// Parse a format string and write output to the buffer
inline void vformat_to(buffer &out, string_view fmt, format_args args)
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

// Core formatting function vformat_to_n
inline format_to_result vformat_to_n(char *out, size_t n, string_view fmt, format_args args)
{
    if (n == 0)
    {
        return format_to_result{out, true}; // No space for anything, not even null.
    }

    buffer temp_buffer(out, n);         // Operates in external mode.
    vformat_to(temp_buffer, fmt, args); // Writes formatted string into `out` via `temp_buffer`. Does NOT null terminate.

    size_t content_len = temp_buffer.size();
    bool format_overflowed = temp_buffer.is_truncated(); // True if vformat_to tried to write > n chars.

    if (format_overflowed)
    {
        // Content was longer than n. temp_buffer (and thus `out`) contains the first n chars.
        // Overwrite last char of `out` to place the null terminator. Result is truncated.
        out[n - 1] = '\0';
        return format_to_result{out, true};
    }
    else
    {
        // All content fit, or exactly filled the buffer. content_len <= n.
        if (content_len < n)
        {
            // Content fit and there's space in `out` for null.
            out[content_len] = '\0';
            return format_to_result{out, false}; // Not truncated
        }
        else
        {
            // Content exactly filled n bytes (content_len == n).
            // No space for a separate null, must overwrite last char of `out`. Result is truncated.
            out[n - 1] = '\0';
            return format_to_result{out, true};
        }
    }
}

// =============== Public API Functions ===============

// Format to buffer
template <typename... Args>
inline void format_to(buffer &out, string_view fmt, const Args &...args)
{
    vformat_to(out, fmt, make_format_args(args...));
}

// Format to a fixed-size array using external char[]
template <size_t N, typename... Args>
inline format_to_result format_to(char (&out)[N], string_view fmt, const Args &...args)
{
    return vformat_to_n(out, N, fmt, make_format_args(args...));
}

// Format to a buffer with size limitation using external buffer mode
template <typename... Args>
inline format_to_result format_to_n(char *out, size_t n, string_view fmt, const Args &...args)
{
    return vformat_to_n(out, n, fmt, make_format_args(args...));
}

// Get the size that would be required for formatting
template <typename... Args>
inline size_t formatted_size(string_view fmt, const Args &...args)
{
    buffer buf;
    vformat_to(buf, fmt, make_format_args(args...));
    return buf.size(); // excluding null terminator
}

inline std::string vformat(string_view fmt, format_args args)
{
    buffer buf;
    vformat_to(buf, fmt, args);
    return std::string(buf.data(), buf.size());
}

// Format to std::string
template <typename... Args>
inline std::string format(string_view fmt, const Args &...args)
{
    return vformat(fmt, make_format_args(args...));
}

// Format for Arduino String
#if AFMT_HAS_ARDUINO
inline String avformat(string_view fmt, format_args args)
{
    buffer buf;
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
