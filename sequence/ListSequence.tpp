#include "ListSequence.hpp"

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
const T &ListSequence<T>::GetFirst() const { return this->items.GetFirst(); }

template <class T>
const T &ListSequence<T>::GetLast() const { return this->items.GetLast(); }

template <class T>
const T &ListSequence<T>::Get(int index) const { return this->items.Get(index); }

template <class T>
int ListSequence<T>::GetLength() const { return this->items.GetLength(); }

// Caller responsible for deletion
template <class T>
Sequence<T> *ListSequence<T>::GetSubsequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= this->items.GetLength() || start_index > end_index) {
        throw IndexOutOfRange("GetSubSequence(): Index out of range");
    }

    ISequenceBuilder<T> *builder = this->CreateBuilder();
    int new_size = end_index - start_index + 1;
    for (int i = 0; i < new_size; i++) {
        builder->Append(this->items.Get(start_index + i));
    }

    Sequence<T> *result = builder->Build();
    delete builder;
    return result;
}

// Using instance 
template <class T>
Sequence<T> *ListSequence<T>::Append(T item) {
    return Instance()->AppendInternal(item);
}

template <class T>
Sequence<T> *ListSequence<T>::Prepend(T item) {
    return Instance()->PrependInternal(item);
}

template <class T> 
Sequence<T> *ListSequence<T>::InsertAt(T item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

// Returns new heap-allocated sequence
template <class T>
Sequence<T> *ListSequence<T>::Concat(const Sequence<T> *list) const {
    if (!list) throw EmptyCollectionError("Concat: empty collection");

    ISequenceBuilder<T> *builder = this->CreateBuilder();
    for (int i = 0; i < this->GetLength(); i++) {
        builder->Append(this->Get(i));
    }
    for (int i = 0; i < list->GetLength(); i++) {
        builder->Append(list->Get(i));
    }

    Sequence<T> *result = builder->Build();
    delete builder;
    return result;
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

// ---------- Iterators ----------

template <class T>
IEnumerator<T> *ListSequence<T>::GetEnumerator() const {
    return this->items.GetEnumerator();
}


