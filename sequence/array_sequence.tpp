#include "array_sequence.h"

// ---------- Constructors and Destructor ----------
template <class T>
ArraySequence<T>::ArraySequence() : items() {}

template <class T>
ArraySequence<T>::ArraySequence(T *items, int count) : items(items, count) {}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T> &list) : items(list.items) {}

template <class T>
ArraySequence<T>::~ArraySequence() {}

// ---------- Sequence Interface Implementation ----------
template <class T>
T ArraySequence<T>::GetFirst() const { return items.Get(0); }

template <class T>
T ArraySequence<T>::GetLast() const { return items.Get(items.GetSize() - 1); }

template <class T>
T ArraySequence<T>::Get(int index) const { return items.Get(index); }

template <class T>
int ArraySequence<T>::GetLength() const { return items.GetSize(); }

template <class T>
ArraySequence<T> *ArraySequence<T>::GetSubSequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= items.GetSize() || start_index > end_index) {
        throw std::out_of_range("Index out of range");
    }

    int new_size = end_index - start_index + 1;

    // Create a temporary C-array to hold the subsequence items
    T *sub_items = new T[new_size];
    for (int i = 0; i < new_size; i++) {
        sub_items[i] = items.Get(start_index + i);
    }

    ArraySequence<T> *sub_sequence = new ArraySequence<T>(sub_items, new_size);
    delete[] sub_items;
    return sub_sequence;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::Append(T item) {
    return Instance()->AppendInternal(item);
}

template <class T>
ArraySequence<T> *ArraySequence<T>::Prepend(T item) {
    return Instance()->PrependInternal(item);
}

template <class T>
ArraySequence<T> *ArraySequence<T>::InsertAt(T item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

template <class T>
ArraySequence<T> *ArraySequence<T>::Concat(Sequence<T> *list) {
    return Instance()->ConcatInternal(list);
}

// ---------- Internal Methods ----------
template <class T>
ArraySequence<T> *ArraySequence<T>::AppendInternal(T item) {
    items.Resize(items.GetSize() + 1);
    items.Set(items.GetSize() - 1, item);
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::PrependInternal(T item) {
    items.Resize(items.GetSize() + 1);
    for (int i = items.GetSize() - 1; i > 0; i--) {
        items.Set(i, items.Get(i - 1));
    }
    items.Set(0, item);
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::InsertAtInternal(T item, int index) {
    if (index < 0 || index > items.GetSize()) throw std::out_of_range("Index out of range");
    items.Resize(items.GetSize() + 1);
    for (int i = items.GetSize() - 1; i > index; i--) {
        items.Set(i, items.Get(i - 1));
    }
    items.Set(index, item);
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::ConcatInternal(Sequence<T> *list) {
    int total_size = GetLength() + list->GetLength();

    // Create a temporary C-array to hold the concatenated items
    T *concat_items = new T[total_size];
    for (int i = 0; i < GetLength(); i++) {
        concat_items[i] = Get(i);
    }
    for (int i = 0; i < list->GetLength(); i++) {
        concat_items[GetLength() + i] = list->Get(i);
    }
    ArraySequence<T> *concat_sequence = new ArraySequence<T>(concat_items, total_size);
    delete[] concat_items;
    return concat_sequence;
}

// ---------- Operators ----------
template <class T>
ArraySequence<T> &ArraySequence<T>::operator=(const ArraySequence<T> &other) {
    if (this != &other) {
        items = other.items;
    }
    return *this;
}

// ---------- Mutable and Immutable Versions ----------
template <class T>
MutableArraySequence<T>::MutableArraySequence() : ArraySequence<T>() {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence<T> &list) : ArraySequence<T>(list) {}

template <class T>
MutableArraySequence<T>::~MutableArraySequence() {}

template <class T>
ArraySequence<T> *MutableArraySequence<T>::Instance() {
    return this;
}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence() : ArraySequence<T>() {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T> &list) : ArraySequence<T>(list) {}

template <class T>
ImmutableArraySequence<T>::~ImmutableArraySequence() {}

template <class T>
ArraySequence<T> *ImmutableArraySequence<T>::Instance() {
    return new ImmutableArraySequence<T>(*this);
}
