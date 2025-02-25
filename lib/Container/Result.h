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