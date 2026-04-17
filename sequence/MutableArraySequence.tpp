#include "MutableArraySequence.hpp"

template <class T>
MutableArraySequence<T> &MutableArraySequence<T>::operator=(const MutableArraySequence<T> &other) {
    if (this != &other) {
        this->items = other.items;
    }
    return *this;
}

template <class T>
bool MutableArraySequence<T>::operator==(const MutableArraySequence<T> &other) const {
    if (this->GetLength() != other.GetLength()) return false;
    for (int i = 0; i < this->GetLength(); ++i) {
        if (this->Get(i) != other.Get(i)) return false;
    }
    return true;
}

template <class T>
bool MutableArraySequence<T>::operator!=(const MutableArraySequence<T> &other) const {
    return !(*this == other);
}

template <class T>
T &MutableArraySequence<T>::operator[](int index) {
    return this->items[index];
}

template <class T>
const T &MutableArraySequence<T>::operator[](int index) const {
    return this->items.Get(index);
}

template <class T>
MutableArraySequence<T> MutableArraySequence<T>::operator+(const MutableArraySequence<T> &other) const {
    MutableArraySequence<T> result(*this);
    for (int i = 0; i < other.GetLength(); ++i) {
        result.Append(other.Get(i));
    }
    return result;
}

template <class T>
MutableArraySequence<T> &MutableArraySequence<T>::operator+=(const T &value) {
    this->Append(value);
    return *this;
}

template <class T>
MutableArraySequence<T> &MutableArraySequence<T>::operator+=(const MutableArraySequence<T> &other) {
    for (int i = 0; i < other.GetLength(); ++i) {
        this->Append(other.Get(i));
    }
    return *this;
}
