#pragma once

template <typename T, typename Event>
struct Result
{
    union
    {
        T value;
        Event error;
    };
    bool isError;

    constexpr Result(T v) : value(v), isError(false) {}
    constexpr Result(Event err) : error(err), isError(true) {}
    bool hasError() const { return isError; }
    bool isOk() const { return !isError; }
};

template <typename T>
struct Optional
{
    T value;
    bool hasValue;

    constexpr Optional() : value{}, hasValue(false) {}
    constexpr Optional(T t) : value(t), hasValue(true) {}
    operator bool() const { return hasValue; }
    auto operator==(T t) const -> bool { return hasValue && value == t; }
    auto operator!=(T t) const -> bool { return !hasValue || value != t; }
};