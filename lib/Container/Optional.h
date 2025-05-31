#pragma once

#include <new>      // For placement new
#include <utility>  // For std::forward
#include <assert.h>  // For assert

#define OPTIONAL_ASSERT(condition) assert(condition && "Optional does not have a value")

// Define Nullopt struct and a global nullopt instance
struct Nullopt {};
constexpr Nullopt nullopt{};

// Storage base manages the actual storage and tracks whether value is present
template <typename T>
struct OptionalStorageBase 
{
    union 
    {
        struct {} dummy_;  // Active when no value
        T value_;         // Active when there is a value
    };
    bool hasValue_;

    constexpr OptionalStorageBase() 
        : dummy_(), hasValue_(false) {}

    template <typename... Args>
    constexpr OptionalStorageBase(bool, Args&&... args)
        : value_(std::forward<Args>(args)...), hasValue_(true) {}

    ~OptionalStorageBase() 
    {
        if (hasValue_) 
        {
            value_.~T();
            hasValue_ = false;
        }
    }
};

// Operations base provides construction/destruction operations
template <typename T>
struct OptionalOperationsBase : OptionalStorageBase<T> 
{
    using OptionalStorageBase<T>::OptionalStorageBase;

    template <typename... Args>
    void construct(Args&&... args) 
    {
        // Placement new: constructs T in the exact memory location of value_
        new (&this->value_) T(std::forward<Args>(args)...);
        this->hasValue_ = true;
    }

    void destroy() 
    {
        if (this->hasValue_) 
        {
            this->value_.~T();
            this->hasValue_ = false;
        }
    }

    void reset() 
    {
        destroy();
    }

    bool hasValue() const 
    {
        return this->hasValue_;
    }

    T& getValue() 
    {
        return this->value_;
    }

    const T& getValue() const 
    {
        return this->value_;
    }
};

template <typename T>
class Optional : private OptionalOperationsBase<T> 
{
private:
    using base = OptionalOperationsBase<T>;

public:
    using value_type = T;

    constexpr Optional() = default;

    constexpr Optional(Nullopt) : base() {}

    template <typename U = T,
              typename = typename std::enable_if<
                  std::is_constructible<T, U&&>::value &&
                  !std::is_same<typename std::decay<U>::type, Optional<T>>::value &&
                  !std::is_same<typename std::decay<U>::type, Nullopt>::value>::type>
    constexpr Optional(U&& value) 
        : base(true, std::forward<U>(value)) {}

    Optional(const Optional& other) : base() 
    {
        if (other.hasValue()) 
        {
            this->construct(other.getValue());
        }
    }

    ~Optional() = default;

    Optional& operator=(Nullopt) 
    {
        this->reset();
        return *this;
    }

    template <typename U = T,
              typename = typename std::enable_if<
                  std::is_constructible<T, U&&>::value &&
                  std::is_assignable<T&, U&&>::value &&
                  !std::is_same<typename std::decay<U>::type, Optional<T>>::value &&
                  !std::is_same<typename std::decay<U>::type, Nullopt>::value>::type>
    Optional& operator=(U&& value) 
    {
        if (this->hasValue()) 
        {
            this->getValue() = std::forward<U>(value);
        } else 
        {
            this->construct(std::forward<U>(value));
        }
        return *this;
    }

    Optional& operator=(const Optional& other) 
    {
        if (this == &other) 
        {
            return *this;
        }

        if (other.hasValue()) 
        {
            if (this->hasValue()) 
            {
                this->getValue() = other.getValue();
            } else 
            {
                this->construct(other.getValue());
            }
        } else 
        {
            this->reset();
        }
        return *this;
    }

    bool hasValue() const
    {
        return this->hasValue_;
    }

    T& value()
    {
        OPTIONAL_ASSERT(this->hasValue());
        return this->getValue();
    }

    const T& value() const 
    {
        OPTIONAL_ASSERT(this->hasValue());
        return this->getValue();
    }

    T valueOr(const T& defaultValue) const 
    {
        return this->hasValue() ? this->getValue() : defaultValue;
    }

    void reset()
    {
        this->destroy();
    }

    explicit operator bool() const
    {
        return this->hasValue();
    }

    T& operator*() 
    {
        OPTIONAL_ASSERT(this->hasValue());
        return this->getValue();
    }

    const T& operator*() const 
    {
        OPTIONAL_ASSERT(this->hasValue());
        return this->getValue();
    }

    T* operator->() 
    {
        OPTIONAL_ASSERT(this->hasValue());
        return &this->getValue();
    }

    const T* operator->() const
    {
        OPTIONAL_ASSERT(this->hasValue());
        return &this->getValue();
    }

    bool operator==(const Optional<T>& other) const
    {
        if (this->hasValue() != other.hasValue())
        {
            return false;
        }
        if (!this->hasValue()) 
        {
            return true; // Both are empty
        }
        return this->getValue() == other.getValue();
    }

    bool operator!=(const Optional<T>& other) const 
    {
        return !(*this == other);
    }
};