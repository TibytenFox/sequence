#include <algorithm>
#include <stdexcept>
#include "dynamic_array.hpp"

// ---------- Constructors and Destructor ----------

template <class T>
DynamicArray<T>::DynamicArray() : size(0), data(new T[0]()) {}

template <class T>
DynamicArray<T>::DynamicArray(int s) : size(s) {
    if (s < 0) throw std::invalid_argument("Size must be positive");
    data = new T[size](); // Value-initialization (zero for primitives)
}

template <class T>
DynamicArray<T>::DynamicArray(T *items, int count) : DynamicArray(count) {
    if (count < 0) throw std::invalid_argument("Count must be non-negative");
    std::copy(items, items + count, data);
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray &other) : DynamicArray(other.size) {
    std::copy(other.data, other.data + size, data);
}

template <class T>
DynamicArray<T>::~DynamicArray() { delete[] data; }

// ---------- Getters and Setters ----------

template <class T>
T &DynamicArray<T>::Get(int index) {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
    return data[index];
}

template <class T>
const T &DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
    return data[index];
}

template <class T>
int DynamicArray<T>::GetSize() const { return size; }

template <class T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
    data[index] = value;
}

/* 
    If new_size > size, new elements are default-initialized
    If new_size < size, extra elements are lost
*/
template <class T>
void DynamicArray<T>::Resize(int new_size) {
    if (new_size < 0) throw std::invalid_argument("Size must be positive");
    T *new_data = new T[new_size];
    // Copy min(old size, new size) elements
    for (int i = 0; i < ((size < new_size) ? size : new_size); i++) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    size = new_size;
}

// ---------- Operators ----------

template <class T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray<T> &other) {
    if (this != &other) {
        size = other.size;

        if (data) delete[] data;
        data = new T[size]();

        std::copy(other.data, other.data + size, data);
    }
    return *this;
}
