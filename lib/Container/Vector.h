#pragma once

#include <utility>
#include <assert.h>
#include <cstdint>

template <class T>
class Vector
{
public:
    // Standard typedefs
    typedef T                               value_type;
    typedef T&                              reference;
    typedef const T&                        const_reference;
    typedef T*                              pointer;
    typedef const T*                        const_pointer;
    typedef T*                              iterator;
    typedef const T*                        const_iterator;
    typedef uint32_t                        size_type;
    typedef int32_t                         difference_type;

private:
    T *data;
    uint32_t length;
    uint32_t max_capacity;

    void resize_internal()
    {
        uint32_t newCapacity = (uint32_t)(max_capacity * 1.75);
        T *newData = new T[newCapacity];
        for (uint32_t i = 0; i < length; i++)
        {
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
        max_capacity = newCapacity;
    }

public:
    Vector(uint32_t initialCapacity = 5)
        : data(new T[initialCapacity]), length(0), max_capacity(initialCapacity) {}

    ~Vector() { delete[] data; }

    // Copy constructor
    Vector(const Vector& other)
        : data(new T[other.max_capacity]), length(other.length), max_capacity(other.max_capacity)
    {
        for (uint32_t i = 0; i < length; i++)
        {
            data[i] = other.data[i];
        }
    }

    // Copy assignment
    Vector& operator=(const Vector& other)
    {
        if (this != &other)
        {
            delete[] data;
            max_capacity = other.max_capacity;
            length = other.length;
            data = new T[max_capacity];
            for (uint32_t i = 0; i < length; i++)
            {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move constructor
    Vector(Vector&& other) noexcept
        : data(other.data), length(other.length), max_capacity(other.max_capacity)
    {
        other.data = nullptr;
        other.length = 0;
        other.max_capacity = 0;
    }

    // Move assignment
    Vector& operator=(Vector&& other) noexcept
    {
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            length = other.length;
            max_capacity = other.max_capacity;
            other.data = nullptr;
            other.length = 0;
            other.max_capacity = 0;
        }
        return *this;
    }

    // Iterator methods
    iterator begin() { return data; }
    const_iterator begin() const { return data; }
    const_iterator cbegin() const { return data; }
    
    iterator end() { return data + length; }
    const_iterator end() const { return data + length; }
    const_iterator cend() const { return data + length; }

    // Capacity methods
    bool isEmpty() const { return length == 0; }
    bool empty() const { return length == 0; }
    void clear() { length = 0; }
    uint32_t size() const { return length; }
    uint32_t capacity() const { return max_capacity; }
    bool full() const { return length == max_capacity; }

    // Element access
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

    T &operator[](uint32_t index) { return at(index); }
    const T &operator[](uint32_t index) const { return at(index); }

    T &front() { return data[0]; }
    const T &front() const { return data[0]; }

    T &back() { return data[length - 1]; }
    const T &back() const { return data[length - 1]; }

    T* get_data() { return data; }
    const T* get_data() const { return data; }

    // Modifiers
    void push_back(const T &item)
    {
        if (length == max_capacity)
            resize_internal();
        data[length++] = item;
    }

    void push_back(T &&item)
    {
        if (length == max_capacity)
            resize_internal();
        data[length++] = std::move(item);
    }

    template<typename... Args>
    reference emplace_back(Args&&... args)
    {
        if (length == max_capacity)
            resize_internal();
        new (data + length) T(std::forward<Args>(args)...);
        ++length;
        return back();
    }

    void pop_back() 
    { 
        assert(length > 0);
        --length;
    }

    T pop() 
    { 
        assert(length > 0);
        return data[--length]; 
    }

    void push(const T &item) { push_back(item); }

    // Insert element at position
    iterator insert(const_iterator position, const T& value)
    {
        assert(position >= begin() && position <= end());
        uint32_t index = position - begin();
        
        if (length == max_capacity)
            resize_internal();

        // Shift elements to the right
        for (uint32_t i = length; i > index; --i)
        {
            data[i] = std::move(data[i - 1]);
        }
        
        data[index] = value;
        ++length;
        return data + index;
    }

    // Erase element at position
    iterator erase(const_iterator position)
    {
        assert(position >= begin() && position < end());
        uint32_t index = position - begin();
        
        // Shift elements to the left
        for (uint32_t i = index; i < length - 1; ++i)
        {
            data[i] = std::move(data[i + 1]);
        }
        
        --length;
        return data + index;
    }

    // Erase range
    iterator erase(const_iterator first, const_iterator last)
    {
        assert(first >= begin() && first <= end());
        assert(last >= first && last <= end());
        
        uint32_t start_index = first - begin();
        uint32_t end_index = last - begin();
        uint32_t erase_count = end_index - start_index;
        
        // Shift elements to the left
        for (uint32_t i = start_index; i < length - erase_count; ++i)
        {
            data[i] = std::move(data[i + erase_count]);
        }
        
        length -= erase_count;
        return data + start_index;
    }

    // Resize vector
    void resize(uint32_t new_size)
    {
        if (new_size > max_capacity)
        {
            reserve(new_size);
        }
        length = new_size;
    }

    void resize(uint32_t new_size, const T& value)
    {
        uint32_t old_size = length;
        resize(new_size);
        
        // Fill new elements with value
        for (uint32_t i = old_size; i < new_size; ++i)
        {
            data[i] = value;
        }
    }

    // Reserve capacity
    void reserve(uint32_t new_capacity)
    {
        if (new_capacity > max_capacity)
        {
            T* new_data = new T[new_capacity];
            for (uint32_t i = 0; i < length; ++i)
            {
                new_data[i] = std::move(data[i]);
            }
            delete[] data;
            data = new_data;
            max_capacity = new_capacity;
        }
    }
};