#pragma once

#include <utility>

template <class T>
class Vector
{
private:
    T *data;
    uint32_t length;
    uint32_t capacity;

    void resize()
    {
        uint32_t newCapacity = (uint32_t)(capacity * 1.75);
        T *newData = new T[newCapacity];
        for (uint32_t i = 0; i < length; i++)
        {
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Vector(uint32_t initialCapacity = 5)
        : data(new T[initialCapacity]), length(0), capacity(initialCapacity) {}

    ~Vector() { delete[] data; }

    void push(const T &item)
    {
        if (length == capacity)
            resize();

        data[length++] = item;
    }

    T pop() { return data[--length]; }
    const T pop() const { return data[--length]; }

    T &front() { return data[0]; }
    const T &front() const { return data[0]; }

    T &back() { return data[length - 1]; }
    const T &back() const { return data[length - 1]; }

    T &at(uint32_t index)
    {
        assert(index < length);
        return data[index];
    }
    const T &at(uint32_t index) const
    {
        assert(index < length);
        return data[index];
    }

    bool isEmpty() const { return length == 0; }
    void clear() { length = 0; }
    uint32_t size() const { return length; }
    uint32_t capacity() const { return capacity; }

    T &operator[](uint32_t index) { return at(index); }
    const T &operator[](uint32_t index) const { return at(index); }
};