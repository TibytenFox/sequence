#pragma once

template <class T>
class DynamicArray {
public:
    // ---------- Constructors / Destructor ----------
    DynamicArray();                              // Empty array
    explicit DynamicArray(int s);                // Array of size s (default-initialized)
    DynamicArray(T *items, int count);           // Copy from C-array
    DynamicArray(const DynamicArray &other);     // Copy constructor
    ~DynamicArray();                             // Destructor

    // ---------- Accessors ----------
    T &Get(int index);                     // Get element at index
    const T &Get(int index) const;
    int GetSize() const;                         // Return current size
    void Set(int index, T value);                // Assign value at index
    void Resize(int new_size);                   // Change size (preserve existing data)

    // ---------- Operators ----------
    DynamicArray<T> &operator=(const DynamicArray<T> &other);
private:
    T *data;     // Raw pointer to heap-allocated array
    int size;    // Number of elements
};

#include "dynamic_array.tpp"