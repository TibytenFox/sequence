#include "array_sequence.h"

template <class T>
ArraySequence<T>::ArraySequence() : items(new DynamicArray<T>(0)) {}

template <class T>
ArraySequence<T>::ArraySequence(T *items, int count) : items(new DynamicArray<T>(items, count)) {}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T> &list) : items(new DynamicArray<T>(*list.items)) {}

template <class T>
ArraySequence<T>::~ArraySequence() { delete items; }

template <class T>
T ArraySequence<T>::GetFirst() const { return items->Get(0); }

template <class T>
T ArraySequence<T>::GetLast() const { return items->Get(items->GetSize() - 1); }

template <class T>
T ArraySequence<T>::Get(int index) const { return items->Get(index); }

template <class T>
ArraySequence<T> *ArraySequence<T>::GetSubSequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= items->GetSize() || start_index > end_index) {
        throw std::out_of_range("Index out of range");
    }
    ArraySequence<T> *sublist = new ArraySequence<T>();
    for (int i = start_index; i <= end_index; i++) {
        sublist->Append(items->Get(i));
    }
    return sublist;
}

template <class T>
int ArraySequence<T>::GetLength() const { return items->GetSize(); }

template <class T>
void ArraySequence<T>::Append(T item) {
    items->Resize(items->GetSize() + 1);
    items->Set(items->GetSize() - 1, item);
}

template <class T>
void ArraySequence<T>::Prepend(T item) {
    items->Resize(items->GetSize() + 1);
    for (int i = items->GetSize() - 1; i > 0; i--) {
        items->Set(i, items->Get(i - 1));
    }
    items->Set(0, item);
}

template <class T>
void ArraySequence<T>::InsertAt(T item, int index) {
    if (index < 0 || index > items->GetSize()) throw std::out_of_range("Index out of range");
    items->Resize(items->GetSize() + 1);
    for (int i = items->GetSize() - 1; i > index; i--) {
        items->Set(i, items->Get(i - 1));
    }
    items->Set(index, item);
}

template <class T>
ArraySequence<T> *ArraySequence<T>::Concat(ArraySequence<T> *list) {
    ArraySequence<T> *new_list = new ArraySequence<T>(*this);
    for (int i = 0; i < list->GetLength(); i++) {
        new_list->Append(list->Get(i));
    }
    return new_list;
}