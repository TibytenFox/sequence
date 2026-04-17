#include "MutableListSequence.hpp"

template <class T>
MutableListSequence<T> &MutableListSequence<T>::operator=(const MutableListSequence<T> &other) {
    if (this != &other) {
        this->items = other.items;
    }
    return *this;
}

template <class T>
bool MutableListSequence<T>::operator==(const MutableListSequence<T> &other) const {
    if (this->GetLength() != other.GetLength()) return false;
    for (int i = 0; i < this->GetLength(); ++i) {
        if (this->Get(i) != other.Get(i)) return false;
    }
    return true;
}

template <class T>
bool MutableListSequence<T>::operator!=(const MutableListSequence<T> &other) const {
    return !(*this == other);
}

template <class T>
T &MutableListSequence<T>::operator[](int index) {
    if (index < 0 || index >= this->GetLength()) throw IndexOutOfRange("Operator[]: Index out of range");
    typename ListSequence<T>::Enumerator *current = this->begin();
    for (int i = 0; i < index; ++i) {
        current->Increment();
    }
    T &value = current->Dereference();
    delete current;
    return value;
}

template <class T>
const T &MutableListSequence<T>::operator[](int index) const {
    if (index < 0 || index >= this->GetLength()) throw IndexOutOfRange("Operator[]: Index out of range");
    typename ListSequence<T>::ConstEnumerator *current = this->begin();
    for (int i = 0; i < index; ++i) {
        current->Increment();
    }
    const T &value = current->Dereference();
    delete current;
    return value;
}

template <class T>
MutableListSequence<T> MutableListSequence<T>::operator+(const MutableListSequence<T> &other) const {
    MutableListSequence<T> result(*this);
    for (int i = 0; i < other.GetLength(); ++i) {
        result.Append(other.Get(i));
    }
    return result;
}

template <class T>
MutableListSequence<T> &MutableListSequence<T>::operator+=(const T &value) {
    this->Append(value);
    return *this;
}

template <class T>
MutableListSequence<T> &MutableListSequence<T>::operator+=(const MutableListSequence<T> &other) {
    for (int i = 0; i < other.GetLength(); ++i) {
        this->Append(other.Get(i));
    }
    return *this;
}
