#include "ImmutableArraySequence.hpp"

template <class T>
ImmutableArraySequence<T> &ImmutableArraySequence<T>::operator=(const ImmutableArraySequence<T> &other) {
	if (this != &other) {
		this->items = other.items;
	}
	return *this;
}

template <class T>
bool ImmutableArraySequence<T>::operator==(const ImmutableArraySequence<T> &other) const {
	if (this->GetLength() != other.GetLength()) return false;
	for (int i = 0; i < this->GetLength(); ++i) {
		if (this->Get(i) != other.Get(i)) return false;
	}
	return true;
}

template <class T>
bool ImmutableArraySequence<T>::operator!=(const ImmutableArraySequence<T> &other) const {
	return !(*this == other);
}

template <class T>
T &ImmutableArraySequence<T>::operator[](int index) {
	return this->items[index];
}

template <class T>
const T &ImmutableArraySequence<T>::operator[](int index) const {
	return this->items.Get(index);
}

template <class T>
ImmutableArraySequence<T> ImmutableArraySequence<T>::operator+(const ImmutableArraySequence<T> &other) const {
	ImmutableArraySequence<T> result(*this);
	for (int i = 0; i < other.GetLength(); ++i) {
		result.AppendInternal(other.Get(i));
	}
	return result;
}

template <class T>
ImmutableArraySequence<T> &ImmutableArraySequence<T>::operator+=(const T &value) {
	ImmutableArraySequence<T> result(*this);
	result.AppendInternal(value);
	*this = result;
	return *this;
}

template <class T>
ImmutableArraySequence<T> &ImmutableArraySequence<T>::operator+=(const ImmutableArraySequence<T> &other) {
	ImmutableArraySequence<T> result(*this);
	for (int i = 0; i < other.GetLength(); ++i) {
		result.AppendInternal(other.Get(i));
	}
	*this = result;
	return *this;
}
