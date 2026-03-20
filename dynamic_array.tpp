#include "dynamic_array.h"

template <class T>
DynamicArray<T>::DynamicArray(int s) : size(s), data(new T[s]()) {}

template <class T>
DynamicArray<T>::DynamicArray(T *items, int count) : DynamicArray(count) {
    std::copy(items, items + count, data);
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray &other) : DynamicArray(other.size) {
    std::copy(other.data, other.data + size, data);
}

template <class T>
T DynamicArray<T>::Get(int index) const {
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

template <class T>
void DynamicArray<T>::Resize(int new_size) {
    if (new_size <= 0) throw std::invalid_argument("Size must be positive");
    T *new_data = new T[new_size];
    for (int i = 0; i < ((size < new_size) ? size : new_size); i++) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    size = new_size;
}

template <class T>
DynamicArray<T>::~DynamicArray() { delete[] data; }