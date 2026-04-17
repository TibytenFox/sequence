#include "ImmutableListSequence.hpp"

template <class T>
ImmutableListSequence<T> &ImmutableListSequence<T>::operator=(const ImmutableListSequence<T> &other) {
    if (this != &other) {
        this->items = other.items;
    }
    return *this;
}

template <class T>
bool ImmutableListSequence<T>::operator==(const ImmutableListSequence<T> &other) const {
    if (this->GetLength() != other.GetLength()) return false;
    for (int i = 0; i < this->GetLength(); ++i) {
        if (this->Get(i) != other.Get(i)) return false;
    }
    return true;
}

template <class T>
bool ImmutableListSequence<T>::operator!=(const ImmutableListSequence<T> &other) const {
    return !(*this == other);
}

template <class T>
T &ImmutableListSequence<T>::operator[](int index) {
    if (index < 0 || index >= this->GetLength()) throw IndexOutOfRange("Operator[]: Index out of range");
    typename LinkedList<T>::Node *current = this->items.GetHead();
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    T &value = current->value;
    return value;
}

template <class T>
const T &ImmutableListSequence<T>::operator[](int index) const {
    if (index < 0 || index >= this->GetLength()) throw IndexOutOfRange("Operator[]: Index out of range");
    IEnumerator<T> *current = this->GetEnumerator();
    current->MoveNext();
    for (int i = 0; i < index; ++i) {
        current->MoveNext();
    }
    const T &value = current->GetCurrent();
    delete current;
    return value;
}

template <class T>
ImmutableListSequence<T> ImmutableListSequence<T>::operator+(const ImmutableListSequence<T> &other) const {
    ImmutableListSequence<T> result(*this);
    for (int i = 0; i < other.GetLength(); ++i) {
        result.AppendInternal(other.Get(i));
    }
    return result;
}

template <class T>
ImmutableListSequence<T> &ImmutableListSequence<T>::operator+=(const T &value) {
    ImmutableListSequence<T> result(*this);
    result.AppendInternal(value);
    *this = result;
    return *this;
}

template <class T>
ImmutableListSequence<T> &ImmutableListSequence<T>::operator+=(const ImmutableListSequence<T> &other) {
    ImmutableListSequence<T> result(*this);
    for (int i = 0; i < other.GetLength(); ++i) {
        result.AppendInternal(other.Get(i));
    }
    *this = result;
    return *this;
}
