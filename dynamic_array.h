#pragma once
#include <algorithm>
#include <stdexcept>

template <class T>
class DynamicArray {
private:
    T *data;
    int size;
public:
    DynamicArray(int s);
    DynamicArray(T *items, int count);
    DynamicArray(const DynamicArray &other);
    T Get(int index) const;
    int GetSize() const;
    void Set(int index, T value);
    void Resize(int new_size);
    ~DynamicArray();
};

#include "dynamic_array.tpp"