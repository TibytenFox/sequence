#include "list_sequence.h"

// ---------- Constructors and Destructor ----------
template <class T>
ListSequence<T>::ListSequence() : items() {}

template <class T>
ListSequence<T>::ListSequence(T *items, int count) : items(items, count) {}

template <class T>
ListSequence<T>::ListSequence(const ListSequence<T> &list) : items(list.items) {}

template <class T>
ListSequence<T>::~ListSequence() {}

// ---------- Sequence Interface Implementation ----------
template <class T>
T ListSequence<T>::GetFirst() const { return items.GetFirst(); }

template <class T>
T ListSequence<T>::GetLast() const { return items.GetLast(); }

template <class T>
T ListSequence<T>::Get(int index) const { return items.Get(index); }

template <class T>
int ListSequence<T>::GetLength() const { return items.GetLength(); }

template <class T>
ListSequence<T> *ListSequence<T>::GetSubSequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= items.GetLength() || start_index > end_index) {
        throw std::out_of_range("Index out of range");
    }

    // Create a temporary C-array to hold the subsequence items
    LinkedList<T> *sub_items = items.GetSubList(start_index, end_index);
    ListSequence<T> *sub_sequence = new ListSequence<T>();
    for (int i = 0; i < sub_items->GetLength(); i++) {
        // Using sub_sequence->items.Append() directly to avoid using the public Append method which may not modify the current instance
        sub_sequence->items.Append(sub_items->Get(i));
    }
    delete sub_items;
    return sub_sequence;
}

template <class T>
ListSequence<T> *ListSequence<T>::Append(T item) {
    return Instance()->AppendInternal(item);
}

template <class T>
ListSequence<T> *ListSequence<T>::Prepend(T item) {
    return Instance()->PrependInternal(item);
}

template <class T> 
ListSequence<T> *ListSequence<T>::InsertAt(T item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

template <class T>
ListSequence<T> *ListSequence<T>::Concat(Sequence<T> *list) {
    return Instance()->ConcatInternal(list);
}

// ---------- Internal Methods ----------
template <class T>
ListSequence<T> *ListSequence<T>::AppendInternal(T item) { 
    items.Append(item);
    return this;
 }

template <class T>
ListSequence<T> *ListSequence<T>::PrependInternal(T item) { 
    items.Prepend(item); 
    return this; 
}

template <class T>
ListSequence<T> *ListSequence<T>::InsertAtInternal(T item, int index) { 
    items.InsertAt(item, index); 
    return this; 
}

template <class T>
ListSequence<T> *ListSequence<T>::ConcatInternal(Sequence<T> *list) {
    // Create a temporary C-array to hold the subsequence items
    ListSequence<T> *new_sequence = new ListSequence<T>(*this);
    for (int i = 0; i < list->GetLength(); i++) {
        // Using sub_sequence->items.Append() directly to avoid using the public Append method which may not modify the current instance
        new_sequence->items.Append(list->Get(i));
    }
    return new_sequence;
}

// ---------- Operators ----------
template <class T>
ListSequence<T> &ListSequence<T>::operator=(const ListSequence<T> &other) {
    if (this != &other) {
        items = other.items;
    }
    return *this;
}

// ---------- Mutable and Immutable Versions ----------
template <class T>
MutableListSequence<T>::MutableListSequence() : ListSequence<T>() {}

template <class T>
MutableListSequence<T>::MutableListSequence(T *items, int count) : ListSequence<T>(items, count) {}

template <class T>
MutableListSequence<T>::MutableListSequence(const MutableListSequence<T> &list) : ListSequence<T>(list) {}

template <class T>
MutableListSequence<T>::~MutableListSequence() {}

template <class T>
ListSequence<T> *MutableListSequence<T>::Instance() {
    return this;
}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence() : ListSequence<T>() {}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence(T *items, int count) : ListSequence<T>(items, count) {}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence(const ImmutableListSequence<T> &list) : ListSequence<T>(list) {}

template <class T>
ImmutableListSequence<T>::~ImmutableListSequence() {}

template <class T>
ListSequence<T> *ImmutableListSequence<T>::Instance() {
    return new ImmutableListSequence<T>(*this);
}