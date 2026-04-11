#include <stdexcept>
#include "list_sequence.hpp"

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

// Caller responsible for deletion
template <class T>
ListSequence<T> *ListSequence<T>::GetSubSequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= items.GetLength() || start_index > end_index) {
        throw std::out_of_range("Index out of range");
    }

    // Create a temporary C-array to hold the subsequence items
    ListSequence<T> *sub_sequence = new ListSequence<T>();
    int new_size = end_index - start_index + 1;
    for (int i = 0; i < new_size; i++) {
        // Using sub_sequence->items.Append() directly to avoid using the public Append method which may not modify the current instance
        sub_sequence->items.Append(items.Get(start_index + i));
    }
    return sub_sequence;
}

// Using instance 
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

// Returns new heap-allocated sequence
template <class T>
ListSequence<T> *ListSequence<T>::Concat(const Sequence<T> *list) const {
    // Create a temporary C-array to hold the subsequence items
    ListSequence<T> *new_sequence = new ListSequence<T>(*this);
    for (int i = 0; i < list->GetLength(); i++) {
        new_sequence->AppendInternal(list->Get(i));
    }
    return new_sequence;
}

// ---------- Map, Where, Reduce ----------

template <class T>
template <class T2>
ListSequence<T2> *ListSequence<T>::Map(T2 (*func)(T)) const {
    ListSequence<T2> *mapped_sequence = new ListSequence<T2>();
    for (int i = 0; i < GetLength(); i++) {
        mapped_sequence->AppendInternal(func(Get(i)));
    }
    return mapped_sequence;
}

template <class T>
ListSequence<T> *ListSequence<T>::Where(bool (*predicate)(T)) const {
    ListSequence<T> *filtered_sequence = new ListSequence<T>();
    for (int i = 0; i < GetLength(); i++) {
        if (predicate(Get(i))) filtered_sequence->AppendInternal(Get(i));
    }
    return filtered_sequence;
}  

template <class T>
template <class T2>
T2 ListSequence<T>::Reduce(T2 (*func)(T2, T), T2 initial) const {
    T2 result = initial;
    for (int i = 0; i < GetLength(); i++) {
        result = func(result, Get(i));
    }
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
ListSequence<T>::ListSequenceIterator::ListSequenceIterator(NodePointerType ptr) : current(ptr) {}

template <class T>
void ListSequence<T>::ListSequenceIterator::Increment() {
    current = current->next;
}

template <class T>
void ListSequence<T>::ListSequenceIterator::Decrement() {
    current = current->prev;
}

template <class T>
T &ListSequence<T>::ListSequenceIterator::Dereference() const {
    return current->value;
}

template <class T>
bool ListSequence<T>::ListSequenceIterator::Equals(const Iterator<T> &other) const {
    const ListSequenceIterator *other_dereferenced = dynamic_cast<const ListSequenceIterator*>(&other);
    return other_dereferenced && (current == other_dereferenced->current);
}

template <class T>
typename ListSequence<T>::ListSequenceIterator ListSequence<T>::begin() {
    return ListSequenceIterator(items.GetHead());
}

template <class T>
typename ListSequence<T>::ListSequenceIterator ListSequence<T>::end() {
    return ListSequenceIterator(items.GetTail() == nullptr ? nullptr : items.GetTail()->next);
}

// ---------- Const Iterators ----------

template <class T>
ListSequence<T>::ConstListSequenceIterator::ConstListSequenceIterator(NodePointerType ptr) : current(ptr) {}

template <class T>
void ListSequence<T>::ConstListSequenceIterator::Increment() { 
    current = current->next; 
}

template <class T>
void ListSequence<T>::ConstListSequenceIterator::Decrement() { 
    current = current->prev; 
}

template <class T>
const T &ListSequence<T>::ConstListSequenceIterator::Dereference() const { 
    return current->value; 
}

template <class T>
bool ListSequence<T>::ConstListSequenceIterator::Equals(const ConstIterator<T>& other) const {
    const ConstListSequenceIterator* otherDerived = dynamic_cast<const ConstListSequenceIterator*>(&other);
    return otherDerived && current == otherDerived->current;
}

template <class T>
typename ListSequence<T>::ConstListSequenceIterator ListSequence<T>::begin() const {
    return ConstListSequenceIterator(items.GetHead());
}

template <class T>
typename ListSequence<T>::ConstListSequenceIterator ListSequence<T>::end() const {
    return ConstListSequenceIterator(items.GetTail() == nullptr ? nullptr : items.GetTail()->next);
}

// ---------- Operators ----------

template <class T>
ListSequence<T> &ListSequence<T>::operator=(const ListSequence<T> &other) {
    if (this != &other) {
        items = other.items;
    }
    return *this;
}

template <class T>
bool ListSequence<T>::operator==(const ListSequence<T> &other) const {
    if (this->GetLength() != other.GetLength()) return false;

    ConstListSequenceIterator this_current = begin();
    ConstListSequenceIterator other_current = other.begin();
    ConstListSequenceIterator end_it = end();

    while (this_current != end_it) {
        if (*this_current != *other_current) return false;
        ++this_current;
        ++other_current;
    }

    return true;
}

template <class T>
bool ListSequence<T>::operator!=(const ListSequence<T> &other) const {
    return !(*this == other);
}

template <class T>
T &ListSequence<T>::operator[](int index) {
    if (index < 0 || index >= GetLength()) throw std::out_of_range("Index out of range");
    ListSequenceIterator current = begin();
    for (int i = 0; i < index; i++) ++current;
    return *current;
}

template <class T>
const T &ListSequence<T>::operator[](int index) const {
    if (index < 0 || index >= GetLength()) throw std::out_of_range("Index out of range");
    ConstListSequenceIterator current = begin();
    for (int i = 0; i < index; i++) ++current;
    return *current;
}

template <class T>
ListSequence<T> ListSequence<T>::operator+(const ListSequence<T> &other) const {
    ListSequence<T> result(*this);
    for (int i = 0; i < other.GetLength(); ++i)
        result.Append(other.Get(i));        
    return result;
}

template <class T>
ListSequence<T> &ListSequence<T>::operator+=(const T &value) {
    return *Append(value);
}

template <class T>
ListSequence<T> &ListSequence<T>::operator+=(const ListSequence<T> &other) {
    *this = *Concat(&other);
    return *this;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const ListSequence<T> &seq) {
    os << '[';
    for (int i = 0; i < seq.GetLength(); i++) {
        os << seq.Get(i);
        if (i != seq.GetLength() - 1) os << ", ";
    }
    os << ']';
    return os;
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