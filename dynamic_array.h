#pragma once
#include <algorithm>
#include <stdexcept>

template <class T>
class DynamicArray {
private: 
    T *data;
    int size;
public:
    DynamicArray(int s) : size(s), data(new T[s]) {}
    DynamicArray(T *items, int count) : DynamicArray(count) {
        std::copy(items, items + count, data);
    }
    DynamicArray(const DynamicArray &other) : DynamicArray(other.size) {
        std::copy(other.data, other.data + size, data);
    }
    T Get(int index) const { 
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index]; 
    }
    int GetSize() const { return size; }
    void Set(int index, T value) { 
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        data[index] = value; 
    }
    void Resize(int new_size) {
        if (new_size <= 0) {
            throw std::invalid_argument("Size must be positive");
        }

        T *new_data = new T[new_size];

        for (int i = 0; i < ((size < new_size) ? size : new_size); i++) {
            new_data[i] = data[i];
        }
        delete[] data;
        
        data = new_data;
        size = new_size;
    }
    ~DynamicArray() { delete[] data; }
};