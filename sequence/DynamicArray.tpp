#include "DynamicArray.hpp"

// ---------- Constructors and Destructor ----------

template <class T>
DynamicArray<T>::DynamicArray() : data(new T[0]()), size(0) {}

template <class T>
DynamicArray<T>::DynamicArray(int s) : size(s) {
    if (s < 0) throw IndexOutOfRange("DynamicArray(): Size must be positive");
    this->data = new T[size](); // Value-initialization (zero for primitives)
}

template <class T>
DynamicArray<T>::DynamicArray(T *items, int count) : DynamicArray(count) {
    if (count < 0) throw IndexOutOfRange("DynamicArray(): Count must be non-negative");
    std::copy(items, items + count, this->data);
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray &other) : DynamicArray(other.size) {
    std::copy(other.data, other.data + size, this->data);
}

template <class T>
DynamicArray<T>::~DynamicArray() { delete[] this->data; }

// ---------- Getters and Setters ----------

template <class T>
const T &DynamicArray<T>::Get(int index) const {
    if (this->size == 0) throw EmptyCollectionError("Get(): Emprty collection");
    if (index < 0 || index >= this->size) throw IndexOutOfRange("Get(): Index out of range");
    return this->data[index];
}

template <class T>
int DynamicArray<T>::GetSize() const { return this->size; }

template <class T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= this->size) throw IndexOutOfRange("Set(): Index out of range");
    this->data[index] = value;
}

/* 
    If new_size > size, new elements are default-initialized
    If new_size < size, extra elements are lost
*/
template <class T>
void DynamicArray<T>::Resize(int new_size) {
    if (new_size < 0) throw IndexOutOfRange("Resize(): Size must be positive");
    T *new_data = new T[new_size];
    // Copy min(old size, new size) elements
    for (int i = 0; i < ((this->size < new_size) ? this->size : new_size); i++) {
        new_data[i] = this->data[i];
    }
    delete[] this->data;
    this->data = new_data;
    this->size = new_size;
}

template <class T>
IEnumerator<T> *DynamicArray<T>::GetEnumerator() const {
    return new Enumerator(this);
}

// ---------- Operators ----------

template <class T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray<T> &other) {
    if (this != &other) {
        this->size = other.size;

        if (this->data) delete[] this->data;
        this->data = new T[size]();

        std::copy(other.data, other.data + size, this->data);
    }
    return *this;
}

template <class T>
T &DynamicArray<T>::operator[](int index) {
    if (index < 0 || index >= this->GetSize()) {
        throw IndexOutOfRange("Operator[]: Index out of range");
    }
    return this->data[index];
}
