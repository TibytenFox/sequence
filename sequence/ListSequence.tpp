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
const T &ListSequence<T>::GetFirst() const { return items.GetFirst(); }

template <class T>
const T &ListSequence<T>::GetLast() const { return items.GetLast(); }

template <class T>
const T &ListSequence<T>::Get(int index) const { return items.Get(index); }

template <class T>
int ListSequence<T>::GetLength() const { return items.GetLength(); }

// Caller responsible for deletion
template <class T>
Sequence<T> *ListSequence<T>::GetSubsequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= items.GetLength() || start_index > end_index) {
        throw IndexOutOfRange("GetSubSequence(): Index out of range");
    }

    ISequenceBuilder<T> *builder = this->CreateBuilder();
    int new_size = end_index - start_index + 1;
    for (int i = 0; i < new_size; i++) {
        builder->Append(items.Get(start_index + i));
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
    ISequenceBuilder<T> *builder = this->CreateBuilder();
    for (int i = 0; i < list->GetLength(); i++) {
        builder->Append(list->Get(i));
    }

    Sequence<T> *result = builder->Build();
    delete builder;
    return result;
}

// ---------- Map, Where, Reduce ----------

// template <class T>
// template <class T2>
// ListSequence<T2> *ListSequence<T>::Map(T2 (*func)(T)) const {
//     ListSequence<T2> *mapped_sequence = new ListSequence<T2>();
//     for (int i = 0; i < GetLength(); i++) {
//         mapped_sequence->AppendInternal(func(Get(i)));
//     }
//     return mapped_sequence;
// }

// template <class T>
// ListSequence<T> *ListSequence<T>::Where(bool (*predicate)(T)) const {
//     ListSequence<T> *filtered_sequence = new ListSequence<T>();
//     for (int i = 0; i < GetLength(); i++) {
//         if (predicate(Get(i))) filtered_sequence->AppendInternal(Get(i));
//     }
//     return filtered_sequence;
// }  

// template <class T>
// template <class T2>
// T2 ListSequence<T>::Reduce(T2 (*func)(T2, T), T2 initial) const {
//     T2 result = initial;
//     for (int i = 0; i < GetLength(); i++) {
//         result = func(result, Get(i));
//     }
//     return result;
// }

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
ListSequence<T>::Enumerator::Enumerator(NodePointerType ptr) : current(ptr) {}

template <class T>
void ListSequence<T>::Enumerator::Increment() {
    current = current->next;
}

template <class T>
void ListSequence<T>::Enumerator::Decrement() {
    current = current->prev;
}

template <class T>
T &ListSequence<T>::Enumerator::Dereference() const {
    return current->value;
}

template <class T>
bool ListSequence<T>::Enumerator::Equals(const IEnumerator<T> &other) const {
    const Enumerator *other_dereferenced = dynamic_cast<const Enumerator*>(&other);
    return other_dereferenced && (current == other_dereferenced->current);
}

template <class T>
typename ListSequence<T>::Enumerator *ListSequence<T>::begin() {
    return new Enumerator(items.GetHead());
}

template <class T>
typename ListSequence<T>::Enumerator*ListSequence<T>::end() {
    return new Enumerator(items.GetTail() == nullptr ? nullptr : items.GetTail()->next);
}

// ---------- Const Iterators ----------

template <class T>
ListSequence<T>::ConstEnumerator::ConstEnumerator(NodePointerType ptr) : current(ptr) {}

template <class T>
void ListSequence<T>::ConstEnumerator::Increment() { 
    current = current->next; 
}

template <class T>
void ListSequence<T>::ConstEnumerator::Decrement() { 
    current = current->prev; 
}

template <class T>
const T &ListSequence<T>::ConstEnumerator::Dereference() const { 
    return current->value; 
}

template <class T>
bool ListSequence<T>::ConstEnumerator::Equals(const IConstEnumerator<T> &other) const {
    const ConstEnumerator* otherDerived = dynamic_cast<const ConstEnumerator*>(&other);
    return otherDerived && current == otherDerived->current;
}

template <class T>
typename ListSequence<T>::ConstEnumerator *ListSequence<T>::begin() const {
    return new ConstEnumerator(items.GetHead());
}

template <class T>
typename ListSequence<T>::ConstEnumerator *ListSequence<T>::end() const {
    return new ConstEnumerator(items.GetTail() == nullptr ? nullptr : items.GetTail()->next);
}
