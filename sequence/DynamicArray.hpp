#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "Exceptions.hpp"

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
    const T &Get(int index) const;               // Get element at index
    int GetSize() const;                         // Return current size
    void Set(int index, T value);                // Assign value at index
    void Resize(int new_size);                   // Change size (preserve existing data)

    // ---------- Operators ----------
    DynamicArray<T> &operator=(const DynamicArray<T> &other);
    T &operator[](int index);
private:
    T *data;     // Raw pointer to heap-allocated array
    int size;    // Number of elements
};

#include "DynamicArray.tpp"
#endif // DYNAMIC_ARRAY_HPP