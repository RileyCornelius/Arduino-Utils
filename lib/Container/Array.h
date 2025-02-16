#pragma once

#include <Arduino.h>
#include <assert.h>

template <typename T>
class Array
{
private:
    T *array;
    uint32_t length;

public:
    template <uint32_t size_>
    Array(T (&ptr)[size_]) : array(ptr), length(size_) {}

    uint32_t size() const { return length; }

    T *data() { return array; }
    const T *data() const { return array; }

    T &front() { return array[0]; }
    const T &front() const { return array[0]; }

    T &back() { return array[length - 1]; }
    const T &back() const { return array[length - 1]; }

    T &at(uint32_t index)
    {
        assert(index < length);
        return array[index];
    }
    const T &at(uint32_t index) const
    {
        assert(index < length);
        return array[index];
    }

    T &operator[](uint32_t index) { return at(index); }
    const T &operator[](uint32_t index) const { return at(index); }
};